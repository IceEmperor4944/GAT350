#include "Renderer.h"
#include "Framebuffer.h"

#include <SDL.h>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 1000, 1000);

    Framebuffer framebuffer(renderer, 500, 500);

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        // clear screen
        //SDL_SetRenderDrawColor(renderer.m_renderer, 0, 0, 0, 0);
        //SDL_RenderClear(renderer.m_renderer);

        framebuffer.Clear(color_t{ 0, 0, 0, 255 });

        for (int i = 0; i < 15; i++) {
            int x = rand() % 250;
            int y = rand() % 250;
            int x2 = rand() % 250;
            int y2 = rand() % 250;
            int x3 = rand() % 250;
            int y3 = rand() % 250;
            color_t col = { (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 255 };
            //framebuffer.DrawPoint(x, y, { 255, 255, 255, 255 });
            //framebuffer.DrawRect(100, 100, 10, 10, col);
            framebuffer.DrawLine(0, 0, 100, 100, col);
            framebuffer.DrawCircle(100, 200, 50, col);
            framebuffer.DrawTriangle(200, 200, 300, 100, 400, 200, col);
        }

        framebuffer.Update();

        renderer = framebuffer;
        //renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}