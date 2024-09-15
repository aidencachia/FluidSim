#include "GameObject.h"
#include "../App.h"
namespace graphics {
    
    
    void updateLine(GameObject& line, glm::vec2 to, glm::vec2 from){
        float deltaX = from.x - to.x;
        float deltaY = from.y - to.y;
        float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        
        line.transform2d.scale.x = distance/2;
        
        float midpointX = (to.x + from.x)/2;
        float midpointY = (to.y + from.y)/2;
        
        line.transform2d.translation = {midpointX, midpointY};
        
        float theta = atan2(deltaY, deltaX);
        
        line.transform2d.rotation = theta;
    }
    
    void updateRigidbody(GameObject obj, float deltaTimeMillis){
    }
    
    GameObject GameObject::createGameObject(){
        static id_t currentId = 0;
        GameObject obj{currentId++, [](GameObject& obj, float) { }};
        obj.isInBoundingAreaFunc = [](GameObject&, glm::vec2){
            return false;
        };
        return obj;
    }
    
    GameObject GameObject::createRigidbodyObject(){
        static id_t currentId = 0;
        return {currentId++, [](GameObject& obj, float deltaTimeSecs) {
            
            glm::vec2 finalVelocity{obj.rigidBody2d.velocity.x, obj.rigidBody2d.velocity.y + appSpace::App::GRAVITY * deltaTimeSecs};
            obj.rigidBody2d.velocity = finalVelocity;
            glm::vec2 translation = obj.transform2d.translation + static_cast<float>(deltaTimeSecs) * obj.rigidBody2d.velocity;
            
            if(abs(translation.y)+obj.transform2d.scale.y > 1){
                if(abs(finalVelocity.y) > 0.00001) {
                    obj.rigidBody2d.velocity.y *= -.80;
                    if(finalVelocity.y > 0) translation.y = 1 - obj.transform2d.scale.y;
                    else translation.y = - 1 + obj.transform2d.scale.y;
                }
                else{
                    glm::vec2 newTranslation{translation.x, 1-obj.transform2d.scale.y};
                    obj.rigidBody2d.velocity.y = 0;
                    translation = newTranslation;
                }
            }
            
            if(abs(translation.x)+obj.transform2d.scale.x > 1){
                obj.rigidBody2d.velocity.x *= -.90;
                translation = obj.transform2d.translation + static_cast<float>(deltaTimeSecs) * obj.rigidBody2d.velocity;
            }
            
            
            
            obj.transform2d.translation = translation;
            obj.rigidBody2d.velocity.x *= 0.9995;
        }};
    }
    
    GameObject GameObject::createLine(Device& device, glm::vec2 to, glm::vec2 from,
                                      float stroke) {
        GameObject line = createGameObject();
        line.model = Model::createRectangleModel(device);
        line.transform2d.scale.y = 0.0025f * stroke;
        
        updateLine(line, to, from);
        
        return line;
    }
    
    GameObject GameObject::createLine(Device &device, glm::vec2 to,
                                      GameObject &from, float stroke) {
        GameObject line = createLine(device, to, from.transform2d.translation+from.transform2d.scale, stroke);
        line.tracking1 = &from;
        line.updateBehaviour = [to](GameObject& gameObject, float deltaTimeSecs){
            return updateLine(gameObject, to, gameObject.tracking1->transform2d.translation+gameObject.tracking1->transform2d.translation);
        };
        return line;
    }
    
    GameObject GameObject::createLine(graphics::Device &device,
                                      graphics::GameObject &to, glm::vec2 from,
                                      float stroke) {
        return GameObject::createLine(device, from, to, stroke);
    }

}