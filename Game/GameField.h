#pragma once

#include <list>
#include "Objects/Rigidbody.h"

namespace FluidSim{
    
    class GameField{
        
        class Quadtree {
        private:
            static const int MAX_OBJECTS = 4;  // Max objects before subdivision
            static const int MAX_LEVELS = 5;   // Max depth of the tree
            
            int level;                // Current level
            Rect bounds;              // Bounding box of this node
            std::vector<Rigidbody*> objects; // Rigidbodys in this node
            std::unique_ptr<Quadtree> nodes[4]; // Child nodes
        
        public:
            Quadtree(int pLevel, const Rect& pBounds)
                    : level(pLevel), bounds(pBounds) {}
            
            // Clear the Quadtree
            void clear() {
                objects.clear();
                for (int i = 0; i < 4; ++i) {
                    if (nodes[i]) {
                        nodes[i]->clear();
                        nodes[i].reset();
                    }
                }
            }
            
            // Split the node into 4 subnodes
            void split() {
                float subWidth = bounds.width / 2.0f;
                float subHeight = bounds.height / 2.0f;
                float x = bounds.x;
                float y = bounds.y;
                
                // Top-Left
                nodes[0] = std::make_unique<Quadtree>(level + 1, Rect(x - subWidth / 2, y - subHeight / 2, subWidth, subHeight));
                // Top-Right
                nodes[1] = std::make_unique<Quadtree>(level + 1, Rect(x + subWidth / 2, y - subHeight / 2, subWidth, subHeight));
                // Bottom-Left
                nodes[2] = std::make_unique<Quadtree>(level + 1, Rect(x - subWidth / 2, y + subHeight / 2, subWidth, subHeight));
                // Bottom-Right
                nodes[3] = std::make_unique<Quadtree>(level + 1, Rect(x + subWidth / 2, y + subHeight / 2, subWidth, subHeight));
            }
            
            // Determine which node the object belongs to
            int getIndex(const Rigidbody* obj) const {
                int index = -1;
                double verticalMidpoint = bounds.x;
                double horizontalMidpoint = bounds.y;
                
                // Rigidbody's position relative to center
                bool topQuadrant = obj->y < horizontalMidpoint;
                bool bottomQuadrant = obj->y >= horizontalMidpoint;
                bool leftQuadrant = obj->x < verticalMidpoint;
                bool rightQuadrant = obj->x >= verticalMidpoint;
                
                if (leftQuadrant) {
                    if (topQuadrant) {
                        index = 0; // Top-Left
                    }
                    else if (bottomQuadrant) {
                        index = 2; // Bottom-Left
                    }
                }
                else if (rightQuadrant) {
                    if (topQuadrant) {
                        index = 1; // Top-Right
                    }
                    else if (bottomQuadrant) {
                        index = 3; // Bottom-Right
                    }
                }
                
                return index;
            }
            
            // Insert an object into the Quadtree
            void insert(Rigidbody* obj) {
                if (nodes[0]) {
                    int index = getIndex(obj);
                    
                    if (index != -1) {
                        nodes[index]->insert(obj);
                        return;
                    }
                }
                
                objects.push_back(obj);
                
                if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
                    if (!nodes[0]) {
                        split();
                    }
                    
                    int i = 0;
                    while (i < objects.size()) {
                        int index = getIndex(objects[i]);
                        if (index != -1) {
                            nodes[index]->insert(objects[i]);
                            objects.erase(objects.begin() + i);
                        }
                        else {
                            i++;
                        }
                    }
                }
            }
            
            // Retrieve all objects that could collide with the given object
            std::vector<Rigidbody*> retrieve(const Rigidbody* obj) const {
                std::vector<Rigidbody*> returnRigidbodys;
                int index = getIndex(obj);
                if (index != -1 && nodes[0]) {
                    std::vector<Rigidbody*> childRigidbodys = nodes[index]->retrieve(obj);
                    returnRigidbodys.insert(returnRigidbodys.end(), childRigidbodys.begin(), childRigidbodys.end());
                }
                
                // Add objects at this level
                returnRigidbodys.insert(returnRigidbodys.end(), objects.begin(), objects.end());
                
                return returnRigidbodys;
            }
        };
    
        std::unordered_map<int, std::list<GameRigidbody&>> objectMap;
        graphics::Device &gameDevice;
    
    };
    
}