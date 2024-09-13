#include <array>
#include <chrono>
#include <iostream>
#include "App.h"
#include "glm/gtc/constants.hpp"

namespace appSpace{
    
    App::App() {
        loadGameObjects();
    }
    
    App::~App() {}
    
    void App::run() {
        std::shared_ptr<graphics::Model> circleModel = graphics::Model::createCircleModel(device, 64);
        std::vector<graphics::GameObject> physicsObjects{};
        auto red = graphics::GameObject::createRigidbodyObject();
        red.transform2d.scale = glm::vec2{.2};
        red.transform2d.translation = {.0, -.5};
        red.color = {1.f, 0.f, 0.f};
        red.rigidBody2d.velocity = {.001f, .0f};
        red.model = circleModel;
        physicsObjects.push_back(std::move(red));
        
        graphics::FirstRenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};
        
        
        auto prevTime = std::chrono::system_clock::now();
        while (!window.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = renderer.beginFrame()){
                
                for (graphics::GameObject& obj: physicsObjects) {
                    obj.update(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - prevTime));
                }
                
                renderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, physicsObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        
        vkDeviceWaitIdle(device.device());
    }
    
    void App::loadGameObjects() {
        std::vector<graphics::Model::Vertex> vertices{
                {{.0,  -.5}, {1.0, .0,  .0}},
                {{.5,  .5},  {.0,  1.0, .0}},
                {{-.5, .5},  {.0,  .0,  1.0}}};

        auto model = std::make_shared<graphics::Model>(device, vertices);
        
        auto triangle = graphics::GameObject::createGameObject();
        triangle.model = model;
        triangle.color = {.1, .8, .1};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();
        
        gameObjects.push_back(std::move(triangle));
    }
    
}