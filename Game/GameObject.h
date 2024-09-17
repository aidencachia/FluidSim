#pragma once

#include <memory>
#include <chrono>
#include <utility>
#include "glm/vec3.hpp"
#include "../Graphics/Model.h"
namespace appSpace {

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

    struct RigidBody2dComponent {
        glm::vec2 velocity;
        float mass{1.0f};
    };

    class GameObject{
    public:
        using id_t = unsigned int;
        using UpdateBehaviour = std::function<void(GameObject&, float)>;

        static GameObject createGameObject();
        static GameObject createRigidbodyObject();

        static GameObject createLine(graphics::Device& device, glm::vec2 to, glm::vec2 from, float stroke);
        static GameObject createLine(graphics::Device& device, GameObject& to, glm::vec2 from, float stroke);
        static GameObject createLine(graphics::Device& device, glm::vec2 to, GameObject& from, float stroke);
        static GameObject createLine(graphics::Device& device, GameObject& to, GameObject& from, float stroke);


        GameObject(const GameObject&) = delete;
        GameObject &operator=(const GameObject&) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) =default;

        id_t getId() { return id; }

        std::shared_ptr<graphics::Model> model{};
        glm::vec3 color{};
        std::function<bool(GameObject&, glm::vec2)> isInBoundingAreaFunc;

        bool isInBoundingArea(glm::vec2 coords){ return isInBoundingAreaFunc(*this, coords); }

        void update(float deltaTimeSecs){ updateBehaviour(*this, deltaTimeSecs); };

        Transform2dComponent transform2d{};
        RigidBody2dComponent rigidBody2d{};

    private:
        GameObject(id_t objId, UpdateBehaviour updateBehaviour):
            id{objId},
            updateBehaviour{std::move(updateBehaviour)} {}

        UpdateBehaviour updateBehaviour;
        id_t id;

        GameObject* tracking1;
        GameObject* tracking2;
    };
}
