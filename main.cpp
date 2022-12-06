#include <QApplication>
#include "fenetreprincipale.h"
#include "grandefenetre.h"
#include "mafenetre.h"
#include "fenetreaccueil.h"

int main(int argc, char *argv[])

{

    QApplication app(argc, argv);

    FenetreAccueil myWin;
    myWin.show();

    return app.exec();

}
