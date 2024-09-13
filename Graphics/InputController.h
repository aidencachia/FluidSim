#pragma once

#include "GameObject.h"
#include "Window.h"

namespace graphics{
    class InputController{
    public:
        
        InputController() = default;
        
        struct KeyMappings{
            int reset = GLFW_KEY_R;
            int pause = GLFW_KEY_ESCAPE;
            int drag = GLFW_MOUSE_BUTTON_1;
        };
        
        KeyMappings keys{};
        
        bool isToReset(){
            bool flag = reset && !resetPrev;
            resetPrev = reset;
            return flag;
        }
        bool isToTogglePause(){
            bool flag = togglePause && !togglePausePrev;
            togglePausePrev = togglePause;
            return flag;
        }
        
        void update(GLFWwindow* window);
        
        
    private:
        void checkTogglePause(GLFWwindow* window);
        void checkReset(GLFWwindow* window);
        void checkDrag(GLFWwindow* window);
        
        bool togglePause{};
        bool togglePausePrev{};
        
        bool reset{};
        bool resetPrev{};
        
        bool drag{};
        bool dragPrev{};
    };
}