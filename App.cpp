#include <array>
#include "App.h"
#include "glm/gtc/constants.hpp"

namespace appSpace{
    
    App::App() {
        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }
    
    App::~App() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }
    
    void App::run() {
        while (!window.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }
        
        vkDeviceWaitIdle(device.device());
    }
    
    void App::loadGameObjects() {
        std::vector<graphics::Model::Vertex> vertices{
                {{.0,  -.5}, {1.0, .0,  .0}},
                {{.5,  .5},  {.0,  1.0, .0}},
                {{-.5, .5},  {.0,  .0,  1.0}}};
//        addSquareFromTLCorner(vertices, 0.6, {-0.3f, -0.3f});

        auto model = std::make_shared<graphics::Model>(device, vertices);
        
        auto triangle = graphics::GameObject::createGameObject();
        triangle.model = model;
        triangle.color = {.1, .8, .1};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();
        
        
        gameObjects.push_back(std::move(triangle));
    }
    
    void App::createPipelineLayout() {
        
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
    
    void App::recreateSwapChain() {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0){
            extent = window.getExtent();
            glfwWaitEvents();
        }
        
        vkDeviceWaitIdle(device.device());
        if(swapChain = nullptr){
            swapChain = std::make_unique<graphics::SwapChain>(device, extent);
        } else {
            swapChain = std::make_unique<graphics::SwapChain>(device, extent, std::move(swapChain));
            if(swapChain->imageCount() != commandBuffers.size()){
                if(commandBuffers.size() > 0)
                    freeCommandBuffers();
                createCommandBuffers();
            }
        }
        createPipeline();
    }
    
    void App::createPipeline() {
        assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        
        graphics::PipelineConfigInfo pipelineConfig{};
        graphics::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = swapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<graphics::Pipeline>(device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
        
    }
    
    void App::createCommandBuffers() {
        commandBuffers.resize(swapChain->imageCount());
        
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        
        if(vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers!");
        }
        
    }
    
    void App::freeCommandBuffers() {
        vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
 
    void App::recordCommandBuffer(int imageIndex) {
        static int frame = 0;
        
        frame = (frame+1) % 1000;
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        
        if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);
        
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();
        
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);
        
        renderGameObjects(commandBuffers[imageIndex]);
        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
            throw std::runtime_error("failed to record command buffer");
        }
    }
    
    void App::renderGameObjects(VkCommandBuffer commandBuffer) {
        pipeline->bind(commandBuffer);
        
        for(auto& obj: gameObjects) {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.001f, glm::two_pi<float>());
            
            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();
            
            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT |VK_SHADER_STAGE_FRAGMENT_BIT, 0 ,sizeof(SimplePushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
    
    void App::drawFrame() {
        uint32_t imageIndex;
        auto result = swapChain->acquireNextImage(&imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return;
        }
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image");
        }
        
        recordCommandBuffer(imageIndex);
        result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()){
            window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS){
            throw std::runtime_error("failed to present swap chain image");
        }
        
    }
    
    void App::addSquareFromTLCorner(
            std::vector<graphics::Model::Vertex> &vertices,
            float length,
            std::vector<float> corner) {
        float aspectRatio = swapChain == nullptr ? WIDTH/HEIGHT :(float) swapChain->width()/swapChain->height();
        
        float x = corner[0];
        float y = corner[1];
        float xD = corner[0] + length;
        float yD = corner[1] + length*aspectRatio;
        
        // Triangle 1
        vertices.push_back({{x, y}, {1.0, 0.0 , 0.0}});
        vertices.push_back({{xD, y}, { 0.0 , 1.0 , 0.0 }});
        vertices.push_back({{xD,yD}, {0.0 , 0.0, 1.0}});
        
        //Triangle 2
        vertices.push_back({{x, y}, {1.0 , 0.0, 0.0}});
        vertices.push_back({{x, yD}, {1.0, 1.0, 0.0}});
        vertices.push_back({{xD, yD}, {0.0, 0.0, 1.0}});
    }
}