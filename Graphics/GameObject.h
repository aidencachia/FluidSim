#pragma once

#include <memory>
#include <chrono>
#include "glm/vec3.hpp"
#include "Model.h"

namespace graphics {
    
    
    struct Transform2dComponent{
        glm::vec2 translation{};
        glm::vec2 scale{1., 1.};
        float rotation;
        
        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotationMat{{c,s},{-s, c}};
            glm::mat2 scaleMat{{scale.x, .0}, {0.f, scale.y}};
            return rotationMat* scaleMat;
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
        
        GameObject(const GameObject&) = delete;
        GameObject &operator=(const GameObject&) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) =default;
        
        id_t getId() { return id; }
        
        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        
        void update(std::chrono::milliseconds deltaTime){ updateBehaviour(*this, deltaTime.count()); };
        
        Transform2dComponent transform2d{};
        RigidBody2dComponent rigidBody2d{};
        
    private:
        GameObject(id_t objId, UpdateBehaviour updateBehaviour):id{objId}, updateBehaviour{updateBehaviour} {}
        UpdateBehaviour updateBehaviour;
        id_t id;
    };
    
}