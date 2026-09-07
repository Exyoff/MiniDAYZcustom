#include "renderer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace mdz {
namespace {

// GL 2.0 entry points are not in the 1.1 headers every platform ships, so they
// are loaded through SDL rather than a loader library. Only what the renderer
// actually uses is loaded.
struct GLFuncs {
    unsigned (APIENTRY *CreateShader)(unsigned);
    void (APIENTRY *ShaderSource)(unsigned, int, const char* const*, const int*);
    void (APIENTRY *CompileShader)(unsigned);
    void (APIENTRY *GetShaderiv)(unsigned, unsigned, int*);
    void (APIENTRY *GetShaderInfoLog)(unsigned, int, int*, char*);
    unsigned (APIENTRY *CreateProgram)();
    void (APIENTRY *AttachShader)(unsigned, unsigned);
    void (APIENTRY *LinkProgram)(unsigned);
    void (APIENTRY *GetProgramiv)(unsigned, unsigned, int*);
    void (APIENTRY *GetProgramInfoLog)(unsigned, int, int*, char*);
    void (APIENTRY *UseProgram)(unsigned);
    void (APIENTRY *DeleteShader)(unsigned);
    void (APIENTRY *GenBuffers)(int, unsigned*);
    void (APIENTRY *BindBuffer)(unsigned, unsigned);
    void (APIENTRY *BufferData)(unsigned, ptrdiff_t, const void*, unsigned);
    int  (APIENTRY *GetAttribLocation)(unsigned, const char*);
    int  (APIENTRY *GetUniformLocation)(unsigned, const char*);
    void (APIENTRY *EnableVertexAttribArray)(unsigned);
    void (APIENTRY *VertexAttribPointer)(unsigned, int, unsigned, unsigned char, int, const void*);
    void (APIENTRY *UniformMatrix4fv)(int, int, unsigned char, const float*);
    void (APIENTRY *Uniform1i)(int, int);
};
GLFuncs gl;

template <typename T>
bool load_fn(T* slot, const char* name) {
    *slot = reinterpret_cast<T>(SDL_GL_GetProcAddress(name));
    if (!*slot) std::fprintf(stderr, "GL: missing %s\n", name);
    return *slot != nullptr;
}

bool load_gl() {
    bool ok = true;
    ok &= load_fn(&gl.CreateShader, "glCreateShader");
    ok &= load_fn(&gl.ShaderSource, "glShaderSource");
    ok &= load_fn(&gl.CompileShader, "glCompileShader");
    ok &= load_fn(&gl.GetShaderiv, "glGetShaderiv");
    ok &= load_fn(&gl.GetShaderInfoLog, "glGetShaderInfoLog");
    ok &= load_fn(&gl.CreateProgram, "glCreateProgram");
    ok &= load_fn(&gl.AttachShader, "glAttachShader");
    ok &= load_fn(&gl.LinkProgram, "glLinkProgram");
    ok &= load_fn(&gl.GetProgramiv, "glGetProgramiv");
    ok &= load_fn(&gl.GetProgramInfoLog, "glGetProgramInfoLog");
    ok &= load_fn(&gl.UseProgram, "glUseProgram");
    ok &= load_fn(&gl.DeleteShader, "glDeleteShader");
    ok &= load_fn(&gl.GenBuffers, "glGenBuffers");
    ok &= load_fn(&gl.BindBuffer, "glBindBuffer");
    ok &= load_fn(&gl.BufferData, "glBufferData");
    ok &= load_fn(&gl.GetAttribLocation, "glGetAttribLocation");
    ok &= load_fn(&gl.GetUniformLocation, "glGetUniformLocation");
    ok &= load_fn(&gl.EnableVertexAttribArray, "glEnableVertexAttribArray");
    ok &= load_fn(&gl.VertexAttribPointer, "glVertexAttribPointer");
    ok &= load_fn(&gl.UniformMatrix4fv, "glUniformMatrix4fv");
    ok &= load_fn(&gl.Uniform1i, "glUniform1i");
    return ok;
}

const char* kVertexShader = R"(
attribute vec2 a_pos;
attribute vec2 a_uv;
uniform mat4 u_projection;
varying vec2 v_uv;
void main() {
    v_uv = a_uv;
    gl_Position = u_projection * vec4(a_pos, 0.0, 1.0);
}
)";

const char* kFragmentShader = R"(
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D u_texture;
varying vec2 v_uv;
void main() {
    vec4 c = texture2D(u_texture, v_uv);
    if (c.a < 0.01) discard;
    gl_FragColor = c;
}
)";

unsigned compile(unsigned type, const char* src) {
    unsigned s = gl.CreateShader(type);
    gl.ShaderSource(s, 1, &src, nullptr);
    gl.CompileShader(s);
    int ok = 0;
    gl.GetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024] = {0};
        gl.GetShaderInfoLog(s, sizeof(log) - 1, nullptr, log);
        std::fprintf(stderr, "shader compile failed: %s\n", log);
        gl.DeleteShader(s);
        return 0;
    }
    return s;
}

}  // namespace

