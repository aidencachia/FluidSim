#pragma once

#include <memory>
#include "Graphics/Window.h"
#include "Graphics/Device.h"
#include "Graphics/Renderer.h"
#include "Graphics/Model.h"
#include "Graphics/RenderSystems/FirstRenderSystem.h"
#include "Graphics/GameObject.h"

#define GLM_FORCE_REDIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"


namespace appSpace{
    
    class App{
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 800;
        static constexpr int GRAVITY = 100;
        
        App();
        ~App();
        
        App(const App &) = delete;
        App &operator=(const App &) = delete;
        
        void run();
    private:
        void loadGameObjects();
        
        graphics::Window window{WIDTH, HEIGHT, "HIIII"};
        graphics::Device device{window};
        graphics::Renderer renderer{window, device};
        
        std::vector<graphics::GameObject> gameObjects;
    };
}