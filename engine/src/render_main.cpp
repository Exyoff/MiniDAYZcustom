// Opens a window and draws a layout from the original game data.
//
//   mdz_view <data.js> <game_dir> [--layout NAME] [--fit] [--center X,Y]
//            [--zoom Z] [--size WxH] [--screenshot FILE]
//
// With --screenshot it renders one frame through SDL's offscreen driver and
// writes a PNG, so it works without a display.
//
// Interactive: arrow keys / drag to pan, +/- or wheel to zoom, ESC to quit.

#include <SDL2/SDL.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "project.hpp"
#include "renderer.hpp"

using namespace mdz;

namespace {

const char* arg_value(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc - 1; ++i)
        if (std::strcmp(argv[i], flag) == 0) return argv[i + 1];
    return nullptr;
}
bool has_flag(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc; ++i) if (std::strcmp(argv[i], flag) == 0) return true;
    return false;
}

// Frames the instances rather than the layout bounds: several layouts are far
// larger than the area actually populated, so fitting the declared size would
// show mostly empty space.
Camera fit_to_instances(const Layout& layout, int vw, int vh) {
    Camera cam;
    if (layout.instances.empty()) {
        cam.x = layout.width / 2.0;
        cam.y = layout.height / 2.0;
        cam.zoom = 1.0;
        return cam;
    }
    double minx = 1e18, miny = 1e18, maxx = -1e18, maxy = -1e18;
    for (const Instance& i : layout.instances) {
        minx = std::min(minx, i.x); maxx = std::max(maxx, i.x);
        miny = std::min(miny, i.y); maxy = std::max(maxy, i.y);
    }
    cam.x = (minx + maxx) / 2.0;
    cam.y = (miny + maxy) / 2.0;
    const double w = std::max(1.0, maxx - minx), h = std::max(1.0, maxy - miny);
    cam.zoom = std::min(vw / w, vh / h) * 0.95;
    return cam;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr,
                     "usage: mdz_view <data.js> <game_dir> [--layout NAME] [--fit]\n"
                     "                [--center X,Y] [--zoom Z] [--size WxH]\n"
                     "                [--screenshot FILE]\n");
        return 2;
    }
    const std::string data_js = argv[1];
    const std::string game_dir = argv[2];
    const char* shot = arg_value(argc, argv, "--screenshot");

    int vw = 1024, vh = 768;
    if (const char* size = arg_value(argc, argv, "--size")) {
        int a = 0, b = 0;
        if (std::sscanf(size, "%dx%d", &a, &b) == 2 && a > 0 && b > 0) { vw = a; vh = b; }
    }

    try {
        Project project = Project::load(data_js);

        const char* want = arg_value(argc, argv, "--layout");
        const Layout* layout = nullptr;
        for (const Layout& l : project.layouts) {
            if (want) { if (l.name == want) { layout = &l; break; } }
            else if (!layout || l.instances.size() > layout->instances.size()) layout = &l;
        }
        if (!layout) {
            std::fprintf(stderr, "layout not found. available:");
            for (const Layout& l : project.layouts) std::fprintf(stderr, " %s", l.name.c_str());
            std::fprintf(stderr, "\n");
            return 1;
        }
        std::printf("layout %s  %dx%d  %zu instances\n", layout->name.c_str(),
                    layout->width, layout->height, layout->instances.size());

        Renderer renderer;
        if (!renderer.init(vw, vh, "MiniDayZ viewer", shot != nullptr)) return 1;

        const int loaded = renderer.load_textures(project, *layout, game_dir);
        std::printf("textures loaded %d, missing %zu\n", loaded, renderer.missing_textures());
        for (size_t i = 0; i < renderer.missing().size() && i < 5; ++i)
            std::printf("  missing: %s\n", renderer.missing()[i].c_str());

        Camera cam = fit_to_instances(*layout, vw, vh);
        if (has_flag(argc, argv, "--fit")) { /* already fitted */ }
        if (const char* c = arg_value(argc, argv, "--center")) {
            double x = 0, y = 0;
            if (std::sscanf(c, "%lf,%lf", &x, &y) == 2) { cam.x = x; cam.y = y; cam.zoom = 1.0; }
        }
        if (const char* z = arg_value(argc, argv, "--zoom")) cam.zoom = std::atof(z);
        std::printf("camera (%.0f, %.0f) zoom %.4f\n", cam.x, cam.y, cam.zoom);

        if (shot) {
            renderer.draw_layout(project, *layout, cam);
            std::printf("quads drawn %zu\n", renderer.quads_drawn());
            const bool ok = renderer.save_png(shot);
            std::printf("%s %s\n", ok ? "wrote" : "FAILED to write", shot);
            return ok ? 0 : 1;
        }

        bool running = true;
        bool dragging = false;
        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = false;
                else if (e.type == SDL_KEYDOWN) {
                    const double step = 64.0 / cam.zoom;
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: running = false; break;
                        case SDLK_LEFT:   cam.x -= step; break;
                        case SDLK_RIGHT:  cam.x += step; break;
                        case SDLK_UP:     cam.y -= step; break;
                        case SDLK_DOWN:   cam.y += step; break;
                        case SDLK_EQUALS:
                        case SDLK_PLUS:   cam.zoom *= 1.25; break;
                        case SDLK_MINUS:  cam.zoom /= 1.25; break;
                        default: break;
                    }
                } else if (e.type == SDL_MOUSEWHEEL) {
                    cam.zoom *= (e.wheel.y > 0) ? 1.1 : (1.0 / 1.1);
                } else if (e.type == SDL_MOUSEBUTTONDOWN) dragging = true;
                else if (e.type == SDL_MOUSEBUTTONUP) dragging = false;
                else if (e.type == SDL_MOUSEMOTION && dragging) {
                    cam.x -= e.motion.xrel / cam.zoom;
                    cam.y -= e.motion.yrel / cam.zoom;
                }
            }
            renderer.draw_layout(project, *layout, cam);
            renderer.present();
            SDL_Delay(16);
        }
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
