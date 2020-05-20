//
// Created by Jake Meegan on 03/05/2020.
//

#ifndef ASSIGNMENT_2_SPRING_H
#define ASSIGNMENT_2_SPRING_H

#include <glm/glm.hpp>

class PointMass;

class Spring {
public:
    Spring(float springConstant, float restLength, PointMass *m1, PointMass *m2);

    void setSpringConstant(float springConstant);

    void setDampingConstant(float dampingConstant);

    void setRestLength(float restLength);

    void render();

    glm::vec3 getForce(PointMass *point);

    float spring_constant;
    float damping_constant{1.0f};
    float rest_length;

    PointMass *m1;
    PointMass *m2;
};


#endif //ASSIGNMENT_2_SPRING_H
