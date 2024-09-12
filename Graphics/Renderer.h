#pragma once

#include <memory>
#include <cassert>
#include "Window.h"
#include "Device.h"
#include "SwapChain.h"

namespace graphics{
    
    class Renderer{
    
    public:
        Renderer(Window& window, Device& device);
        ~Renderer();
        
        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;
        
        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
        
        bool isFrameInProgress() const { return isFrameStarted; }
        VkCommandBuffer getCurrentCommandBuffer() const{
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }
        VkRenderPass getSwapChainRenderPass() const {return swapChain->getRenderPass(); }
        
        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }
        
    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();
        
        Window& window;
        Device& device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers;
        
        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted;
    };
}