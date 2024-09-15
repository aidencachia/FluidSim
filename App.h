#pragma once

#include <memory>
#include "Graphics/Window.h"
#include "Graphics/Device.h"
#include "Graphics/Renderer.h"
#include "Graphics/Model.h"
#include "Graphics/RenderSystems/FirstRenderSystem.h"
#include "Graphics/GameObject.h"
#include "Graphics/InputController.h"

#define GLM_FORCE_REDIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"


namespace appSpace{
    
    class App{
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 800;
        static constexpr float GRAVITY = 9.81;
        
        App();
        ~App();
        
        App(const App &) = delete;
        App &operator=(const App &) = delete;
        
        void run();
        void reset();
        void pause();
        
        void registerInput();
        void dragObject();
        
    private:
        void loadGameObjects();
        
        graphics::Window window{WIDTH, HEIGHT, "Ball"};
        graphics::Device device{window};
        graphics::Renderer renderer{window, device};
        
        std::vector<graphics::GameObject> gameObjects;
        
        graphics::InputController input{};
        
        int objectIdToDrag;
        glm::vec2 distanceFromCenter;
        
        bool isPaused{};
    };
}