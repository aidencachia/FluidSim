#pragma once

#include <memory>
#include <chrono>
#include <utility>
#include "glm/vec3.hpp"
#include "../../Graphics/Model.h"
namespace FluidSim {

    struct Transform2dComponent{
        glm::vec2 translation{};
        glm::vec2 scale{1., 1.};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotationMat{{c,s},{-s, c}};
            glm::mat2 scaleMat{{scale.x, .0}, {0.f, scale.y}};
            return rotationMat * scaleMat;
        }
    };

    class GameObject{
    public:
        using UpdateBehaviour = std::function<void(GameObject&, float)>;

        GameObject(const GameObject&) = delete;
        GameObject &operator=(const GameObject&) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) =default;

        std::shared_ptr<graphics::Model> model{};
        glm::vec3 color{};

        Transform2dComponent transform2d{};
        
    protected:
        GameObject() = default;
    };
}
