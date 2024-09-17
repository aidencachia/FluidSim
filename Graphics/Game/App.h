#pragma once

#include <memory>
#include "../Window.h"
#include "../Device.h"
#include "../Renderer.h"
#include "../Model.h"
#include "../RenderSystems/FirstRenderSystem.h"
#include "GameObject.h"
#include "../InputController.h"

#define GLM_FORCE_REDIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"


namespace appSpace{
    
    class App{
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 800;
        static constexpr int GRAVITY = 10;
        
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
        
        std::vector<graphics::gameSpace::GameObject> gameObjects;
        
        graphics::InputController input{};
        
        int objectIdToDrag;
        glm::vec2 distanceFromCenter;
        
        bool isPaused{};
    };
}