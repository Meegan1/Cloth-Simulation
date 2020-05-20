#pragma once

#include <QListWidget>
#include "Rendering/Engine.h"
#include "MediaWidget.h"

class UIWindow : public QWidget
{
Q_OBJECT
	Engine *engine;
    QVBoxLayout *layout;
    MediaWidget *media;
public:
    UIWindow(Engine *engine) : QWidget(), engine(engine)
	{
        // set window settings
	    this->setWindowTitle("Control Window");
	    this->resize(300, 300);
	    this->show();

	    // create layout
	    layout = new QVBoxLayout();
	    this->setLayout(layout);
	    layout->setMargin(0);

        // add play/pause button
        media = new MediaWidget(this, engine);
        layout->addWidget(media);

    }
};