Renderer::~Renderer() { shutdown(); }

bool Renderer::init(int width, int height, const std::string& title, bool headless) {
    width_ = width;
    height_ = height;

    // The offscreen driver renders through EGL with no display attached, which
    // is what makes it possible to produce and check a frame from a terminal.
    if (headless) SDL_SetHint(SDL_HINT_VIDEODRIVER, "offscreen");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* w = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, width, height,
                                     SDL_WINDOW_OPENGL | (headless ? 0 : SDL_WINDOW_SHOWN));
    if (!w) { std::fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError()); return false; }
    window_ = w;

    SDL_GLContext ctx = SDL_GL_CreateContext(w);
    if (!ctx) { std::fprintf(stderr, "SDL_GL_CreateContext: %s\n", SDL_GetError()); return false; }
    gl_ = ctx;

    if (!load_gl()) return false;
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
        return false;
    }
    if (!build_shader()) return false;

    gl.GenBuffers(1, &vbo_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, width_, height_);
    return true;
}

bool Renderer::build_shader() {
    unsigned vs = compile(GL_VERTEX_SHADER, kVertexShader);
    unsigned fs = compile(GL_FRAGMENT_SHADER, kFragmentShader);
    if (!vs || !fs) return false;
    program_ = gl.CreateProgram();
    gl.AttachShader(program_, vs);
    gl.AttachShader(program_, fs);
    gl.LinkProgram(program_);
    int ok = 0;
    gl.GetProgramiv(program_, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024] = {0};
        gl.GetProgramInfoLog(program_, sizeof(log) - 1, nullptr, log);
        std::fprintf(stderr, "program link failed: %s\n", log);
        return false;
    }
    gl.DeleteShader(vs);
    gl.DeleteShader(fs);
    uniform_projection_ = gl.GetUniformLocation(program_, "u_projection");
    return true;
}

void Renderer::shutdown() {
    if (gl_) { SDL_GL_DeleteContext(static_cast<SDL_GLContext>(gl_)); gl_ = nullptr; }
    if (window_) { SDL_DestroyWindow(static_cast<SDL_Window*>(window_)); window_ = nullptr; }
    if (SDL_WasInit(SDL_INIT_VIDEO)) { IMG_Quit(); SDL_Quit(); }
}

int Renderer::load_textures(const Project& project, const Layout& layout,
                            const std::string& game_dir) {
    // Only the sheets this layout's types actually reference.
    std::vector<std::string> wanted;
    for (const Instance& inst : layout.instances) {
        if (inst.object_type < 0 || inst.object_type >= static_cast<int>(project.object_types.size()))
            continue;
        for (const Animation& a : project.type(inst.object_type).animations)
            for (const Frame& f : a.frames) wanted.push_back(f.image);
    }
    std::sort(wanted.begin(), wanted.end());
    wanted.erase(std::unique(wanted.begin(), wanted.end()), wanted.end());

    int loaded = 0;
    for (const std::string& rel : wanted) {
        if (textures_.count(rel)) continue;
        const std::string path = game_dir + "/" + rel;
        SDL_Surface* raw = IMG_Load(path.c_str());
        if (!raw) { missing_.push_back(rel); continue; }
        SDL_Surface* rgba = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_FreeSurface(raw);
        if (!rgba) { missing_.push_back(rel); continue; }

        Texture t;
        t.w = rgba->w;
        t.h = rgba->h;
        glGenTextures(1, &t.id);
        glBindTexture(GL_TEXTURE_2D, t.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba->w, rgba->h, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, rgba->pixels);
        SDL_FreeSurface(rgba);
        textures_[rel] = t;
        ++loaded;
    }
    return loaded;
}

void Renderer::flush(unsigned texture) {
    if (batch_.empty()) return;
    glBindTexture(GL_TEXTURE_2D, texture);
    gl.BindBuffer(GL_ARRAY_BUFFER, vbo_);
    gl.BufferData(GL_ARRAY_BUFFER,
                  static_cast<ptrdiff_t>(batch_.size() * sizeof(Vertex)),
                  batch_.data(), GL_STREAM_DRAW);
    int a_pos = gl.GetAttribLocation(program_, "a_pos");
    int a_uv = gl.GetAttribLocation(program_, "a_uv");
    gl.EnableVertexAttribArray(static_cast<unsigned>(a_pos));
    gl.EnableVertexAttribArray(static_cast<unsigned>(a_uv));
    gl.VertexAttribPointer(static_cast<unsigned>(a_pos), 2, GL_FLOAT, GL_FALSE,
                           sizeof(Vertex), reinterpret_cast<void*>(0));
    gl.VertexAttribPointer(static_cast<unsigned>(a_uv), 2, GL_FLOAT, GL_FALSE,
                           sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 2));
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(batch_.size()));
    batch_.clear();
}

