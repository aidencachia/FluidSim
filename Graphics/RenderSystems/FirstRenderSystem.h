#pragma once

#include <memory>
#include "../Pipeline.h"
#include "../Device.h"
#include "../Model.h"
#include "../../Game/Objects/GameObject.h"

#define GLM_FORCE_REDIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "../../Game/GameField.h"


namespace graphics{
    
    struct SimplePushConstantData{
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    
    class FirstRenderSystem{
    
    public:
        
        FirstRenderSystem(Device& device, VkRenderPass renderPass);
        ~FirstRenderSystem();
        
        FirstRenderSystem(const FirstRenderSystem &) = delete;
        FirstRenderSystem &operator=(const FirstRenderSystem &) = delete;
        
        void renderGameField(VkCommandBuffer commandBuffer, FluidSim::GameField& gameField);
        
        void run();
    private:
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
        
        Device& device;
        
        std::unique_ptr<graphics::Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}