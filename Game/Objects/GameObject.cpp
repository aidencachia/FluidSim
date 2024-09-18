#include "GameObject.h"
#include "../App.h"

namespace FluidSim {
    
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
    
//
//    GameObject GameObject::createLine(graphics::Device& device, glm::vec2 to, glm::vec2 from,
//                                                            float stroke) {
//        GameObject line = createGameObject();
//        line.model = graphics::Model::createRectangleModel(device);
//        line.transform2d.scale.y = 0.0025f * stroke;
//
//        updateLine(line, to, from);
//
//        return line;
//    }
//
//    GameObject GameObject::createLine(graphics::Device &device, glm::vec2 to,
//                                                            GameObject &from, float stroke) {
//        GameObject line = createLine(device, to, from.transform2d.translation+from.transform2d.scale, stroke);
//        line.tracking1 = &from;
//        line.updateBehaviour = [to](GameObject& gameObject, float deltaTimeSecs){
//            return updateLine(gameObject, to, gameObject.tracking1->transform2d.translation+gameObject.tracking1->transform2d.translation);
//        };
//        return line;
//    }
//
//    GameObject GameObject::createLine(graphics::Device &device, GameObject &to, glm::vec2 from, float stroke) {
//        return GameObject::createLine(device, from, to, stroke);
//    }

}