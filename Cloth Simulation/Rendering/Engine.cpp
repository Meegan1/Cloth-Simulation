//
// Created by Jake Meegan on 03/03/2020.
//

#include "Engine.h"
#include "../Video.h"
#include <QThread>
#include <QOpenGLShaderProgram>
#include <fstream>
#include <filesystem>
#include <QMessageBox>

Engine::Engine(QWidget *parent) : QOpenGLWidget(parent), camera({5, 4, 20}), massSpring(50, 50, 0.3f) {
    setWindowTitle("Animation Viewer");
}

void Engine::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST); // enable z-buffering
    // set lighting parameters
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL); // enable colour

    // enable lighting
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColorMaterial(GL_FRONT, GL_SPECULAR);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // set background colour to purple
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    sphere.init(5.0f);
    sphere.position = glm::vec3(5.0f, -10.0f, 5.0f);
    massSpring.addBall(&sphere);

    startLoop(); // start game loop
}

void Engine::startLoop() {
    // start elapsed timer
    elapsed_timer.start();

    // start game loop
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&Engine::loop));
    timer.start();
}

void Engine::loop() {
    // get time from previous frame
    GLfloat frame_time = elapsed_timer.nsecsElapsed() * 0.000000001;
    elapsed_timer.restart();

    camera.update();

    if(is_playing) {
        delta_accumulator += frame_time;

        // integrate in steps
        while (delta_accumulator >= DELTA_TIME) {
            massSpring.update(DELTA_TIME);

            delta_accumulator -= DELTA_TIME;
        }

        // catch remainder in accumulator and integrate
        if(delta_accumulator >= 0.0f) {
            massSpring.update(delta_accumulator);

            delta_accumulator = 0.0f;
        }
    }


    if(input.keyboard.KEY_W)
        camera.zoom(frame_time * 5);
    if(input.keyboard.KEY_S)
        camera.zoom(-frame_time * 5);
    if(input.keyboard.KEY_A)
        camera.pan(frame_time * 5, 0);
    if(input.keyboard.KEY_D)
        camera.pan(-frame_time * 5, 0);

    // call window/opengl to update
    update();

    if((1000/FPS) - elapsed_timer.elapsed() >= 0) {
        timer.stop();

    	if(FPS == 0 || (1000 / (int)FPS) - elapsed_timer.elapsed() < 0)
			timer.start(0);
		else
			timer.start((1000 / (int) FPS) - elapsed_timer.elapsed());
    }

    frame++;
}

void Engine::resizeGL(int w, int h) {
    const float aspectRatio = (float) w / (float) h;

    // resize viewport to window size
    glViewport(0, 0, w, h);

    // set projection matrix to glFrustum based on window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, aspectRatio, 0.1f, 100.0);

	window_size = { w, h };
}

void Engine::paintGL() {
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // set model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glMultMatrixf(&camera.getView()[0][0]);

    glPushMatrix();

    /*
     * Draw objects
     */

    if (show_ball)
        sphere.render();

    massSpring.render(show_faces, show_springs, show_points);

    glPopMatrix();

    GLfloat light_position[] = {5.0f, 4.0f, 100.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    if (is_recording) {
        frame_caps.push_back(static_cast<GLubyte *>(malloc(4 * window_size.x * window_size.y)));
        glReadPixels(0, 0, window_size.x, window_size.y, GL_RGBA, GL_UNSIGNED_BYTE, frame_caps.back());
    }
}

void Engine::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W)
        input.keyboard.KEY_W = true;
    else if(event->key() == Qt::Key_A)
        input.keyboard.KEY_A = true;
    else if(event->key() == Qt::Key_S)
        input.keyboard.KEY_S = true;
    else if(event->key() == Qt::Key_D)
        input.keyboard.KEY_D = true;
    else if(event->key() == Qt::Key_Space)
        input.keyboard.KEY_SPACE = true;
}

void Engine::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W)
        input.keyboard.KEY_W = false;
    else if(event->key() == Qt::Key_A)
        input.keyboard.KEY_A = false;
    else if(event->key() == Qt::Key_S)
        input.keyboard.KEY_S = false;
    else if(event->key() == Qt::Key_D)
        input.keyboard.KEY_D = false;
    else if(event->key() == Qt::Key_Space)
        input.keyboard.KEY_SPACE = false;
}

