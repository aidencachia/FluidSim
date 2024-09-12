#include <array>
#include "App.h"
#include "glm/gtc/constants.hpp"

namespace appSpace{
    
    App::App() {
        loadGameObjects();
    }
    
    App::~App() {}
    
    void App::run() {
        graphics::FirstRenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};
        
        while (!window.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = renderer.beginFrame()){
                renderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, gameObjects);
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