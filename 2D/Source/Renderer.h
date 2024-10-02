#pragma once
#include <SDL.h>
#include <string>

class Renderer {
public:
    void Initialize();
    bool CreateWindow(std::string title, int width, int height);
    void CopyFramebuffer(const class Framebuffer& buffer);

    void operator = (const class Framebuffer& buffer);
public:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
};