#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QWidget>
#include <QtWidgets>
#include <QtOpenGL>
#include <QGLWidget>

class FenetrePrincipale : public QGLWidget
{

    Q_OBJECT

public:

    explicit FenetrePrincipale(double framesPerSecond = 0.0, QWidget *parent = nullptr, QString name = "");
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    void toggleFullWindow();

public slots:

    virtual void timeOutSlot();

private:

    QTimer *t_Timer;
    bool b_Fullscreen;

};

#endif // FENETREPRINCIPALE_H
