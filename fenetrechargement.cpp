#include "fenetrechargement.h"

FenetreChargement::FenetreChargement(int total, QString txt)
{

    this->setObjectName("Traitement en cours");
    this->setGeometry(0, 0, 300, 200);

    nombreActions = total;

    layout = new QGridLayout();


    bar = new QProgressBar();

    texte = new QLabel(txt);

    bar->setMaximum(nombreActions);
    bar->setValue(0);

    bar->setMinimumSize(200, 100);

    layout->addWidget(texte, 0, 0);
    layout->addWidget(bar, 1, 0);

    setLayout(layout);

}

void FenetreChargement::changerValeur(int v) {

    bar->setValue(v);

    if (v == nombreActions) {

        delete this;

    }

}
