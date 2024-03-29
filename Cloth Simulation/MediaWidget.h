//
// Created by Jake Meegan on 28/04/2020.
//

#ifndef ASSIGNMENT_1_MEDIAWIDGET_H
#define ASSIGNMENT_1_MEDIAWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>
#include "Rendering/Engine.h"

class MediaWidget : public QWidget {
Q_OBJECT
    Engine *engine;
    QPushButton *play;
    QPushButton *record;
    QPushButton *save;
    QPushButton *load;
    QPushButton *faces;
    QPushButton *springs;
    QPushButton *points;
    QPushButton *scenario;
	QSpinBox *frame_rate;
	QPushButton* wind;
public:
    MediaWidget(QWidget *parent, Engine *engine) : QWidget(parent), engine(engine) {
        play = new QPushButton("Pause", this);
        record = new QPushButton("Record", this);
        save = new QPushButton("Save", this);
        // load = new QPushButton("Load", this);
        
        faces = new QPushButton("Hide Faces", this);
        springs = new QPushButton("Show Springs", this);
        points = new QPushButton("Show Points", this);

        scenario = new QPushButton("Load Scenario 2", this);

		frame_rate = new QSpinBox(this);
		frame_rate->setValue(60);

		wind = new QPushButton("Disable Wind", this);

        connect(engine, SIGNAL(playChanged(bool)), this, SLOT(updatePlayButton(bool)));
        connect(engine, SIGNAL(recordChanged(bool)), this, SLOT(updateRecordButton(bool)));
        connect(engine, SIGNAL(facesChanged(bool)), this, SLOT(toggleFaces(bool)));
        connect(engine, SIGNAL(springsChanged(bool)), this, SLOT(toggleSprings(bool)));
        connect(engine, SIGNAL(pointsChanged(bool)), this, SLOT(togglePoints(bool)));
        connect(engine, SIGNAL(scenarioChanged(int)), this, SLOT(setScenario(int)));
		connect(engine, SIGNAL(windToggled(bool)), this, SLOT(windToggled(bool)));

        connect(play, SIGNAL(released()), this, SLOT(playPressed()));
        connect(record, SIGNAL(released()), this, SLOT(recordPressed()));
        connect(save, SIGNAL(released()), this, SLOT(savePressed()));
        // connect(load, SIGNAL(released()), this, SLOT(loadPressed()));
        connect(faces, SIGNAL(released()), this, SLOT(facesPressed()));
        connect(springs, SIGNAL(released()), this, SLOT(springsPressed()));
        connect(points, SIGNAL(released()), this, SLOT(pointsPressed()));
        connect(scenario, SIGNAL(released()), this, SLOT(scenarioPressed()));
		connect(frame_rate, SIGNAL(valueChanged(int)), this, SLOT(frameRateSet(int)));
		connect(wind, SIGNAL(released()), this, SLOT(windPressed()));

        // create layout
        QBoxLayout *layout = new QVBoxLayout();
        this->setLayout(layout);
        layout->setMargin(5);

        layout->addWidget(play);
        layout->addWidget(record);
        layout->addWidget(save);
        // layout->addWidget(load);
        layout->addWidget(faces);
        layout->addWidget(springs);
        layout->addWidget(points);
        layout->addWidget(scenario);

		QHBoxLayout* frame_rate_layout = new QHBoxLayout();
		frame_rate_layout->addWidget(new QLabel("Frame Rate:"));
		frame_rate_layout->addWidget(frame_rate);
		layout->addLayout(frame_rate_layout);

		wind->setVisible(false);
		layout->addWidget(wind);
    }

private slots:
    void playPressed() {
        engine->togglePlay();
    }

    void recordPressed() {
        engine->toggleRecord();
    }

    void savePressed() {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setNameFilter(tr("*.obj"));
        dialog.setFileMode(QFileDialog::AnyFile);
        if(dialog.exec()) {
			engine->massSpring.save(dialog.selectedFiles().first().toStdString());
        }
    }

    void updatePlayButton(bool isPlaying) {
        if(!isPlaying)
            play->setText("Play");
        else
            play->setText("Pause");
    }

    void updateRecordButton(bool isRecording) {
        if(!isRecording)
            record->setText("Record");
        else
            record->setText("Stop Recording");
    }

    void loadPressed() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if(dialog.exec()) {
        }
    }
    
    void facesPressed() {
        engine->toggleFaces();
    }
    
    void springsPressed() {
        engine->toggleSprings();
    }
    
    void pointsPressed() {
        engine->togglePoints();
    }
    
    void toggleFaces(bool showFaces) {
        if(showFaces)
            faces->setText("Hide Faces");
        else
            faces->setText("Show Faces");
    }
    
    void toggleSprings(bool showSprings) {
        if(showSprings)
            springs->setText("Hide Springs");
        else
            springs->setText("Show Springs");
    }

    void togglePoints(bool showPoints) {
        if(showPoints)
            points->setText("Hide Points");
        else
            points->setText("Show Points");
    }

    void scenarioPressed() {
        engine->toggleScenario();
    }

    void setScenario(int scenario) {
		if (scenario == 1) {
			this->scenario->setText("Load Scenario 2");
			this->wind->setVisible(false);
		}
		else {
			this->scenario->setText("Load Scenario 1");
			this->wind->setVisible(true);
		}
    }

	void frameRateSet(int frame_rate) {
		engine->setFrameRate(frame_rate);
    }

	void windPressed() {
		engine->toggleWind();
	}

	void windToggled(bool value) {
		if (value)
			this->wind->setText("Disable Wind");
		else
			this->wind->setText("Enable Wind");
	}
};
#endif //ASSIGNMENT_1_MEDIAWIDGET_H
