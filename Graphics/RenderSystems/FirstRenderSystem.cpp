#include <array>
#include "FirstRenderSystem.h"
#include "glm/gtc/constants.hpp"

namespace graphics{
    
    FirstRenderSystem::FirstRenderSystem(Device& device, VkRenderPass renderPass): device(device) {
        createPipelineLayout();
        createPipeline(renderPass);
    }
    
    FirstRenderSystem::~FirstRenderSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }
    
    void FirstRenderSystem::createPipelineLayout() {
        
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        
        if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
            throw  std::runtime_error("failed to create pipeline layout!");
        }
    }
    
    void FirstRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        
        graphics::PipelineConfigInfo pipelineConfig{};
        graphics::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<graphics::Pipeline>(device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
        
    }
    
    void FirstRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<FluidSim::GameObject>& gameObjects) {
        pipeline->bind(commandBuffer);
        
        for(auto& obj: gameObjects) {
            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            
            glm::mat2 aspectRatioTransform{{1/device.getAspectRatio(),0},{0,1}};
            
            push.transform = obj.transform2d.mat2()*aspectRatioTransform;
            
            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT |VK_SHADER_STAGE_FRAGMENT_BIT, 0 ,sizeof(SimplePushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
    
}