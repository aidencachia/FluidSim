#pragma once

#include <memory>
#include "Device.h"

#define GLM_FORCE_REDIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

namespace graphics {
    class Model {
    public:
        
        struct Vertex{
            glm::vec2 position;
            glm::vec3 color;
            
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        
        Model(Device &device, const std::vector<Vertex> &vertices);
        ~Model();
        
        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;
        
        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
        
        
        static std::unique_ptr<graphics::Model> createCircleModel(graphics::Device& device, unsigned int numSides);
    
    
    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        
        Device& device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}