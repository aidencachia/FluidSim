cmake_minimum_required(VERSION 3.28.1)
project(Fluid_Sim)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FETCHCONTENT_DECLARE(GLFW GIT_REPOSITORY https://github.com/glfw/glfw.git)
FETCHCONTENT_MAKEAVAILABLE(GLFW)

FETCHCONTENT_DECLARE(GLM GIT_REPOSITORY https://github.com/g-truc/glm.git)
FETCHCONTENT_MAKEAVAILABLE(GLM)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    link_directories(${Vulkan_LIBRARY_DIRS})
endif ()

add_executable(Fluid_Sim main.cpp
        Graphics/Window.cpp
        Graphics/Window.h
        App.cpp
        App.h
        Graphics/Pipeline.cpp
        Graphics/Pipeline.h
        Graphics/Device.cpp
        Graphics/Device.h
        Graphics/SwapChain.cpp
        Graphics/SwapChain.h
        Graphics/Model.cpp
        Graphics/Model.h
        Graphics/GameObject.h
        Graphics/Renderer.h
        Graphics/Renderer.cpp
        Graphics/RenderSystems/FirstRenderSystem.cpp
        Graphics/RenderSystems/FirstRenderSystem.h
        Graphics/GameObject.cpp
        Graphics/InputController.cpp
        Graphics/InputController.h)

target_link_libraries(Fluid_Sim glfw glm ${Vulkan_LIBRARIES})