#ifndef FLUID_SIM_WINDOW_H
#define FLUID_SIM_WINDOW_H

#define GLFW_INCLUDE_VULKAN

#include <string>
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

namespace graphics{

class Window {
public:
    Window(int w, int h, std::string name);
    ~Window();
    
    bool shouldClose(){ return glfwWindowShouldClose(window);};
    VkExtent2D getExtent(){ return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    bool wasWindowResized() { return framebufferResized; }
    
    
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    
    void resetWindowResizedFlag() { framebufferResized = false; }
    
    glm::vec2 getCursorPos();
    
    GLFWwindow* getGLFWwindow(){ return window; }
    
    float getAspectRatio() const{ return ((float) width)/height; }

private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();
    
    int width;
    int height;
    bool framebufferResized;
    
    std::string windowName;
    
    GLFWwindow *window;
};

}


#endif //FLUID_SIM_WINDOW_H
