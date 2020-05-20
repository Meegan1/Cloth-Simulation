//
// Created by Jake Meegan on 03/05/2020.
//

#ifndef ASSIGNMENT_2_POINTMASS_H
#define ASSIGNMENT_2_POINTMASS_H


#include <glm/vec3.hpp>
#include <vector>
#include "Spring.h"

class PointMass {
public:
    PointMass(glm::vec3 position, float mass);

    glm::vec3 position;

    void setPosition(const glm::vec3 &position);

    void setIsFixed(bool isFixed);

    void setMass(float mass);

    void addSpring(Spring *spring);

    glm::vec3 calculateAcceleration();

    void update(float dt);

    void render();

    glm::vec3 velocity{0};
    glm::vec3 acceleration{0};
    bool is_fixed{false};

    float mass;

    std::vector<Spring*> springs;
};


#endif //ASSIGNMENT_2_POINTMASS_H
