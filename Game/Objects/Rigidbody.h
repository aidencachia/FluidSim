#pragma once

#include <memory>
#include <chrono>
#include <utility>
#include "glm/vec3.hpp"
#include "../../Graphics/Model.h"
#include "GameObject.h"

namespace FluidSim {
    
    struct RigidBody2dComponent {
        glm::vec2 velocity;
        float mass{1.0f};
    };
    
    class Rigidbody: public GameObject{
    public:
        using UpdateBehaviour = std::function<void(Rigidbody&, float)>;
        
        static RigidBody2dComponent createRigidbodyObject();
        
        Rigidbody(const Rigidbody&) = delete;
        Rigidbody &operator=(const Rigidbody&) = delete;
        Rigidbody(Rigidbody &&) = default;
        Rigidbody &operator=(Rigidbody &&) =default;
        
        bool isInBoundingArea(glm::vec2 coords){ return isInBoundingAreaFunc(*this, coords); }
        void update(float deltaTimeSecs);
        
        std::shared_ptr<graphics::Model> model{};
        glm::vec3 color{};
        std::function<bool(Rigidbody&, glm::vec2)> isInBoundingAreaFunc;
        RigidBody2dComponent rigidBody2d{};
    
    private:
        Rigidbody(UpdateBehaviour updateBehaviour):
                updateBehaviour{std::move(updateBehaviour)} {}
        
        UpdateBehaviour updateBehaviour;
    };
}
