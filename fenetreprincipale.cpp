#include "fenetreprincipale.h"

FenetrePrincipale::FenetrePrincipale(double framesPerSecond, QWidget *parent, QString name)
    : QGLWidget(parent)
{
    setWindowTitle(name);

    if(framesPerSecond == 0.) {

        t_Timer = nullptr;

    }

    else

    {
        int seconde = 1000;
        int timerInterval = int(seconde / framesPerSecond);
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );

    }

    b_Fullscreen = false;

}

void FenetrePrincipale::keyPressEvent(QKeyEvent *keyEvent) {

    switch(keyEvent->key())

    {

    case Qt::Key_Escape:

        close();
        break;

    case Qt::Key_F1:

        toggleFullWindow();
        break;

    }

}

void FenetrePrincipale::timeOutSlot() {

    updateGL();

}

void FenetrePrincipale::toggleFullWindow() {

    if(b_Fullscreen) {

        showNormal();
        b_Fullscreen = false;

    }

    else {

        showFullScreen();
        b_Fullscreen = true;

    }

}
