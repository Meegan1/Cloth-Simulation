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
#include "GL/freeglut.h"
#include "../GObject.h"
#include "MassSpring.h"
#include "../Video.h"
#include <glm/mat4x2.hpp>



class Engine : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT
public:
    Ball sphere;
    MassSpring massSpring;
	

    explicit Engine(QWidget *parent);


	void togglePlay();
	void toggleRecord();
	bool isPlaying();
	bool isRecording();

    void toggleFaces();
    void toggleSprings();
    void togglePoints();

	void toggleWind();

	void toggleScenario();
	void setFrameRate(int frame_rate);

signals:
	void playChanged(bool value);
	void recordChanged(bool value);
	void facesChanged(bool value);
	void springsChanged(bool value);
	void pointsChanged(bool value);
	void scenarioChanged(int value);
	void windToggled(bool value);

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
	bool enable_wind = false;
	int scenario{0};
	glm::vec2 window_size;
	std::vector<GLubyte*> frame_caps;
	glm::mat4 target_position;
	int frame{0}, prev_frame;
		
	// physics fixed timestep
	float DELTA_TIME { 0.01f };
	// animation frame rate
	float FPS{ 60 };

private:
};


#endif //NATURAL_PHENOMENA_ENGINE_H
