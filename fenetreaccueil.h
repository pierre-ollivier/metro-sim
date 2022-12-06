#ifndef FENETREACCUEIL_H
#define FENETREACCUEIL_H

#include <QWidget>
#include <QtWidgets>
#include "grandefenetre.h"


class FenetreAccueil : public QWidget
{

    Q_OBJECT

public:
    FenetreAccueil();

public slots:

    void demoActif();
    void satelliteActif();
    void validation();

private:

    QGridLayout *layout;
    QLabel *label1;
    QLabel *label2;
    QSpinBox *boxnbRames;
    QSpinBox *boxPointDepart;
    QCheckBox *checkModeDemo;
    QCheckBox *checkModeSatellite;
    QPushButton *boutonValidation;

    GrandeFenetre *gf;

};

#endif // FENETREACCUEIL_H
