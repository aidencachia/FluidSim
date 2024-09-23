#include <array>
#include <chrono>
#include <iostream>
#include "App.h"
#include "glm/gtc/constants.hpp"
#include "Objects/GameObject.h"

namespace FluidSim{
    
    App::App() {
        loadGameObjects();
    }
    
    App::~App() {}
    
    void App::run() {
        
        graphics::FirstRenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};

        auto prevTime = std::chrono::high_resolution_clock::now();
        while (!window.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = renderer.beginFrame()){
                registerInput();
                
                auto nextTime = std::chrono::high_resolution_clock::now();
                float deltaTimeSecs = std::chrono::duration<float, std::chrono::seconds::period>(nextTime- prevTime).count();
                prevTime = nextTime;
                
                if(!isPaused) {
                    gameField.update(deltaTimeSecs);
                }
                renderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameField(commandBuffer, gameField);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        
        vkDeviceWaitIdle(device.device());
    }
    
    void App::loadGameObjects() {
//        gameField.addCircle(
//                .15,
//                {.0, -.5},
//                {1.f, 0.f, 0.f});

        gameField.createCircleGrid(20, 0.1, 0.02, {0.3, .08, 1});
    }
    
    void App::reset() {
        vkDeviceWaitIdle(device.device());
        gameField.clear();
        loadGameObjects();
    }
    
    void App::pause() {
        isPaused = !isPaused;
    }
    
    void App::registerInput(){
        input.update(window.getGLFWwindow());
        if(input.isToReset()) reset();
        if(input.isToTogglePause()) pause();
        if(input.isToDrag()) gameField.dragObject(window.getCursorPos(), input.isToStartDragging());
    }
}