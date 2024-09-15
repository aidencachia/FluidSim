#include <stdexcept>
#include "Window.h"
namespace graphics{
    
    Window::Window(int w, int h, std::string name): width(w), height(h), windowName(name) {
        initWindow();
    }
    
    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    void Window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr,
                                  nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
    
    void Window::createWindowSurface(VkInstance instance,
                                     VkSurfaceKHR *surface) {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }
    
    void Window::framebufferResizeCallback(GLFWwindow *window, int width,
                                                  int height) {
        auto windowobj = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowobj->framebufferResized= true;
        windowobj->width = width;
        windowobj->height = height;
    }
    
    glm::vec2 Window::getCursorPos() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {x/width*2-1, y/height*2-1};
    }
    
}