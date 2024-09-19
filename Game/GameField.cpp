#include "GameField.h"

namespace FluidSim{
    void GameField::createCircleGrid(int size, float elementSize = 0.06, float padding = 0.02) {
        int width = ceil(sqrt(size));
        int height = size/width+1;
        int extra = size%width;

        glm::vec2 basePosition{
                0.5-((width*elementSize+padding)*size-padding)/2,
                0.5-((height*elementSize+padding)*size-padding)/2+elementSize};

        for (int i = 0; i < height-1; ++i) {
            for (int j = 0; j < width; ++j) {
                glm::vec2 position{
                        basePosition.x+(padding+elementSize)*j,
                        basePosition.y+(padding+elementSize)*i};

                addCircle(elementSize, position);
            }
        }

        for (int i = 0; i < extra; ++i) {

        }
    }

    void GameField::addCircle(float size, glm::vec2 position) {
        std::shared_ptr<graphics::Model> circleModel = graphics::Model::createCircleModel(gameDevice, 64);

        Rigidbody ball{};
        ball.transform2d.scale = glm::vec2{.15};
        ball.transform2d.translation = position;
        ball.color = {1.f, 0.f, 0.f};
        ball.rigidBody2d.velocity = {.001f, .0f};
        ball.model = circleModel;
        ball.isInBoundingAreaFunc = [](GameObject &gameObject, glm::vec2 coords){
            float r = gameObject.transform2d.scale.x;

            float diffInX = coords.x - gameObject.transform2d.translation.x;
            float diffInY = coords.y - gameObject.transform2d.translation.y;

            return diffInX*diffInX + diffInY*diffInY <= r*r;
        };
        ball.transform2d.translation.y = -0.5f;
        objects.push_back(std::move(ball));
    }
}