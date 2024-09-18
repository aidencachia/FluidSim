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
                    for (GameObject &obj: gameObjects) {
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
        
        auto ball = GameObject::createRigidbodyObject();
        ball.transform2d.scale = glm::vec2{.15};
        ball.transform2d.translation = {.0, -.5};
        ball.color = {1.f, 0.f, 0.f};
        ball.rigidBody2d.velocity = {.001f, .0f};
        ball.model = circleModel;
        ball.isInBoundingAreaFunc = [](GameObject &gameObject, glm::vec2 coords){
            float r = gameObject.transform2d.scale.x;
            
            float diffInX = coords.x - gameObject.transform2d.translation.x;
            float diffInY = coords.y - gameObject.transform2d.translation.y;
            
            return diffInX*diffInX + diffInY*diffInY <= r*r;
        };
        ball.transform2d.translation.y = -0.5f;
        gameObjects.push_back(std::move(ball));

        auto line = GameObject::createLine(device, ball, {0, 0}, 1);
        line.color = {1.f, 1.f, 1.f};

        gameObjects.push_back(std::move(line));
        
    }
    
    void App::reset() {
        vkDeviceWaitIdle(device.device());
        gameObjects.clear();
        loadGameObjects();
    }
    
    void App::pause() {
        isPaused = !isPaused;
    }
    
    void App::dragObject() {
        glm::vec2 currentCursorPos = window.getCursorPos();
        if(input.isToStartDragging()){
            bool found = false;
            for (int i = 0; i < gameObjects.size() && !found; i++){
                
                if(gameObjects[i].isInBoundingArea(currentCursorPos)){
                    found = true;
                    objectIdToDrag = i;
                    
                    float deltaX = currentCursorPos.x - (gameObjects[i].transform2d.translation.x+gameObjects[i].transform2d.scale.x);
                    float deltaY = currentCursorPos.y - (gameObjects[i].transform2d.translation.y+gameObjects[i].transform2d.scale.y);
                    
                    distanceFromCenter = {deltaX, deltaY};
                }
            }
            
            if(!found){
                objectIdToDrag = -1;
            }
        }
        if(objectIdToDrag != -1){
            float deltaX = currentCursorPos.x - (gameObjects[objectIdToDrag].transform2d.translation.x+gameObjects[objectIdToDrag].transform2d.scale.x);
            float deltaY = currentCursorPos.y - (gameObjects[objectIdToDrag].transform2d.translation.y+gameObjects[objectIdToDrag].transform2d.scale.y);
            
            glm::vec2 newDistance{deltaX, deltaY};
            
            gameObjects[objectIdToDrag].rigidBody2d.velocity += (newDistance-distanceFromCenter)*glm::vec2{0.1};
        }
    }
    
    void App::registerInput(){
        input.update(window.getGLFWwindow());
        if(input.isToReset()) reset();
        if(input.isToTogglePause()) pause();
        if(input.isToDrag()) dragObject();
    }
}