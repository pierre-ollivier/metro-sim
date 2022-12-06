#include "fenetreaccueil.h"

FenetreAccueil::FenetreAccueil()
{

    label1 = new QLabel("Nombre de rames:");
    label2 = new QLabel("Point de départ:");

    checkModeDemo = new QCheckBox("Mode démo");
    checkModeDemo->setChecked(false);
    QObject::connect(checkModeDemo, SIGNAL(stateChanged(int)), this, SLOT(demoActif()));

    checkModeSatellite = new QCheckBox("Mode satellite");
    checkModeSatellite->setChecked(false);
    QObject::connect(checkModeSatellite, SIGNAL(stateChanged(int)), this, SLOT(satelliteActif()));

    boxnbRames = new QSpinBox();
    boxnbRames->setValue(5);
    boxnbRames->setRange(0, 11);

    boxPointDepart = new QSpinBox();
    boxPointDepart->setRange(-1000, 130000);

    boutonValidation = new QPushButton("Valider");
    QObject::connect(boutonValidation, SIGNAL(clicked()), this, SLOT(validation()));

    layout = new QGridLayout();

    layout->addWidget(label1, 1, 1);
    layout->addWidget(boxnbRames, 1, 2);
    layout->addWidget(label2, 2, 1);
    layout->addWidget(boxPointDepart, 2, 2);
    layout->addWidget(checkModeDemo, 3, 1, 1, 2);
    layout->addWidget(checkModeSatellite, 4, 1, 1, 2);
    layout->addWidget(boutonValidation, 5, 1, 1, 2);

    setLayout(layout);

}

void FenetreAccueil::demoActif() {

    boxnbRames->setDisabled(checkModeDemo->isChecked() || checkModeSatellite->isChecked());
    boxPointDepart->setDisabled(checkModeDemo->isChecked() ||checkModeSatellite->isChecked());

}

void FenetreAccueil::satelliteActif() {

    boxnbRames->setDisabled(checkModeDemo->isChecked() || checkModeSatellite->isChecked());
    boxPointDepart->setDisabled(checkModeDemo->isChecked() || checkModeSatellite->isChecked());

}

void FenetreAccueil::validation() {

    gf = new GrandeFenetre(nullptr, boxnbRames->value(), boxPointDepart->value(), checkModeDemo->isChecked(), checkModeSatellite->isChecked());
    gf->showMaximized();


    this->setDisabled(true);
    this->setHidden(true);

}
