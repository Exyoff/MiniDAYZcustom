// Draws a Construct 2 layout with SDL2 + OpenGL.
//
// Sprites live in packed spritesheets, so a frame is a rect inside a sheet
// rather than a whole image. Draws are batched per texture, and instances are
// drawn in layer order so the layout composites the way the original does.
//
// Deliberately small and hackable: one shader, one dynamic vertex buffer, and
// a camera. It is the starting point for reworking the game, not a finished
// engine.
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "project.hpp"

namespace mdz {

struct Camera {
    double x = 0.0, y = 0.0;   // world point at the centre of the window
    double zoom = 1.0;
};

class Renderer {
public:
    ~Renderer();

    // Creates the window and GL context. `headless` uses SDL's offscreen video
    // driver so a frame can be rendered and saved without a display.
    bool init(int width, int height, const std::string& title, bool headless);
    void shutdown();

    // Uploads every spritesheet the layout's object types reference.
    // Returns the number of textures loaded; missing files are counted, not fatal.
    int load_textures(const Project& project, const Layout& layout,
                      const std::string& game_dir);

    void draw_layout(const Project& project, const Layout& layout, const Camera& camera);
    void present();

    bool save_png(const std::string& path);

    int width() const { return width_; }
    int height() const { return height_; }
    size_t missing_textures() const { return missing_.size(); }
    const std::vector<std::string>& missing() const { return missing_; }
    size_t quads_drawn() const { return quads_drawn_; }

private:
    struct Texture { unsigned id = 0; int w = 0, h = 0; };
    struct Vertex { float x, y, u, v; };

    void* window_ = nullptr;
    void* gl_ = nullptr;
    int width_ = 0, height_ = 0;
    unsigned program_ = 0, vbo_ = 0;
    int uniform_projection_ = -1;

    std::unordered_map<std::string, Texture> textures_;
    std::vector<std::string> missing_;
    std::vector<Vertex> batch_;
    size_t quads_drawn_ = 0;

    bool build_shader();
    void flush(unsigned texture);
};

}  // namespace mdz
