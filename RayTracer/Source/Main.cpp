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

void InitScene(Scene& scene);

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
    camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

    Scene scene;
    InitScene(scene);
    scene.Update();

    scene.Render(framebuffer, camera, 100, 10);
    framebuffer.Update();

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

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}

void InitScene(Scene& scene) {
    auto gray   = std::make_shared<Metal>(color3_t{ 0.5f }, 0.5f);
    auto red    = std::make_shared<Dielectric>(color3_t{ 1, 0, 0 }, 1.33f);
    auto blue   = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.33f);
    auto green  = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.33f);
    auto yellow = std::make_shared<Lambertian>(color3_t{ 1, 1, 0 });
    auto purple = std::make_shared<Lambertian>(color3_t{ 1, 0, 1 });
    auto cyan = std::make_shared<Emissive>(color3_t{ 0, 1, 1 });

    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(red);
    materials.push_back(blue);
    materials.push_back(green);
    materials.push_back(yellow);
    materials.push_back(purple);
    materials.push_back(cyan);

    auto plane = std::make_unique<Plane>(Transform{ glm::vec3{ 0, -2, 0 }, glm::vec3{ 0, 0, 0 } }, gray);
    scene.AddObject(std::move(plane));

    /*for (auto& mat : materials) {
        float randRad = randomf(-10.0f, 10.0f);

        auto object = std::make_unique<Sphere>(Transform{ random(glm::vec3{ -10 }, glm::vec3{ 10 }) }, randRad, cyan);
        scene.AddObject(std::move(object));
    }*/

    /*auto triangle = std::make_unique<Triangle>(glm::vec3{ -5, 0, 0 }, glm::vec3{ 0, 5, 0 }, glm::vec3{ 5, 0, 0 }, Transform{ glm::vec3{ 0, 1, 0 } }, red);
    scene.AddObject(std::move(triangle));*/

    auto model = std::make_unique<Model>(Transform{ glm::vec3{2} , glm::vec3{40, 40, 0}, glm::vec3{4} }, purple);
    model->Load("Models/cube.obj");
    scene.AddObject(std::move(model));
}