void Renderer::draw_layout(const Project& project, const Layout& layout, const Camera& cam) {
    quads_drawn_ = 0;
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    gl.UseProgram(program_);

    // Orthographic projection centred on the camera. Construct 2's Y axis runs
    // downward, so the matrix flips Y.
    const double halfw = width_ / (2.0 * cam.zoom);
    const double halfh = height_ / (2.0 * cam.zoom);
    const double l = cam.x - halfw, r = cam.x + halfw;
    const double t = cam.y - halfh, b = cam.y + halfh;
    float m[16] = {0};
    m[0] = static_cast<float>(2.0 / (r - l));
    m[5] = static_cast<float>(-2.0 / (b - t));
    m[10] = 1.0f;
    m[12] = static_cast<float>(-(r + l) / (r - l));
    m[13] = static_cast<float>((b + t) / (b - t));
    m[15] = 1.0f;
    gl.UniformMatrix4fv(uniform_projection_, 1, GL_FALSE, m);

    // Layer order decides compositing, so sort by it before drawing.
    std::vector<const Instance*> ordered;
    ordered.reserve(layout.instances.size());
    for (const Instance& i : layout.instances) ordered.push_back(&i);
    std::stable_sort(ordered.begin(), ordered.end(),
                     [](const Instance* a, const Instance* b) { return a->layer < b->layer; });

    unsigned current = 0;
    for (const Instance* inst : ordered) {
        // Skip hidden layers: spawn markers ship visible=false, and UI layers
        // ship at opacity 0 for events to fade in.
        if (!layout.layer_draws(inst->layer)) continue;
        if (inst->object_type < 0 ||
            inst->object_type >= static_cast<int>(project.object_types.size())) continue;
        const Frame* f = project.type(inst->object_type).first_frame();
        if (!f) continue;
        auto it = textures_.find(f->image);
        if (it == textures_.end()) continue;
        const Texture& tex = it->second;
        if (tex.w <= 0 || tex.h <= 0) continue;

        // Cull anything fully outside the view.
        const double w = inst->width != 0 ? inst->width : f->w;
        const double h = inst->height != 0 ? inst->height : f->h;
        const double radius = std::sqrt(w * w + h * h);
        if (inst->x + radius < l || inst->x - radius > r ||
            inst->y + radius < t || inst->y - radius > b) continue;

        if (tex.id != current) { flush(current); current = tex.id; }

        // Quad corners about the hotspot, rotated by the instance angle.
        const double ox = -f->hotspot_x * w, oy = -f->hotspot_y * h;
        const double ca = std::cos(inst->angle), sa = std::sin(inst->angle);
        const double cx[4] = { ox, ox + w, ox + w, ox };
        const double cy[4] = { oy, oy,     oy + h, oy + h };
        float px[4], py[4];
        for (int k = 0; k < 4; ++k) {
            px[k] = static_cast<float>(inst->x + cx[k] * ca - cy[k] * sa);
            py[k] = static_cast<float>(inst->y + cx[k] * sa + cy[k] * ca);
        }

        const float u0 = static_cast<float>(f->x) / tex.w;
        const float v0 = static_cast<float>(f->y) / tex.h;
        const float u1 = static_cast<float>(f->x + f->w) / tex.w;
        const float v1 = static_cast<float>(f->y + f->h) / tex.h;
        const float uu[4] = { u0, u1, u1, u0 };
        const float vv[4] = { v0, v0, v1, v1 };

        const int tri[6] = { 0, 1, 2, 0, 2, 3 };
        for (int k : tri) batch_.push_back(Vertex{ px[k], py[k], uu[k], vv[k] });
        ++quads_drawn_;
    }
    flush(current);
}

void Renderer::present() {
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(window_));
}

bool Renderer::save_png(const std::string& path) {
    std::vector<unsigned char> pixels(static_cast<size_t>(width_) * height_ * 4);
    glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    // glReadPixels returns bottom-up; PNG wants top-down.
    const size_t stride = static_cast<size_t>(width_) * 4;
    std::vector<unsigned char> flipped(pixels.size());
    for (int y = 0; y < height_; ++y)
        std::memcpy(&flipped[static_cast<size_t>(y) * stride],
                    &pixels[static_cast<size_t>(height_ - 1 - y) * stride], stride);

    SDL_Surface* s = SDL_CreateRGBSurfaceWithFormatFrom(
        flipped.data(), width_, height_, 32, static_cast<int>(stride), SDL_PIXELFORMAT_ABGR8888);
    if (!s) { std::fprintf(stderr, "surface: %s\n", SDL_GetError()); return false; }
    const bool ok = IMG_SavePNG(s, path.c_str()) == 0;
    if (!ok) std::fprintf(stderr, "IMG_SavePNG: %s\n", IMG_GetError());
    SDL_FreeSurface(s);
    return ok;
}

}  // namespace mdz
