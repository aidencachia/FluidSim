#include "GameField.h"

namespace FluidSim{
    void GameField::createCircleGrid(int size, float elementSize, float padding, glm::vec3 color) {
        int width = ceil(sqrt(size));
        int height = size/width+1;
        int extra = size%width;

        glm::vec2 basePosition{
                (elementSize*(width-1) + padding * (width -2))*-1,
                elementSize*(height-1)+ padding * (height-2)
        };

        for (int i = 0; i < height-1; ++i) {
            for (int j = 0; j < width; ++j) {
                glm::vec2 position{
                        basePosition.x+(padding+elementSize*2)*j,
                        basePosition.y+(padding+elementSize*2)*i*-1};

                addCircle(elementSize, position, color);
            }
        }

        for (int i = 0; i < extra; ++i) {
            glm::vec2 position{
                    basePosition.x+(padding+elementSize*2)*i,
                    basePosition.y+(padding+elementSize*2)*(height-1)*-1};

            addCircle(elementSize, position, color);
        }
    }

    void GameField::addCircle(float size, glm::vec2 position, glm::vec3 color) {
        std::shared_ptr<graphics::Model> circleModel = graphics::Model::createCircleModel(gameDevice, 64);

        Rigidbody ball{};
        ball.transform2d.scale = glm::vec2{size};
        ball.transform2d.translation = position;
        ball.color = color;
        ball.rigidBody2d.velocity = {.001f, .0f};
        ball.model = circleModel;
        ball.isInBoundingAreaFunc = [](GameObject &gameObject, glm::vec2 coords){
            float r = gameObject.transform2d.scale.x;

            float diffInX = coords.x - gameObject.transform2d.translation.x;
            float diffInY = coords.y - gameObject.transform2d.translation.y;

            return diffInX*diffInX + diffInY*diffInY <= r*r;
        };
        rigidbodies.push_back(std::move(ball));
    }

    void GameField::clear() {
        rigidbodies.clear();
        objects.clear();
    }

    std::vector<GameObject>& GameField::getObjects() {
        return objects;
    }

    std::vector<Rigidbody>& GameField::getRigidbodies() {
        return rigidbodies;
    }


    void GameField::dragObject(glm::vec2 cursorPos, bool startDragging) {
        if(startDragging){
            bool found = false;
            for (int i = 0; i < rigidbodies.size() && !found; i++){

                if(rigidbodies[i].isInBoundingArea(cursorPos)){
                    found = true;
                    objectIdToDrag = i;

                    float deltaX = cursorPos.x - (rigidbodies[i].transform2d.translation.x+rigidbodies[i].transform2d.scale.x);
                    float deltaY = cursorPos.y - (rigidbodies[i].transform2d.translation.y+rigidbodies[i].transform2d.scale.y);

                    distanceFromCenter = {deltaX, deltaY};
                }
            }

            if(!found){
                objectIdToDrag = -1;
            }
        }
        if(objectIdToDrag != -1){
            float deltaX = cursorPos.x - (rigidbodies[objectIdToDrag].transform2d.translation.x+rigidbodies[objectIdToDrag].transform2d.scale.x);
            float deltaY = cursorPos.y - (rigidbodies[objectIdToDrag].transform2d.translation.y+rigidbodies[objectIdToDrag].transform2d.scale.y);

            glm::vec2 newDistance{deltaX, deltaY};

            rigidbodies[objectIdToDrag].rigidBody2d.velocity += (newDistance-distanceFromCenter)*glm::vec2{1.5};
        }
    }
}