#include <array>
#include "App.h"

namespace appSpace{
    
    App::App() {
        loadModels();
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
    
    void App::loadModels() {
        std::vector<graphics::Model::Vertex> vertices {};
        addSquareFromTLCorner(vertices, 0.6, {-0.3f, -0.3f});
        model = std::make_unique<graphics::Model>(device, vertices);
    }
    
    void App::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        
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
        swapChain = nullptr;l
        swapChain = std::make_unique<graphics::SwapChain>(device, extent);
        createPipeline();
    }
    
    void App::createPipeline() {
        auto pipelineConfig = graphics::Pipeline::defaultPipelineConfigInfo(swapChain->width(), swapChain->height());
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
 
    void App::recordCommandBuffer(int imageIndex) {
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
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        pipeline->bind(commandBuffers[imageIndex]);
        model->bind(commandBuffers[imageIndex]);
        model->draw(commandBuffers[imageIndex]);
        
        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
            throw std::runtime_error("failed to record command buffer");
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
        float aspectRatio =(float) WIDTH/HEIGHT;
        
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