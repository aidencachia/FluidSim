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
        
        graphics::FirstRenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};
        
        
        auto prevTime = std::chrono::system_clock::now();
        while (!window.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = renderer.beginFrame()){
                
                for (graphics::GameObject& obj: gameObjects) {
                    obj.update(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - prevTime));
                }
                
                renderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        
        vkDeviceWaitIdle(device.device());
    }
    
    void App::loadGameObjects() {
        std::shared_ptr<graphics::Model> circleModel = graphics::Model::createCircleModel(device, 64);
        
        auto red = graphics::GameObject::createRigidbodyObject();
        red.transform2d.scale = glm::vec2{.2};
        red.transform2d.translation = {.0, -.5};
        red.color = {1.f, 0.f, 0.f};
        red.rigidBody2d.velocity = {.001f, .0f};
        red.model = circleModel;
        gameObjects.push_back(std::move(red));
    }
    
}