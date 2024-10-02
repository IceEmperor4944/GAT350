#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>

void Renderer::Initialize() {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
    }
}

bool Renderer::CreateWindow(std::string title, int width, int height) {
    // create window
    // returns pointer to window if successful or nullptr if failed
    m_window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    return true;
}

void Renderer::CopyFramebuffer(const Framebuffer& buffer) {
    SDL_RenderCopy(m_renderer, buffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& buffer) {
    SDL_RenderCopy(m_renderer, buffer.m_texture, NULL, NULL);
}