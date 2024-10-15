#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "MathUtils.h"
#include "Color.h"

#include <SDL.h>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 1000, 1000);

    Framebuffer framebuffer(renderer, 1000, 1000);

    Image image;
    image.Load("bridge.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 128);

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

        framebuffer.Clear(color_t{ 0, 0, 0, 255 });

        for (int i = 0; i < 5; i++) {
            int x = rand() % 750;
            int y = rand() % 750;
            int x2 = rand() % 750;
            int y2 = rand() % 750;
            int x3 = rand() % 750;
            int y3 = rand() % 750;
            color_t col = { (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 255 };
            //framebuffer.DrawPoint(x, y, { 255, 255, 255, 255 });
            //framebuffer.DrawRect(100, 100, 10, 10, col);
            //framebuffer.DrawLine(0, 0, 100, 100, col);
            //framebuffer.DrawCircle(100, 200, 50, col);
            //framebuffer.DrawTriangle(200, 200, 300, 100, 400, 200, col);
            //framebuffer.DrawImage(x, y, image);
        }

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        //framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255, 255, 255, 255 });
        //framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, 10, { 255, 255, 255, 255 });
        //framebuffer.DrawCubicCurve(300, 700, 700, 700, mx, my, 600, 400, 10, { 255, 255, 255, 255 });

        int ticks = SDL_GetTicks();
        float time = ticks * 0.001f;
        float t = abs(std::sin(time));
        //int x, y;
        //CubicPoint(300, 700, 700, 700, mx, my, 600, 400, t, x, y);
        //framebuffer.DrawRect(x - 20, y - 20, 40, 40, { 0, 255, 0, 255 });
        
        SetBlendMode(BlendMode::NORMAL);
        framebuffer.DrawImage(100, 300, image);
        SetBlendMode(BlendMode::ADDITIVE);
        framebuffer.DrawImage(mx - 100, my - 100, imageAlpha);

        //PostProcess::Invert(framebuffer.m_buffer);
        //PostProcess::Monochrome(framebuffer.m_buffer);
        //PostProcess::Brightness(framebuffer.m_buffer, 20);
        //PostProcess::ColorBalance(framebuffer.m_buffer, 100, 100, -10);
        //PostProcess::Noise(framebuffer.m_buffer, 10);
        //PostProcess::Threshold(framebuffer.m_buffer, 150);
        //PostProcess::Posterize(framebuffer.m_buffer, 10);
        //PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 128);
        //PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        framebuffer.Update();

        renderer = framebuffer;
        //renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}