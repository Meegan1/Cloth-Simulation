//
// Created by Jake Meegan on 03/05/2020.
//

#ifndef ASSIGNMENT_2_MASSSPRING_H
#define ASSIGNMENT_2_MASSSPRING_H


#include <vector>
#include "Spring.h"
#include "PointMass.h"
#include "../GObject.h"

struct Ball {
public:
    glm::vec3 position;

    void init(float radius) {
        this->radius = radius;
        obj.init("../models/sphere.obj", radius);
    }

    void render() {
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        obj.render();
        glPopMatrix();
    }

    float getRadius() const {
        return radius;
    }

private:
    float radius;
    CGObject obj;
};

class MassSpring {
public:
    MassSpring(const unsigned long width, const unsigned long height, float scale);

    std::shared_ptr<Spring> addSpring(const Spring &spring);
    std::shared_ptr<PointMass> addPoint(const PointMass &point);
    void addBall(Ball* ball);
    void addFace(PointMass *v0, PointMass *v1, PointMass *v2);

    void update(float dt);
    void render(bool showFaces, bool showSprings, bool showPoints);

    std::vector<std::shared_ptr<Spring>> springs;
    std::vector<std::shared_ptr<PointMass>> points;
    std::vector<Ball*> balls;
    unsigned long width, height;
    std::vector<PointMass*> faces;
};


#endif //ASSIGNMENT_2_MASSSPRING_H
