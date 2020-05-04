//
// Created by Jake Meegan on 03/05/2020.
//

#include <gl.h>
#include "PointMass.h"

PointMass::PointMass(glm::vec3 position, float mass) : mass(mass), position(position) {

}

void PointMass::setPosition(const glm::vec3 &position) {
    this->position = position;
}

void PointMass::setIsFixed(bool isFixed) {
    this->is_fixed = isFixed;
}

void PointMass::setMass(float mass) {
    this->mass = mass;
}

void PointMass::addSpring(Spring *spring) {
    this->springs.emplace_back(spring);
}


void PointMass::updateAcceleration(float scale) {
    glm::vec3 gravity = glm::vec3(0, -9.8f, 0) * mass;

    acceleration = glm::vec3(0.0f);
    acceleration += gravity;
//    acceleration += glm::vec3(10.0f, 0.0f, 0.0f);

    for(auto &spring : springs) {
        acceleration += spring->getForce(this, scale);
    }

    acceleration /= mass;
}

void PointMass::update(float dt, float scale) {
    if(is_fixed)
        return;

    updateAcceleration(scale);

    velocity = velocity + acceleration * dt;

    position = position + velocity * dt;
}

void PointMass::render() {
    glPointSize(4.0);
    glBegin(GL_POINTS);
    glVertex3f(position.x, position.y, position.z);
    glEnd();
}
