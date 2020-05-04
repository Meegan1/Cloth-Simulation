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

    void updateAcceleration(float scale);

    void update(float dt, float scale);

    void render();

    glm::vec3 velocity{0};
    glm::vec3 acceleration{0};
    bool is_fixed;

    float mass;

    std::vector<Spring*> springs;
};


#endif //ASSIGNMENT_2_POINTMASS_H
