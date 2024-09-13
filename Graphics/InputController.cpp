#include "InputController.h"

namespace graphics{
    void InputController::checkReset(GLFWwindow *window) {
        if(glfwGetKey(window, keys.reset) == GLFW_PRESS) reset = true;
        else reset = false;
    }
    
    void InputController::checkTogglePause(GLFWwindow *window) {
        if(glfwGetKey(window, keys.pause) == GLFW_PRESS) togglePause = true;
        else togglePause = false;
    }
    
    void InputController::checkDrag(GLFWwindow *window) {
        if(glfwGetKey(window, keys.drag) == GLFW_PRESS) drag = true;
        else drag = false;
    }
    
    void InputController::update(GLFWwindow *window) {
        checkTogglePause(window);
        checkReset(window);
        checkDrag(window);
    }
}
