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
#include "Random.h"
#include "Shader.h"

#include <SDL.h>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[]) {
    //initialize
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 800, 600);
    
    Input input;
    input.Initialize();
    input.Update();

    SetBlendMode(BlendMode::NORMAL);

    Camera camera(renderer.m_width, renderer.m_height);
    camera.SetView(glm::vec3{ 0, 0, -50 }, glm::vec3{ 0 });
    camera.SetProjection(90.0f, 800.0f / 600, 0.1f, 1000.0f);
    Transform camTrans{ {0, 0, -20} };

    Framebuffer framebuffer(renderer, 800, 600);
    Image image;
    image.Load("sky.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 128);

    /*auto tree = std::make_shared<Model>();
    auto stool = std::make_shared<Model>();
    auto teapot = std::make_shared<Model>();
    tree->SetColor  ({ 0, 255, 0, 255 });
    stool->SetColor ({ 139, 69, 19, 255 });
    teapot->SetColor({ 128, 128, 128, 255 });
    tree->Load("Models/tree.obj");
    stool->Load("Models/stool.obj");
    teapot->Load("Models/log.obj");*/

    //shader
    VertexShader::uniforms.view = camera.GetView();
    VertexShader::uniforms.projection = camera.GetProjection();
    VertexShader::uniforms.ambient = color3_t{ 0.01f };

    VertexShader::uniforms.light.position = glm::vec3{ 10, 10, -10 };
    VertexShader::uniforms.light.direction = glm::vec3{ 0, -1, 0 }; // light pointing down
    VertexShader::uniforms.light.color = color3_t{ 1 }; // white light

    Shader::framebuffer = &framebuffer;

    //models
    auto sphere = std::make_shared<Model>();
    sphere->SetColor({ 255, 0, 0, 255 });
    sphere->Load("Models/sphere.obj");

    //actors
    std::vector<std::unique_ptr<Actor>> actors;
    Transform transform{ glm::vec3{ 0 }, glm::vec3{ 0 }, glm::vec3{ 2 } };
    std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, sphere);
    actors.push_back(std::move(actor));

    /*
    for (int i = 0; i < 1; i++) {
        Transform transform1{ {0, -100, 200}, glm::vec3{0, 0, 0}, glm::vec3{2} };
        Transform transform2{ {0, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{40} };
        Transform transform3{ {0, 40, 0}, glm::vec3{0, 0, 0}, glm::vec3{15} };
        auto actor1 = std::make_unique<Actor>(transform1, tree);
        actor1->SetColor({ 0, 255, 0, 255 });
        auto actor2 = std::make_unique<Actor>(transform2, stool);
        actor2->SetColor({ 139, 69, 19, 255 });
        auto actor3 = std::make_unique<Actor>(transform3, teapot);
        actor3->SetColor({ 128, 128, 128, 255 });
        actors.push_back(std::move(actor1));
        actors.push_back(std::move(actor2));
        actors.push_back(std::move(actor3));
    }*/

    /*auto model = std::make_shared<Model>();
    model->Load("teapot.obj");
    model->SetColor({ 0, 255, 0, 255 });

    std::vector<std::unique_ptr<Actor>> actors;
    for (int i = 0; i < 1; i++) {
        Transform transform{ {randomf(-10.0f, 10.0f), 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{80} };
        auto actor = std::make_unique<Actor>(transform, model);
        actor->SetColor({ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256), 255 });
        actors.push_back(std::move(actor));
    }*/

    bool quit = false;
    while (!quit) {
        time.Tick();
        input.Update();

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

        //int x, y;
        //CubicPoint(300, 700, 700, 700, mx, my, 600, 400, t, x, y);
        //framebuffer.DrawRect(x - 20, y - 20, 40, 40, { 0, 255, 0, 255 });

        SetBlendMode(BlendMode::NORMAL);
        framebuffer.DrawImage(0, 0, 800, 600, image);
        //SetBlendMode(BlendMode::MULTIPLY);
        //framebuffer.DrawImage(mx - 100, my - 100, imageAlpha);

        //PostProcess::Invert(framebuffer.m_buffer);
        //PostProcess::Monochrome(framebuffer.m_buffer);
        PostProcess::Brightness(framebuffer.m_buffer, -75);
        //PostProcess::ColorBalance(framebuffer.m_buffer, 100, 100, -10);
        //PostProcess::Noise(framebuffer.m_buffer, 10);
        //PostProcess::Threshold(framebuffer.m_buffer, 150);
        //PostProcess::Posterize(framebuffer.m_buffer, 10);
        //PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 128);
        //PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        
        if (input.GetMouseButtonDown(2)) {
            input.SetRelativeMode(true);

            glm::vec3 direction{ 0 };
            if (input.GetKeyDown(SDL_SCANCODE_D)) direction.x =  0.25f;
            if (input.GetKeyDown(SDL_SCANCODE_A)) direction.x = -0.25f;
            if (input.GetKeyDown(SDL_SCANCODE_E)) direction.y =  0.25f;
            if (input.GetKeyDown(SDL_SCANCODE_Q)) direction.y = -0.25f;
            if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z =  0.25f;
            if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -0.25f;

            camTrans.rotation.y += input.GetMouseRelative().x * 0.25f;
            camTrans.rotation.x += input.GetMouseRelative().y * 0.25f;

            glm::vec3 offset = camTrans.GetMatrix() * glm::vec4{ direction, 0 };

            camTrans.position += offset * 70.0f * time.GetDeltaTime();
        }
        else {
            input.SetRelativeMode(false);
        }
                
        camera.SetView(camTrans.position, camTrans.position + camTrans.GetForward());
        VertexShader::uniforms.view = camera.GetView();

        for (auto& actor : actors)
        {
            actor->GetTransform().rotation.y += time.GetDeltaTime() * 90;
            actor->Draw();
        }

        framebuffer.Update();
        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}