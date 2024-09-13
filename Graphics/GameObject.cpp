#include "GameObject.h"
#include "../App.h"
namespace graphics {
    
    void updateRigidbody(GameObject obj, float deltaTimeMillis){
    }
    
    GameObject GameObject::createGameObject(){
        static id_t currentId = 0;
        return GameObject(currentId++, [](GameObject& obj, float) { });
    }
    
    GameObject GameObject::createRigidbodyObject(){
        static id_t currentId = 0;
        return GameObject(currentId++, [](GameObject& obj, float deltaTimeMillis) {
            
            glm::vec2 finalVelocity{obj.rigidBody2d.velocity.x, obj.rigidBody2d.velocity.y + appSpace::App::GRAVITY * deltaTimeMillis/1000};
            obj.rigidBody2d.velocity = finalVelocity;
            glm::vec2 translation = obj.transform2d.translation + static_cast<float>(deltaTimeMillis)/1000 * obj.rigidBody2d.velocity;
            
            if(abs(translation.y)+obj.transform2d.scale.y > 1){
                if(abs(finalVelocity.y) > 0.00001) {
                    obj.rigidBody2d.velocity.y *= -.80;
                    translation.y = 1 - obj.transform2d.scale.y;
                }
                else{
                    glm::vec2 newTranslation{translation.x, 1-obj.transform2d.scale.y};
                    obj.rigidBody2d.velocity.y = 0;
                    translation = newTranslation;
                }
            }
            
            if(abs(translation.x)+obj.transform2d.scale.x > 1){
                obj.rigidBody2d.velocity.x *= -.90;
                translation = obj.transform2d.translation + static_cast<float>(deltaTimeMillis)/1000 * obj.rigidBody2d.velocity;
            }
            
            obj.transform2d.translation = translation;
            obj.rigidBody2d.velocity.x *= 0.9995;
        });
    }

}