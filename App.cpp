#include <array>
#include <chrono>
#include <iostream>
#include "App.h"
#include "glm/gtc/constants.hpp"

namespace appSpace{
    
    
    std::unique_ptr<graphics::Model> createCircleModel(graphics::Device& device, unsigned int numSides) {
        std::vector<graphics::Model::Vertex> uniqueVertices{};
        for (int i = 0; i < numSides; i++) {
            float angle = i * glm::two_pi<float>() / numSides;
            uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
        }
        uniqueVertices.push_back({});  // adds center vertex at 0, 0
        
        std::vector<graphics::Model::Vertex> vertices{};
        for (int i = 0; i < numSides; i++) {
            vertices.push_back(uniqueVertices[i]);
            vertices.push_back(uniqueVertices[(i + 1) % numSides]);
            vertices.push_back(uniqueVertices[numSides]);
        }
        return std::make_unique<graphics::Model>(device, vertices);
    }
    
    void updateRigidbodies(std::vector<graphics::GameObject>& objects, std::chrono::milliseconds deltaTime){
        for (auto iterator = objects.begin(); iterator != objects.end() ; ++iterator) {
            auto& obj = *iterator;
            glm::vec2 finalVelocity{obj.rigidBody2d.velocity.x, obj.rigidBody2d.velocity.y + App::GRAVITY * deltaTime.count()/1000};
            obj.rigidBody2d.velocity = finalVelocity;
            glm::vec2 translation = obj.transform2d.translation + static_cast<float>(deltaTime.count())/1000 * obj.rigidBody2d.velocity;
            
            if(abs(translation.y)+obj.transform2d.scale.y > 1){
                if(abs(finalVelocity.y) > 0.00001) {
                    obj.rigidBody2d.velocity.y *= -.80;
                    translation = obj.transform2d.translation +
                                  static_cast<float>(deltaTime.count()) / 1000 *
                                  obj.rigidBody2d.velocity;
                }
                else{
                    glm::vec2 newTranslation{translation.x, 1-obj.transform2d.scale.y};
                    obj.rigidBody2d.velocity.y = 0;
                    translation = newTranslation;
                }
            }
            
            if(abs(translation.x)+obj.transform2d.scale.x > 1){
                obj.rigidBody2d.velocity.x *= -.90;
                translation = obj.transform2d.translation + static_cast<float>(deltaTime.count())/1000 * obj.rigidBody2d.velocity;
            }
            
            obj.transform2d.translation = translation;
            obj.rigidBody2d.velocity.x *= 0.9995;
            std::cout << "Velocity:\tX=" << obj.rigidBody2d.velocity.x << "\tY:" <<  obj.rigidBody2d.velocity.y << std::endl;
        }
    }
    
    
    
    App::App() {
        loadGameObjects();
    }
    
    App::~App() {}
    
    void App::run() {
        std::shared_ptr<graphics::Model> circleModel = createCircleModel(device, 64);
        std::vector<graphics::GameObject> physicsObjects{};
        auto red = graphics::GameObject::createGameObject();
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
                updateRigidbodies(physicsObjects, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - prevTime));
                
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