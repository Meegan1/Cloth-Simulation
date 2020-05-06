//
// Created by Jake Meegan on 03/05/2020.
//

#include "MassSpring.h"


MassSpring::MassSpring(const unsigned long width, const unsigned long height, float scale) : width(width), height(height) {
    PointMass* grid[width][height];

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            grid[i][j] = addPoint(PointMass(glm::vec3(i * scale, 0.0f, j * scale), 1.0f)).get();
        }
    }

    // create face-index

    for(int i = 0; i < width - 1; i++) {
        for (int j = 0; j < height - 1; j++) {
            addFace(grid[i][j], grid[i+1][j], grid[i+1][j+1]);
            addFace(grid[i][j], grid[i+1][j+1], grid[i][j+1]);
        }
    }

    for (int i = 0; i < width - 1; i++) {
        for (int j = 0; j < height - 1; j++) {
            addSpring(Spring(1.0f, scale, grid[i][j], grid[i+1][j]));
            addSpring(Spring(1.0f, scale, grid[i][j], grid[i][j+1]));
            addSpring(Spring(1.0f, scale * 1.4f, grid[i][j], grid[i+1][j+1]));
            addSpring(Spring(1.0f, scale * 1.4f, grid[i+1][j], grid[i][j+1]));
        }
    }

    for(int i = 0; i < width - 1; i++) {
        addSpring(Spring(1.0f, scale, grid[i][height - 1], grid[i + 1][height - 1]));
    }

    for(int j = 0; j < height - 1; j++) {
        addSpring(Spring(1.0f, scale, grid[width - 1][j], grid[width - 1][j + 1]));
    }

}

std::shared_ptr<Spring> MassSpring::addSpring(const Spring &spring) {
    springs.emplace_back(new Spring(spring));
    Spring* new_spring = springs.back().get();
    new_spring->m1->addSpring(new_spring);
    new_spring->m2->addSpring(new_spring);

    return springs.back();
}

std::shared_ptr<PointMass> MassSpring::addPoint(const PointMass &point) {
    points.emplace_back(new PointMass(point));
    return points.back();
}

void MassSpring::update(float dt, float scale) {
    for(auto &point : points) {
        point->update(dt, scale);

        // resolve ball collision
        for(auto &ball : balls) {
            float d = glm::distance(point->position, ball->position);
            if(d <= ball->getRadius() * 1.02f) {
                glm::vec3 resolve = glm::normalize(point->position - ball->position) * ball->getRadius() * 1.02f;
                point->position = ball->position + resolve;
                point->setIsFixed(true);
            }
        }
    }
}

void MassSpring::render() {
    for(auto &spring : springs) {
        spring->render();
    }

    for(auto &point : points) {
        point->render();
    }

    glPushAttrib(GL_COLOR);
        glBegin(GL_TRIANGLES);
            float color[] = {0.0f, 0.0f, 1.0f};
            float specular[] = {1.0f, 1.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            for(unsigned long face = 0; face < faces.size() / 3; face++) {
//                glNormal3f(1.0f, 1.0f, -1.0f);
                glVertex3fv(&(faces[face * 3]->position.x));
                glVertex3fv(&(faces[face * 3 + 1]->position.x));
                glVertex3fv(&(faces[face * 3 + 2]->position.x));
            }
        glEnd();
    glPopAttrib();
}

void MassSpring::addBall(Ball *ball) {
    balls.emplace_back(ball);
}

void MassSpring::addFace(PointMass *v0, PointMass *v1, PointMass *v2) {
    faces.emplace_back(v0);
    faces.emplace_back(v1);
    faces.emplace_back(v2);
}
