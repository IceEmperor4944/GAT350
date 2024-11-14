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
void InitScene01(Scene& scene, Camera& camera);
void InitSceneCBox(Scene& scene, Camera& camera);
void InitSceneFinal(Scene& scene, Camera& camera);

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
    camera.SetView({ 0, 5, -20 }, { 0, 5, 0 });

    Scene scene;
    //InitScene(scene);
    InitSceneFinal(scene, camera);
    scene.Update();

    scene.Render(framebuffer, camera, 80, 6);
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
    scene.SetSky(HSVtoRGB(29, 0.81f, 0.60f), HSVtoRGB(214, 1.0f, 0.49f));

    auto gray       = std::make_shared<Metal>(color3_t{ 0.2f }, 0.5f);
    auto red        = std::make_shared<Dielectric>(color3_t{ 1, 0, 0 }, 1.33f);
    auto blue       = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.33f);
    auto green      = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.33f);
    auto yellow     = std::make_shared<Lambertian>(color3_t{ 1, 1, 0 });
    auto magenta    = std::make_shared<Lambertian>(color3_t{ 1, 0, 1 });
    auto cyan       = std::make_shared<Emissive>(color3_t{ 0, 1, 1 });
    auto navy       = std::make_shared<Lambertian>(HSVtoRGB(222.3f, 0.64f, 0.46f));

    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(gray);
    materials.push_back(red);
    materials.push_back(blue);
    materials.push_back(green);
    materials.push_back(yellow);
    materials.push_back(magenta);
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

    auto model = std::make_unique<Model>(Transform{ glm::vec3{2} , glm::vec3{40, 40, 0}, glm::vec3{4} }, navy);
    model->Load("Models/cube.obj");
    scene.AddObject(std::move(model));
}

void InitScene01(Scene& scene, Camera& camera) {
    camera.SetFOV(20.0f);
    camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

    auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
    scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomf();
            glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    auto fuzz = randomf(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

    auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}

void InitSceneCBox(Scene& scene, Camera& camera) {
    scene.SetSky(HSVtoRGB(29, 0.81f, 0.60f), HSVtoRGB(214, 1.0f, 0.49f));
    camera.SetFOV(70.0f);
    camera.SetView({ 0, 0, -4 }, { 0, 0, 0 });

    auto white  = std::make_shared<Lambertian>(color3_t{ 1, 1, 1 });
    auto red    = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    auto green  = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
    auto light  = std::make_shared<Emissive>(color3_t{ 1, 1, 1 });

    auto floor = std::make_unique<Plane>(Transform{ glm::vec3{ 0, -2, 0 }, glm::vec3{ 0, 0, 0 } }, white);
    scene.AddObject(std::move(floor));

    auto lWall = std::make_unique<Plane>(Transform{ glm::vec3{ -2, 0, 0 }, glm::vec3{ 0, 0, 90 } }, red);
    scene.AddObject(std::move(lWall));

    auto rWall = std::make_unique<Plane>(Transform{ glm::vec3{ 2, 0, 0 }, glm::vec3{ 0, 0, -90 } }, green);
    scene.AddObject(std::move(rWall));

    auto back = std::make_unique<Plane>(Transform{ glm::vec3{ 0, 0, 1 }, glm::vec3{ 90, 0, 0 } }, white);
    scene.AddObject(std::move(back));

    auto ceiling = std::make_unique<Plane>(Transform{ glm::vec3{ 0, 2, 0 }, glm::vec3{ 0, 0, 0 } }, white);
    scene.AddObject(std::move(ceiling));

    auto lamp = std::make_unique<Model>(Transform{ glm::vec3{0, 1.99f, 0}, glm::vec3{90, 180, 0}, glm::vec3{0.75f} }, light);
    lamp->Load("Models/quad.obj");
    scene.AddObject(std::move(lamp));

    auto rect = std::make_unique<Model>(Transform{ glm::vec3{-0.75f, -0.5f, 0.75f}, glm::vec3{0, 30, 0}, glm::vec3{1.25f, 3, 1.25f} }, white);
    rect->Load("Models/cube.obj");
    scene.AddObject(std::move(rect));

    auto sphere = std::make_unique<Sphere>(Transform{ glm::vec3{ 0.5f, -1.25f, -0.5f }, glm::vec3{ 0, 0, 0 } }, 0.75f, white);
    scene.AddObject(std::move(sphere));
}

void InitSceneFinal(Scene& scene, Camera& camera) {
    scene.SetSky(HSVtoRGB(214, 1.0f, 0.10f), HSVtoRGB(29, 0.81f, 0.12f));
    camera.SetFOV(20.0f);
    camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

    auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
    scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

    auto light = std::make_shared<Emissive>(color3_t{ 1, 1, 1 });

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomf();
            glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    auto fuzz = randomf(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

    auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));

    auto teapot = std::make_unique<Model>(Transform{ glm::vec3{0, 0, 2}, glm::vec3{0, 90, 0}, glm::vec3{0.25f} }, light);
    teapot->Load("Models/teapot.obj");
    scene.AddObject(std::move(teapot));
}