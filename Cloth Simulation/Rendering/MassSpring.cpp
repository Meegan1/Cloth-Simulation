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

void MassSpring::update(float dt) {
    for(auto &point : points) {
        point->update(dt);

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

void MassSpring::render(bool showFaces, bool showSprings, bool showPoints) {
    if(showSprings) {
        for (auto &spring : springs) {
            spring->render();
        }
    }

    if(showPoints) {
        for (auto &point : points) {
            point->render();
        }
    }

    if(showFaces) {
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glBegin(GL_TRIANGLES);
                    float color[] = {0.0f, 0.0f, 0.6f, 1.0f};
                    float specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
                    for (unsigned long face = 0; face < faces.size() / 3; face++) {
                        glm::vec3 A = faces[face * 3 + 1]->position - faces[face * 3]->position;
                        glm::vec3 B = faces[face * 3 + 2]->position - faces[face * 3]->position;
                        glm::vec3 normal = glm::cross(B, A);
                        glNormal3fv(&normal.x);
                        glVertex3fv(&(faces[face * 3]->position.x));
                        glVertex3fv(&(faces[face * 3 + 1]->position.x));
                        glVertex3fv(&(faces[face * 3 + 2]->position.x));
                    }
                glEnd();
            glPopAttrib();
        glPopMatrix();
    }
}

void MassSpring::addBall(Ball *ball) {
    balls.emplace_back(ball);
}

void MassSpring::addFace(PointMass *v0, PointMass *v1, PointMass *v2) {
    faces.emplace_back(v0);
    faces.emplace_back(v1);
    faces.emplace_back(v2);
}
