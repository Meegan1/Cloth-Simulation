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

glm::vec3 Spring::getForce(PointMass *mass, float scale) {
    if (mass != m1 && mass != m2) {
        return glm::vec3(0, 0, 0);
    }

    // spring force
    float dist = glm::distance(m1->position, m2->position);
    float scalar = 100.0f * spring_constant * (dist - rest_length);
    glm::vec3 dir = glm::normalize(m2->position - m1->position);

    // find speed of contraction/expansion for damping force
    float s1 = glm::dot(m1->velocity, dir);
    float s2 = glm::dot(m2->velocity, dir);
    float dampingScalar = -damping_constant * (s1 + s2);

    if (mass == m1) {
        return (scalar + dampingScalar) * dir;
    } else {
        return (-scalar + dampingScalar) * dir;
    }
}