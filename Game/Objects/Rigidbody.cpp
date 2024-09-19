#include "Rigidbody.h"
#include "../App.h"

namespace FluidSim{
    void Rigidbody::update(float deltaTimeSecs){
        
        glm::vec2 finalVelocity{rigidBody2d.velocity.x, rigidBody2d.velocity.y + FluidSim::App::GRAVITY * deltaTimeSecs};
        rigidBody2d.velocity = finalVelocity;
        glm::vec2 translation = transform2d.translation + static_cast<float>(deltaTimeSecs) * rigidBody2d.velocity;
        
        if(abs(translation.y)+transform2d.scale.y > 1){
            if(abs(finalVelocity.y) > 0.00001) {
                rigidBody2d.velocity.y *= -.80;
                if(finalVelocity.y > 0) translation.y = 1 - transform2d.scale.y;
                else translation.y = - 1 + transform2d.scale.y;
            }
            else{
                glm::vec2 newTranslation{translation.x, 1-transform2d.scale.y};
                rigidBody2d.velocity.y = 0;
                translation = newTranslation;
            }
        }
        
        if(abs(translation.x)+transform2d.scale.x > 1){
            rigidBody2d.velocity.x *= -.90;
            translation = transform2d.translation + static_cast<float>(deltaTimeSecs) * rigidBody2d.velocity;
        }
        
        transform2d.translation = translation;
        rigidBody2d.velocity.x *= 0.9995;
    }
    
};