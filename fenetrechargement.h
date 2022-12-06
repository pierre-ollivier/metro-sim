#ifndef FENETRECHARGEMENT_H
#define FENETRECHARGEMENT_H


#include <QWidget>
#include <QtWidgets>


class FenetreChargement : public QWidget
{

    Q_OBJECT

public:

    FenetreChargement(int total, QString txt = "Chargement des images... ");

public slots:

    void changerValeur(int v);

private:

    QProgressBar *bar;
    QLabel *texte;
    QGridLayout *layout;
    int nombreActions;
};

#endif // FENETRECHARGEMENT_H
