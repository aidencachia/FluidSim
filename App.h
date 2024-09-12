#pragma once

#include <memory>
#include "Graphics/Window.h"
#include "Graphics/Pipeline.h"
#include "Graphics/Device.h"
#include "Graphics/SwapChain.h"
#include "Graphics/Model.h"


#define GLM_FORCE_REDIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "Graphics/GameObject.h"


namespace appSpace{
    
    struct SimplePushConstantData{
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    
    class App{
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
        App();
        ~App();
        
        App(const App &) = delete;
        App &operator=(const App &) = delete;
        
        void run();
    private:
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderGameObjects(VkCommandBuffer commandBuffer);
        
        graphics::Window window{WIDTH, HEIGHT, "HIIII"};
        graphics::Device device{window};
        std::unique_ptr<graphics::SwapChain> swapChain;
        std::unique_ptr<graphics::Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<graphics::GameObject> gameObjects;
        
        void addSquareFromTLCorner(std::vector<graphics::Model::Vertex> &vertices, float length, std::vector<float> corner);
    };
}