void Engine::mouseMoveEvent(QMouseEvent *event) {
    float sensitivity = 0.01;
    if (event->buttons() & Qt::LeftButton) { // left click to control rotation
        float xoffset = event->x() - last_m_pos.x();
        float yoffset = event->y() - last_m_pos.y();

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.rotate(xoffset, yoffset);
    }
	
	if (event->buttons() & Qt::RightButton)
	{
		float xoffset = event->x() - last_m_pos.x();
		float yoffset = event->y() - last_m_pos.y();

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		glm::mat4 inverse = glm::inverse(target_position);
		target_position = glm::translate(target_position, glm::vec3(inverse * glm::vec4(camera.right(), 0.0f)) * xoffset);
		target_position = glm::translate(target_position, glm::vec3(inverse * glm::vec4(-camera.up(), 0.0f)) * yoffset);
	}

    last_m_pos = event->pos(); // store last mouse position
}

void Engine::mousePressEvent(QMouseEvent *event) {
    last_m_pos = event->pos(); // clear mouse position upon click
}

void Engine::togglePlay() {
    this->is_playing = !this->is_playing;

    emit playChanged(isPlaying());
}

void Engine::toggleRecord() {
    if(this->is_recording)
        stopRecording();
    else
        startRecording();

    emit recordChanged(isRecording());
}

bool Engine::isPlaying() {
    return this->is_playing;
}

bool Engine::isRecording() {
    return this->is_recording;
}

void Engine::toggleFaces() {
    show_faces = !show_faces;

    emit facesChanged(show_faces);
}

void Engine::toggleSprings() {
    show_springs = !show_springs;

    emit springsChanged(show_springs);
}

void Engine::togglePoints() {
    show_points = !show_points;

    emit pointsChanged(show_points);
}

void Engine::toggleWind() {
	if (!enable_wind) {
		for (auto& point : massSpring.points) {
			point->setForce(glm::vec3(0, 0, 10));
		}
	} else {
		for (auto& point : massSpring.points) {
			point->setForce(glm::vec3(0, 0, 0));
		}
	}

	enable_wind = !enable_wind;

	emit windToggled(enable_wind);
}

void Engine::toggleScenario()
{
	if (scenario == 2)
	{
		massSpring = MassSpring(50, 50, 0.3f);
		massSpring.addBall(&sphere);
		show_ball = true;

		scenario = 1;
	}
	else
	{
		massSpring = MassSpring(10, 10, 1.0f);
		show_ball = false;

		massSpring.points.at(0)->setIsFixed(true);
		massSpring.points.at(90)->setIsFixed(true);

		this->enable_wind = false;
		toggleWind();

		scenario = 2;
	}

	emit scenarioChanged(scenario);
}

void Engine::setFrameRate(int frame_rate) {
	this->is_playing = false;
	emit playChanged(isPlaying());
	
	this->FPS = frame_rate;
}

void Engine::startRecording() {
    // clear buffer
    for(auto *pixels: frame_caps) {
        free(pixels);
    }
    frame_caps.clear(); // clear vector

    is_recording = true;
}

void Engine::stopRecording() {
    is_recording = false;

	QMessageBox msgBox;
	msgBox.setText("The frames will now be exported into the video folder!");
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();

	if(ret == QMessageBox::Ok)
		outputImages();
}

void Engine::outputImages() {
    std::filesystem::remove_all("video"); // cleanup video folder
    std::filesystem::create_directory("video"); // create new video folder
	

    int i = 0;
    for(auto *pixels : this->frame_caps) {
        Video::create_ppm("video/tmp", i, window_size.x, window_size.y, 255, 4, pixels);
        i++;
    }

    // create file with ffmpeg command to render video
    std::ofstream file("video/render.sh");
    file << "ffmpeg -framerate " << FPS <<" -i \"tmp%d.ppm\" output.mp4";
    file.close();

	// create readme
	std::ofstream readme("video/readme.txt");
	readme << "Compile all the images via ffmpeg using this command: ffmpeg -framerate " << FPS << " -i \"tmp%d.ppm\" output.mp4";
	readme.close();
}
