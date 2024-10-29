#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "MathUtils.h"
#include "Color.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Camera.h"
#include "Actor.h"
#include "Tracer.h"
#include "Random.h"
#include "Scene.h"

#include <SDL.h>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    //initialize
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Ray Tracer", 800, 600);

    SetBlendMode(BlendMode::NORMAL);

    Framebuffer framebuffer(renderer, renderer.m_width, renderer.m_height);

    Camera camera{ 70.0f, framebuffer.m_width / (float)framebuffer.m_height };
    camera.SetView({ 0, 0, 20 }, { 0, 0, 0 });

    Scene scene;

    auto gray = std::make_shared<Lambertian>(color3_t{ 0.2f });



    auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 2.0f, gray);
    scene.AddObject(std::move(sphere));

    bool quit = false;
    while (!quit) {
        time.Tick();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        framebuffer.Clear(ColorConvert(color4_t{ 0, 0.25f, 0, 1 }));

        scene.Render(framebuffer, camera);

        framebuffer.Update();
        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}