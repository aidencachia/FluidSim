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
        
        
        auto prevTime = std::chrono::high_resolution_clock ::now();
        while (!window.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = renderer.beginFrame()){
                
                registerInput(); // placed here because of pause function;
                
                auto nextTime = std::chrono::high_resolution_clock::now();
                
                float deltaTimeSecs = std::chrono::duration<float, std::chrono::seconds::period>(nextTime- prevTime).count();
                prevTime = nextTime;
                if(!isPaused) {
                    for (graphics::GameObject &obj: gameObjects) {
                        obj.update(deltaTimeSecs);
                    }
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
    
    void App::reset() {
        vkDeviceWaitIdle(device.device());
        gameObjects.clear();
        loadGameObjects();
    }
    
    void App::pause() {
        isPaused = !isPaused;
    }
    
    void App::registerInput(){
        input.update(window.getGLFWwindow());
        if(input.isToReset()) reset();
        if(input.isToTogglePause()) pause();
    }
    
}