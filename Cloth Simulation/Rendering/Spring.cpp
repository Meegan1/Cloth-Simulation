//
// Created by Jake Meegan on 03/05/2020.
//

#include <gl.h>
#include "Spring.h"
#include "PointMass.h"


Spring::Spring(float springConstant, float restLength, PointMass *m1, PointMass *m2) {
    setSpringConstant(springConstant);
    setRestLength(restLength);

    this->m1 = m1;
    this->m2 = m2;
}

void Spring::setSpringConstant(float springConstant) {
    spring_constant = springConstant;
}

void Spring::setDampingConstant(float dampingConstant) {
    damping_constant = dampingConstant;
}

void Spring::setRestLength(float restLength) {
    rest_length = restLength;
}

void Spring::render() {
    glBegin(GL_LINES);
        glVertex3fv(&m1->position.x);
        glVertex3fv(&m2->position.x);
    glEnd();
}

glm::vec3 Spring::getForce(PointMass *point, float scale) {
    float sign = point == m1 ? -1 : 1;

    // calculate force scalar
    float dist = glm::distance(m2->position, m1->position);
    float linear_spring = -spring_constant * (dist - rest_length) * 100.0f;

    // calculate direction vector
    glm::vec3 dir = glm::normalize(m2->position - m1->position);

    // calculate damping scalar
    float s1 = glm::dot(m1->velocity, dir);
    float s2 = glm::dot(m2->velocity, dir);
    float damper = -damping_constant * s2 - s1;

    return (sign * linear_spring + damper) * dir;
}
