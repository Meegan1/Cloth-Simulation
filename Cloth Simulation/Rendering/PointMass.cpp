//
// Created by Jake Meegan on 03/05/2020.
//

#include <QOpenGLFunctions>
#include "PointMass.h"

PointMass::PointMass(glm::vec3 position, float mass) : mass(mass), position(position) {

}

PointMass::PointMass()
{
}

void PointMass::setPosition(const glm::vec3 &position) {
    this->position = position;
}

void PointMass::setForce(const glm::vec3& force) {
	this->force = force;
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


glm::vec3 PointMass::calculateAcceleration() {
    // create initial acceleration
    glm::vec3 acceleration = glm::vec3(0.0f);

    // add gravity
    acceleration += glm::vec3(0, -9.8f, 0) * mass;

	// add forces
	acceleration += force;

    // calculate forces from attached springs
    for(auto &spring : springs) {
        acceleration += spring->getForce(this);
    }

    // divide by mass to get resulting force
    acceleration /= mass;
    return acceleration;
}

void PointMass::update(float dt) {
    if(is_fixed) // don't update if fixed
        return;

    velocity = velocity + calculateAcceleration() * dt; // calculate velocity
    position = position + velocity * dt; // calculate new position
}

void PointMass::render() {
    glPointSize(4.0);
    glBegin(GL_POINTS);
    glVertex3f(position.x, position.y, position.z);
    glEnd();
}
