//
// Created by Jake Meegan on 03/03/2020.
//

#ifndef NATURAL_PHENOMENA_ENGINE_H
#define NATURAL_PHENOMENA_ENGINE_H
#define GL_SILENCE_DEPRECATION

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QElapsedTimer>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "QKeyEvent"
#include "qdebug.h"
#include "Input.h"
#include "../Animation/BVH.h"
#include "GL/freeglut.h"
#include "../GObject.h"
#include "MassSpring.h"
#include "../Video.h"
#include <glm/mat4x2.hpp>



// physics fixed timestep
#define DELTA_TIME 0.01f
#define FPS 60

class Engine : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT
public:
    Ball sphere;
    MassSpring massSpring;

    explicit Engine(QWidget *parent);

	void selectJoint(BVH::Joint* joint);

	void togglePlay();
	void toggleRecord();
	bool isPlaying();
	bool isRecording();

    void toggleFaces();
    void toggleSprings();
    void togglePoints();

    void toggleScenario() {
        if(scenario == 1) {
            massSpring = MassSpring(50, 50, 0.3f);
            massSpring.addBall(&sphere);
            show_ball = true;

            scenario = 2;
        }
        else {
            massSpring = MassSpring(10, 10, 1.0f);
            show_ball = false;

            massSpring.points.at(0)->setIsFixed(true);
            massSpring.points.at(90)->setIsFixed(true);

            scenario = 1;
        }

        emit scenarioChanged(scenario);
    }

signals:
    void playChanged(bool value);
    void recordChanged(bool value);
    void facesChanged(bool value);
    void springsChanged(bool value);
    void pointsChanged(bool value);
    void scenarioChanged(int value);

protected:
    /*
     * OpenGL Methods
     */
    void initializeGL() override;

    void startLoop();

    void loop();

    void resizeGL(int w, int h) override;

    void paintGL() override;

    /*
     * Input Events
     */
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    /*
     * Mouse Events
     */
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    /*
     * Video Recording
     */
    void startRecording();
    void stopRecording();
    void outputImages();

    /*
     * Class members
     */
    QTimer timer;
    QElapsedTimer elapsed_timer;
    GLfloat delta_accumulator{0.0f};
    Camera camera;
    Input input;
    QPoint last_m_pos;
    bool is_playing = true;
    bool is_recording = false;
    bool show_faces = true;
    bool show_springs = false;
    bool show_points = false;
    bool show_ball = true;
    int scenario{0};
	glm::vec2 window_size;
	std::vector<GLubyte*> frame_caps;
	glm::mat4 target_position;
	int frame{0}, prev_frame;

private:
};


#endif //NATURAL_PHENOMENA_ENGINE_H
