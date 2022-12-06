#include "mafenetre.h"
#include "fenetrechargement.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <QtOpenGL>
#define PI 3.1415926535
#define V2 1.4142135624

maFenetre::maFenetre(QWidget *parent, int nbRames, int d, int fa, bool modeDemo, bool modeSatellite)
    : FenetrePrincipale(62.5, parent, QString("Programme 3D"))
{

    fc = new FenetreChargement(130 - 1);

    QObject::connect(this, SIGNAL(emettreTextureChargee(int)), fc, SLOT(changerValeur(int)));

    fc->show();

    facteurAcceleration = fa;

    demo = modeDemo;
    satellite = modeSatellite;



    if (modeSatellite) {

        addInfo("3 - Mode satellite dans le cstr de GrandeFenetre");

    }

    else {

        addInfo("3 - Pas de mode satellite dans le cstr de GrandeFenetre");

    }


    feuRetour[99] = 1;

    distanceTotale = d;
    nombreRames = nbRames;
    temps = temps_position(distanceTotale) + 43;
    tempsAutres = 0;

    if (distanceTotale <= 304) {

        distanceTotale += 104955.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2 - 310;
        temps += 1752;

    }

    f_x = x(distanceTotale);
    f_y = y(distanceTotale);
    f_z = z(distanceTotale);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        distanceTotaleAutres[compteur] = position_temps(int(temps_position(distanceTotale) + (1 + compteur) * (1752 / (nombreRames + 1))));

        if (distanceTotaleAutres[compteur] <= 304) {

            distanceTotaleAutres[compteur] += 104955.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2 - 310;

        }

        while (estEnStation(distanceTotaleAutres[compteur])) { //2ème condition provisoire

            distanceTotaleAutres[compteur] += 0.5;

            if (distanceTotaleAutres[compteur] >= 5477.57587195 && distanceTotaleAutres[compteur] < 99000) {

                distanceTotaleAutres[compteur] = 100005;

            }

        }

        biais_temps[compteur] = temps_position(distanceTotaleAutres[compteur]);

    }

}

void maFenetre::keyPressEvent(QKeyEvent *keyEvent) {

    satellite = false; //provisoire, à cause des problèmes du mode satellite

    switch(keyEvent->key()) {

    case Qt::Key_Space:

        if (!satellite) {

            avancerC();

        }

        break;

    case Qt::Key_Up:

        tournerHC();

        break;

    case Qt::Key_Down:

        tournerTC();

        break;

    case Qt::Key_Right:

        if (satellite) {

            augmenterxS();

        }

        break;

    case Qt::Key_Left:

        if (satellite) {

            diminuerxS();

        }

        break;

    case Qt::Key_Plus:

        if (satellite) {

            diminuerzS();

        }

        break;

    case Qt::Key_Minus:

        if (satellite) {

            augmenterzS();

        }

        break;

    case Qt::Key_9:

        if (satellite) {

            diminueryS2(); //en fait c'est le KeyPressEvent de GrandeFenetre qui s'applique

        }

        break;

    case Qt::Key_2:

        if (satellite) {

            diminuerxS2();

        }

        break;

    case Qt::Key_8:

        if (satellite) {

            augmenterxS2();

        }

        break;

    case Qt::Key_6:

        if (satellite) {

            augmenteryS2();

        }

    }

}

void maFenetre::initializeGL() {

    glMatrixMode(GL_MODELVIEW);

    GLfloat blanc[4] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, blanc);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, blanc);
    glLightfv(GL_LIGHT0, GL_SPECULAR, blanc);

    GLfloat position[4];
    position[0] = 6.0; position[1] = 224.0; position[2] = 5.0;
    position[3] = 1.0;
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 2.0);

    GLfloat direction[]={0.0, 0.0, -1.0, 0.0}; //demi-axe -z
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0); /* ce spot éclairera jusqu'à 45° autour de son axe */
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.5);/* coefficient d'atténuation angulaire */

    for (int i = 0; i < 10; i++) {

        for (int j = 0; j < 100; j++) {

            xAller[i][j] = 0;
            yAller[i][j] = 0;
            textAller[i][j] = 0;
            orientationAller[i][j] = 0;
            xRetour[i][j] = 0;
            yRetour[i][j] = 0;
            textRetour[i][j] = 0;
            orientationRetour[i][j] = 0;

        }

    }

    srand(unsigned(time(nullptr)));

    switch(QDate::currentDate().dayOfYear()) {

    case 167: //16 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 167.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 168: //17 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 168.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 169: //18 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 169.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 170: //19 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 170.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 171: //20 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 171.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 172: //21 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 172.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 173: //22 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 173.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 174: //23 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 174.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 175: //24 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 175.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 176: //25 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 176.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 177: //26 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 177.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 178: //27 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 178.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 179: //28 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 179.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 180: //29 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 180.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 181: //30 juin

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 181.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 181.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 182: //1er juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 182.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 182.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 183: //2 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 183.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 183.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 184: //3 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 184.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 184.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 185: //4 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 185.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 185.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 186: //5 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 186.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 186.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 187: //6 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 187.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 187.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 188: //7 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 188.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 188.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 189: //8 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 189.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 189.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 190: //9 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 190.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 190.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 191: //10 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 191.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 191.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 192: //11 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 192.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 192.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 193: //12 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 193.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 193.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 194: //13 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 194.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 194.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 195: //14 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 195.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 195.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 196: //15 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 196.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 196.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 197: //16 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 197.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 197.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    case 198: //17 juillet

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République 198.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 198.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");
        break;

    default:

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bouchers.png");

        if (QDate::currentDate().dayOfYear() >= 167 && QDate::currentDate().dayOfYear() <= 272) { //visible Rue des Chenilles: +4 -> -8 et +2 -> -10 --> +4 -> -10 pour modification de la texture - 5 à 6 carreaux de haut

            int i = QDate::currentDate().dayOfYear();
            QString s;
            s.setNum(i);

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles " + s + ".png");

        }

        else if (QDate::currentDate().dayOfYear() >= 100 && QDate::currentDate().dayOfYear() < 167) {

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles.png");

        }

        else {

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rue des Chenilles 272.png");

        }

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Boulevard de l'Hôpital.png");
        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers.png");

        if (QDate::currentDate().dayOfYear() >= 26 && QDate::currentDate().dayOfYear() < 100) {

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville 25.png");

        }

        else if (QDate::currentDate().dayOfYear() >= 100 && QDate::currentDate().dayOfYear() < 274) {

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville.png");

        }

        else if (QDate::currentDate().dayOfYear() >= 274) {

            int i = QDate::currentDate().dayOfYear();
            QString s;
            s.setNum(i);

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville " + s + ".png");

        }

        else if (QDate::currentDate().dayOfYear() <= 25) {

            int i = QDate::currentDate().dayOfYear();
            QString s;
            s.setNum(i);

            loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Blainville " + s + ".png");

        }

        break;

    }

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Mirla 295.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Histiaux.png");



    if (QDate::currentDate().dayOfYear() >= 6 && QDate::currentDate().dayOfYear() < 100) {

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse 127.png");

    }

    else if (QDate::currentDate().dayOfYear() >= 100 && QDate::currentDate().dayOfYear() < 244) {

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse.png");

    }

    else if (QDate::currentDate().dayOfYear() >= 244) {

        int i = QDate::currentDate().dayOfYear() - 244 + 1;
        QString s;
        s.setNum(i);

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse " + s + ".png");

    }

    else if (QDate::currentDate().dayOfYear() <= 5) {

        int i = QDate::currentDate().dayOfYear() + 122;
        QString s;
        s.setNum(i);

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse " + s + ".png");

    }


    if (QDate::currentDate().dayOfYear() < 100 || QDate::currentDate().dayOfYear() >= 321) {

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse D 87.png");

    }

    else if (QDate::currentDate().dayOfYear() >= 100 && QDate::currentDate().dayOfYear() < 303) {

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse D.png");

    }

    else if (QDate::currentDate().dayOfYear() >= 303) {

        int i = QDate::currentDate().dayOfYear() - 244 + 1;
        QString s;
        s.setNum(i);

        loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Bourse D " + s + ".png");

    }

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 10.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 15.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 20.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 25.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 30.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 35.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 40.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 45.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 50.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 55.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 60.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 65.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/TIV 70.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tunnel.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Plafond station 20.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rails std.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Aiguillage.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Haut quai.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Côté quai.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tunnel courbe.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Noir.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Ciel 2.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Colline.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Rouge.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Demi-rails.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Gris voie.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Aiguillage large.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Noir tunnel.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Sans direction.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Arrière train.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Voie aiguillage large.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Grand plafond.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Côté train.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Feu vert.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Feu jaune.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Feu rouge.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Porte local technique.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couloirs 1.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 0.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 1.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 3.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 4.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 5.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 6.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 7.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 8.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Chiffre 9.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Tympan.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction Bourse.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Direction République.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Mirla-C1.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Mirla-C2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers-C1.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Cerisiers - Framboisiers-C2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Haut sortie.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Mur République 1.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/République fantôme piédroit.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Mur de pierres.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ0.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ1.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ3.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ4.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ5.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ6.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ7.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ8.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQ9.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQDeuxPoints.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQMoins.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/IQ/IQPlus.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Place des Remparts a.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Place des Remparts b.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Place des Remparts haut de quai.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Place des Remparts bas de quai.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Place des Remparts IDD.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Place des Remparts c.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Bleu.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Marron.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Noir 2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Taupe.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Bleu vert.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Bleu violet.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Jaune orange.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Lilas.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Orange rouge.png");
    //cette dernière texture est maudite, elle change la valeur du booléen satellite!!
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Rouge 2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Rouge marron.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Vert jaune.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Vert.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Blanc.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Couleurs/Beige.png");

    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Personnages/P1.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Personnages/P2.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Personnages/P3.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Personnages/P4.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Personnages/P5.png");
    loadTexture("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug/Textures/Personnages/P6.png");

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);

    glEnable(GL_COLOR_MATERIAL);


    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    f_x = x(distanceTotale);
    f_y = y(distanceTotale);
    f_z = z(distanceTotale);

    if (!satellite) {

        gluLookAt(f_x, f_y, f_z, f_x - sin(rot*PI/180), f_y + cos(rot*PI/180), f_z, 0.0, 0.0, 1.0);
        satellite = false;

    }

    else {

        gluLookAt(xS, yS, zS, xS, yS, zS, 0.0, 0.0, 1.0);
        satellite = true;

    }

    passagersDescendants = 0;

    paintGL();

}

void maFenetre::resizeGL(int width, int height) {

    if (height == 0) {

        height = 1;

    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, double(float(width)/float(height)), 0.2, 1200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void maFenetre::paintGL() {

    frameCount++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );

    glLoadIdentity();

    temps += 0.016;
    tempsAutres += 0.016;

    distanceTotale += facteurAcceleration * vitesse/62.5;

    if (distanceTotale >= 103286.35 + 931 * PI/4 + 200 * PI/3 && distanceTotale <= 103400 + 931 * PI/4 + 200 * PI/3 && voieEntreeRepublique == 4) { //on désactive cette instruction

        distanceTotale = 125335 + distanceTotale - (103286.35 + 931 * PI/4 + 200 * PI/3); //régularisation et passage de la voie 2 à la voie 3 (10m après la sortie de Bouchers)

    }

    double m = 0.0;

    for (int n = 0; n < nombreRames; n++) {

        distanceTotaleAutres[n] += vitesseAutres[n] / 62.5;

        if (vitesseAutres[n] < 0.01) {

            tempsImmobilisation[n] += 0.016;

            if ((abs(tempsImmobilisation[n] - 40) < 0.008 && codePosition(distTotaleAutres(n)) != "RP1" && (codePosition(distTotaleAutres(n)) != "BS2" || abs(tempsImmobilisation[n] - 80) < 0.008))) {

                QString s;
                s.setNum(n);
                emit addInfo("Ralentissement de la rame " + s + " en position " + codePosition(distTotaleAutres(n)));

            }

            if (abs(tempsImmobilisation[n] - 180) < 0.008) {

                QString s;
                s.setNum(n);
                emit addInfo("Blocage de la rame " + s + " en position " + codePosition(distTotaleAutres(n)));

            }

            if (abs(tempsImmobilisation[n] - 480) < 0.008) {

                distanceTotaleAutres[n] += 0.2; //Triche... pour éviter les problèmes!

            }

        }

        else {

            if (tempsImmobilisation[n] > 40) {

                QString s;
                s.setNum(n);
                emit addInfo("Fin de problème pour la rame n° " + s);

            }

            tempsImmobilisation[n] = 0.0;

        }

        if (tempsImmobilisation[n] > m)
            m = tempsImmobilisation[n];

    }

    if (abs(m - 180) < 0.017) {

        emit emettreIconePert(3);

    }

    else if (abs(m - 80) < 0.017) {

        emit emettreIconePert(2);

    }

    else if (m < 80) {

        emit emettreIconePert(1);

    }

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        avance[compteur] = int(-temps_position(distanceTotaleAutres[compteur]) + tempsAutres + biais_temps[compteur]);

    }

    f_x = x(distanceTotale);
    f_y = y(distanceTotale);
    f_z = z(distanceTotale);

    rot = r(distanceTotale);

    rot += rot2;

    if (vitesse > 1) {

        rotc = 0;

    }

    if (satellite) {

        gluLookAt(xS, yS, zS, xS, yS, zS - 1, -1.0, 0.0, 0.0);
        satellite = true;

    }

    if (!satellite) {

        gluLookAt(f_x + c_x, f_y + c_y, f_z, f_x + c_x + sin(rot + rotc), f_y + c_y + cos(rot + rotc), f_z, 0.0, 0.0, 1.0);
        satellite = false;

    }



    GLfloat fond[4] = {1.0, 1.0, 1.0, 1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fond);

    GLfloat jaune[4] = { 10.0, 10.0, 0.0, 1.0 };

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, jaune);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, jaune);
    glLightfv(GL_LIGHT1, GL_SPECULAR, jaune);

    emit changementVitesse();

    for (int n = 0; n < nombreRames; n++) {

        emit changementVitesseAutres(n);

    }

    dessinerPersonnages();

    //armoires électriques sous tunnel

    glBindTexture(GL_TEXTURE_2D, texture5[3]);

    glBegin(GL_QUADS); //armoires électriques

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 133.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 133.5f, 3.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.5f, 135.0f, 3.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.5f, 135.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.5f, 135.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.5f, 135.0f, 3.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.5f, 137.5f, 3.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.5f, 137.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.5f, 137.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.5f, 137.5f, 3.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.5f, 140.0f, 3.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.5f, 140.0f, 0.0f);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    //tunnel


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -370.0f, -522.0f, 3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f, -522.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -645.0f, -522.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -645.0f, -522.0f, 3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -345.0f, -516.0f, 3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -345.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -552.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -552.0f, -516.0f, 3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -410.0f, -510.0f, 3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -410.0f, -510.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -645.0f, -510.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -645.0f, -510.0f, 3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -310.0f,   3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -150.0f,   3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, -150.0f,   9.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, -310.0f,   9.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -310.0f,  -2.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -150.0f,  -2.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, -150.0f,   4.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, -310.0f,   4.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, -310.0f,   3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, -150.0f,   3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f, -150.0f,   9.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, -310.0f,   9.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, -150.0f,   1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f,    0.0f,   1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f,    0.0f,   9.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, -150.0f,   9.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, -150.0f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,    0.0f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,    0.0f,  5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, -150.0f,  5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -100.0f,   4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,    0.0f,   4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,    0.0f,   9.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -100.0f,   9.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f,    0.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  150.0f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  150.0f,   5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f,    0.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,    0.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  150.0f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  150.0f,   5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,    0.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f,  225.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  375.0f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  375.0f,   5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f,  225.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  225.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  375.0f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  375.0f,   5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  225.0f,   5.1f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  375.0f,  -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f,  525.0f,  -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f,  525.0f,   5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  375.0f,   5.1f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  325.0f,  -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  500.0f,  -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  500.0f,   5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  325.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f,  525.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  675.0f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  675.0f,   5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f,  525.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  500.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  675.0f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  675.0f,   5.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  500.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f,  750.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  900.0f,  -1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  900.0f,   3.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f,  750.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  750.0f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  900.0f,  -1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  900.0f,   3.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  750.0f,   5.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1050.0f,  -3.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  900.0f,  -1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  900.0f,   3.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1050.0f,   2.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1050.0f,  -3.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  900.0f,  -1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  900.0f,   3.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1050.0f,   2.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1050.0f,  -3.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1200.0f,  -4.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1200.0f,   0.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1050.0f,   2.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1050.0f,  -3.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1200.0f,  -4.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1200.0f,   0.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1050.0f,   2.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1350.0f,  -6.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1200.0f,  -4.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1200.0f,   0.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1350.0f,  -0.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1350.0f,  -6.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1200.0f,  -4.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1200.0f,   0.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1350.0f,  -0.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(   25.046681f, 1524.953319f, -6.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(   25.046681f, 1524.953319f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  237.178715f, 1737.085353f,  2.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  237.178715f, 1737.085353f, -3.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(   33.531963f, 1516.468037f, -6.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(   33.531963f, 1516.468037f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  245.663997f, 1728.600071f,  2.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  245.663997f, 1728.600071f, -3.1f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(  325.213510f, 1808.149584f, -3.1f );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  325.213510f, 1808.149584f,  2.1f );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  453.376614f, 1936.312688f,  9.25f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  453.376614f, 1936.312688f,  4.15f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(  316.728228f, 1816.634866f, -3.1f );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  316.728228f, 1816.634866f,  2.1f );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  444.891332f, 1944.797970f,  9.25f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  444.891332f, 1944.797970f,  4.15f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  453.376614f, 1936.312688f,  4.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  453.376614f, 1936.312688f,  9.25f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  581.539718f, 2064.475792f, 16.6f );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  581.539718f, 2064.475792f, 11.4f );

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  444.891332f, 1944.797970f,  4.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  444.891332f, 1944.797970f,  9.25f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  573.054436f, 2072.961074f, 16.6f ); //pans de mur de 181.25m
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  573.054436f, 2072.961074f, 11.4f );

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1260.0f, 3265.0f, float( 9.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1260.0f, 3265.0f, float(14.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1410.0f, 3265.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1410.0f, 3265.0f, float( 6.7 + 6.75 * PI)); //pente de 2%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1260.0f, 3277.0f, float( 9.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1260.0f, 3277.0f, float(14.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1410.0f, 3277.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1410.0f, 3277.0f, float( 6.7 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1410.0f, 3265.0f, float( 6.7 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1410.0f, 3265.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1560.0f, 3265.0f, float( 7.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1560.0f, 3265.0f, float( 2.2 + 6.75 * PI));  //pente de 3%

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1410.0f, 3277.0f, float( 6.7 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1410.0f, 3277.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1560.0f, 3277.0f, float( 7.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1560.0f, 3277.0f, float( 2.2 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1560.0f     , 3265.0f,  float(2.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1560.0f     , 3265.0f,  float(7.3 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1713.333333f, 3265.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1713.333333f, 3265.0f,  float(0.0 + 6.75 * PI)); //pente de 1.5%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1560.0f     , 3277.0f,  float(2.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1560.0f     , 3277.0f,  float(7.3 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1713.333333f, 3277.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1713.333333f, 3277.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3265.0f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3265.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2000.0f, 3265.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2000.0f, 3265.0f, float(-0.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3277.0f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3277.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2000.0f, 3277.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2000.0f, 3277.0f, float(-0.1 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2000.0f     , 3277.0f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2000.0f     , 3277.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3277.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3277.0f, float(-0.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2100.0f, 3238.205081f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2100.0f, 3238.205081f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3238.205081f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3238.205081f, float(-0.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2100.0f, 3253.205081f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2100.0f, 3253.205081f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3253.205081f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3253.205081f, float(-0.1 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2060.0f     , 3265.0f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2060.0f     , 3265.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3265.0f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3265.0f, float(-0.1 + 6.75 * PI));


    //rapport de ~27 entre largeur et hauteur

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glBegin(GL_QUADS);

    //sol

    glColor3d(0.5, 0.5, 0.5);

    for (int c = 0; c < 96; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 - 81 * sin(c       * PI / 192)), float(-441 - 81 * cos(c       * PI/192)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 - 81 * sin((c + 1) * PI / 192)), float(-441 - 81 * cos((c + 1) * PI/192)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 - 69 * sin((c + 1) * PI / 192)), float(-441 - 69 * cos((c + 1) * PI/192)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 - 69 * sin(c       * PI / 192)), float(-441 - 69 * cos(c       * PI/192)), 4.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -726.0f,  -441.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -726.0f,  -404.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -714.0f,  -404.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -714.0f,  -441.0f,  4.0f);

    for (int c = 0; c < 256; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 - 81 * cos(c       * PI / 256)), float(-404 + 81 * sin(c       * PI/256)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 - 81 * cos((c + 1) * PI / 256)), float(-404 + 81 * sin((c + 1) * PI/256)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 - 69 * cos((c + 1) * PI / 256)), float(-404 + 69 * sin((c + 1) * PI/256)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 - 69 * cos(c       * PI / 256)), float(-404 + 69 * sin(c       * PI/256)), 4.0f);

    }

    for (int c = 0; c < 128; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-470 - 106 * sin(c       * PI / 256)), float(-404 - 106 * cos(c       * PI/256)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-470 - 106 * sin((c + 1) * PI / 256)), float(-404 - 106 * cos((c + 1) * PI/256)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-470 -  94 * sin((c + 1) * PI / 256)), float(-404 -  94 * cos((c + 1) * PI/256)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-470 -  94 * sin(c       * PI / 256)), float(-404 -  94 * cos(c       * PI/256)), 4.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -625.0f,  -522.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f,  -522.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -370.0f,  -510.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -625.0f,  -510.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -450.0f,  -510.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f,  -510.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -370.0f,  -498.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -450.0f,  -498.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -370.0f,  -516.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -335.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -335.0f,  -504.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -370.0f,  -504.0f,  4.0f);


    glTexCoord2f(0.0f, 1.0f); glVertex3f( -335.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -335.0f,  -504.0f,  4.0f);
    glColor3d(luminanceStations[0], luminanceStations[0], luminanceStations[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f,  -504.0f,  4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f,  -516.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f,  -516.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f,  -504.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f,  -504.0f,  4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f,  -504.0f,  4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f,  -516.0f,  4.0f);
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -235.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -235.0f,  -504.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -215.0f,  -516.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -206.0f,  -504.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -215.0f,  -504.0f,  4.0f);

    for (int c = 0; c < 64; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-206 + 156 * cos((c + 64) * PI / 256)), float(-360 - 156 * sin((c + 64) * PI / 256)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-206 + 156 * cos((c + 65) * PI / 256)), float(-360 - 156 * sin((c + 65) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-206 + 144 * cos((c + 65) * PI / 256)), float(-360 - 144 * sin((c + 65) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-206 + 144 * cos((c + 64) * PI / 256)), float(-360 - 144 * sin((c + 64) * PI / 256)), 4.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-156 + 156 * cos(c       * PI / 256)), float(-310 - 156 * sin(c       * PI / 256)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-156 + 156 * cos((c + 1) * PI / 256)), float(-310 - 156 * sin((c + 1) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-156 + 144 * cos((c + 1) * PI / 256)), float(-310 - 144 * sin((c + 1) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-156 + 144 * cos(c       * PI / 256)), float(-310 - 144 * sin(c       * PI / 256)), 4.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -54.176624f,  -411.823376f,   4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -45.691342f,  -420.308658f,   4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -95.691342f,  -470.308658f,   4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -104.176624f,  -461.823376f,   4.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  100.0f,   0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  120.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  120.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  100.0f,   0.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  140.0f,   0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  145.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  145.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  140.0f,   0.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  145.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  145.0f,   0.0f);
    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  150.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  150.0f,   0.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  150.0f,   0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  225.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  225.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  150.0f,   0.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  225.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  225.0f,   0.0f);
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  230.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  230.0f,   0.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  270.0f,   0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  670.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  670.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  270.0f,   0.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  670.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  670.0f,   0.0f);
    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  675.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  675.0f,   0.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  675.0f,   0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  750.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  750.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  675.0f,   0.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  750.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  750.0f,   0.0f);
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  755.0f,  -0.05f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  755.0f,  -0.05f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  755.0f,  -0.05f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1345.0f,  -5.95f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1345.0f,  -5.95f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  755.0f,  -0.05f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1345.0f,  -5.95f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1345.0f,  -5.95f);
    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1350.0f,  -6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1350.0f,  -6.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1350.0f,  -6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1425.0f,  -6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1425.0f,  -6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1350.0f,  -6.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1425.0f,  -6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1425.0f,  -6.0f);
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1430.0f,  -6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1430.0f,  -6.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1430.0f,  -6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1450.0f,  -6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1450.0f,  -6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1430.0f,  -6.0f);

    for (int c = 128; c > 96; c--) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 106*cos(c      *PI/128)), float(1450.0 + 106*sin(c      *PI/128)),  -6.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 106*cos((c - 1)*PI/128)), float(1450.0 + 106*sin((c - 1)*PI/128)),  -6.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 +  94*cos((c - 1)*PI/128)), float(1450.0 +  94*sin((c - 1)*PI/128)),  -6.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 +  94*cos(c      *PI/128)), float(1450.0 +  94*sin(c      *PI/128)),  -6.0f);

    }

    for (int c = 10; c < 128; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-c      *PI/512)), float(2546.015511 + 681*sin(-c      *PI/512)),  float(11.5 + 0.04 * (-c + 128) * 675 * PI/512));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(c + 1)*PI/512)), float(2546.015511 + 681*sin(-(c + 1)*PI/512)),  float(11.5 + 0.04 * (-c + 127) * 675 * PI/512));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(c + 1)*PI/512)), float(2546.015511 + 669*sin(-(c + 1)*PI/512)),  float(11.5 + 0.04 * (-c + 127) * 675 * PI/512));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-c      *PI/512)), float(2546.015511 + 669*sin(-c      *PI/512)),  float(11.5 + 0.04 * (-c + 128) * 675 * PI/512));

    }

    for (int c = 1; c < 9; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-c      *PI/512)), float(2546.015511 + 681*sin(-c      *PI/512)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(c + 1)*PI/512)), float(2546.015511 + 681*sin(-(c + 1)*PI/512)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(c + 1)*PI/512)), float(2546.015511 + 669*sin(-(c + 1)*PI/512)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-c      *PI/512)), float(2546.015511 + 669*sin(-c      *PI/512)),  float(10.0 + 6.75 * PI));

    }


    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(0 + 1)*PI/512)), float(2546.015511 + 681*sin(-(0 + 1)*PI/512)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(0 + 1)*PI/512)), float(2546.015511 + 669*sin(-(0 + 1)*PI/512)),  float(10.0 + 6.75 * PI));
    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-0      *PI/512)), float(2546.015511 + 669*sin(-0      *PI/512)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-0      *PI/512)), float(2546.015511 + 681*sin(-0      *PI/512)),  float(10.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    //c = 9 correspond à une pente intermédiaire entre 0% et 4%

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-9      *PI/512)), float(2546.015511 + 681*sin(-9      *PI/512)),  float(10.0 + 6.75 * PI                       ));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(9 + 1)*PI/512)), float(2546.015511 + 681*sin(-(9 + 1)*PI/512)),  float(11.5 + 0.04 * (-9 + 127) * 675 * PI/512));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(9 + 1)*PI/512)), float(2546.015511 + 669*sin(-(9 + 1)*PI/512)),  float(11.5 + 0.04 * (-9 + 127) * 675 * PI/512));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-9      *PI/512)), float(2546.015511 + 669*sin(-9      *PI/512)),  float(10.0 + 6.75 * PI                       ));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(   25.046681f, 1524.953319f, -6.00f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   33.531963f, 1516.468037f, -6.00f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  242.128463f, 1725.064537f, -3.05f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  233.643181f, 1733.549819f, -3.05f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  242.128463f, 1725.064537f, -3.05f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  233.643181f, 1733.549819f, -3.05f);
    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  237.178715f, 1737.085353f, -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  245.663997f, 1728.600071f, -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  245.663997f, 1728.600071f, -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  237.178715f, 1737.085353f, -3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  290.211724f, 1790.118362f, -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  298.697006f, 1781.633080f, -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  298.697006f, 1781.633080f, -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  290.211724f, 1790.118362f, -3.0f);
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  293.747258f, 1793.653896f, -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  302.232540f, 1785.168614f, -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  293.747258f, 1793.653896f, -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  302.232540f, 1785.168614f, -3.0f); //on gagne 1.5m de hauteur de chaque côté -> à 4%, cela correspond à 37.5m de longueur
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  325.213510f, 1808.149584f, -3.0f); //couleur restant à appliquer
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  316.728228f, 1816.634866f, -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  325.213510f, 1808.149584f, -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  316.728228f, 1816.634866f, -3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  573.054436f, 2072.961074f, 11.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  581.539718f, 2064.475792f, 11.5f);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 2546.015511f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 2546.015511f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 2621.0f,      float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 2621.0f,      float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 2621.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 2621.0f, float(10.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 2626.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 2626.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 2626.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 2626.0f, float(10.0 + 6.75 * PI));
    glColor3d(0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel());
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 2646.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 2646.0f, float(10.0 + 6.75 * PI));

    /****************************Ombres**********************************/

    int deltamS = QTime(14, 0, 0).msecsTo(QTime::currentTime());

    if (deltamS < -12 * 3600000) deltamS += 86400000; //Désormais deltamS est compris entre -33200000 et +33200000
    alpha = deltamS * PI / 33200000.;

    int d = (QDate::currentDate().dayOfYear() + 10) % 366;
    double hauteurMaxSoleil = 45 - 22 * cos(d * PI/183); //en °
    double mod = -4 * cos(d * PI / 183); //la durée du jour vaut 12h + mod

    if (abs(deltamS) >= (12 + mod) * 3600000 / 2) {

        beta = -1 * PI/180; //Soleil couché

    }

    else {

        beta = PI/180 * hauteurMaxSoleil * (1 - abs(deltamS) / ((12 + mod) * 1800000));

    }

    double cotAlpha = 1/tan(alpha);
    double cotBeta = 1/tan(beta);

    if (alpha <= 0 && abs(cotAlpha) < 29/24 && beta > 0) {

        cotAlpha *= -1;

        for (double abscissePilier = 2661.0; abscissePilier < 2981.0; abscissePilier += 20.0) {
            //base x=781
            glColor3d(1, 1, 0);

            glTexCoord2f(float(max(0, 1 - 0.2 * cotBeta)), 1.0f);                                      glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(abscissePilier + 19.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
            glTexCoord2f(1.0f                    , float(19           /(19 + 12 * cotAlpha))); glVertex3f(  781.0f,                     float(abscissePilier + 19.5),                 float(10.0 + 6.75 * PI));
            glTexCoord2f(1.0f                    , 0.0f);                                      glVertex3f(  781.0f,                     float(abscissePilier + 0.5),                  float(10.0 + 6.75 * PI));
            glTexCoord2f(float(max(0, 1 - 0.2 * cotBeta)), float(12 * cotAlpha/(19 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(abscissePilier + 0.5 + 12 * cotAlpha * cotBeta/5),  float(10.0 + 6.75 * PI));

            glColor3d(0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel());

            glTexCoord2f(float(max(0, 1 - 0.2 * cotBeta)), float(12*cotAlpha/(1 + 12 * cotAlpha)));     glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(abscissePilier - 0.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
            glTexCoord2f(float(max(0, 1 - 0.2 * cotBeta)), 1.0f);                                       glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(abscissePilier + 0.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
            glTexCoord2f(1.0f, 0.0f);                                                           glVertex3f(  781.0f, float(abscissePilier + 0.5), float(10.0 + 6.75 * PI));
            glTexCoord2f(1.0f, float(1/(1 + 12 * cotAlpha)));                                   glVertex3f(  781.0f, float(abscissePilier - 0.5), float(10.0 + 6.75 * PI));

        }

        glTexCoord2f(float(max(0, 1 - 0.2 * cotBeta)), float(12*cotAlpha/(1 + 12 * cotAlpha)));     glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(2980.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(max(0, 1 - 0.2 * cotBeta)), 1.0f);                                       glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(2981.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f);                                                           glVertex3f(  781.0f, 2981.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, float(1/(1 + 12 * cotAlpha)));                                   glVertex3f(  781.0f, 2980.5f, float(10.0 + 6.75 * PI)); //dernière zone d'ombre de pilier

        glTexCoord2f(float(max(0, 1 - 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.7 * cotBeta)), float(2660.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(max(0, 1 - 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.7 * cotBeta)), float(2981.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(max(0, 1 - 0.2   * cotBeta)), float(2.4 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(2981.5 + 12   * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(max(0, 1 - 0.2   * cotBeta)), float(2.4 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.4 * cotBeta)), float(2660.5 + 12   * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI)); //zone d'ombre supérieure

        glColor3d(1, 1, 0);

        glTexCoord2f(0.0f                      , float(12            * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(781 - 12           ), float(2660.5 + 12   * cotAlpha), float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f                      , float(12            * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(781 - 12           ), float(2981.5 + 12   * cotAlpha), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(max(0, 1 - 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.7 * cotBeta)), float(2981.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(max(0, 1 - 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(max(769, 781 - 2.7 * cotBeta)), float(2660.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI)); //zone ensoleillée

        glColor3d(0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel());

        glTexCoord2f(1.0f, 0.0f);                                      glVertex3f(781.0f, 2646.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, float(15/(15 + 12 * cotAlpha)));            glVertex3f(781.0f, 2660.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, float(12 * cotAlpha/(15 + 12 * cotAlpha))); glVertex3f(769.0f, float(2660.5 + 12 * cotAlpha ), float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f);                                      glVertex3f(769.0f, 2646.0f, float(10.0 + 6.75 * PI));

        glTexCoord2f(1.0f, 0.0f);                                      glVertex3f(781.0f, 2996.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, float(15/(15 + 12 * cotAlpha)));            glVertex3f(781.0f, 2981.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, float(12 * cotAlpha/(15 + 12 * cotAlpha))); glVertex3f(769.0f, float(2981.5 + 12 * cotAlpha ), float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f);                                      glVertex3f(769.0f, 2996.0f, float(10.0 + 6.75 * PI));

    }

    else if (alpha < PI && abs(cotAlpha) < 29/24 && beta > 0) {

        for (double abscissePilier = 2661.0; abscissePilier < 2981.0; abscissePilier += 20.0) {
            //base x=769
            glColor3d(1, 1, 0);

            glTexCoord2f(0.0f                , 0.0f);                                      glVertex3f(  769.0f,                     float(abscissePilier + 0.5),                  float(10.0 + 6.75 * PI));
            glTexCoord2f(float(min(1, 0.2 * cotBeta)), float(12 * cotAlpha/(19 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(abscissePilier + 0.5 + 12 * cotAlpha * cotBeta/5),  float(10.0 + 6.75 * PI));
            glTexCoord2f(float(min(1, 0.2 * cotBeta)), 1.0f);                                      glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(abscissePilier + 19.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
            glTexCoord2f(0.0f                , float(19           /(19 + 12 * cotAlpha))); glVertex3f(  769.0f,                     float(abscissePilier + 19.5),                 float(10.0 + 6.75 * PI));


            glColor3d(0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel());

            glTexCoord2f(0.0f, 0.0f);                                                     glVertex3f(  769.0f, float(abscissePilier + 0.5), float(10.0 + 6.75 * PI));
            glTexCoord2f(0.0f, float(1/(1 + 12 * cotAlpha)));                             glVertex3f(  769.0f, float(abscissePilier - 0.5), float(10.0 + 6.75 * PI));
            glTexCoord2f(float(min(1, 0.2 * cotBeta)), float(12 * cotAlpha/(1 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(abscissePilier - 0.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
            glTexCoord2f(float(min(1, 0.2 * cotBeta)), 1.0f);                                     glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(abscissePilier + 0.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));

        }

        glTexCoord2f(0.0f, 0.0f);                                                     glVertex3f(  769.0f, 2981.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, float(1/(1 + 12 * cotAlpha)));                             glVertex3f(  769.0f, 2980.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.2 * cotBeta)), float(12 * cotAlpha/(1 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(2980.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.2 * cotBeta)), 1.0f);                                     glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(2981.5 + 12 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI)); //dernière zone d'ombre de pilier

        glTexCoord2f(float(min(1, 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.7 * cotBeta)), float(2660.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.7 * cotBeta)), float(2981.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.2   * cotBeta)), float(2.4 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(2981.5 + 12   * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.2   * cotBeta)), float(2.4 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.4 * cotBeta)), float(2660.5 + 12   * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI)); //zone d'ombre supérieure

        glColor3d(1, 1, 0);

        glTexCoord2f(1.0f                  , float(12            * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f( 781.0f, float(2660.5 + 12 * cotAlpha ), float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f                  , float(12            * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f( 781.0f, float(2981.5 + 12 * cotAlpha ), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.7 * cotBeta)), float(2981.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI));
        glTexCoord2f(float(min(1, 0.225 * cotBeta)), float(2.7 * cotBeta * cotAlpha/(321 + 12 * cotAlpha))); glVertex3f(  float(min(781, 769 + 2.7 * cotBeta)), float(2660.5 + 13.5 * cotAlpha * cotBeta/5), float(10.0 + 6.75 * PI)); //zone ensoleillée

        glColor3d(0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel(), 0.21 + 0.69 * bleuCiel());

        glTexCoord2f(0.0f, 0.0f);                                      glVertex3f(769.0f, 2646.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, float(15/(15 + 12 * cotAlpha)));            glVertex3f(769.0f, 2660.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, float(12 * cotAlpha/(15 + 12 * cotAlpha))); glVertex3f(781.0f, float(2660.5 + 12 * cotAlpha ), float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f);                                      glVertex3f(781.0f, 2646.0f, float(10.0 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f);                                      glVertex3f(769.0f, 2996.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, float(15/(15 + 12 * cotAlpha)));            glVertex3f(769.0f, 2981.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, float(12 * cotAlpha/(15 + 12 * cotAlpha))); glVertex3f(781.0f, float(2981.5 + 12 * cotAlpha ), float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f);                                      glVertex3f(781.0f, 2996.0f, float(10.0 + 6.75 * PI));

    }

    else {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(769.0f, 2646.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(781.0f, 2646.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(781.0f, 2996.0f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(769.0f, 2996.0f, float(10.0 + 6.75 * PI));

    }

    /***************************Fin************************************/

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 2996.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 2996.0f, float(10.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 3016.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 3016.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 3016.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 3016.0f, float(10.0 + 6.75 * PI));
    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 3021.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 3021.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 3021.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 3021.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 3096.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 3096.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 69*cos(128      *PI/128)), float(3096.0 + 69*sin(128      *PI/128)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 81*cos(128      *PI/128)), float(3096.0 + 81*sin(128      *PI/128)),  float(10.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 81*cos((128 - 1)*PI/128)), float(3096.0 + 81*sin((128 - 1)*PI/128)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 69*cos((128 - 1)*PI/128)), float(3096.0 + 69*sin((128 - 1)*PI/128)),  float(10.0 + 6.75 * PI));

    for (int c = 127; c > 64; c--) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 81*cos(c      *PI/128)), float(3096.0 + 81*sin(c      *PI/128)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 81*cos((c - 1)*PI/128)), float(3096.0 + 81*sin((c - 1)*PI/128)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 69*cos((c - 1)*PI/128)), float(3096.0 + 69*sin((c - 1)*PI/128)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 69*cos(c      *PI/128)), float(3096.0 + 69*sin(c      *PI/128)),  float(10.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 850.0f, 3165.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 850.0f, 3177.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 890.0f, 3177.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 890.0f, 3165.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 910.0f, 3165.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 910.0f, 3177.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 950.0f, 3177.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 950.0f, 3165.0f, float(10.0 + 6.75 * PI));

    for (int c = -96; c < -32; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(950.0 + 106*cos(c      *PI/192)), float(3271.0 + 106*sin(c      *PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(950.0 + 106*cos((c + 1)*PI/192)), float(3271.0 + 106*sin((c + 1)*PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(950.0 +  94*cos((c + 1)*PI/192)), float(3271.0 +  94*sin((c + 1)*PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(950.0 +  94*cos(c      *PI/192)), float(3271.0 +  94*sin(c      *PI/192)),  float(10.0 + 6.75 * PI));

    }

    for (int c = 160; c > 96; c--) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(1123.205081 + 106*cos(c      *PI/192)), float(3171.0 + 106*sin(c      *PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(1123.205081 + 106*cos((c - 1)*PI/192)), float(3171.0 + 106*sin((c - 1)*PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(1123.205081 +  94*cos((c - 1)*PI/192)), float(3171.0 +  94*sin((c - 1)*PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(1123.205081 +  94*cos(c      *PI/192)), float(3171.0 +  94*sin(c      *PI/192)),  float(10.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1123.205081f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1123.205081f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1135.0f     , 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1135.0f     , 3265.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1135.0f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1135.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3265.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3265.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1220.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1220.0f, 3265.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1220.0f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1220.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1240.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1240.0f, 3265.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1240.0f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1240.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1260.0f, 3277.0f, float( 9.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1260.0f, 3265.0f, float( 9.8 + 6.75 * PI)); //pente de 1%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1260.0f, 3265.0f, float( 9.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1260.0f, 3277.0f, float( 9.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1410.0f, 3277.0f, float( 6.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1410.0f, 3265.0f, float( 6.8 + 6.75 * PI)); //pente de 2%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1410.0f, 3265.0f, float( 6.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1410.0f, 3277.0f, float( 6.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1560.0f, 3277.0f, float( 2.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1560.0f, 3265.0f, float( 2.3 + 6.75 * PI));  //pente de 3%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1560.0f     , 3265.0f,  float(2.3 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1560.0f     , 3277.0f,  float(2.3 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1713.333333f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1713.333333f, 3265.0f,  float(0.0 + 6.75 * PI)); //pente de 1.5%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1713.333333f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1713.333333f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1720.0f     , 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1720.0f     , 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1720.0f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1720.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1805.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1805.0f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1805.0f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1805.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1850.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1850.0f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1870.0f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1870.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2000.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2000.0f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2190.0f     , 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2190.0f     , 3277.0f,  float(0.0 + 6.75 * PI));    
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2194.652123f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2194.652123f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2194.652123f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2194.652123f, 3277.0f,  float(0.0 + 6.75 * PI));
    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2199.652123f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3265.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2199.652123f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2274.652123f, 3265.0f,  float(0.0 + 6.75 * PI));

    glColor3d(0.5, 0.5, 0.5);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2090.0f, 3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2090.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2170.0f, 3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2170.0f, 3265.0f,  float(0.0 + 6.75 * PI));


    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture3[2]);

    glBegin(GL_QUADS);

    //demi-rails

    for (double c = 0.0; c > - 10 * PI; c -= 0.25 * PI) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  -12.0f, float(c - 120)            , float(-1.6 + 0.8 * sin(c              /20 - PI/2)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  -12.0f, float(c - 120 - 0.25 * PI), float(-1.6 + 0.8 * sin((c - 0.25 * PI)/20 - PI/2)));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(   -6.0f, float(c - 120 - 0.25 * PI), float(-1.6 + 0.8 * sin((c - 0.25 * PI)/20 - PI/2)));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(   -6.0f, float(c - 120)            , float(-1.6 + 0.8 * sin(c              /20 - PI/2)));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  -12.0f, float(-c - 240 - 20    * PI), float(3.2 + 0.8 * sin(c              /20 + PI/2)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  -12.0f, float(-c - 240 - 19.75 * PI), float(3.2 + 0.8 * sin((c - 0.25 * PI)/20 + PI/2)));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(   -6.0f, float(-c - 240 - 19.75 * PI), float(3.2 + 0.8 * sin((c - 0.25 * PI)/20 + PI/2)));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(   -6.0f, float(-c - 240 - 20    * PI), float(3.2 + 0.8 * sin(c              /20 + PI/2)));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -12.0f, float(-120 - 10 * PI), -1.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -12.0f, float(-240 - 10 * PI),  3.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   -6.0f, float(-240 - 10 * PI),  3.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   -6.0f, float(-120 - 10 * PI), -1.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -310.0f              , 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -6.0f, -310.0f              , 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -6.0f, float(-240 - 20 * PI), 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, float(-240 - 20 * PI), 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -310.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -310.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -100.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -100.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -310.0f, -2.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -310.0f, -2.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -120.0f, -2.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -120.0f, -2.4f);



    for (int c = 0; c < 100; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f, float(-100 + 2 * c), float(2 + 2* sin(-c       * PI/100 + PI/2)));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  -6.0f, float(-100 + 2 * c), float(2 + 2* sin(-c       * PI/100 + PI/2)));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  -6.0f, float( -98 + 2 * c), float(2 + 2* sin(-(c + 1) * PI/100 + PI/2)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f, float( -98 + 2 * c), float(2 + 2* sin(-(c + 1) * PI/100 + PI/2)));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -6.0f, -100.0f    , -2.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -100.0f    , -2.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, -34.510867f, -2.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -6.0f, -34.510867f, -2.4f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -100.0f    , -2.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -100.0f    , -2.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -68.620983f, -2.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -68.620983f, -2.4f);

    for (int c = 0; c < 20; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-100 + 100 * cos(c       * 0.0122783)), float(-20 -100 * sin(c       * 0.0122783)), -2.4f); //0.245566 = Arccos(0.97), sachant que ce morceau de courbe de rayon 100m sert à "gagner" 3m vers la droite (depuis République)
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float( -94 + 100 * cos(c       * 0.0122783)), float(-20 -100 * sin(c       * 0.0122783)), -2.4f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float( -94 + 100 * cos((c + 1) * 0.0122783)), float(-20 -100 * sin((c + 1) * 0.0122783)), -2.4f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-100 + 100 * cos((c + 1) * 0.0122783)), float(-20 -100 * sin((c + 1) * 0.0122783)), -2.4f);

    }

    for (int c = 0; c < 20; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float( 94 - 100 * cos(c       * 0.0122783)), float(-68.620983 + 100 * sin(c       * 0.0122783)), -2.4f); //0.245566 = Arccos(0.97), sachant que ce morceau de courbe de rayon 100m sert à "gagner" 3m vers la droite (depuis République)
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(100 - 100 * cos(c       * 0.0122783)), float(-68.620983 + 100 * sin(c       * 0.0122783)), -2.4f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(100 - 100 * cos((c + 1) * 0.0122783)), float(-68.620983 + 100 * sin((c + 1) * 0.0122783)), -2.4f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float( 94 - 100 * cos((c + 1) * 0.0122783)), float(-68.620983 + 100 * sin((c + 1) * 0.0122783)), -2.4f);

    }

    for (int c = 0; c < 10; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(91 - 100 * cos(c       * 0.0173422)), float(-100 * sin(c       * 0.0173422)), -2.4f); //0.173422 = Arccos(0.985), sachant que ce morceau de courbe de rayon 100m sert à "gagner" 1.5m vers la gauche (depuis République)
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(85 - 100 * cos(c       * 0.0173422)), float(-100 * sin(c       * 0.0173422)), -2.4f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(85 - 100 * cos((c + 1) * 0.0173422)), float(-100 * sin((c + 1) * 0.0173422)), -2.4f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(91 - 100 * cos((c + 1) * 0.0173422)), float(-100 * sin((c + 1) * 0.0173422)), -2.4f);

    }

    for (int c = 0; c < 10; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-106 + 100 * cos(c       * 0.0173422)), float(-34.510867 + 100 * sin(c       * 0.0173422)), -2.4f); //0.173422 = Arccos(0.985), sachant que ce morceau de courbe de rayon 100m sert à "gagner" 1.5m vers la gauche (depuis République)
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-112 + 100 * cos(c       * 0.0173422)), float(-34.510867 + 100 * sin(c       * 0.0173422)), -2.4f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-112 + 100 * cos((c + 1) * 0.0173422)), float(-34.510867 + 100 * sin((c + 1) * 0.0173422)), -2.4f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-106 + 100 * cos((c + 1) * 0.0173422)), float(-34.510867 + 100 * sin((c + 1) * 0.0173422)), -2.4f);

    }

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 0.0f, -2.4f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 0.0f, -2.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 5.0f, -2.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 5.0f, -2.4f);

    for (int c = 0; c < 60; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, float( 5 + 2 * c), float(-1.2 + 1.2* sin(c       * PI/60 - PI/2)));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, float( 5 + 2 * c), float(-1.2 + 1.2* sin(c       * PI/60 - PI/2)));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, float( 7 + 2 * c), float(-1.2 + 1.2* sin((c + 1) * PI/60 - PI/2)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, float( 7 + 2 * c), float(-1.2 + 1.2* sin((c + 1) * PI/60 - PI/2)));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  0.0f, float( -20 + 2 * c), float(-1.2 + 1.2* sin(c       * PI/60 - PI/2)));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, float( -20 + 2 * c), float(-1.2 + 1.2* sin(c       * PI/60 - PI/2)));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, float( -18 + 2 * c), float(-1.2 + 1.2* sin((c + 1) * PI/60 - PI/2)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  0.0f, float( -18 + 2 * c), float(-1.2 + 1.2* sin((c + 1) * PI/60 - PI/2)));

    }

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 125.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 125.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 145.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 145.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 145.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 145.0f, 0.0f);
    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 150.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 150.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 225.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 225.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 225.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 225.0f, 0.0f);
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 230.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 230.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 230.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 230.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 250.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 250.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 270.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f, 270.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f, 345.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 345.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2000.0f, 3271.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2000.0f, 3277.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2090.0f, 3277.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2090.0f, 3271.0f, float(0.0 + 6.75 * PI));

    for (int c = 96; c < 144; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2000.0 + 100*cos(c      *PI/288)), float(3171.0 + 100*sin(c      *PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2000.0 + 100*cos((c + 1)*PI/288)), float(3171.0 + 100*sin((c + 1)*PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2000.0 +  94*cos((c + 1)*PI/288)), float(3171.0 +  94*sin((c + 1)*PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2000.0 +  94*cos(c      *PI/288)), float(3171.0 +  94*sin(c      *PI/288)),   float(0.0 + 6.75 * PI)); //voie unique

    }

    for (int c = -192; c < -144; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2100.0 + 100*cos(c      *PI/288)), float(3344.205081 + 100*sin(c      *PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2100.0 + 100*cos((c + 1)*PI/288)), float(3344.205081 + 100*sin((c + 1)*PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2100.0 + 106*cos((c + 1)*PI/288)), float(3344.205081 + 106*sin((c + 1)*PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2100.0 + 106*cos(c      *PI/288)), float(3344.205081 + 106*sin(c      *PI/288)),   float(0.0 + 6.75 * PI)); //voie unique

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2100.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2100.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2175.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2175.0f, 3244.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2195.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2195.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2245.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2245.0f, 3244.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2100.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2100.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2175.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2175.0f, 3253.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2195.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2195.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2245.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2245.0f, 3253.205081f, float(0.0 + 6.75 * PI)); //il faut aller jusqu'à 2625!

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2245.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2245.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3244.205081f, float(0.0 + 6.75 * PI));


    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 57 * cos(144 * PI/288)), float(3181.205081 + 57 * sin(144 * PI/288)),   float(0.0 - 0 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 63 * cos(144 * PI/288)), float(3181.205081 + 63 * sin(144 * PI/288)),   float(0.0 - 0 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glColor3d(0.25 + 0.5 * luminanceStations[9], 0.25 + 0.5 * luminanceStations[9], 0.25 + 0.5 * luminanceStations[9]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 63 * cos(143 * PI/288)), float(3181.205081 + 63 * sin(143 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 57 * cos(143 * PI/288)), float(3181.205081 + 57 * sin(143 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 57 * cos(143 * PI/288)), float(3181.205081 + 57 * sin(143 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 63 * cos(143 * PI/288)), float(3181.205081 + 63 * sin(143 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 63 * cos(142 * PI/288)), float(3181.205081 + 63 * sin(142 * PI/288)),   float(0.0 - 2 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 57 * cos(142 * PI/288)), float(3181.205081 + 57 * sin(142 * PI/288)),   float(0.0 - 2 * 0.05 * 60 * PI/288 + 6.75 * PI));


    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2245.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2245.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3253.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3253.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2330.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2330.0f, 3253.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2330.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2330.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2625.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2625.0f, 3253.205081f, float(0.0 + 6.75 * PI));

    for (int c = 96; c < 142; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 57 * cos(c       * PI/288)), float(3181.205081 + 57 * sin(c       * PI/288)),   float(0.0 + (-144 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 57 * cos((c + 1) * PI/288)), float(3181.205081 + 57 * sin((c + 1) * PI/288)),   float(0.0 + (-143 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 63 * cos((c + 1) * PI/288)), float(3181.205081 + 63 * sin((c + 1) * PI/288)),   float(0.0 + (-143 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 63 * cos(c       * PI/288)), float(3181.205081 + 63 * sin(c       * PI/288)),   float(0.0 + (-144 + c) * 0.05 * 60 * PI/288 + 6.75 * PI)); //voie unique, boucle (descente)

    }

    for (int c = -192; c < -72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/288)), float(3285.128129 + 57*sin(c      *PI/288)),   float(0.0 + (-192 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/288)), float(3285.128129 + 57*sin((c + 1)*PI/288)),   float(0.0 + (-193 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/288)), float(3285.128129 + 63*sin((c + 1)*PI/288)),   float(0.0 + (-193 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/288)), float(3285.128129 + 63*sin(c      *PI/288)),   float(0.0 + (-192 - c) * 0.05 * 60 * PI/288 + 6.25 * PI)); //voie unique, boucle (hauteur stable sous la voie de garage)

    }

    for (int c = -72; c < 72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/288)), float(3285.128129 + 57*sin(c      *PI/288)),   float(0.0 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/288)), float(3285.128129 + 57*sin((c + 1)*PI/288)),   float(0.0 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/288)), float(3285.128129 + 63*sin((c + 1)*PI/288)),   float(0.0 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/288)), float(3285.128129 + 63*sin(c      *PI/288)),   float(0.0 + 5.00 * PI)); //voie unique, boucle (montée)

    }

    for (int c = 72; c < 240; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/288)), float(3285.128129 + 57*sin(c      *PI/288)),   float(0.0 + (-72 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/288)), float(3285.128129 + 57*sin((c + 1)*PI/288)),   float(0.0 + (-71 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/288)), float(3285.128129 + 63*sin((c + 1)*PI/288)),   float(0.0 + (-71 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/288)), float(3285.128129 + 63*sin(c      *PI/288)),   float(0.0 + (-72 + c) * 0.05 * 60 * PI/288 + 5.00 * PI)); //voie unique

    }

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2330.435387f, 3316.628129f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2335.636844f, 3313.628129f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2329.209818f, 3302.5f     , float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2324.017898f, 3305.5f     , float(0.0 + 6.75 * PI)); //TB

    for (int c = -140; c < -48; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57 * cos(c       * PI/288)), float(3334.0 + 57 * sin(c       * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57 * cos((c + 1) * PI/288)), float(3334.0 + 57 * sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63 * cos((c + 1) * PI/288)), float(3334.0 + 63 * sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63 * cos(c       * PI/288)), float(3334.0 + 63 * sin(c       * PI/288)),   float(0.0 + 6.75 * PI)); //voie unique

    }

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63 * cos(-144 * PI/288)), float(3334.0 + 63 * sin(-144 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57 * cos(-144 * PI/288)), float(3334.0 + 57 * sin(-144 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.125 + 0.75 * luminanceStations[10], 0.125 + 0.75 * luminanceStations[10], 0.125 + 0.75 * luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57 * cos(-143 * PI/288)), float(3334.0 + 57 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63 * cos(-143 * PI/288)), float(3334.0 + 63 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63 * cos(-143 * PI/288)), float(3334.0 + 63 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57 * cos(-143 * PI/288)), float(3334.0 + 57 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.25 + 0.5 * luminanceStations[10], 0.25 + 0.5 * luminanceStations[10], 0.25 + 0.5 * luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57 * cos(-142 * PI/288)), float(3334.0 + 57 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63 * cos(-142 * PI/288)), float(3334.0 + 63 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63 * cos(-142 * PI/288)), float(3334.0 + 63 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57 * cos(-142 * PI/288)), float(3334.0 + 57 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.375 + 0.25 * luminanceStations[10], 0.375 + 0.25 * luminanceStations[10], 0.375 + 0.25 * luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57 * cos(-141 * PI/288)), float(3334.0 + 57 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63 * cos(-141 * PI/288)), float(3334.0 + 63 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63 * cos(-141 * PI/288)), float(3334.0 + 63 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57 * cos(-141 * PI/288)), float(3334.0 + 57 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57 * cos(-140 * PI/288)), float(3334.0 + 57 * sin(-140 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63 * cos(-140 * PI/288)), float(3334.0 + 63 * sin(-140 * PI/288)), float(0.0 + 6.75 * PI));


    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2274.652123f, 3265.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2274.652123f, 3271.0f, float(0.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3271.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3265.0f, float(0.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f(2279.652123f, 3265.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2279.652123f, 3271.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2574.652123f, 3271.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2574.652123f, 3265.0f, float(0.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture3[5]);

    glBegin(GL_QUADS);

    glColor3d(1, 1, 1);

    //plafond de tunnel

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -730.0f, -315.0f, 9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -315.0f, 9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -525.0f, 9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -730.0f, -525.0f, 9.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -205.9f, -503.9f, 9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -503.9f, 9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -516.1f, 9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -205.9f, -516.1f, 9.0f);

    for (int c = 0; c < 64; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-206 + 156 * cos((c + 64) * PI / 256)), float(-360 - 156 * sin((c + 64) * PI / 256)), 9.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-206 + 156 * cos((c + 65) * PI / 256)), float(-360 - 156 * sin((c + 65) * PI / 256)), 9.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-206 + 143 * cos((c + 65) * PI / 256)), float(-360 - 143 * sin((c + 65) * PI / 256)), 9.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-206 + 143 * cos((c + 64) * PI / 256)), float(-360 - 143 * sin((c + 64) * PI / 256)), 9.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-156 + 156 * cos(c       * PI / 256)), float(-310 - 156 * sin(c       * PI / 256)), 9.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-156 + 156 * cos((c + 1) * PI / 256)), float(-310 - 156 * sin((c + 1) * PI / 256)), 9.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-156 + 143 * cos((c + 1) * PI / 256)), float(-310 - 143 * sin((c + 1) * PI / 256)), 9.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-156 + 143 * cos(c       * PI / 256)), float(-310 - 143 * sin(c       * PI / 256)), 9.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -54.176624f,  -411.823376f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -45.691342f,  -420.308658f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -95.691342f,  -470.308658f,   9.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -104.176624f,  -461.823376f,   9.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(   -6.0f,  -310.0f,   1.9f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(   -6.0f,  -100.0f,   1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(    0.0f,  -100.0f,   1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(    0.0f,  -310.0f,   1.9f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -12.0f,  -310.0f,   9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -12.0f,  -100.0f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(    0.0f,  -100.0f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(    0.0f,  -310.0f,   9.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  -100.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,     0.0f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   6.0f,     0.0f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   6.0f,  -100.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  -150.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  -100.0f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f,  -100.0f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f,  -150.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f,  -100.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f,  -100.0f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -6.0f,  -100.0f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -6.0f,  -100.0f,  1.9f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f,  -100.0f,   9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f,   100.0f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   0.0f,   100.0f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   0.0f,  -100.0f,   9.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  0.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  0.0f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.0f,  0.0f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.0f,  0.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,  0.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f,  0.0f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   6.0f,  0.0f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   6.0f,  0.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f,  100.0f,  5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f,  100.0f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   0.0f,  100.0f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   0.0f,  100.0f,  5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,    0.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  150.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.0f,  150.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.0f,    0.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f,    0.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,  150.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   6.0f,  150.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   6.0f,    0.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.0f,   100.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.0f,   150.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   6.0f,   150.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   6.0f,   100.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  225.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  400.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   6.0f,  400.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   6.0f,  225.0f,   5.0f);


    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  400.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  548.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  548.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  400.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  552.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  548.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.0f,  548.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  552.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  675.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  552.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  552.0f,   5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  675.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  2.0f,  552.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  2.0f,  548.0f,   5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  548.0f,   5.0f); //création d'une aération 4*4m
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  552.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.0f,  548.0f,   4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.0f,  548.0f,   5.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  2.0f,  548.0f,   5.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  2.0f,  548.0f,   4.8f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.0f,  552.0f,   4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.0f,  552.0f,   5.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  2.0f,  552.0f,   5.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  2.0f,  552.0f,   4.8f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.0f,  548.0f,   4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.0f,  548.0f,   5.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.0f,  552.0f,   5.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  552.0f,   4.8f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  2.0f,  548.0f,   4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  2.0f,  548.0f,   5.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  2.0f,  552.0f,   5.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  2.0f,  552.0f,   4.8f);


    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  750.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1350.0f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1350.0f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  750.0f,   5.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1425.0f,  -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1450.0f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1450.0f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1425.0f,  -1.0f);

    for (int c = 64; c > 48; c--) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 106.1*cos(c      *PI/64)), float(1450.0 + 106.1*sin(c      *PI/64)),  -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 106.1*cos((c - 1)*PI/64)), float(1450.0 + 106.1*sin((c - 1)*PI/64)),  -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 +  93.9*cos((c - 1)*PI/64)), float(1450.0 +  93.9*sin((c - 1)*PI/64)),  -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 +  93.9*cos(c      *PI/64)), float(1450.0 +  93.9*sin(c      *PI/64)),  -1.0f);

    }

    for (int c = 3; c < 32; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 681.2*cos(-c      *PI/128)), float(2546.015511 + 681.2*sin(-c      *PI/128)),  float(16.5 + 0.04 * (-c + 32) * 675 * PI/128));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 681.2*cos(-(c + 1)*PI/128)), float(2546.015511 + 681.2*sin(-(c + 1)*PI/128)),  float(16.5 + 0.04 * (-c + 31) * 675 * PI/128));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.8*cos(-(c + 1)*PI/128)), float(2546.015511 + 668.8*sin(-(c + 1)*PI/128)),  float(16.5 + 0.04 * (-c + 31) * 675 * PI/128));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.8*cos(-c      *PI/128)), float(2546.015511 + 668.8*sin(-c      *PI/128)),  float(16.5 + 0.04 * (-c + 32) * 675 * PI/128));

    }

    for (int c = 0; c < 2; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 681.2*cos(-c      *PI/128)), float(2546.015511 + 681.2*sin(-c      *PI/128)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 681.2*cos(-(c + 1)*PI/128)), float(2546.015511 + 681.2*sin(-(c + 1)*PI/128)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.8*cos(-(c + 1)*PI/128)), float(2546.015511 + 668.8*sin(-(c + 1)*PI/128)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.8*cos(-c      *PI/128)), float(2546.015511 + 668.8*sin(-c      *PI/128)),  float(15.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 681.2*cos(-2      *PI/128)), float(2546.015511 + 681.2*sin(-2      *PI/128)),  float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 681.2*cos(-(2 + 1)*PI/128)), float(2546.015511 + 681.2*sin(-(2 + 1)*PI/128)),  float(16.5 + 0.04 * (-2 + 31) * 675 * PI/128));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.8*cos(-(2 + 1)*PI/128)), float(2546.015511 + 668.8*sin(-(2 + 1)*PI/128)),  float(16.5 + 0.04 * (-2 + 31) * 675 * PI/128));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.8*cos(-2      *PI/128)), float(2546.015511 + 668.8*sin(-2      *PI/128)),  float(15.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f(   25.046681f, 1524.953319f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   33.531963f, 1516.468037f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  245.663997f, 1728.600071f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  237.178715f, 1737.085353f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  290.211724f, 1790.118362f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  298.697006f, 1781.633080f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  325.213510f, 1808.149584f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  316.728228f, 1816.634866f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  325.213510f, 1808.149584f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  316.728228f, 1816.634866f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  573.054436f, 2072.961074f, 16.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  581.539718f, 2064.475792f, 16.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 2621.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 2621.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 2641.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 3021.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 3021.0f, float(15.0 + 6.75 * PI));

    for (int c = 32; c > 16; c--) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 81.1*cos(c      *PI/32)), float(3096.0 + 81.1*sin(c      *PI/32)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 81.1*cos((c - 1)*PI/32)), float(3096.0 + 81.1*sin((c - 1)*PI/32)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 68.9*cos((c - 1)*PI/32)), float(3096.0 + 68.9*sin((c - 1)*PI/32)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 68.9*cos(c      *PI/32)), float(3096.0 + 68.9*sin(c      *PI/32)),  float(15.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  850.0f, 3165.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  850.0f, 3177.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  950.0f, 3177.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  950.0f, 3165.0f, float(15.0 + 6.75 * PI));

    for (int c = -36; c < -12; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(950.0 + 106.1*cos(c      *PI/72)), float(3271.0 + 106.1*sin(c      *PI/72)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(950.0 + 106.1*cos((c + 1)*PI/72)), float(3271.0 + 106.1*sin((c + 1)*PI/72)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(950.0 +  93.9*cos((c + 1)*PI/72)), float(3271.0 +  93.9*sin((c + 1)*PI/72)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(950.0 +  93.9*cos(c      *PI/72)), float(3271.0 +  93.9*sin(c      *PI/72)),  float(15.0 + 6.75 * PI));

    }

    for (int c = 60; c > 36; c--) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(1123.205081 + 106.1*cos(c      *PI/72)), float(3171.0 + 106.1*sin(c      *PI/72)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(1123.205081 + 106.1*cos((c - 1)*PI/72)), float(3171.0 + 106.1*sin((c - 1)*PI/72)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(1123.205081 +  93.9*cos((c - 1)*PI/72)), float(3171.0 +  93.9*sin((c - 1)*PI/72)),  float(15.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(1123.205081 +  93.9*cos(c      *PI/72)), float(3171.0 +  93.9*sin(c      *PI/72)),  float(15.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1123.205081f, 3265.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1123.205081f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f     , 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f     , 3265.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1240.0f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1240.0f, 3265.0f, float(15.0 + 6.75 * PI)); //fin de la station: 1215

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1240.0f, 3265.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1240.0f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1260.0f, 3277.0f, float(14.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1260.0f, 3265.0f, float(14.8 + 6.75 * PI)); //pente de 1%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1260.0f, 3265.0f, float(14.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1260.0f, 3277.0f, float(14.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1410.0f, 3277.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1410.0f, 3265.0f, float(11.8 + 6.75 * PI)); //pente de 2%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1410.0f, 3265.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1410.0f, 3277.0f, float(11.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1560.0f, 3277.0f, float( 7.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1560.0f, 3265.0f, float( 7.3 + 6.75 * PI));  //pente de 3%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1560.0f     , 3265.0f,  float(7.3 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1560.0f     , 3277.0f,  float(7.3 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1713.333333f, 3277.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1713.333333f, 3265.0f,  float(5.0 + 6.75 * PI)); //pente de 1.5%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1713.333333f, 3265.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1713.333333f, 3277.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f     , 3277.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f     , 3265.0f,  float(5.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f     , 3265.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f     , 3277.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2199.652123f, 3277.0f,  float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3265.0f,  float(5.0 + 6.75 * PI));

    for (int c = 24; c < 36; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2000.0 + 100.1*cos(c      *PI/72)), float(3171.0 + 100.1*sin(c      *PI/72)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2000.0 + 100.1*cos((c + 1)*PI/72)), float(3171.0 + 100.1*sin((c + 1)*PI/72)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2000.0 +  93.9*cos((c + 1)*PI/72)), float(3171.0 +  93.9*sin((c + 1)*PI/72)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2000.0 +  93.9*cos(c      *PI/72)), float(3171.0 +  93.9*sin(c      *PI/72)),   float(5.0 + 6.75 * PI)); //voie unique

    }

    for (int c = -48; c < -36; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2100.0 +  99.9*cos(c      *PI/72)), float(3344.205081 +  99.9*sin(c      *PI/72)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2100.0 +  99.9*cos((c + 1)*PI/72)), float(3344.205081 +  99.9*sin((c + 1)*PI/72)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2100.0 + 106.1*cos((c + 1)*PI/72)), float(3344.205081 + 106.1*sin((c + 1)*PI/72)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2100.0 + 106.1*cos(c      *PI/72)), float(3344.205081 + 106.1*sin(c      *PI/72)),   float(5.0 + 6.75 * PI)); //voie unique

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2100.0f, 3238.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2100.0f, 3253.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3253.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3238.205081f, float(5.0 + 6.75 * PI));

    for (int c = 96; c < 144; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 56.9*cos(c      *PI/288)), float(3181.205081 + 56.9*sin(c      *PI/288)),  float(5.0 + 6.75 * PI)); // float(5.0 + (-144 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 56.9*cos((c + 1)*PI/288)), float(3181.205081 + 56.9*sin((c + 1)*PI/288)),  float(5.0 + 6.75 * PI)); // float(5.0 + (-143 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 63.1*cos((c + 1)*PI/288)), float(3181.205081 + 63.1*sin((c + 1)*PI/288)),  float(5.0 + 6.75 * PI)); // float(5.0 + (-143 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 63.1*cos(c      *PI/288)), float(3181.205081 + 63.1*sin(c      *PI/288)),  float(5.0 + 6.75 * PI)); // float(5.0 + (-144 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));

    }

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 56.9*cos(96      *PI/288)), float(3181.205081 + 56.9*sin(96      *PI/288)),  float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 56.9*cos(96      *PI/288)), float(3181.205081 + 56.9*sin(96      *PI/288)),  float(5.0 + (-144 + 96) * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 63.1*cos(96      *PI/288)), float(3181.205081 + 63.1*sin(96      *PI/288)),  float(5.0 + (-144 + 96) * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 63.1*cos(96      *PI/288)), float(3181.205081 + 63.1*sin(96      *PI/288)),  float(5.0 + 6.75 * PI));

    for (int c = -192; c < -72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 56.9*cos(c      *PI/288)), float(3285.128129 + 56.9*sin(c      *PI/288)),   float(5.0 + (-192 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 56.9*cos((c + 1)*PI/288)), float(3285.128129 + 56.9*sin((c + 1)*PI/288)),   float(5.0 + (-193 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63.1*cos((c + 1)*PI/288)), float(3285.128129 + 63.1*sin((c + 1)*PI/288)),   float(5.0 + (-193 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63.1*cos(c      *PI/288)), float(3285.128129 + 63.1*sin(c      *PI/288)),   float(5.0 + (-192 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));

    }

    for (int c = -72; c < 72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 56.9*cos(c      *PI/288)), float(3285.128129 + 56.9*sin(c      *PI/288)),   float(5.0 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 56.9*cos((c + 1)*PI/288)), float(3285.128129 + 56.9*sin((c + 1)*PI/288)),   float(5.0 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63.1*cos((c + 1)*PI/288)), float(3285.128129 + 63.1*sin((c + 1)*PI/288)),   float(5.0 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63.1*cos(c      *PI/288)), float(3285.128129 + 63.1*sin(c      *PI/288)),   float(5.0 + 5.00 * PI));

    }

    for (int c = 72; c < 240; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 56.9*cos(c      *PI/288)), float(3285.128129 + 56.9*sin(c      *PI/288)),   float(5.0 + (-72 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 56.9*cos((c + 1)*PI/288)), float(3285.128129 + 56.9*sin((c + 1)*PI/288)),   float(5.0 + (-71 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63.1*cos((c + 1)*PI/288)), float(3285.128129 + 63.1*sin((c + 1)*PI/288)),   float(5.0 + (-71 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63.1*cos(c      *PI/288)), float(3285.128129 + 63.1*sin(c      *PI/288)),   float(5.0 + (-72 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));

    }

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2330.435387f, 3316.628129f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2335.636844f, 3313.628129f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2329.209818f, 3302.5f     , float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2324.017898f, 3305.5f     , float(5.0 + 6.75 * PI));

    for (int c = -144; c < -48; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 56.9*cos(c      *PI/288)), float(3334.0 + 56.9*sin(c      *PI/288)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 56.9*cos((c + 1)*PI/288)), float(3334.0 + 56.9*sin((c + 1)*PI/288)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63.1*cos((c + 1)*PI/288)), float(3334.0 + 63.1*sin((c + 1)*PI/288)),   float(5.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63.1*cos(c      *PI/288)), float(3334.0 + 63.1*sin(c      *PI/288)),   float(5.0 + 6.75 * PI)); //voie unique

    }

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2325.0f, 3253.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3240.205081f, float(5.0 + 6.75 * PI)); //prolongation liée à la sortie (configuration particulière)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2350.0f, 3240.205081f, float(5.0 + 6.75 * PI)); //même chose
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3253.205081f, float(5.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3253.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(5.0 + 6.75 * PI)); //prolongation liée à la sortie (configuration particulière)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2625.0f, 3247.205081f, float(5.0 + 6.75 * PI)); //même chose
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2625.0f, 3253.205081f, float(5.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2274.652123f, 3265.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2274.652123f, 3271.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2574.652123f, 3271.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2574.652123f, 3265.0f, float(5.0 + 6.75 * PI));

    glEnd();

    if (!demo) {

        afficherCachesTIV();

    }

    //utilisation de la couleur noire (hors caches TIV)

    glBindTexture(GL_TEXTURE_2D, texture2[4]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -370.0f, -504.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f, -504.0f,  9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -370.0f, -498.0f,  9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -370.0f, -498.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -370.0f, -522.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f, -522.0f,  9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -370.0f, -516.0f,  9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -370.0f, -516.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -310.0f, -2.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -310.0f,  0.3f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, -150.0f, -2.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, -150.0f, -2.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -310.0f, -2.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -310.0f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -310.0f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -310.0f, -2.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, -150.0f, -2.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, -150.0f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, -150.0f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -150.0f, -2.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, -100.0f,   2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,    0.0f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,    0.0f,   4.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, -100.0f,   4.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f,   0.0f,   2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f,   0.0f,   9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,   0.0f,   9.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,   0.0f,   2.0f);
        
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 125.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 125.0f, 5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 125.0f, 5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 125.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 140.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 140.0f, 5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 140.0f, 5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 140.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 250.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 250.0f, 5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 250.0f, 5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 250.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 325.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 325.0f, 5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 325.0f, 5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 325.0f, 0.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 3001.0f, float(10.0 + 6.75 * PI));
    glColor3d(0.22 + 0.78 * bleuCiel(), 0.22 + 0.78 * bleuCiel(), 0.22 + 0.78 * bleuCiel());
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2981.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2981.0f, float(10.0 + 6.75 * PI));
    glColor3d(1, 1, 1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3001.0f, float(10.0 + 6.75 * PI));

    glColor3d(1, 1, 1);

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 2641.0f, float(10.0 + 6.75 * PI));
    glColor3d(0.22 + 0.78 * bleuCiel(), 0.22 + 0.78 * bleuCiel(), 0.22 + 0.78 * bleuCiel());
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2661.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2661.0f, float(10.0 + 6.75 * PI));
    glColor3d(1, 1, 1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2641.0f, float(10.0 + 6.75 * PI)); //"triangles" encadrant le passage aérien

    glEnd();

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2172.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2172.0f, 3244.205081f, float(5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2172.0f, 3247.205081f, float(5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2172.0f, 3247.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.0f, 3244.205081f, float(5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2240.0f, 3247.205081f, float(5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2240.0f, 3247.205081f, float(0.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3244.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3244.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(4.0 + 6.75 * PI)); //sortie Bourse arrivée

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3247.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3247.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3247.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(4.0 + 6.75 * PI)); //sortie Bourse arrivée

    //"grillage"

    for (double x = 2325.5; x < 2350; x++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x - 0.1), 3247.205081f, float(0.8 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x - 0.1), 3247.205081f, float(4.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x + 0.1), 3247.205081f, float(4.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x + 0.1), 3247.205081f, float(0.8 + 6.75 * PI));

    }

    for (double y = -309.0; y < -150; y += 5) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, float(y - 0.8), 9.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, float(y - 0.8), 5.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, float(y + 0.8), 5.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, float(y + 0.8), 9.1f);

    }


    for (int n = 0; n < nombreRamesMax; n++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n)      ) + 1.5*cos(r(distTotaleAutres(n)      ))), float(y(distTotaleAutres(n)      ) - 1.5*sin(r(distTotaleAutres(n)      ))), float(z(distTotaleAutres(n)      ) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 0.3) + 1.5*cos(r(distTotaleAutres(n) - 0.3))), float(y(distTotaleAutres(n) - 0.3) - 1.5*sin(r(distTotaleAutres(n) - 0.3))), float(z(distTotaleAutres(n) - 0.3) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 0.3) + 1.5*cos(r(distTotaleAutres(n) - 0.3))), float(y(distTotaleAutres(n) - 0.3) - 1.5*sin(r(distTotaleAutres(n) - 0.3))), float(z(distTotaleAutres(n) - 0.3) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n)      ) + 1.5*cos(r(distTotaleAutres(n)      ))), float(y(distTotaleAutres(n)      ) - 1.5*sin(r(distTotaleAutres(n)      ))), float(z(distTotaleAutres(n)      ) + 1.5));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n)      ) - 1.5*cos(r(distTotaleAutres(n)      ))), float(y(distTotaleAutres(n)      ) + 1.5*sin(r(distTotaleAutres(n)      ))), float(z(distTotaleAutres(n)      ) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 0.3) - 1.5*cos(r(distTotaleAutres(n) - 0.3))), float(y(distTotaleAutres(n) - 0.3) + 1.5*sin(r(distTotaleAutres(n) - 0.3))), float(z(distTotaleAutres(n) - 0.3) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 0.3) - 1.5*cos(r(distTotaleAutres(n) - 0.3))), float(y(distTotaleAutres(n) - 0.3) + 1.5*sin(r(distTotaleAutres(n) - 0.3))), float(z(distTotaleAutres(n) - 0.3) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n)      ) - 1.5*cos(r(distTotaleAutres(n)      ))), float(y(distTotaleAutres(n)      ) + 1.5*sin(r(distTotaleAutres(n)      ))), float(z(distTotaleAutres(n)      ) + 1.5));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 74.7) + 1.5*cos(r(distTotaleAutres(n) - 74.7))), float(y(distTotaleAutres(n) - 74.7) - 1.5*sin(r(distTotaleAutres(n) - 74.7))), float(z(distTotaleAutres(n) - 74.7) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 75.0) + 1.5*cos(r(distTotaleAutres(n) - 75.0))), float(y(distTotaleAutres(n) - 75.0) - 1.5*sin(r(distTotaleAutres(n) - 75.0))), float(z(distTotaleAutres(n) - 75.0) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 75.0) + 1.5*cos(r(distTotaleAutres(n) - 75.0))), float(y(distTotaleAutres(n) - 75.0) - 1.5*sin(r(distTotaleAutres(n) - 75.0))), float(z(distTotaleAutres(n) - 75.0) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 74.7) + 1.5*cos(r(distTotaleAutres(n) - 74.7))), float(y(distTotaleAutres(n) - 74.7) - 1.5*sin(r(distTotaleAutres(n) - 74.7))), float(z(distTotaleAutres(n) - 74.7) + 1.5));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 74.7) - 1.5*cos(r(distTotaleAutres(n) - 74.7))), float(y(distTotaleAutres(n) - 74.7) + 1.5*sin(r(distTotaleAutres(n) - 74.7))), float(z(distTotaleAutres(n) - 74.7) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 75.0) - 1.5*cos(r(distTotaleAutres(n) - 75.0))), float(y(distTotaleAutres(n) - 75.0) + 1.5*sin(r(distTotaleAutres(n) - 75.0))), float(z(distTotaleAutres(n) - 75.0) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 75.0) - 1.5*cos(r(distTotaleAutres(n) - 75.0))), float(y(distTotaleAutres(n) - 75.0) + 1.5*sin(r(distTotaleAutres(n) - 75.0))), float(z(distTotaleAutres(n) - 75.0) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 74.7) - 1.5*cos(r(distTotaleAutres(n) - 74.7))), float(y(distTotaleAutres(n) - 74.7) + 1.5*sin(r(distTotaleAutres(n) - 74.7))), float(z(distTotaleAutres(n) - 74.7) + 1.5));

        for (int i = 1; i < 5; i++) {

            glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3) + 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3))), float(y(distTotaleAutres(n) - 15 * i - 0.3) - 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3))), float(z(distTotaleAutres(n) - 15 * i - 0.3) - 1.5));
            glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i + 0.3) + 1.5*cos(r(distTotaleAutres(n) - 15 * i + 0.3))), float(y(distTotaleAutres(n) - 15 * i + 0.3) - 1.5*sin(r(distTotaleAutres(n) - 15 * i + 0.3))), float(z(distTotaleAutres(n) - 15 * i + 0.3) - 1.5));
            glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i + 0.3) + 1.5*cos(r(distTotaleAutres(n) - 15 * i + 0.3))), float(y(distTotaleAutres(n) - 15 * i + 0.3) - 1.5*sin(r(distTotaleAutres(n) - 15 * i + 0.3))), float(z(distTotaleAutres(n) - 15 * i + 0.3) + 1.5));
            glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3) + 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3))), float(y(distTotaleAutres(n) - 15 * i - 0.3) - 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3))), float(z(distTotaleAutres(n) - 15 * i - 0.3) + 1.5));

            glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3) - 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3))), float(y(distTotaleAutres(n) - 15 * i - 0.3) + 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3))), float(z(distTotaleAutres(n) - 15 * i - 0.3) - 1.5));
            glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i + 0.3) - 1.5*cos(r(distTotaleAutres(n) - 15 * i + 0.3))), float(y(distTotaleAutres(n) - 15 * i + 0.3) + 1.5*sin(r(distTotaleAutres(n) - 15 * i + 0.3))), float(z(distTotaleAutres(n) - 15 * i + 0.3) - 1.5));
            glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i + 0.3) - 1.5*cos(r(distTotaleAutres(n) - 15 * i + 0.3))), float(y(distTotaleAutres(n) - 15 * i + 0.3) + 1.5*sin(r(distTotaleAutres(n) - 15 * i + 0.3))), float(z(distTotaleAutres(n) - 15 * i + 0.3) + 1.5));
            glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3) - 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3))), float(y(distTotaleAutres(n) - 15 * i - 0.3) + 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3))), float(z(distTotaleAutres(n) - 15 * i - 0.3) + 1.5));

        }

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotale()      ) + 1.5*cos(r(distTotale()      ))), float(y(distTotale()      ) - 1.5*sin(r(distTotale()      ))), float(z(distTotale()      ) - 1.5));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotale() - 0.3) + 1.5*cos(r(distTotale() - 0.3))), float(y(distTotale() - 0.3) - 1.5*sin(r(distTotale() - 0.3))), float(z(distTotale() - 0.3) - 1.5));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotale() - 0.3) + 1.5*cos(r(distTotale() - 0.3))), float(y(distTotale() - 0.3) - 1.5*sin(r(distTotale() - 0.3))), float(z(distTotale() - 0.3) + 1.5));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotale()      ) + 1.5*cos(r(distTotale()      ))), float(y(distTotale()      ) - 1.5*sin(r(distTotale()      ))), float(z(distTotale()      ) + 1.5));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotale()      ) - 1.5*cos(r(distTotale()      ))), float(y(distTotale()      ) + 1.5*sin(r(distTotale()      ))), float(z(distTotale()      ) - 1.5));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotale() - 0.3) - 1.5*cos(r(distTotale() - 0.3))), float(y(distTotale() - 0.3) + 1.5*sin(r(distTotale() - 0.3))), float(z(distTotale() - 0.3) - 1.5));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotale() - 0.3) - 1.5*cos(r(distTotale() - 0.3))), float(y(distTotale() - 0.3) + 1.5*sin(r(distTotale() - 0.3))), float(z(distTotale() - 0.3) + 1.5));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotale()      ) - 1.5*cos(r(distTotale()      ))), float(y(distTotale()      ) + 1.5*sin(r(distTotale()      ))), float(z(distTotale()      ) + 1.5));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x(distTotale() - 74.7) + 1.5*cos(r(distTotale() - 74.7))), float(y(distTotale() - 74.7) - 1.5*sin(r(distTotale() - 74.7))), float(z(distTotale() - 74.7) - 1.5));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x(distTotale() - 75.0) + 1.5*cos(r(distTotale() - 75.0))), float(y(distTotale() - 75.0) - 1.5*sin(r(distTotale() - 75.0))), float(z(distTotale() - 75.0) - 1.5));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x(distTotale() - 75.0) + 1.5*cos(r(distTotale() - 75.0))), float(y(distTotale() - 75.0) - 1.5*sin(r(distTotale() - 75.0))), float(z(distTotale() - 75.0) + 1.5));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x(distTotale() - 74.7) + 1.5*cos(r(distTotale() - 74.7))), float(y(distTotale() - 74.7) - 1.5*sin(r(distTotale() - 74.7))), float(z(distTotale() - 74.7) + 1.5));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x(distTotale() - 74.7) - 1.5*cos(r(distTotale() - 74.7))), float(y(distTotale() - 74.7) + 1.5*sin(r(distTotale() - 74.7))), float(z(distTotale() - 74.7) - 1.5));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x(distTotale() - 75.0) - 1.5*cos(r(distTotale() - 75.0))), float(y(distTotale() - 75.0) + 1.5*sin(r(distTotale() - 75.0))), float(z(distTotale() - 75.0) - 1.5));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x(distTotale() - 75.0) - 1.5*cos(r(distTotale() - 75.0))), float(y(distTotale() - 75.0) + 1.5*sin(r(distTotale() - 75.0))), float(z(distTotale() - 75.0) + 1.5));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x(distTotale() - 74.7) - 1.5*cos(r(distTotale() - 74.7))), float(y(distTotale() - 74.7) + 1.5*sin(r(distTotale() - 74.7))), float(z(distTotale() - 74.7) + 1.5));

    for (int i = 1; i < 5; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x(distTotale() - 15 * i - 0.3) + 1.5*cos(r(distTotale() - 15 * i - 0.3))), float(y(distTotale() - 15 * i - 0.3) - 1.5*sin(r(distTotale() - 15 * i - 0.3))), float(z(distTotale() - 15 * i - 0.3) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x(distTotale() - 15 * i + 0.3) + 1.5*cos(r(distTotale() - 15 * i + 0.3))), float(y(distTotale() - 15 * i + 0.3) - 1.5*sin(r(distTotale() - 15 * i + 0.3))), float(z(distTotale() - 15 * i + 0.3) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x(distTotale() - 15 * i + 0.3) + 1.5*cos(r(distTotale() - 15 * i + 0.3))), float(y(distTotale() - 15 * i + 0.3) - 1.5*sin(r(distTotale() - 15 * i + 0.3))), float(z(distTotale() - 15 * i + 0.3) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x(distTotale() - 15 * i - 0.3) + 1.5*cos(r(distTotale() - 15 * i - 0.3))), float(y(distTotale() - 15 * i - 0.3) - 1.5*sin(r(distTotale() - 15 * i - 0.3))), float(z(distTotale() - 15 * i - 0.3) + 1.5));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x(distTotale() - 15 * i - 0.3) - 1.5*cos(r(distTotale() - 15 * i - 0.3))), float(y(distTotale() - 15 * i - 0.3) + 1.5*sin(r(distTotale() - 15 * i - 0.3))), float(z(distTotale() - 15 * i - 0.3) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x(distTotale() - 15 * i + 0.3) - 1.5*cos(r(distTotale() - 15 * i + 0.3))), float(y(distTotale() - 15 * i + 0.3) + 1.5*sin(r(distTotale() - 15 * i + 0.3))), float(z(distTotale() - 15 * i + 0.3) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x(distTotale() - 15 * i + 0.3) - 1.5*cos(r(distTotale() - 15 * i + 0.3))), float(y(distTotale() - 15 * i + 0.3) + 1.5*sin(r(distTotale() - 15 * i + 0.3))), float(z(distTotale() - 15 * i + 0.3) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x(distTotale() - 15 * i - 0.3) - 1.5*cos(r(distTotale() - 15 * i - 0.3))), float(y(distTotale() - 15 * i - 0.3) + 1.5*sin(r(distTotale() - 15 * i - 0.3))), float(z(distTotale() - 15 * i - 0.3) + 1.5));

    }

    for (int c = 56; c < 72; c++) { //fragments des 4 pans de mur retirés à la sortie de Bourse-arrivée; précision quadruplée

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), float(3181.205081 + 63*sin(c      *PI/144)),  float(-0.3 + (-18 + 0.25 * c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), float(3181.205081 + 63*sin(c      *PI/144)),  float( 0.8 + 6.75 * PI)                                       );
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), float(3181.205081 + 63*sin((c + 1)*PI/144)),  float( 0.8 + 6.75 * PI)                                       );
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), float(3181.205081 + 63*sin((c + 1)*PI/144)),  float(-0.3 + (-17 + 0.25 * c) * 0.05 * 60 * PI/36 + 6.75 * PI)); //bas

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), float(3181.205081 + 63*sin(c      *PI/144)),  float( 4.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), float(3181.205081 + 63*sin(c      *PI/144)),  float( 5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), float(3181.205081 + 63*sin((c + 1)*PI/144)),  float( 5.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), float(3181.205081 + 63*sin((c + 1)*PI/144)),  float( 4.0 + 6.75 * PI)); //haut

        //grillage

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos((c - 0.1) * PI/144)), float(3181.205081 + 63*sin((c - 0.1) * PI/144)),  float(5.1 + (-18 + 0.25 * c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos((c - 0.1) * PI/144)), float(3181.205081 + 63*sin((c - 0.1) * PI/144)),  float(0.8 + 6.75 * PI)                                       );
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos((c + 0.1) * PI/144)), float(3181.205081 + 63*sin((c + 0.1) * PI/144)),  float(0.8 + 6.75 * PI)                                       );
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos((c + 0.1) * PI/144)), float(3181.205081 + 63*sin((c + 0.1) * PI/144)),  float(5.1 + (-17 + 0.25 * c) * 0.05 * 60 * PI/36 + 6.75 * PI)); //"grillage"

    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glBegin(GL_QUADS);

    //aiguillages
    //rappel: conseillé entre 15 et 25, valeur théo 12

    glColor3d(0.5, 0.5, 0.5);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -645.0f,  -522.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -625.0f,  -522.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -625.0f,  -510.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -645.0f,  -510.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -470.0f,  -510.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -450.0f,  -510.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -450.0f,  -498.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.0f,  -498.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -350.0f,  -516.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -370.0f,  -504.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -350.0f,  -504.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -235.0f,  -516.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -215.0f,  -516.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -215.0f,  -504.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -235.0f,  -504.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -120.0f,  -2.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, -100.0f,  -2.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(   0.0f, -100.0f,  -2.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(   0.0f, -120.0f,  -2.4f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  120.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  140.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f,  140.0f,   0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f,  120.0f,   0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  230.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  250.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f,  250.0f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f,  230.0f,   0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 910.0f,  3165.0f,  float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 890.0f,  3165.0f,  float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 890.0f,  3177.0f,  float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 910.0f,  3177.0f,  float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1870.0f,  3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1850.0f,  3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1850.0f,  3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1870.0f,  3277.0f,  float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2190.0f,  3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2170.0f,  3265.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2170.0f,  3277.0f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2190.0f,  3277.0f,  float(0.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture3[4]);

    glBegin(GL_QUADS);

    //aiguillages larges
    //rappel: conseillé entre 15 et 25, valeur théo 12

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2195.0f,  3238.205081f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2175.0f,  3238.205081f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2175.0f,  3253.205081f,  float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2195.0f,  3253.205081f,  float(0.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture4[2]);

    glBegin(GL_QUADS);

    //complexes 3 voies avec aiguillage

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  270.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  250.0f,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(   6.0f,  250.0f,  0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(   6.0f,  270.0f,  0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureCouleurs[13]);

    glBegin(GL_QUADS);

    glColor3d(rougeCiel(), vertCiel(), bleuCiel());

    //ciel

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -60000.0f,  60000.0f,   100.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -60000.0f, -60000.0f,   100.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  60000.0f, -60000.0f,   100.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  60000.0f,  60000.0f,   100.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 600.0f, 3102.0f,  60.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 600.0f, 3102.0f, 120.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 900.0f, 3102.0f, 120.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 900.0f, 3102.0f,  60.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 600.0f, 2540.0f,  60.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 600.0f, 2540.0f, 120.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 900.0f, 2540.0f, 120.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 900.0f, 2540.0f,  60.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 600.0f, 2540.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 600.0f, 2540.0f, 120.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 600.0f, 3202.0f, 120.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 600.0f, 3202.0f,   0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 900.0f, 2540.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 900.0f, 2540.0f, 120.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 900.0f, 3164.0f, 120.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 900.0f, 3164.0f,   0.0f);

    glColor3d(1, 1, 1);

    glEnd();

    dessinerPetitsBoutsTunnel();

    if (!demo) {

        afficherTIV();

    }

    glBindTexture(GL_TEXTURE_2D, texturePlaceDesRemparts[2]);

    glBegin(GL_QUADS); //haut de quai droit et de quai gauche à Place des Remparts (décoration particulière)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -504.0f, 4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -496.5f, 4.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -496.5f, 4.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -504.0f, 4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -516.0f, 4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -523.5f, 4.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -523.5f, 4.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -516.0f, 4.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[4]);

    glBegin(GL_QUADS);

    //haut du quai de droite

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 220.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 220.0f, 0.8f); //station fantôme

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 150.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -19.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -19.0f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 675.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 750.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 675.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1350.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1350.0f, -5.2f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 245.663997f, 1728.600071f, -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 248.492424f, 1725.771644f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 301.525433f, 1778.804653f, -2.2f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 298.697006f, 1781.633080f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2621.0f     , float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2621.0f     , float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3265.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3261.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3261.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3261.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3261.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3245.705081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3245.705081f, float(0.8 + 6.75 * PI)); //quai allongé

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3261.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3261.0f, float(0.8 + 6.75 * PI));

    //haut du quai de gauche

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 150.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.5f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.5f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-9.0f, 150.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-9.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.5f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.5f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 675.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 750.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 675.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -6.0f, 1350.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -6.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1350.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.178715f, 1737.085353f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 234.350288f, 1739.913780f, -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 287.383296f, 1792.946789f, -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 290.211724f, 1790.118362f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 2621.0f     , float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2621.0f     , float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3277.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3281.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3281.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3281.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3245.705081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3245.705081f, float(0.8 + 6.75 * PI)); //quai allongé

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3281.0f, float(0.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texturePlaceDesRemparts[3]);
    glBegin(GL_QUADS); //bas du quai de droite et du quai de gauche à Place des Remparts (décoration particulière)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -504.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -504.0f, 4.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -330.0f, -504.0f, 4.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f, -504.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -516.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -516.0f, 4.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -330.0f, -516.0f, 4.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f, -516.0f, 4.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);

    glBegin(GL_QUADS);

    //bas du quai de droite

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 220.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 220.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 225.0f, 0.0f); //station fantôme

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 150.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 225.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 675.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 675.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 750.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 750.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 1350.0f, -6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 1350.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 1425.0f, -6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 245.663997f, 1728.600071f, -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 245.663997f, 1728.600071f, -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 298.697006f, 1781.633080f, -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 298.697006f, 1781.633080f, -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2546.015511f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2621.0f,      float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2621.0f,      float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 3021.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3096.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3265.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3265.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3265.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3265.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3265.0f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.0 + 6.75 * PI)); //quai allongé

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.0 + 6.75 * PI)); //quai allongé

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3265.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3265.0f, float(0.0 + 6.75 * PI));

    //bas du quai de gauche

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 150.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 225.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-9.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-9.0f, 150.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-9.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-9.0f, 225.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 675.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 675.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.0f, 750.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 750.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 1350.0f, -6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 1350.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 1425.0f, -6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.178715f, 1737.085353f, -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 237.178715f, 1737.085353f, -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 290.211724f, 1790.118362f, -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 290.211724f, 1790.118362f, -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2546.015511f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2621.0f,      float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 2621.0f,      float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3021.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 3096.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3277.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3277.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3277.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3277.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3277.0f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.0 + 6.75 * PI)); //quai central

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.0 + 6.75 * PI)); //quai central, allongé

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3277.0f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3277.0f, float(0.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture4[3]);

    glBegin(GL_QUADS);

    //plafond de la station (29m) //texture à modifier

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-19.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-19.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 225.0f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 150.0f, 6.0f);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, texture6[4]);

    glBegin(GL_QUADS);

    //zone au-dessus des sorties

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 225.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 225.0f, 4.0f); //au-dessus de la sortie 1 sur le tympan de République

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -19.0f, 225.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -19.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -16.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -16.0f, 225.0f, 4.0f); //au-dessus de la sortie 1 sur le tympan de République

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 750.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 750.0f, 4.0f); //au-dessus de la sortie 1 sur le tympan de Bouchers

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 750.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( - 7.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( - 7.0f, 750.0f, 4.0f); //au-dessus de la sortie 1 sur le tympan de Bouchers

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 675.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -7.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 675.0f, 4.0f); //au-dessus de la sortie 2 sur le tympan de Bouchers

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 1425.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 1425.0f, -2.0f); //au-dessus de la sortie 1 sur le tympan de Rue des Chenilles

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 1425.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -7.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -7.0f, 1425.0f, -2.0f); //au-dessus de la sortie 1 sur le tympan de Rue des Chenilles

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 236.471608f, 1737.792460f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 236.471608f, 1737.792460f, 3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 234.350288f, 1739.913780f, 3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 234.350288f, 1739.913780f, 1.0f); //au-dessus de la sortie 1 sur le tympan de Boulevard de l'Hôpital

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 248.492424f, 1725.771644f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 248.492424f, 1725.771644f, 3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 246.371104f, 1727.892964f, 3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 246.371104f, 1727.892964f, 1.0f); //au-dessus de la sortie 1 sur le tympan de Boulevard de l'Hôpital

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 287.383296f, 1792.946789f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 287.383296f, 1792.946789f, 3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 289.504616f, 1790.805469f, 3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 289.504616f, 1790.805469f, 1.0f); //au-dessus de la sortie 2 sur le tympan de Boulevard de l'Hôpital

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2621.0f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de CSF

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2546.015511f, float(14.0 + 6.75 * PI)); //CSF sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2546.015511f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 2 sur le tympan de CSF

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2621.0f, float(14.0 + 6.75 * PI)); //CSF sortie 2

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 3021.0f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de Blainville

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3021.0f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de Blainville

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3264.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3264.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3261.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3261.0f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de Mirla

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3281.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3281.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3278.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3278.0f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de Mirla

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3261.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3261.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f, 3264.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3264.0f, float(14.0 + 6.75 * PI)); //au-dessus de la sortie 2 sur le tympan de Mirla

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3261.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3264.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3264.0f, float(4.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de Histiaux

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3278.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3278.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3281.0f, float(4.0 + 6.75 * PI)); //au-dessus de la sortie 1 sur le tympan de Histiaux

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3247.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3247.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3244.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2325.0f, 3244.205081f, float(4.0 + 6.75 * PI)); //station Bourse arrivée

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2274.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2274.652123f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3278.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3278.0f, float(4.0 + 6.75 * PI)); //station Bourse départ

    glEnd();


    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glBegin(GL_QUADS);

    //plafond de la station (20m)

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 750.0f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 675.0f, 6.0f);

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 1350.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 1425.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1425.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1350.0f, 0.0f);

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 248.492424f, 1725.771644f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 301.525433f, 1778.804653f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 287.383296f, 1792.946789f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 234.350288f, 1739.913780f,  3.0f);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2621.0f     , float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2621.0f     , float(16.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(16.0 + 6.75 * PI));

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(16.0 + 6.75 * PI));

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3261.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3261.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3281.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3281.0f, float(16.0 + 6.75 * PI));

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3281.0f, float(6.0 + 6.75 * PI));

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3235.705081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2325.0f, 3235.705081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3255.705081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3255.705081f, float(6.0 + 6.75 * PI));

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3281.0f, float(6.0 + 6.75 * PI));

    glEnd();


    glBindTexture(GL_TEXTURE_2D, textureCouleurs[13]); //couleur blanche

    glBegin(GL_QUADS);

    //République

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 241.0f, 8.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 241.0f, 8.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 225.0f, 0.8f); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -16.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -16.0f, 241.0f, 8.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -19.0f, 241.0f, 8.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -19.0f, 225.0f, 0.8f); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -19.0f, 241.0f, 8.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -19.0f, 256.0f, 8.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  10.0f, 256.0f, 8.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  10.0f, 241.0f, 8.8f); //provisoire - sol de la salle des billets

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 225.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 225.0f,  4.0f); //provisoire - plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -19.0f, 225.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -19.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -16.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -16.0f, 225.0f,  4.0f); //provisoire - plafond de la sortie 1

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    //Bouchers

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 766.0f, 8.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 766.0f, 8.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 750.0f, 0.8f); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -7.0f, 766.0f, 8.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 766.0f, 8.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 750.0f, 0.8f); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 675.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -7.0f, 647.0f, 14.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 647.0f, 14.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 675.0f,  0.8f); //provisoire - sol de la sortie 2

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 766.0f, 8.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 781.0f, 8.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  10.0f, 781.0f, 8.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  10.0f, 766.0f, 8.8f); //provisoire - sol de la salle des billets

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 750.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 750.0f,  4.0f); //provisoire - plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 750.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -7.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 750.0f,  4.0f); //provisoire - plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 675.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -7.0f, 647.0f, 18.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 647.0f, 18.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 675.0f,  4.0f); //provisoire - plafond de la sortie 2

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    //Rue des Chenilles

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1430.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  7.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 1427.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1427.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1430.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 12.0f, 1430.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1430.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1425.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 12.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1409.0f,  2.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1409.0f,  2.8f); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 1430.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -7.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 1427.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1427.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1430.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, 1430.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1430.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1425.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1425.0f, -5.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1409.0f,  2.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1409.0f,  2.8f); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 1425.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  7.0f, 1430.0f, -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 1427.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 1430.0f, -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 12.0f, 1430.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1425.0f, -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 12.0f, 1425.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1409.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1409.0f,  6.0f); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 1425.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -7.0f, 1430.0f, -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 1427.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 1430.0f, -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, 1430.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1425.0f, -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, 1425.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1409.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1409.0f,  6.0f); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 1409.0f, 2.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1399.0f, 2.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  15.0f, 1399.0f, 2.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  15.0f, 1409.0f, 2.8f); //provisoire - sol de la salle des billets

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    //Boulevard de l'Hôpital

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 234.350288f, 1739.913780f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 223.036579f, 1728.600071f,  5.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 225.157900f, 1726.478751f,  5.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 236.471608f, 1737.792460f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 246.371104f, 1727.892964f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 235.057396f, 1716.579256f,  5.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 237.178716f, 1714.457936f,  5.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 248.492424f, 1725.771644f, -2.2f); //sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 234.350288f, 1739.913780f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 223.036579f, 1728.600071f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 225.157900f, 1726.478751f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 236.471608f, 1737.792460f,  1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 246.371104f, 1727.892964f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 235.057396f, 1716.579256f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 237.178716f, 1714.457936f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 248.492424f, 1725.771644f,  1.0f); //plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 223.036579f, 1728.600071f,  5.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 212.429977f, 1717.993469f,  5.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 226.572113f, 1703.851333f,  5.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 237.178716f, 1714.457936f,  5.8f); //sol de la salle des billets

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 287.383296f, 1792.946789f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 304.353259f, 1809.917352f,  9.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 306.475179f, 1807.796031f,  9.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 289.504616f, 1790.805469f, -2.2f); //sol sortie 2

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 287.383296f, 1792.946789f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 304.353259f, 1809.917352f, 13.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 306.475179f, 1807.796031f, 13.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 289.504616f, 1790.805469f,  1.0f); //plafond sortie 2

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    //CSF sortie côté intérieur


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2562.015511f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2562.015511f, float(18.8 + 6.75 * PI)); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2562.015511f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2562.015511f, float(18.8 + 6.75 * PI)); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2541.015511f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2541.015511f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2546.015511f, float(14.0 + 6.75 * PI));//1425 -> 2621        1430 -> 2616       1427 -> 2619       1409 -> 2637

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2562.015511f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2562.015511f, float(22.0 + 6.75 * PI)); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2541.015511f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2541.015511f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2546.015511f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2546.0155110f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2562.015511f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2562.015511f, float(22.0 + 6.75 * PI)); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2562.015511f, float(18.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2572.015511f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2572.015511f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2562.015511f, float(18.8 + 6.75 * PI)); //provisoire - sol de la salle des billets


    //CSF sortie côté extérieur

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2621.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2605.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2605.0f, float(18.8 + 6.75 * PI)); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2626.0f, float(10.8 + 6.75 * PI)); //3021 -> 2621    3019 -> 2623    3016 -> 2626      3037 -> 2605
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2621.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2605.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2605.0f, float(18.8 + 6.75 * PI)); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2626.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2626.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2621.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2605.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2605.0f, float(22.0 + 6.75 * PI)); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2626.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2626.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2621.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2605.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2605.0f, float(22.0 + 6.75 * PI)); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2605.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2595.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2595.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2605.0f, float(18.8 + 6.75 * PI)); //provisoire - sol de la salle des billets

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    //Blainville

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3037.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3037.0f, float(18.8 + 6.75 * PI)); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 3037.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 3037.0f, float(18.8 + 6.75 * PI)); //sol de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 3016.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 3016.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3021.0f, float(14.0 + 6.75 * PI));//1425 -> 2621        1430 -> 2616       1427 -> 2619       1409 -> 2637

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3037.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3037.0f, float(22.0 + 6.75 * PI)); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 3016.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3016.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 3021.0f, float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 3037.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 3037.0f, float(22.0 + 6.75 * PI)); //plafond de la sortie 1


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 3037.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3047.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3047.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 3037.0f, float(18.8 + 6.75 * PI)); //provisoire - sol de la salle des billets

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    //Mirla

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3261.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1231.0f, 3261.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3264.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3264.0f, float(10.8 + 6.75 * PI)); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3278.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1231.0f, 3278.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3281.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3281.0f, float(10.8 + 6.75 * PI)); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1231.0f, 3261.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1231.0f, 3281.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1246.0f, 3281.0f, float(18.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1246.0f, 3261.0f, float(18.8 + 6.75 * PI)); //provisoire - sol de la salle des billets

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3261.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1231.0f, 3261.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3264.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3264.0f, float(14.0 + 6.75 * PI)); //provisoire - plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3278.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1231.0f, 3278.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3281.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3281.0f, float(14.0 + 6.75 * PI)); //provisoire - plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3261.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1112.0f, 3261.0f, float(24.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1112.0f, 3264.0f, float(24.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3264.0f, float(10.8 + 6.75 * PI)); //provisoire - sol de la sortie 2

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3261.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1112.0f, 3261.0f, float(28.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1112.0f, 3264.0f, float(28.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3264.0f, float(14.0 + 6.75 * PI)); //provisoire - plafond de la sortie 2

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    //Histiaux

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3261.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1709.0f, 3261.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3264.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3264.0f, float(0.8 + 6.75 * PI)); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3278.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1709.0f, 3278.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3281.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3281.0f, float(0.8 + 6.75 * PI)); //provisoire - sol de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1709.0f, 3261.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1709.0f, 3281.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1694.0f, 3281.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1694.0f, 3261.0f, float(8.8 + 6.75 * PI)); //provisoire - sol de la salle des billets

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3261.0f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1709.0f, 3261.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3264.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3264.0f, float( 4.0 + 6.75 * PI)); //provisoire - plafond de la sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3278.0f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1709.0f, 3278.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3281.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3281.0f, float( 4.0 + 6.75 * PI)); //provisoire - plafond de la sortie 1

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    //Bourse arrivée

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2346.0f, 3241.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2346.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3241.205081f, float(0.8 + 6.75 * PI)); //provisoire - sol de la sortie Bourse arrivée

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3241.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2366.0f, 3241.205081f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2366.0f, 3244.205081f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI)); //provisoire - sol de la sortie Bourse arrivée

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3241.205081f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2366.0f, 3241.205081f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2366.0f, 3244.205081f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float( 4.0 + 6.75 * PI)); //provisoire - plafond de la sortie Bourse arrivée

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3244.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3247.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3247.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(4.0 + 6.75 * PI)); //provisoire - plafond de la sortie Bourse arrivée (au-dessus du quai)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2346.0f, 3241.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2346.0f, 3244.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3241.205081f, float(4.0 + 6.75 * PI)); //provisoire - sol de la sortie Bourse arrivée


    for (int c = 56; c < 72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), 3244.205081f                               ,  float( 0.8 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), float(3181.205081 + 63*sin(c      *PI/144)),  float( 0.8 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), float(3181.205081 + 63*sin((c + 1)*PI/144)),  float( 0.8 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), 3244.205081f                               ,  float( 0.8 + 6.75 * PI)); //sol

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), 3244.205081f                               ,  float( 4.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos(c      *PI/144)), float(3181.205081 + 63*sin(c      *PI/144)),  float( 4.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), float(3181.205081 + 63*sin((c + 1)*PI/144)),  float( 4.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2325.0 + 63*cos((c + 1)*PI/144)), 3244.205081f                               ,  float( 4.0 + 6.75 * PI)); //plafond

    }

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    //Bourse départ

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2274.652123f, 3278.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3278.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2274.652123f, 3281.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2276.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3285.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3285.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2276.652123f, 3285.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3285.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3301.0f, float(8.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3301.0f, float(8.8 + 6.75 * PI)); //sol de la sortie Bourse départ


    glTexCoord2f(0.0f, 0.0f); glVertex3f(2274.652123f, 3278.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3278.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2274.652123f, 3281.0f, float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2276.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3285.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3285.0f, float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2276.652123f, 3285.0f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3285.0f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3301.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3301.0f, float(12.0 + 6.75 * PI)); //plafond de la sortie Bourse départ


    glEnd();

    //début tympans

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[0]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 150.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  7.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  7.0f, 225.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f , 150.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f , 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -7.52f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -7.52f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f , 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f , 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -7.52f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -7.52f, 225.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f , 150.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f , 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -7.48f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -7.48f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f , 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f , 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -7.48f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -7.48f, 225.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 150.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -19.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -19.0f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -16.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -16.0f, 225.0f, 0.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[1]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 675.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.02f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.02f, 675.0f, 0.8f); //pas de sortie de ce côté de la station

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 7.02f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 7.02f, 750.0f, 0.8f); //tympan raccourci sortie 1

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 675.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.02f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-7.02f, 675.0f, 0.8f); //tympan raccourci sortie 2

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.02f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-7.02f, 750.0f, 0.8f); //tympan raccourci sortie 1

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[2]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1350.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 1350.0f, -5.2f); //pas de sortie de ce côté-ci de la station

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  7.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  7.0f, 1425.0f, -5.2f); //sortie

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -6.0f, 1350.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -6.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 1350.0f, -5.2f); //pas de sortie de ce côté-ci de la station

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -7.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -7.0f, 1425.0f, -5.2f); //sortie

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[3]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 245.663997f, 1728.600071f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 245.663997f, 1728.600071f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 246.371104f, 1727.892964f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 246.371104f, 1727.892964f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 298.697006f, 1781.633080f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 298.697006f, 1781.633080f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 301.525433f, 1778.804653f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 301.525433f, 1778.804653f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.178715f, 1737.085353f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 237.178715f, 1737.085353f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 236.471608f, 1737.792460f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 236.471608f, 1737.792460f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 290.211724f, 1790.118362f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 290.211724f, 1790.118362f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 289.504616f, 1790.805469f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 289.504616f, 1790.805469f, -2.2f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[4]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2621.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2621.0f, float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[5]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 3096.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3096.0f, float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[6]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3265.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3265.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f, 3264.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3264.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3265.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3264.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3264.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3277.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3277.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f, 3281.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3281.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3277.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3278.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3278.0f, float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[7]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3265.0f , float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3265.0f , float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3264.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3264.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3265.0f , float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3265.0f , float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3261.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3277.0f , float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3277.0f , float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3278.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3278.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3277.0f , float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3277.0f , float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3281.0f, float(0.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[8]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3245.705081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3245.705081f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3247.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3245.705081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3245.705081f, float(0.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureTympansStations[9]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3265.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2199.652123f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2199.652123f, 3261.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2274.652123f, 3265.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2274.652123f, 3265.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3261.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3277.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2199.652123f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2199.652123f, 3281.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2274.652123f, 3277.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2274.652123f, 3277.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3278.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3278.0f, float(0.8 + 6.75 * PI));

    glEnd();

    glColor3d(luminanceStations[0], luminanceStations[0], luminanceStations[0]);

    glBindTexture(GL_TEXTURE_2D, texturePlaceDesRemparts[4]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -516.0f,  9.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -516.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -504.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -504.0f,  9.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -516.0f,  9.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -516.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -330.0f, -504.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f, -504.0f,  9.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[0]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 225.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 225.0f, 5.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[1]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 750.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 750.0f, 5.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[2]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 1425.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 1425.0f, -1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[3]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 290.211723f, 1790.118362f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 290.211723f, 1790.118362f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 298.697006f, 1781.633080f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 298.697006f, 1781.633080f,  2.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[4]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2621.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2621.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2621.0f, float(15.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[5]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3096.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3096.0f, float(15.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[6]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3277.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3265.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float(15.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD1Stations[7]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3277.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3277.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3265.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3265.0f, float(5.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[1]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 675.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 675.0f, 5.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[2]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 1350.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 1350.0f, -1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[3]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 245.663997f, 1728.600071f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 245.663997f, 1728.600071f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 237.178714f, 1737.085353f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 237.178714f, 1737.085353f,  2.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[4]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2546.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2546.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2546.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 2546.0f, float(15.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[5]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 3021.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 3021.0f, float(15.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[6]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3265.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3265.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f, 3277.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3277.0f, float(15.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[7]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3265.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3265.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3277.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3277.0f, float(5.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDD2Stations[9]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3265.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3265.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2199.652123f, 3277.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2199.652123f, 3277.0f, float(5.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture4[0]);

    glBegin(GL_QUADS);

    //indications de direction X

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 150.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.0f, 150.0f, 5.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 225.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.0f, 225.0f, 5.0f);

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3238.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3238.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3244.205081f, float(5.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3247.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3253.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3253.205081f, float(5.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3238.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3238.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3244.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2325.0f, 3244.205081f, float(5.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3247.205081f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3247.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3253.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2325.0f, 3253.205081f, float(5.0 + 6.75 * PI));

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2274.652123f, 3265.0f, float(5.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2274.652123f, 3265.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3277.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3277.0f, float(5.0 + 6.75 * PI));

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 150.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 150.0f, 5.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texturePlaceDesRemparts[0]);

    glBegin(GL_QUADS); //noms de station

    glColor3d(luminanceStations[0], luminanceStations[0], luminanceStations[0]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -247.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -247.5f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -277.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -277.5f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -292.5f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -292.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -300.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -300.0f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -322.5f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -322.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -330.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f, -523.5f,  4.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -277.5f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -277.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -300.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -300.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -292.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -292.5f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -322.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -322.5f, -496.5f,  4.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texturePlaceDesRemparts[1]);

    glBegin(GL_QUADS); //cadres publicitaires

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -255.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -255.0f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -523.5f,  4.8f);

    /*

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -262.5f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -262.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -523.5f,  4.8f);

    */

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -277.5f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -277.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -285.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -285.0f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -285.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -285.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -292.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -292.5f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -300.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -300.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -307.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -307.5f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -307.5f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -307.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -315.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -315.0f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -315.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -315.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -322.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -322.5f, -523.5f,  4.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -255.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -255.0f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -496.5f,  4.8f);

    /*
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -262.5f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -262.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -496.5f,  4.8f);
    */

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -277.5f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -277.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -285.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -285.0f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -285.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -285.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -292.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -292.5f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -300.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -300.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -307.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -307.5f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -307.5f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -307.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -315.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -315.0f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -315.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -315.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -322.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -322.5f, -496.5f,  4.8f);



    //on en profite pour dessiner les tympans de la station (qui sont des cadres publicitaires)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -516.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -516.0f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -504.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -504.0f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -330.0f, -516.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f, -516.0f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -330.0f, -504.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -330.0f, -504.0f,  4.8f);

    //et enfin des cadres publicitaires présents dans les accès

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -538.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -531.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -531.0f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -538.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -538.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -538.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -531.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -531.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -531.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -531.0f,  4.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -531.0f,  7.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -531.0f, 12.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -531.0f, 15.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -531.0f, 10.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -538.5f,  7.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -538.5f, 12.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -538.5f, 15.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -538.5f, 10.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -531.0f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -232.5f, -531.0f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -523.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -523.5f, 12.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -225.0f, -531.0f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -225.0f, -531.0f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -225.0f, -523.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -225.0f, -523.5f, 12.6f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -225.0f, -538.5f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -225.0f, -538.5f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -538.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -538.5f, 12.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -504.0f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -232.5f, -504.0f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -511.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -511.5f, 12.6f);



    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -481.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -489.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -489.0f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -481.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -481.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -481.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -489.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -489.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -489.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -489.0f,  4.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -489.0f,  7.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -489.0f, 12.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -489.0f, 15.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -489.0f, 10.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -247.5f, -481.5f,  7.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -247.5f, -481.5f, 12.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -481.5f, 15.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -481.5f, 10.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -489.0f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -232.5f, -489.0f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -496.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -496.5f, 12.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -225.0f, -489.0f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -225.0f, -489.0f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -225.0f, -496.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -225.0f, -496.5f, 12.6f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -225.0f, -481.5f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -225.0f, -481.5f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -481.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -481.5f, 12.6f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texturePlaceDesRemparts[5]);

    glBegin(GL_QUADS);

    //carrelage Place des Remparts simple (utilisé uniquement dans les accès)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -262.5f, -531.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -262.5f, -531.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -538.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -538.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -538.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -531.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -531.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -523.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -523.5f,  4.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -531.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -531.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -247.5f, -531.0f, 12.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -247.5f, -531.0f,  7.4f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -538.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -247.5f, -538.5f, 12.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -247.5f, -538.5f,  7.4f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -531.0f, 10.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -531.0f, 15.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -531.0f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -531.0f, 12.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -538.5f, 10.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -538.5f, 15.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -538.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -538.5f, 12.6f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -225.0f, -538.5f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -225.0f, -538.5f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -225.0f, -531.0f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -225.0f, -531.0f, 12.6f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -262.5f, -489.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -262.5f, -489.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -481.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -262.5f, -481.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -262.5f, -481.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -489.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -270.0f, -489.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -270.0f, -496.5f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -270.0f, -496.5f,  4.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -489.0f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -489.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -247.5f, -489.0f, 12.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -247.5f, -489.0f,  7.4f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -255.0f, -481.5f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -247.5f, -481.5f, 12.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -247.5f, -481.5f,  7.4f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -489.0f, 10.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -489.0f, 15.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -489.0f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -489.0f, 12.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -481.5f, 10.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -240.0f, -481.5f, 15.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -481.5f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -481.5f, 12.6f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -225.0f, -481.5f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -225.0f, -481.5f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -225.0f, -489.0f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -225.0f, -489.0f, 12.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -496.5f, 12.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -232.5f, -496.5f, 17.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -232.5f, -504.0f, 17.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -232.5f, -504.0f, 12.6f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureCouleurs[4]); //couleur bleu vert

    glBegin(GL_QUADS);

    for (int c = 0; c < 50; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, float(-510.0 + 13.5 * cos(c       * PI / 50)), float(10.0 + 6.75 * sin(c       * PI/50)));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, float(-510.0 + 13.5 * cos((c + 1) * PI / 50)), float(10.0 + 6.75 * sin((c + 1) * PI/50)));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, float(-510.0 + 13.5 * cos((c + 1) * PI / 50)), float(10.0 + 6.75 * sin((c + 1) * PI/50)));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, float(-510.0 + 13.5 * cos(c       * PI / 50)), float(10.0 + 6.75 * sin(c       * PI/50)));

    }

    //couloirs (sol)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -523.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -523.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -531.0f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -538.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -538.5f, 12.6f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -531.0f, 12.6f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -496.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -496.5f,  4.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -489.0f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -481.5f,  4.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -481.5f, 12.6f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -489.0f, 12.6f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureCouleurs[5]); //couleur bleu violet

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -496.5f, 10.0f );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -496.5f, 16.75f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -523.5f, 16.75f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -240.0f, -523.5f, 10.0f );

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -496.5f, 10.0f );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -496.5f, 16.75f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -523.5f, 16.75f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -523.5f, 10.0f );

    //couloirs

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -523.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -538.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -538.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -523.5f, 10.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -531.0f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -538.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -538.5f, 17.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -531.0f, 17.8f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -496.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -270.0f, -481.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -481.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -496.5f, 10.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -489.0f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -255.0f, -481.5f, 10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -481.5f, 17.8f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -232.5f, -489.0f, 17.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[0]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-19.0f, 150.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-19.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-19.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-19.0f, 225.0f, 0.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture7[1]);

    glBegin(GL_QUADS);

    //piédroit station fantôme

    glColor3d(1, 1, 1);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 225.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 150.0f, 0.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture7[0]);

    glBegin(GL_QUADS);

    //mur de séparation (République)

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 225.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, 150.0f, 0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureCouleurs[13]);

    glBegin(GL_QUADS);

    glColor3d(1, 1, 1);

    //station fantôme République : cloisons

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 150.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, 150.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 225.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 225.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, 225.0f, 0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture2[1]);

    glBegin(GL_QUADS);

    //station fantôme République : côté voie + revêtement faux plafond

    for (int i = 0; i < 4; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, float(150.00 + 18.75 * i), 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, float(150.00 + 18.75 * i), 6.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, float(168.75 + 18.75 * i), 6.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, float(168.75 + 18.75 * i), 0.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  0.1f, float(150.00 + 18.75 * i), 5.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, float(150.00 + 18.75 * i), 5.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, float(168.75 + 18.75 * i), 5.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.1f, float(168.75 + 18.75 * i), 5.0f);

    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture5[3]); //armoires électriques

    glBegin(GL_QUADS);

    for (int i = 0; i < 28; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(8.0f, float(162.0 + 2*i), 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(8.0f, float(162.0 + 2*i), 2.8f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, float(164.0 + 2*i), 2.8f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, float(164.0 + 2*i), 0.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 8.0f, 162.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 8.0f, 162.0f, 2.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 162.0f, 2.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 162.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(6.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(6.0f, 150.0f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(6.0f, 152.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(6.0f, 152.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 152.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 152.0f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 8.0f, 152.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 8.0f, 152.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 8.0f, 152.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 8.0f, 152.0f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 152.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 152.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 8.0f, 218.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 8.0f, 218.0f, 2.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 218.0f, 2.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 218.0f, 0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture5[4]);

    glBegin(GL_QUADS);

    //sortie 1 République

    glColor3d(luminanceStations[1], luminanceStations[1], luminanceStations[1]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 7.0f, 225.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 7.0f, 225.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 7.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 7.0f, 241.0f,  8.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 225.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 225.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 241.0f,  8.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -16.0f, 225.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -16.0f, 225.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -16.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -16.0f, 241.0f,  8.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -19.0f, 225.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -19.0f, 225.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -19.0f, 241.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -19.0f, 241.0f,  8.8f);

    //sortie 1 Bouchers

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 7.0f, 750.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 7.0f, 750.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 7.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 7.0f, 766.0f,  8.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 750.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 750.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 766.0f,  8.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0f, 750.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.0f, 750.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-7.0f, 766.0f,  8.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 750.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 750.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 766.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 766.0f,  8.8f);

    //sortie 2 Bouchers

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0f, 675.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.0f, 675.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.0f, 647.0f, 18.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-7.0f, 647.0f, 14.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 675.0f,  0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 675.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 647.0f, 18.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 647.0f, 14.8f);

    //sortie Rue des Chenilles

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    //quai d.Bourse

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 7.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 7.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 7.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 7.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 1427.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  7.0f, 1430.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  7.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 1427.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 12.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1427.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 12.0f, 1427.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 12.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 12.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1425.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f, 1409.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, 1409.0f,  2.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 12.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 12.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 12.0f, 1409.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 12.0f, 1409.0f,  2.8f);

    //depuis/vers quai d.République

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -7.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -7.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -7.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -7.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 1427.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -7.0f, 1430.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -7.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 1427.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1427.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, 1427.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, 1427.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1425.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1430.0f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 1430.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, 1409.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, 1409.0f,  2.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, 1425.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, 1409.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, 1409.0f,  2.8f);

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    //sortie 1 Boulevard de l'Hôpital

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 234.350288f, 1739.913780f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 234.350288f, 1739.913780f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 223.036579f, 1728.600071f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 223.036579f, 1728.600071f,  5.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 236.471608f, 1737.792460f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 236.471608f, 1737.792460f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 225.157900f, 1726.478751f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 225.157900f, 1726.478751f,  5.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 246.371104f, 1727.892964f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 246.371104f, 1727.892964f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 235.057396f, 1716.579256f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 235.057396f, 1716.579256f,  5.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 248.492424f, 1725.771644f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 248.492424f, 1725.771644f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 237.178716f, 1714.457936f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 237.178716f, 1714.457936f,  5.8f);

    //sortie 2 Boulevard de l'Hôpital

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 287.383296f, 1792.946789f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 287.383296f, 1792.946789f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 304.353259f, 1809.917352f, 13.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 304.353259f, 1809.917352f,  9.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 289.504616f, 1790.805469f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 289.504616f, 1790.805469f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 306.475179f, 1807.796031f, 13.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 306.475179f, 1807.796031f,  5.8f);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    //sortie CSF côté intérieur

    //quai d.Bourse

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2544.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2544.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 787.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 790.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 790.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2562.015511f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2562.015511f, float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 787.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 2562.015511f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2562.015511f, float(18.8 + 6.75 * PI));

    //quai d.République

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2544.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2541.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2544.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2544.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2544.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2541.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2541.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2562.015511f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2562.015511f, float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2546.015511f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2562.015511f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2562.015511f, float(18.8 + 6.75 * PI));


    //sortie CSF côté extérieur

    //quai d.Bourse

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2623.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2623.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 787.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2621.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 790.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 790.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 2605.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 2605.0f, float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 787.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 2605.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 2605.0f, float(18.8 + 6.75 * PI));

    //quai d.République

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2623.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 2626.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2623.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2623.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2623.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2621.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2626.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2626.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 2605.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 2605.0f, float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 2621.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 2621.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 2605.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 2605.0f, float(18.8 + 6.75 * PI));


    //sortie Blainville

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    //quai d.Bourse

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 782.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 782.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 3019.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 782.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 782.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3019.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 787.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 790.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 790.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 790.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 790.0f, 3037.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 790.0f, 3037.0f, float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 787.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 787.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 787.0f, 3037.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 787.0f, 3037.0f, float(18.8 + 6.75 * PI));

    //quai d.République

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3019.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.0f, 3016.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 3019.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 3019.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 3019.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3016.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 3016.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 760.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 760.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 760.0f, 3037.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 760.0f, 3037.0f, float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 763.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 763.0f, 3021.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 763.0f, 3037.0f, float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 763.0f, 3037.0f, float(18.8 + 6.75 * PI));

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    //sortie 1 Mirla

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3261.0f,  float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3261.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3261.0f,  float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1231.0f, 3261.0f,  float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3264.0f,  float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3264.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3264.0f,  float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1231.0f, 3264.0f,  float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3278.0f,  float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3278.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3278.0f,  float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1231.0f, 3278.0f,  float(18.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3281.0f,  float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3281.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1231.0f, 3281.0f,  float(22.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1231.0f, 3281.0f,  float(18.8 + 6.75 * PI));

    //sortie 2 Mirla

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3261.0f,  float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3261.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1112.0f, 3261.0f,  float(28.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1112.0f, 3261.0f,  float(24.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3264.0f,  float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3264.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1112.0f, 3264.0f,  float(28.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1112.0f, 3264.0f,  float(24.8 + 6.75 * PI));

    //sortie Histiaux

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3261.0f,  float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3261.0f,  float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3261.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1709.0f, 3261.0f,  float( 8.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3264.0f,  float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3264.0f,  float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3264.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1709.0f, 3264.0f,  float( 8.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3278.0f,  float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3278.0f,  float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3278.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1709.0f, 3278.0f,  float( 8.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3281.0f,  float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3281.0f,  float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1709.0f, 3281.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1709.0f, 3281.0f,  float( 8.8 + 6.75 * PI));


    //sortie Bourse arrivée

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f, 3247.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f, 3247.205081f, float(0.8 + 6.75 * PI)); //"tympan"

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3241.205081f, float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2350.0f, 3241.205081f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2366.0f, 3241.205081f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2366.0f, 3241.205081f, float( 8.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2350.0f, 3244.205081f, float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2350.0f, 3244.205081f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2366.0f, 3244.205081f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2366.0f, 3244.205081f, float( 8.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2346.547269f, 3241.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2346.547269f, 3241.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2350.0f     , 3241.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2350.0f     , 3241.205081f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2346.547269f, 3241.205081f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2346.547269f, 3241.205081f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2346.547269f, 3240.405716f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2346.547269f, 3240.405716f, float(0.8 + 6.75 * PI));


    //sortie Bourse départ

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2274.652123f, 3278.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2274.652123f, 3278.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3278.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2279.652123f, 3278.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2274.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2274.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2276.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3281.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2276.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2276.652123f, 3281.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2276.652123f, 3285.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3285.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2279.652123f, 3278.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3278.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3285.0f, float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2279.652123f, 3285.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2276.652123f, 3285.0f, float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2276.652123f, 3285.0f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2276.652123f, 3301.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2276.652123f, 3301.0f, float( 8.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(2279.652123f, 3285.0f, float( 0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2279.652123f, 3285.0f, float( 4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2279.652123f, 3301.0f, float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2279.652123f, 3301.0f, float( 8.8 + 6.75 * PI));


    glEnd();    

    glBindTexture(GL_TEXTURE_2D, textureStations[1]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[2], luminanceStations[2], luminanceStations[2]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 750.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 675.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 675.0f, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 675.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 750.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 750.0f, 0.8f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[2]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[3], luminanceStations[3], luminanceStations[3]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 1425.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 1350.0f, -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, 1350.0f, -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, 1350.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, 1425.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, 1425.0f, -5.2f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture5[3]);

    glBegin(GL_QUADS); //armoires électriques

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.0f, float(1369.0), -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.0f, float(1369.0), -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.2f, float(1371.0), -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.2f, float(1371.0), -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  -9.2f, float(1387.0), -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  -9.2f, float(1387.0), -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.0f, float(1389.0), -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.0f, float(1389.0), -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, float(1384.0), -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, float(1384.0), -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  9.2f, float(1386.0), -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  9.2f, float(1386.0), -5.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  9.2f, float(1402.0), -5.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  9.2f, float(1402.0), -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, float(1404.0), -2.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, float(1404.0), -5.2f);

    for (int i = 0; i < 8; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.2f, float(1371.0 + 2*i), -5.2f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.2f, float(1371.0 + 2*i), -2.2f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.2f, float(1373.0 + 2*i), -2.2f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.2f, float(1373.0 + 2*i), -5.2f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 9.2f, float(1386.0 + 2*i), -5.2f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 9.2f, float(1386.0 + 2*i), -2.2f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 9.2f, float(1388.0 + 2*i), -2.2f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 9.2f, float(1388.0 + 2*i), -5.2f);

    }


    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[3]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[4], luminanceStations[4], luminanceStations[4]);

    //piédroits

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 248.492424f, 1725.771644f, -2.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 248.492424f, 1725.771644f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 301.525433f, 1778.804653f,  3.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 301.525433f, 1778.804653f, -2.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 234.350288f, 1739.913780f, -2.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 234.350288f, 1739.913780f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 287.383296f, 1792.946789f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 287.383296f, 1792.946789f, -2.2f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[4]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 2621.0f,      float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 2621.0f,      float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 2546.015511f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 2546.015511f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 2546.015511f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 2621.0f,      float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 2621.0f,      float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[5]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 785.0f, 3096.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 785.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 785.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 785.0f, 3021.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.0f, 3021.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.0f, 3021.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.0f, 3096.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.0f, 3096.0f, float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[6]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[7], luminanceStations[7], luminanceStations[7]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1140.0f, 3281.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1140.0f, 3281.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3281.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3281.0f, float(10.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3261.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3261.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f, 3261.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f, 3261.0f, float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture5[3]); //armoires électriques

    glBegin(GL_QUADS);

    for (int i = 0; i < 6; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(1203.0 + 2*i), 3261.7f, float(10.8 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(1203.0 + 2*i), 3261.7f, float(13.8 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(1205.0 + 2*i), 3261.7f, float(13.8 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(1205.0 + 2*i), 3261.7f, float(10.8 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(1202.5f, 3261.0f, float(10.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1202.5f, 3261.0f, float(13.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1203.0f, 3261.7f, float(13.8 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1203.0f, 3261.7f, float(10.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture6[0]);

    glBegin(GL_QUADS);

    for (int i = 0; i < 5; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(1142.5 + 12.5 * i), 3261.7f, float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(1142.5 + 12.5 * i), 3261.7f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(1140.0 + 12.5 * i), 3261.7f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(1140.0 + 12.5 * i), 3261.7f, float(11.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(1140.0 + 12.5 * i), 3280.3f, float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(1140.0 + 12.5 * i), 3280.3f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(1142.5 + 12.5 * i), 3280.3f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(1142.5 + 12.5 * i), 3280.3f, float(11.3 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1202.5f, 3280.3f, float(11.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1202.5f, 3280.3f, float(15.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1205.0f, 3280.3f, float(15.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1205.0f, 3280.3f, float(11.3 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture6[1]);

    glBegin(GL_QUADS);

    for (int i = 0; i < 5; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(1152.5 + 12.5 * i), 3261.7f, float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(1152.5 + 12.5 * i), 3261.7f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(1142.5 + 12.5 * i), 3261.7f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(1142.5 + 12.5 * i), 3261.7f, float(11.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(1142.5 + 12.5 * i), 3280.3f, float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(1142.5 + 12.5 * i), 3280.3f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(1152.5 + 12.5 * i), 3280.3f, float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(1152.5 + 12.5 * i), 3280.3f, float(11.3 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1205.0f, 3280.3f, float(11.3 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1205.0f, 3280.3f, float(15.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1215.0f, 3280.3f, float(15.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1215.0f, 3280.3f, float(11.3 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture6[2]);

    glBegin(GL_QUADS);

    for (int i = 0; i < 6; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.7f, float(2556.0 + 12.5 * i), float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.7f, float(2556.0 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.7f, float(2558.5 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.7f, float(2558.5 + 12.5 * i), float(11.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 784.3f, float(2558.5 + 12.5 * i), float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 784.3f, float(2558.5 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 784.3f, float(2556.0 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 784.3f, float(2556.0 + 12.5 * i), float(11.3 + 6.75 * PI));

    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture6[3]);

    glBegin(GL_QUADS);

    for (int i = 0; i < 6; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 765.7f, float(2546.0 + 12.5 * i), float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 765.7f, float(2546.0 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 765.7f, float(2556.0 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 765.7f, float(2556.0 + 12.5 * i), float(11.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 784.3f, float(2556.0 + 12.5 * i), float(11.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 784.3f, float(2556.0 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 784.3f, float(2546.0 + 12.5 * i), float(15.5 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 784.3f, float(2546.0 + 12.5 * i), float(11.3 + 6.75 * PI));

    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[7]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[8], luminanceStations[8], luminanceStations[8]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1725.0f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1725.0f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1800.0f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1800.0f, 3281.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1800.0f, 3261.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1800.0f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f, 3261.0f, float(0.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[8]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2250.0f, 3253.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2250.0f, 3253.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2325.0f, 3253.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2325.0f, 3253.205081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2325.0f, 3238.205081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2325.0f, 3238.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3238.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3238.205081f, float(0.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureStations[9]);

    glBegin(GL_QUADS);

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);

    //piédroits

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2199.652123f, 3281.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2199.652123f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2274.652123f, 3281.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2274.652123f, 3281.0f, float(0.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2274.652123f, 3261.0f, float(0.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2274.652123f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2199.652123f, 3261.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2199.652123f, 3261.0f, float(0.8 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture3[1]);

    glBegin(GL_QUADS);

    //bordures du pont

    glColor3d(0.23 + 0.77 * bleuCiel(), 0.23 + 0.77 * bleuCiel(), 0.23 + 0.77 * bleuCiel());

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2641.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 3001.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 3001.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2641.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2641.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 3001.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 3001.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2641.0f, float(12.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2641.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 3001.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 3001.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2641.0f, float(12.4 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2641.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2641.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.1f, 3001.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.1f, 3001.0f, float(12.7 + 6.75 * PI));

    for (float compteur = 2661.0f; compteur <= 2981.0f; compteur += 20.0f) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, compteur - 0.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, compteur - 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, compteur - 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, compteur - 0.5f, float(10.0 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, compteur + 0.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, compteur + 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, compteur + 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, compteur + 0.5f, float(10.0 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, compteur - 0.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, compteur - 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.1f, compteur + 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.1f, compteur + 0.5f, float(10.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2641.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3001.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 3001.0f, float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2641.0f, float(10.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2641.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 3001.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 3001.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2641.0f, float(12.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2641.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 3001.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 3001.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2641.0f, float(12.4 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2641.0f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2641.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.9f, 3001.0f, float(12.4 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.9f, 3001.0f, float(12.7 + 6.75 * PI));

    for (float compteur = 2661.0f; compteur <= 2981.0f; compteur += 20.0f) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, compteur - 0.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, compteur - 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, compteur - 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, compteur - 0.5f, float(10.0 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, compteur + 0.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, compteur + 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, compteur + 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, compteur + 0.5f, float(10.0 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, compteur - 0.5f, float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, compteur - 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.9f, compteur + 0.5f, float(12.4 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.9f, compteur + 0.5f, float(10.0 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2960.1f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2960.1f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2960.1f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2960.1f, float(12.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2960.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f, 2960.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f, 2960.1f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2960.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2960.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2960.1f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2960.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f, 2960.1f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2960.1f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2960.0f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2960.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2960.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2960.0f, float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2681.9f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2681.9f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2681.9f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2681.9f, float(12.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2682.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f, 2682.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f, 2681.9f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2682.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 2682.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2681.9f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2682.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f, 2681.9f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2681.9f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2682.0f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2682.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2681.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2682.0f, float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2821.1f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2821.1f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2821.1f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.5f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.5f, 2821.1f, float(12.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2821.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f, 2821.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f, 2821.1f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f, 2821.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2821.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2821.1f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2821.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f, 2821.1f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2821.1f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2821.0f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f, 2821.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2821.1f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f, 2821.0f, float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2820.9f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2820.9f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2820.9f, float(12.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.5f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.5f, 2820.9f, float(12.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2821.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f, 2821.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f, 2820.9f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2821.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f, 2821.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2820.9f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2821.0f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f, 2820.9f, float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2820.9f, float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2821.0f, float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f, 2821.0f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f, 2820.9f, float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2821.0f, float(14.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture7[2]);

    glBegin(GL_QUADS);

    //mur en pierres

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2981.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2981.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2989.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 757.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 757.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2989.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 3001.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3001.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3001.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2989.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2981.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2981.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2989.0f, float(15.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2981.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2981.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2989.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 757.0f, 3001.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 757.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2989.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3001.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 3001.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 3001.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3001.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 3001.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 3001.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 3001.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 3001.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2989.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2981.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2981.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2989.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2989.0f, float(39.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2981.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2981.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2989.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2989.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 757.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 757.0f, 3001.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2989.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2989.0f, float(15.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 3001.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 3001.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2989.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2989.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2981.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2981.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2989.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2989.0f, float(15.0 + 6.75 * PI));



    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2661.0f, float(15.0 + 6.75 * PI)); //3001 -> 2641, 2981 -> 2661, 2989 -> 2653
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2661.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2653.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 757.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 757.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2653.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2641.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2641.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2641.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2653.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2661.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2661.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2653.0f, float(15.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2661.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2661.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2653.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 757.0f, 2641.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 757.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2653.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2641.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2641.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f, 2641.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2641.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 2641.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f, 2641.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 2641.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 2641.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2653.0f, float(39.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2661.0f, float(39.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2661.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2653.0f, float(27.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2653.0f, float(39.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.9f, 2661.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.9f, 2661.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2653.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2653.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 757.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 757.0f, 2641.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 757.0f, 2653.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 757.0f, 2653.0f, float(15.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 793.0f, 2641.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 793.0f, 2641.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2653.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2653.0f, float(15.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.1f, 2661.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.1f, 2661.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 793.0f, 2653.0f, float(-3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 793.0f, 2653.0f, float(15.0 + 6.75 * PI));


    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture3[0]);

    glBegin(GL_QUADS);

    //colline

    glColor3d(0.23 + 0.77 * bleuCiel(), 0.23 + 0.77 * bleuCiel(), 0.23 + 0.77 * bleuCiel());

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  600.0f, 2621.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  900.0f, 2621.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  900.0f, 3021.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  600.0f, 3021.0f, 0.0f); //sol


    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  769.0f, 3001.0f,  float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  781.0f, 3001.0f,  float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  781.0f, 3101.0f,  float(40.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  769.0f, 3101.0f,  float(40.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  600.0f, 2801.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  769.0f, 2801.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  769.0f, 3101.0f,  float(40.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  600.0f, 3101.0f,  float(40.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  781.0f, 2801.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  900.0f, 2801.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  900.0f, 3101.0f,  float(40.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  781.0f, 3101.0f,  float(40.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  769.0f, 2641.0f,  float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  781.0f, 2641.0f,  float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  781.0f, 2541.0f,  float(40.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  769.0f, 2541.0f,  float(40.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  600.0f, 2841.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  769.0f, 2841.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  769.0f, 2541.0f,  float(40.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  600.0f, 2541.0f,  float(40.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  781.0f, 2841.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  900.0f, 2841.0f,  float(-35.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  900.0f, 2541.0f,  float(40.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  781.0f, 2541.0f,  float(40.0 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture3[3]);

    glBegin(GL_QUADS);

    glColor3d(0.5, 0.5, 0.5);

    //terre-plein gris (utilisation de la couleur grise)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.9f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.9f, 225.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 225.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 150.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 150.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 225.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 225.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 150.0f, 0.8f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, 220.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, 220.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 220.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 220.0f, 0.0f);

    for (int c = 0; c < 24; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-470 - 94.5 * sin(c       * PI / 48)), float(-404 - 94.5 * cos(c       * PI/48)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-470 - 94.5 * sin((c + 1) * PI / 48)), float(-404 - 94.5 * cos((c + 1) * PI/48)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-470 - 93.0 * sin((c + 1) * PI / 48)), float(-404 - 93.0 * cos((c + 1) * PI/48)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-470 - 93.0 * sin(c       * PI / 48)), float(-404 - 93.0 * cos(c       * PI/48)), 4.0f);

    }

    for (int c = 0; c < 30; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 + 69.5 * cos(c       * PI / 30)), float(-404 + 69.5 * sin(c       * PI/30)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 + 10.0 * cos(c       * PI / 30)), float(-404 + 10.0 * sin(c       * PI/30)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 + 10.0 * cos((c + 1) * PI / 30)), float(-404 + 10.0 * sin((c + 1) * PI/30)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 + 69.5 * cos((c + 1) * PI / 30)), float(-404 + 69.5 * sin((c + 1) * PI/30)), 4.0f); //radical mais efficace (ou recouvre presque tout l'intérieur du demi-disque)

    }

    for (int c = 0; c < 18; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 - 69.5 * cos(c       * PI / 36)), float(-441 - 69.5 * sin(c       * PI/36)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 - 68.0 * cos(c       * PI / 36)), float(-441 - 68.0 * sin(c       * PI/36)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 - 68.0 * cos((c + 1) * PI / 36)), float(-441 - 68.0 * sin((c + 1) * PI/36)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 - 69.5 * cos((c + 1) * PI / 36)), float(-441 - 69.5 * sin((c + 1) * PI/36)), 4.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -370.0f, -509.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -370.0f, -511.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -470.0f, -511.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.0f, -509.0f, 4.0f);

    for (int c = 0; c < 64; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-206 + 144 * cos((c + 64) * PI / 256)), float(-360 - 144 * sin((c + 64) * PI / 256)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-206 + 144 * cos((c + 65) * PI / 256)), float(-360 - 144 * sin((c + 65) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-206 + 143 * cos((c + 65) * PI / 256)), float(-360 - 143 * sin((c + 65) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-206 + 143 * cos((c + 64) * PI / 256)), float(-360 - 143 * sin((c + 64) * PI / 256)), 4.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-156 + 144 * cos(c       * PI / 256)), float(-310 - 144 * sin(c       * PI / 256)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-156 + 144 * cos((c + 1) * PI / 256)), float(-310 - 144 * sin((c + 1) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-156 + 143 * cos((c + 1) * PI / 256)), float(-310 - 143 * sin((c + 1) * PI / 256)), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-156 + 143 * cos(c       * PI / 256)), float(-310 - 143 * sin(c       * PI / 256)), 4.0f);

    }

    for (int c = 0; c < 100; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  -6.0f, float(-100 + 2 * c), float(2 + 2* sin(-c       * PI/100 + PI/2)));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.0f, float(-100 + 2 * c), float(2 + 2* sin(-c       * PI/100 + PI/2)));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.0f, float( -98 + 2 * c), float(2 + 2* sin(-(c + 1) * PI/100 + PI/2)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  -6.0f, float( -98 + 2 * c), float(2 + 2* sin(-(c + 1) * PI/100 + PI/2)));

    }

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -12.0f,   50.0f, -2.5f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -12.0f, -200.0f, -2.5f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(   3.0f, -200.0f, -2.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(   3.0f,   50.0f, -2.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -100.0f,  200.0f, -6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -100.0f, -800.0f, -6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  100.0f, -800.0f, -6.0f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  100.0f,  200.0f, -6.0f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -9.1f, 125.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 125.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 140.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -9.1f, 140.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -9.1f, 265.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 265.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 325.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -9.1f, 325.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 2100.0f, 3244.105081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 2175.0f, 3244.105081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 2175.0f, 3247.305081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 2100.0f, 3247.305081f, float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 2195.0f, 3244.105081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 2240.0f, 3244.105081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 2240.0f, 3247.305081f, float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 2195.0f, 3247.305081f, float(0.0 + 6.75 * PI));

    for (int c = 96; c < 144; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2000.0 + 94.5 * cos(c       * PI/288)), float(3171.0 + 94.5 *sin(c       * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2000.0 + 94.5 * cos((c + 1) * PI/288)), float(3171.0 + 94.5 *sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2000.0 + 93.5 * cos((c + 1) * PI/288)), float(3171.0 + 93.5 *sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2000.0 + 93.5 * cos(c       * PI/288)), float(3171.0 + 93.5 *sin(c       * PI/288)),   float(0.0 + 6.75 * PI));

    }

    for (int c = -192; c < -144; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2100.0 +  99.5 * cos(c       * PI/288)), float(3344.205081 +  99.5 * sin(c       * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2100.0 +  99.5 * cos((c + 1) * PI/288)), float(3344.205081 +  99.5 * sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2100.0 + 100.5 * cos((c + 1) * PI/288)), float(3344.205081 + 100.5 * sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2100.0 + 100.5 * cos(c       * PI/288)), float(3344.205081 + 100.5 * sin(c       * PI/288)),   float(0.0 + 6.75 * PI));

    }

    glColor3d(luminanceStations[9], luminanceStations[9], luminanceStations[9]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 56.5 * cos(144 * PI/288)), float(3181.205081 + 56.5 * sin(144 * PI/288)),   float(0.0 - 0 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 57.5 * cos(144 * PI/288)), float(3181.205081 + 57.5 * sin(144 * PI/288)),   float(0.0 - 0 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glColor3d(0.25 + 0.5 * luminanceStations[9], 0.25 + 0.5 * luminanceStations[9], 0.25 + 0.5 * luminanceStations[9]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 57.5 * cos(143 * PI/288)), float(3181.205081 + 57.5 * sin(143 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 56.5 * cos(143 * PI/288)), float(3181.205081 + 56.5 * sin(143 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 56.5 * cos(143 * PI/288)), float(3181.205081 + 56.5 * sin(142 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 57.5 * cos(143 * PI/288)), float(3181.205081 + 57.5 * sin(142 * PI/288)),   float(0.0 - 1 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 57.5 * cos(142 * PI/288)), float(3181.205081 + 57.5 * sin(143 * PI/288)),   float(0.0 - 2 * 0.05 * 60 * PI/288 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 56.5 * cos(142 * PI/288)), float(3181.205081 + 56.5 * sin(143 * PI/288)),   float(0.0 - 2 * 0.05 * 60 * PI/288 + 6.75 * PI));

    for (int c = 96; c < 142; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 56.5 * cos(c       * PI/288)), float(3181.205081 + 56.5 * sin(c       * PI/288)),   float(0.0 + (-144 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 56.5 * cos((c + 1) * PI/288)), float(3181.205081 + 56.5 * sin((c + 1) * PI/288)),   float(0.0 + (-143 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 57.5 * cos((c + 1) * PI/288)), float(3181.205081 + 57.5 * sin((c + 1) * PI/288)),   float(0.0 + (-143 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 57.5 * cos(c       * PI/288)), float(3181.205081 + 57.5 * sin(c       * PI/288)),   float(0.0 + (-144 + c) * 0.05 * 60 * PI/288 + 6.75 * PI));

    }

    for (int c = -192; c < -72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 56.5 * cos(c       * PI/288)), float(3285.128129 + 56.5 * sin(c       * PI/288)),   float(0.0 + (-192 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 56.5 * cos((c + 1) * PI/288)), float(3285.128129 + 56.5 * sin((c + 1) * PI/288)),   float(0.0 + (-193 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 57.5 * cos((c + 1) * PI/288)), float(3285.128129 + 57.5 * sin((c + 1) * PI/288)),   float(0.0 + (-193 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 57.5 * cos(c       * PI/288)), float(3285.128129 + 57.5 * sin(c       * PI/288)),   float(0.0 + (-192 - c) * 0.05 * 60 * PI/288 + 6.25 * PI));

    }

    for (int c = -72; c < 72; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 56.5 * cos(c       * PI/288)), float(3285.128129 + 56.5 * sin(c       * PI/288)),   float(0.0 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 56.5 * cos((c + 1) * PI/288)), float(3285.128129 + 56.5 * sin((c + 1) * PI/288)),   float(0.0 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 57.5 * cos((c + 1) * PI/288)), float(3285.128129 + 57.5 * sin((c + 1) * PI/288)),   float(0.0 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 57.5 * cos(c       * PI/288)), float(3285.128129 + 57.5 * sin(c       * PI/288)),   float(0.0 + 5.00 * PI));

    }

    for (int c = 72; c < 240; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 56.5 * cos(c       * PI/288)), float(3285.128129 + 56.5 * sin(c       * PI/288)),   float(0.0 + (-72 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 56.5 * cos((c + 1) * PI/288)), float(3285.128129 + 56.5 * sin((c + 1) * PI/288)),   float(0.0 + (-71 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 57.5 * cos((c + 1) * PI/288)), float(3285.128129 + 57.5 * sin((c + 1) * PI/288)),   float(0.0 + (-71 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 57.5 * cos(c       * PI/288)), float(3285.128129 + 57.5 * sin(c       * PI/288)),   float(0.0 + (-72 + c) * 0.05 * 60 * PI/288 + 5.00 * PI));

    }

    for (int c = -140; c < -48; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(c       * PI/288)), float(3334.0 + 56.5 * sin(c       * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 56.5 * cos((c + 1) * PI/288)), float(3334.0 + 56.5 * sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 57.5 * cos((c + 1) * PI/288)), float(3334.0 + 57.5 * sin((c + 1) * PI/288)),   float(0.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(c       * PI/288)), float(3334.0 + 57.5 * sin(c       * PI/288)),   float(0.0 + 6.75 * PI));

    }

    glColor3d(luminanceStations[10], luminanceStations[10], luminanceStations[10]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-144 * PI/288)), float(3334.0 + 56.5 * sin(-144 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-144 * PI/288)), float(3334.0 + 57.5 * sin(-144 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.125 + 0.75 * luminanceStations[10], 0.125 + 0.75 * luminanceStations[10], 0.125 + 0.75 * luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-143 * PI/288)), float(3334.0 + 57.5 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-143 * PI/288)), float(3334.0 + 56.5 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-143 * PI/288)), float(3334.0 + 56.5 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-143 * PI/288)), float(3334.0 + 57.5 * sin(-143 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.25 + 0.5 * luminanceStations[10], 0.25 + 0.5 * luminanceStations[10], 0.25 + 0.5 * luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-142 * PI/288)), float(3334.0 + 57.5 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-142 * PI/288)), float(3334.0 + 56.5 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-142 * PI/288)), float(3334.0 + 56.5 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-142 * PI/288)), float(3334.0 + 57.5 * sin(-142 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.375 + 0.25 * luminanceStations[10], 0.375 + 0.25 * luminanceStations[10], 0.375 + 0.25 * luminanceStations[10]);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-141 * PI/288)), float(3334.0 + 57.5 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-141 * PI/288)), float(3334.0 + 56.5 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-141 * PI/288)), float(3334.0 + 56.5 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-141 * PI/288)), float(3334.0 + 57.5 * sin(-141 * PI/288)), float(0.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57.5 * cos(-140 * PI/288)), float(3334.0 + 57.5 * sin(-140 * PI/288)), float(0.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 56.5 * cos(-140 * PI/288)), float(3334.0 + 56.5 * sin(-140 * PI/288)), float(0.0 + 6.75 * PI));

    for (int c = -96; c < -32; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(950.0 + 94.5 * cos(c       * PI/192)), float(3271.0 + 94.5 * sin(c       * PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(950.0 + 94.5 * cos((c + 1) * PI/192)), float(3271.0 + 94.5 * sin((c + 1) * PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(950.0 + 93.5 * cos((c + 1) * PI/192)), float(3271.0 + 93.5 * sin((c + 1) * PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(950.0 + 93.5 * cos(c       * PI/192)), float(3271.0 + 93.5 * sin(c       * PI/192)),  float(10.0 + 6.75 * PI));

    }

    for (int c = 160; c > 96; c--) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(1123.205081 + 94.5 * cos(c       * PI/192)), float(3171.0 + 94.5 * sin(c       * PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(1123.205081 + 94.5 * cos((c - 1) * PI/192)), float(3171.0 + 94.5 * sin((c - 1) * PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(1123.205081 + 93.5 * cos((c - 1) * PI/192)), float(3171.0 + 93.5 * sin((c - 1) * PI/192)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(1123.205081 + 93.5 * cos(c       * PI/192)), float(3171.0 + 93.5 * sin(c       * PI/192)),  float(10.0 + 6.75 * PI));

    }

    glColor3d(luminanceStations[6], luminanceStations[6], luminanceStations[6]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 69.5 * cos(128 * PI/128)), float(3096.0 + 69.5 * sin(128 * PI/128)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 68.5 * cos(128 * PI/128)), float(3096.0 + 68.5 * sin(128 * PI/128)),  float(10.0 + 6.75 * PI));
    glColor3d(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 68.5 * cos(127 * PI/128)), float(3096.0 + 68.5 * sin(127 * PI/128)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 69.5 * cos(127 * PI/128)), float(3096.0 + 69.5 * sin(127 * PI/128)),  float(10.0 + 6.75 * PI));

    for (int c = 127; c > 64; c--) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 68.5 * cos(c       * PI/128)), float(3096.0 + 68.5 * sin(c       * PI/128)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 68.5 * cos((c - 1) * PI/128)), float(3096.0 + 68.5 * sin((c - 1) * PI/128)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 69.5 * cos((c - 1) * PI/128)), float(3096.0 + 69.5 * sin((c - 1) * PI/128)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 69.5 * cos(c       * PI/128)), float(3096.0 + 69.5 * sin(c       * PI/128)),  float(10.0 + 6.75 * PI));

    }    

    for (int c = 128; c > 96; c--) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 94.5 * cos(c       * PI/128)), float(1450.0 + 94.5 * sin(c       * PI/128)),  -6.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 94.5 * cos((c - 1) * PI/128)), float(1450.0 + 94.5 * sin((c - 1) * PI/128)),  -6.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 93.5 * cos((c - 1) * PI/128)), float(1450.0 + 93.5 * sin((c - 1) * PI/128)),  -6.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 93.5 * cos(c       * PI/128)), float(1450.0 + 93.5 * sin(c       * PI/128)),  -6.0f);

    }

    for (int c = 10; c < 128; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.5 * cos(-c       * PI/512)), float(2546.015511 + 668.5 * sin(-c       * PI/512)),  float(11.5 + 0.04 * (-c + 128) * 675 * PI/512));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.5 * cos(-(c + 1) * PI/512)), float(2546.015511 + 668.5 * sin(-(c + 1) * PI/512)),  float(11.5 + 0.04 * (-c + 127) * 675 * PI/512));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669.5 * cos(-(c + 1) * PI/512)), float(2546.015511 + 669.5 * sin(-(c + 1) * PI/512)),  float(11.5 + 0.04 * (-c + 127) * 675 * PI/512));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669.5 * cos(-c       * PI/512)), float(2546.015511 + 669.5 * sin(-c       * PI/512)),  float(11.5 + 0.04 * (-c + 128) * 675 * PI/512));

    }

    for (int c = 1; c < 9; c++) {

        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.5 * cos(-c       * PI/512)), float(2546.015511 + 668.5 * sin(-c       * PI/512)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.5 * cos(-(c + 1) * PI/512)), float(2546.015511 + 668.5 * sin(-(c + 1) * PI/512)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669.5 * cos(-(c + 1) * PI/512)), float(2546.015511 + 669.5 * sin(-(c + 1) * PI/512)),  float(10.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669.5 * cos(-c       * PI/512)), float(2546.015511 + 669.5 * sin(-c       * PI/512)),  float(10.0 + 6.75 * PI));

    }

    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.5 * cos(-9       * PI/512)), float(2546.015511 + 668.5 * sin(-9       * PI/512)),  float(10.0 + 6.75 * PI                       ));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.5 * cos(-(9 + 1) * PI/512)), float(2546.015511 + 668.5 * sin(-(9 + 1) * PI/512)),  float(11.5 + 0.04 * (-9 + 127) * 675 * PI/512));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669.5 * cos(-(9 + 1) * PI/512)), float(2546.015511 + 669.5 * sin(-(9 + 1) * PI/512)),  float(11.5 + 0.04 * (-9 + 127) * 675 * PI/512));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669.5 * cos(-9       * PI/512)), float(2546.015511 + 669.5 * sin(-9       * PI/512)),  float(10.0 + 6.75 * PI                       ));

    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 668.5 * cos(-(0 + 1) * PI/512)), float(2546.015511 + 668.5 * sin(-(0 + 1) * PI/512)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669.5 * cos(-(0 + 1) * PI/512)), float(2546.015511 + 669.5 * sin(-(0 + 1) * PI/512)),  float(10.0 + 6.75 * PI));
    glColor3d(luminanceStations[5], luminanceStations[5], luminanceStations[5]);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669.5 * cos(-0       * PI/512)), float(2546.015511 + 669.5 * sin(-0       * PI/512)),  float(10.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 668.5 * cos(-0       * PI/512)), float(2546.015511 + 668.5 * sin(-0       * PI/512)),  float(10.0 + 6.75 * PI));


    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture4[1]);

    glBegin(GL_QUADS);

    glColor3d(1, 1, 1);

    //arrière de train

    for (int n = 0; n < nombreRamesMax; n++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 75) - 1.5*cos(r(distTotaleAutres(n) - 75))), float(y(distTotaleAutres(n) - 75) + 1.5*sin(r(distTotaleAutres(n) - 75))), float(z(distTotaleAutres(n) - 75) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 75) + 1.5*cos(r(distTotaleAutres(n) - 75))), float(y(distTotaleAutres(n) - 75) - 1.5*sin(r(distTotaleAutres(n) - 75))), float(z(distTotaleAutres(n) - 75) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 75) + 1.5*cos(r(distTotaleAutres(n) - 75))), float(y(distTotaleAutres(n) - 75) - 1.5*sin(r(distTotaleAutres(n) - 75))), float(z(distTotaleAutres(n) - 75) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 75) - 1.5*cos(r(distTotaleAutres(n) - 75))), float(y(distTotaleAutres(n) - 75) + 1.5*sin(r(distTotaleAutres(n) - 75))), float(z(distTotaleAutres(n) - 75) + 1.5));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n)) - 1.5*cos(r(distTotaleAutres(n)))), float(y(distTotaleAutres(n)) + 1.5*sin(r(distTotaleAutres(n)))), float(z(distTotaleAutres(n)) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n)) + 1.5*cos(r(distTotaleAutres(n)))), float(y(distTotaleAutres(n)) - 1.5*sin(r(distTotaleAutres(n)))), float(z(distTotaleAutres(n)) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n)) + 1.5*cos(r(distTotaleAutres(n)))), float(y(distTotaleAutres(n)) - 1.5*sin(r(distTotaleAutres(n)))), float(z(distTotaleAutres(n)) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n)) - 1.5*cos(r(distTotaleAutres(n)))), float(y(distTotaleAutres(n)) + 1.5*sin(r(distTotaleAutres(n)))), float(z(distTotaleAutres(n)) + 1.5));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotale() - 75) - 1.5*cos(r(distTotale() - 75))), float(y(distTotale() - 75) + 1.5*sin(r(distTotale() - 75))), float(z(distTotale() - 75) - 1.5));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotale() - 75) + 1.5*cos(r(distTotale() - 75))), float(y(distTotale() - 75) - 1.5*sin(r(distTotale() - 75))), float(z(distTotale() - 75) - 1.5));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotale() - 75) + 1.5*cos(r(distTotale() - 75))), float(y(distTotale() - 75) - 1.5*sin(r(distTotale() - 75))), float(z(distTotale() - 75) + 1.5));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotale() - 75) - 1.5*cos(r(distTotale() - 75))), float(y(distTotale() - 75) + 1.5*sin(r(distTotale() - 75))), float(z(distTotale() - 75) + 1.5));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotale()) - 1.5*cos(r(distTotale()))), float(y(distTotale()) + 1.5*sin(r(distTotale()))), float(z(distTotale()) - 1.5));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotale()) + 1.5*cos(r(distTotale()))), float(y(distTotale()) - 1.5*sin(r(distTotale()))), float(z(distTotale()) - 1.5));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotale()) + 1.5*cos(r(distTotale()))), float(y(distTotale()) - 1.5*sin(r(distTotale()))), float(z(distTotale()) + 1.5));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotale()) - 1.5*cos(r(distTotale()))), float(y(distTotale()) + 1.5*sin(r(distTotale()))), float(z(distTotale()) + 1.5));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture4[4]);

    glBegin(GL_QUADS);

    for (int n = 0; n < nombreRamesMax; n++) {

        for (int i = 0; i < 5; i++) {

            glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3 ) + 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(y(distTotaleAutres(n) - 15 * i - 0.3 ) - 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(z(distTotaleAutres(n) - 15 * i - 0.3 ) - 1.5));
            glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 14.7) + 1.5*cos(r(distTotaleAutres(n) - 15 * i - 14.7))), float(y(distTotaleAutres(n) - 15 * i - 14.7) - 1.5*sin(r(distTotaleAutres(n) - 15 * i - 14.7))), float(z(distTotaleAutres(n) - 15 * i - 14.7) - 1.5));
            glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 14.7) + 1.5*cos(r(distTotaleAutres(n) - 15 * i - 14.7))), float(y(distTotaleAutres(n) - 15 * i - 14.7) - 1.5*sin(r(distTotaleAutres(n) - 15 * i - 14.7))), float(z(distTotaleAutres(n) - 15 * i - 14.7) + 1.5));
            glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3 ) + 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(y(distTotaleAutres(n) - 15 * i - 0.3 ) - 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(z(distTotaleAutres(n) - 15 * i - 0.3 ) + 1.5));

            glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3 ) - 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(y(distTotaleAutres(n) - 15 * i - 0.3 ) + 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(z(distTotaleAutres(n) - 15 * i - 0.3 ) - 1.5));
            glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 14.7) - 1.5*cos(r(distTotaleAutres(n) - 15 * i - 14.7))), float(y(distTotaleAutres(n) - 15 * i - 14.7) + 1.5*sin(r(distTotaleAutres(n) - 15 * i - 14.7))), float(z(distTotaleAutres(n) - 15 * i - 14.7) - 1.5));
            glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 14.7) - 1.5*cos(r(distTotaleAutres(n) - 15 * i - 14.7))), float(y(distTotaleAutres(n) - 15 * i - 14.7) + 1.5*sin(r(distTotaleAutres(n) - 15 * i - 14.7))), float(z(distTotaleAutres(n) - 15 * i - 14.7) + 1.5));
            glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotaleAutres(n) - 15 * i - 0.3 ) - 1.5*cos(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(y(distTotaleAutres(n) - 15 * i - 0.3 ) + 1.5*sin(r(distTotaleAutres(n) - 15 * i - 0.3 ))), float(z(distTotaleAutres(n) - 15 * i - 0.3 ) + 1.5));

        }

    }

    for (int i = 0; i < 5; i++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotale() - 15 * i - 0.3 ) + 1.5*cos(r(distTotale() - 15 * i - 0.3 ))), float(y(distTotale() - 15 * i - 0.3 ) - 1.5*sin(r(distTotale() - 15 * i - 0.3 ))), float(z(distTotale() - 15 * i - 0.3 ) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotale() - 15 * i - 14.7) + 1.5*cos(r(distTotale() - 15 * i - 14.7))), float(y(distTotale() - 15 * i - 14.7) - 1.5*sin(r(distTotale() - 15 * i - 14.7))), float(z(distTotale() - 15 * i - 14.7) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotale() - 15 * i - 14.7) + 1.5*cos(r(distTotale() - 15 * i - 14.7))), float(y(distTotale() - 15 * i - 14.7) - 1.5*sin(r(distTotale() - 15 * i - 14.7))), float(z(distTotale() - 15 * i - 14.7) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotale() - 15 * i - 0.3 ) + 1.5*cos(r(distTotale() - 15 * i - 0.3 ))), float(y(distTotale() - 15 * i - 0.3 ) - 1.5*sin(r(distTotale() - 15 * i - 0.3 ))), float(z(distTotale() - 15 * i - 0.3 ) + 1.5));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x(distTotale() - 15 * i - 0.3 ) - 1.5*cos(r(distTotale() - 15 * i - 0.3 ))), float(y(distTotale() - 15 * i - 0.3 ) + 1.5*sin(r(distTotale() - 15 * i - 0.3 ))), float(z(distTotale() - 15 * i - 0.3 ) - 1.5));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x(distTotale() - 15 * i - 14.7) - 1.5*cos(r(distTotale() - 15 * i - 14.7))), float(y(distTotale() - 15 * i - 14.7) + 1.5*sin(r(distTotale() - 15 * i - 14.7))), float(z(distTotale() - 15 * i - 14.7) - 1.5));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x(distTotale() - 15 * i - 14.7) - 1.5*cos(r(distTotale() - 15 * i - 14.7))), float(y(distTotale() - 15 * i - 14.7) + 1.5*sin(r(distTotale() - 15 * i - 14.7))), float(z(distTotale() - 15 * i - 14.7) + 1.5));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x(distTotale() - 15 * i - 0.3 ) - 1.5*cos(r(distTotale() - 15 * i - 0.3 ))), float(y(distTotale() - 15 * i - 0.3 ) + 1.5*sin(r(distTotale() - 15 * i - 0.3 ))), float(z(distTotale() - 15 * i - 0.3 ) + 1.5));

    }

    glEnd();

    afficherTempsAttente();

    if (!demo) {

        afficherFeux();

        if (estEnStation(distanceTotale)) {

            afficherRetard();
            afficherHorlogeRegulation();

        }

    }

}

void maFenetre::afficherRetard() {

    int s = int(abs(retard));
    bool positif = true;

    if (retard < 0)
        positif = false;

    int m = s / 60;
    s %= 60;

    if (Station == 1 || Station == 101) {

        m = 0;
        s = 0;
        positif = true;

    }

    switch(Station) {

    case 1:

        dessinIQ(positif, m, s, -250.0, -516.20, 5.50, 180);
        break;

    case 2:

        dessinIQ(positif, m, s, -5.92, 230.0, 1.50, 270);
        break;

    case 3:

        dessinIQ(positif, m, s, 5.08, 755.0, 1.50, 270);
        break;

    case 4:

        dessinIQ(positif, m, s, 5.08, 1430, -4.50, 270);
        break;

    case 5:

        dessinIQ(positif, m, s, 301.582002, 1785.819152, -1.50, 225);
        break;

    case 6:

        dessinIQ(positif, m, s, 780.08, 2626.0, 11.50 + 6.75 * PI, 270);
        break;

    case 7:

        dessinIQ(positif, m, s, 780.08, 3101.0, 11.50 + 6.75 * PI, 270);
        break;

    case 8:

        dessinIQ(positif, m, s, 1220.0, 3265.92, 11.50 + 6.75 * PI, 180);
        break;

    case 9:

        dessinIQ(positif, m, s, 1805.0, 3265.92, 1.50 + 6.75 * PI, 180);
        break;

    case 10:

        dessinIQ(positif, m, s, 2330.0, 3239.125081, 1.50 + 6.75 * PI, 180);
        break;

    case 101:

        dessinIQ(positif, m, s, 2194.652123, 3276.08, 1.50 + 6.75 * PI, 0);
        break;

    case 102:

        dessinIQ(positif, m, s, 1720.0, 3276.08, 1.50 + 6.75 * PI, 0);
        break;

    case 103:

        dessinIQ(positif, m, s, 1135.0, 3276.08, 11.50 + 6.75 * PI, 0);
        break;

    case 104:

        dessinIQ(positif, m, s, 769.92, 3016, 11.50 + 6.75 * PI, 90);
        break;

    case 105:

        dessinIQ(positif, m, s, 769.92, 2541.0, 11.50 + 6.75 * PI, 90);
        break;

    case 106:

        dessinIQ(positif, m, s, 234.293719, 1732.899281, -1.50, 45);
        break;

    case 107:

        dessinIQ(positif, m, s, -5.08, 1345.0, -4.50, 90);
        break;

    case 108:

        dessinIQ(positif, m, s, -5.08, 670.0, 1.50, 90);
        break;

    case 109:

        dessinIQ(positif, m, s, -14.08, 145.0, 1.50, 90);
        break;

    case 110:

        dessinIQ(positif, m, s, -320.0, -503.80, 5.50, 0);
        break;

    }

}

void maFenetre::afficherTempsAttente() {

    //SIEL République d.Remparts

    int m1, m2;

    double dt = temps_position(103798.35 + 931 * PI/4 + 200 * PI/3) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 109) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(103798.35 + 931 * PI/4 + 200 * PI/3) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 109) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.0f, 187.45f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.0f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.8f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.8f, 187.45f, 5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.0f, 187.55f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.0f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.2f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.2f, 187.55f, 5.6f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.8f, 187.45f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.8f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.6f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.6f, 187.45f, 5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.2f, 187.55f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.2f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.4f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.4f, 187.55f, 5.6f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureCouleurs[7]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -17.0f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -17.0f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -17.0f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -17.0f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.6f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.6f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.4f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.4f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.6f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.6f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.4f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.4f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.0f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.0f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.0f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.0f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.45f, 5.9f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.45f, 5.9f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.55f, 5.9f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.55f, 5.9f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.45f, 5.6f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.45f, 5.6f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.55f, 5.6f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.55f, 5.6f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -17.1f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -17.1f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -15.9f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -15.9f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -17.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -17.1f, 187.55f, 5.5f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -15.9f, 187.55f, 5.5f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -15.9f, 187.45f, 5.5f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.4f, 187.45f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.4f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.2f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.2f, 187.45f, 5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.6f, 187.55f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.6f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.8f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.8f, 187.55f, 5.6f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.2f, 187.45f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.2f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -16.0f, 187.45f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -16.0f, 187.45f, 5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -16.8f, 187.55f, 5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -16.8f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -17.0f, 187.55f, 5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -17.0f, 187.55f, 5.6f);


    glEnd();

    //SIEL Bouchers d.Remparts

    dt = temps_position(103273.35 + 931 * PI/4 + 200 * PI/3) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 108) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(103273.35 + 931 * PI/4 + 200 * PI/3) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 108) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -5.0f, 712.45f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -5.0f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.8f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.8f, 712.45f, 5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.0f, 712.55f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.0f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.2f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.2f, 712.55f, 5.7f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.8f, 712.45f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.8f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.6f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.6f, 712.45f, 5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.2f, 712.55f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.2f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.4f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.4f, 712.55f, 5.7f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.4f, 712.45f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.4f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.2f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.2f, 712.45f, 5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.6f, 712.55f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.6f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.8f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.8f, 712.55f, 5.7f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.2f, 712.45f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.2f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.0f, 712.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.0f, 712.45f, 5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.8f, 712.55f, 5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.8f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.0f, 712.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.0f, 712.55f, 5.7f);


    glEnd();

    //SIEL Chenilles d.Remparts

    dt = temps_position(102598.35 + 931 * PI/4 + 200 * PI/3) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 107) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(102598.35 + 931 * PI/4 + 200 * PI/3) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 107) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -5.0f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -5.0f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.8f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.8f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.0f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.0f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.2f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.2f, 1387.55f, -0.3f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.8f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.8f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.6f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.6f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.2f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.2f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.4f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.4f, 1387.55f, -0.3f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.4f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.4f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.2f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.2f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.6f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.6f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.8f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.8f, 1387.55f, -0.3f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.2f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.2f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -4.0f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -4.0f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -4.8f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -4.8f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.0f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.0f, 1387.55f, -0.3f);


    glEnd();

    //SIEL BDH d.Remparts

    dt = temps_position(102198.35 + 828 * PI/4 + 200 * PI/3) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 106) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(102198.35 + 828 * PI/4 + 200 * PI/3) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 106) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 264.366971f, 1762.859396f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 264.366971f, 1762.859396f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 264.508392f, 1762.717975f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 264.508392f, 1762.717975f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 265.144788f, 1762.248304f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 265.144788f, 1762.248304f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 265.003366f, 1762.364421f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 265.003366f, 1762.364421f,  2.7f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 264.508392f, 1762.717975f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 264.508392f, 1762.717975f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 264.649813f, 1762.576554f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 264.649813f, 1762.576554f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 265.003366f, 1762.364421f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 265.003366f, 1762.364421f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 264.861945f, 1762.505812f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 264.861945f, 1762.505812f,  2.7f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 264.791235f, 1762.435152f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 264.791235f, 1762.435132f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 264.932656f, 1762.293711f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 264.932656f, 1762.293711f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 264.720523f, 1762.647264f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 264.720523f, 1762.647264f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 264.579102f, 1762.788685f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 264.579102f, 1762.788685f,  2.7f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 264.932656f, 1762.293711f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 264.932656f, 1762.293711f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 265.074078f, 1762.152289f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 265.074078f, 1762.152289f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 264.579102f, 1762.788685f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 264.579102f, 1762.788685f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 264.437681f, 1762.930136f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 264.437681f, 1762.930136f,  2.7f);


    glEnd();

    //SIEL Cerisiers -F d.Remparts

    dt = temps_position(101723.35 + 156 * PI/4 + 200 * PI/3) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 105) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(101723.35 + 156 * PI/4 + 200 * PI/3) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 105) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.0f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.0f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.2f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.2f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 771.0f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 771.0f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.8f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.8f, 2584.55f, float(15.7 + 6.75 * PI));
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.2f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.2f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.4f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.4f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.8f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.8f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.6f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.6f, 2584.55f, float(15.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.6f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.6f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.8f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.8f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.4f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.4f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.2f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.2f, 2584.55f, float(15.7 + 6.75 * PI));

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.8f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.8f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 771.0f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 771.0f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.2f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.2f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.0f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.0f, 2584.55f, float(15.7 + 6.75 * PI));


    glEnd();

    //SIEL Blainville d.Remparts

    dt = temps_position(101248.35 + 156 * PI/4 + 200 * PI/3) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 104) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(101248.35 + 156 * PI/4 + 200 * PI/3) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 104) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.0f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.0f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.2f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.2f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 771.0f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 771.0f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.8f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.8f, 3059.55f, float(15.7 + 6.75 * PI));
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.2f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.2f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.4f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.4f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.8f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.8f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.6f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.6f, 3059.55f, float(15.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.6f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.6f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.8f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.8f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.4f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.4f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.2f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.2f, 3059.55f, float(15.7 + 6.75 * PI));

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.8f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.8f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 771.0f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 771.0f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 770.2f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 770.2f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 770.0f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 770.0f, 3059.55f, float(15.7 + 6.75 * PI));


    glEnd();

    //SIEL Mirla d.Remparts

    dt = temps_position(101056.35) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 103) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(101056.35) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 103) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3276.0f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3276.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3275.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3275.8f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3275.0f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3275.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3275.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3275.2f, float(15.7 + 6.75 * PI));



    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3275.8f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3275.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3275.6f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3275.6f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3275.2f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3275.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3275.4f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3275.4f, float(15.7 + 6.75 * PI));


    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3275.4f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3275.4f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3275.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3275.2f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3275.6f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3275.6f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3275.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3275.8f, float(15.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3275.2f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3275.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3275.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3275.0f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3275.8f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3275.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3276.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3276.0f, float(15.7 + 6.75 * PI));


    glEnd();

    //SIEL Histiaux d.Remparts

    dt = temps_position(100471.35) - temps_position(distanceTotale) + 30;

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 102) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(100471.35) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 102) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3276.0f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3276.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3275.8f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3275.0f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3275.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3275.2f, float(5.7 + 6.75 * PI));



    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3275.8f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3275.6f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3275.6f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3275.2f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3275.4f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3275.4f, float(5.7 + 6.75 * PI));


    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3275.4f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3275.4f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3275.2f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3275.6f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3275.6f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3275.8f, float(5.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3275.2f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3275.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3275.0f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3275.8f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3276.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3276.0f, float(5.7 + 6.75 * PI));


    glEnd();

    //SIEL Bourse d.Remparts

    dt = temps_position(4727 + 1705 * PI/4 + 300 * PI/3 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 101) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(4727 + 1705 * PI/4 + 300 * PI/3 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 101) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.102123f, 3276.0f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.102123f, 3276.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.102123f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.102123f, 3275.8f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.202123f, 3275.0f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.202123f, 3275.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.202123f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.202123f, 3275.2f, float(5.7 + 6.75 * PI));



    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.102123f, 3275.8f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.102123f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.102123f, 3275.6f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.102123f, 3275.6f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.202123f, 3275.2f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.202123f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.202123f, 3275.4f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.202123f, 3275.4f, float(5.7 + 6.75 * PI));


    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.102123f, 3275.4f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.102123f, 3275.4f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.102123f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.102123f, 3275.2f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.202123f, 3275.6f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.202123f, 3275.6f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.202123f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.202123f, 3275.8f, float(5.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.102123f, 3275.2f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.102123f, 3275.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.102123f, 3275.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.102123f, 3275.0f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2237.202123f, 3275.8f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2237.202123f, 3275.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2237.202123f, 3276.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2237.202123f, 3276.0f, float(5.7 + 6.75 * PI));


    glEnd();

    //SIEL Bourse d.Bourse

    dt = temps_position(4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 9) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 9) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.45f, 3246.205081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.45f, 3246.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.45f, 3246.005081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.45f, 3246.005081f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.55f, 3245.205081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.55f, 3245.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.55f, 3245.405081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.55f, 3245.405081f, float(5.7 + 6.75 * PI));

    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.45f, 3246.005081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.45f, 3246.005081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.45f, 3245.805081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.45f, 3245.805081f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.55f, 3245.405081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.55f, 3245.405081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.55f, 3245.605081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.55f, 3245.605081f, float(5.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.45f, 3245.605081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.45f, 3245.605081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.45f, 3245.405081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.45f, 3245.405081f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.55f, 3245.805081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.55f, 3245.805081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.55f, 3246.005081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.55f, 3246.005081f, float(5.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.45f, 3245.405081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.45f, 3245.405081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.45f, 3245.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.45f, 3245.205081f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2287.55f, 3246.005081f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2287.55f, 3246.005081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2287.55f, 3246.205081f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2287.55f, 3246.205081f, float(5.7 + 6.75 * PI));


    glEnd();

    //SIEL Histiaux d.Bourse

    dt = temps_position(4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 8) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 8) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3267.0f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3267.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3266.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3266.8f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3266.0f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3266.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3266.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3266.2f, float(5.7 + 6.75 * PI));


    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3266.8f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3266.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3266.6f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3266.6f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3266.2f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3266.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3266.4f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3266.4f, float(5.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3266.4f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3266.4f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3266.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3266.2f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3266.6f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3266.6f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3266.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3266.8f, float(5.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.45f, 3266.2f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.45f, 3266.2f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.45f, 3266.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.45f, 3266.0f, float(5.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1762.55f, 3266.8f, float(5.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1762.55f, 3266.8f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1762.55f, 3267.0f, float(6.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1762.55f, 3267.0f, float(5.7 + 6.75 * PI));


    glEnd();

    //SIEL Mirla d.Bourse

    dt = temps_position(3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 7) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 7) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3267.0f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3267.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3266.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3266.8f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3266.0f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3266.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3266.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3266.2f, float(15.7 + 6.75 * PI));

    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3266.8f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3266.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3266.6f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3266.6f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3266.2f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3266.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3266.4f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3266.4f, float(15.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3266.4f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3266.4f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3266.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3266.2f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3266.6f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3266.6f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3266.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3266.8f, float(15.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.45f, 3266.2f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.45f, 3266.2f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.45f, 3266.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.45f, 3266.0f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1177.55f, 3266.8f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1177.55f, 3266.8f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1177.55f, 3267.0f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1177.55f, 3267.0f, float(15.7 + 6.75 * PI));


    glEnd();

    //SIEL Blainville d.Bourse

    dt = temps_position(3438 + 1081 * PI/4 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 6) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(3438 + 1081 * PI/4 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 6) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.0f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.0f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.2f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.2f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 780.0f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 780.0f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.8f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.8f, 3059.55f, float(15.7 + 6.75 * PI));

    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.2f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.2f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.4f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.4f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.8f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.8f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.6f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.6f, 3059.55f, float(15.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.6f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.6f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.8f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.8f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.4f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.4f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.2f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.2f, 3059.55f, float(15.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.8f, 3059.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.8f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 780.0f, 3059.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 780.0f, 3059.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.2f, 3059.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.2f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.0f, 3059.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.0f, 3059.55f, float(15.7 + 6.75 * PI));


    glEnd();

    //SIEL Cerisiers-F d.Bourse

    dt = temps_position(2963 + 1081 * PI/4 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 5) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(2963 + 1081 * PI/4 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 5) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.0f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.0f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.2f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.2f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 780.0f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 780.0f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.8f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.8f, 2584.55f, float(15.7 + 6.75 * PI));

    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.2f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.2f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.4f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.4f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.8f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.8f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.6f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.6f, 2584.55f, float(15.7 + 6.75 * PI));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.6f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.6f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.8f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.8f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.4f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.4f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.2f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.2f, 2584.55f, float(15.7 + 6.75 * PI));


    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.8f, 2584.45f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.8f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 780.0f, 2584.45f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 780.0f, 2584.45f, float(15.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 779.2f, 2584.55f, float(15.7 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 779.2f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 779.0f, 2584.55f, float(16.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 779.0f, 2584.55f, float(15.7 + 6.75 * PI));


    glEnd();

    //SIEL BDH d.Bourse

    dt = temps_position(2488 + 403 * PI/4 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 4) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(2488 + 403 * PI/4 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 4) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 270.730932f, 1756.495435f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 270.730932f, 1756.495435f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 270.872353f, 1756.354014f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 270.872353f, 1756.354014f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 271.508749f, 1755.884343f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 271.508749f, 1755.884343f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 271.367327f, 1756.000460f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 271.367327f, 1756.000460f,  2.7f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 270.872353f, 1756.354014f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 270.872353f, 1756.354014f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 271.013774f, 1756.212593f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 271.013774f, 1756.212593f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 271.367327f, 1756.000460f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 271.367327f, 1756.000460f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 271.225906f, 1756.141851f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 271.225906f, 1756.141851f,  2.7f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 271.155196f, 1756.071191f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 271.155196f, 1756.071191f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 271.296617f, 1755.929750f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 271.296617f, 1755.929750f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 271.084484f, 1756.283303f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 271.084484f, 1756.283303f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 270.943063f, 1756.424724f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 270.943063f, 1756.424724f,  2.7f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 271.296617f, 1755.929750f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 271.296617f, 1755.929750f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 271.438039f, 1755.788328f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 271.438039f, 1755.788328f,  2.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( 270.943063f, 1756.424724f,  2.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( 270.943063f, 1756.424724f,  3.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( 270.801642f, 1756.566175f,  3.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( 270.801642f, 1756.566175f,  2.7f);


    glEnd();

    //SIEL Chenilles d.Bourse

    dt = temps_position(2088 + 306 * PI/4 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 3) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(2088 + 306 * PI/4 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 3) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.0f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.0f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.2f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.2f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  5.0f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  5.0f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.8f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.8f, 1387.55f, -0.3f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.2f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.2f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.4f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.4f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.8f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.8f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.6f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.6f, 1387.55f, -0.3f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.6f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.6f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.8f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.8f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.4f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.4f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.2f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.2f, 1387.55f, -0.3f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.8f, 1387.45f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.8f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  5.0f, 1387.45f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  5.0f, 1387.45f, -0.3f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.2f, 1387.55f, -0.3f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.2f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.0f, 1387.55f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.0f, 1387.55f, -0.3f);


    glEnd();

    //SIEL Bouchers d.Bourse

    dt = temps_position(1413 + 306 * PI/4 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 2) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(1413 + 306 * PI/4 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 2) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.0f, 712.45f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.0f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.2f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.2f, 712.45f,  5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  5.0f, 712.55f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  5.0f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.8f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.8f, 712.55f,  5.7f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.2f, 712.45f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.2f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.4f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.4f, 712.45f,  5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.8f, 712.55f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.8f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.6f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.6f, 712.55f,  5.7f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.6f, 712.45f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.6f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.8f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.8f, 712.45f,  5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.4f, 712.55f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.4f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.2f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.2f, 712.55f,  5.7f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.8f, 712.45f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.8f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  5.0f, 712.45f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  5.0f, 712.45f,  5.7f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(  4.2f, 712.55f,  5.7f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(  4.2f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(  4.0f, 712.55f,  6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(  4.0f, 712.55f,  5.7f);


    glEnd();

    //SIEL République d.Bourse

    dt = temps_position(888 + 306 * PI/4 + 50 * V2) - temps_position(distanceTotale) + 30;

    if (estEnStation(distanceTotale) && numeroStation(distanceTotale) == 1) {

        dt = 0;

    }

    else if (dt < 0) {

        dt += 1752;

    }

    m1 = int(dt/60);
    m2 = int(dt/60 + 29.20);

    for (int compteur = 0; compteur < nombreRames; compteur++) {

        dt = temps_position(888 + 306 * PI/4 + 50 * V2) - temps_position(distanceTotaleAutres[compteur]) + 30;

        if (estEnStation(distanceTotaleAutres[compteur]) && numeroStation(distanceTotaleAutres[compteur]) == 1) {

            dt = 0;

        }

        else if (dt < 0) {

            dt += 1752;

        }

        if (dt < m1 * 60) {

            m2 = m1;
            m1 = int(dt / 60);

        }

        else if (dt < m2 * 60) {

            m2 = int(dt / 60);

        }

    }

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m1 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.0f, 187.45f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.0f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.8f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.8f, 187.45f,  5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.0f, 187.55f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.0f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.2f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.2f, 187.55f,  5.6f);
    glEnd();

    switch(m1 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m1 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m1 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[4]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.8f, 187.45f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.8f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.6f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.6f, 187.45f,  5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.2f, 187.55f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.2f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.4f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.4f, 187.55f,  5.6f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureCouleurs[7]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -7.0f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -7.0f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -7.0f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -7.0f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.6f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.6f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.4f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.4f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.6f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.6f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.4f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.4f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.0f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.0f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.0f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.0f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.45f, 5.9f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.45f, 5.9f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.55f, 5.9f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.55f, 5.9f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.45f, 5.6f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.45f, 5.6f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.45f, 5.5f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.55f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.55f, 5.6f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.55f, 5.6f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -7.1f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -7.1f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -5.9f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -5.9f, 187.45f, 6.0f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.55f, 6.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.55f, 5.5f);


    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -7.1f, 187.45f, 5.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -7.1f, 187.55f, 5.5f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -5.9f, 187.55f, 5.5f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -5.9f, 187.45f, 5.5f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureChiffres1[int(m2 / 10)]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.4f, 187.45f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.4f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.2f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.2f, 187.45f,  5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.6f, 187.55f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.6f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.8f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.8f, 187.55f,  5.6f);

    glEnd();

    switch(m2 % 10) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

        glBindTexture(GL_TEXTURE_2D, textureChiffres1[m2 % 10]);
        break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[m2 % 10 - 5]);
        break;

    default:

        glBindTexture(GL_TEXTURE_2D, textureChiffres2[0]);

    }


    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.2f, 187.45f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.2f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -6.0f, 187.45f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -6.0f, 187.45f,  5.6f);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f( -6.8f, 187.55f,  5.6f);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f( -6.8f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( -7.0f, 187.55f,  5.9f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( -7.0f, 187.55f,  5.6f);


    glEnd();


}

void maFenetre::loadTexture(QString textureName) {

    bool b = satellite;

    QImage qim_Texture;
    QImage qim_TempTexture;


    if (qim_TempTexture.load(textureName)) {

        qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );


        if (numeroTexture < 10) {

            glGenTextures( 10, &textureStations[numeroTexture] );
            glBindTexture( GL_TEXTURE_2D, textureStations[numeroTexture] );

        }

        else if (numeroTexture < 23) {

            glGenTextures( 13, &textureTIV[numeroTexture - 10] );
            glBindTexture( GL_TEXTURE_2D, textureTIV[numeroTexture - 10] );

        }

        else if (numeroTexture < 29) {

            glGenTextures( 6, &texture[numeroTexture - 23] );
            glBindTexture( GL_TEXTURE_2D, texture[numeroTexture - 23] );

        }

        else if (numeroTexture < 35) {

            glGenTextures( 6, &texture2[numeroTexture - 29] );
            glBindTexture( GL_TEXTURE_2D, texture2[numeroTexture - 29] );

        }

        else if (numeroTexture < 41) {

            glGenTextures( 6, &texture3[numeroTexture - 35] );
            glBindTexture( GL_TEXTURE_2D, texture3[numeroTexture - 35]);

        }

        else if (numeroTexture < 46) {

            glGenTextures( 5, &texture4[numeroTexture - 41] );
            glBindTexture( GL_TEXTURE_2D, texture4[numeroTexture - 41]);

        }

        else if (numeroTexture < 51) {

            glGenTextures( 5, &texture5[numeroTexture - 46] );
            glBindTexture( GL_TEXTURE_2D, texture5[numeroTexture - 46]);

        }

        else if (numeroTexture < 56) {

            glGenTextures( 5, &textureChiffres1[numeroTexture - 51] );
            glBindTexture( GL_TEXTURE_2D, textureChiffres1[numeroTexture - 51]);

        }

        else if (numeroTexture < 61) {

            glGenTextures( 5, &textureChiffres2[numeroTexture - 56] );
            glBindTexture( GL_TEXTURE_2D, textureChiffres2[numeroTexture - 56]);

        }

        else if (numeroTexture < 71) {

            glGenTextures( 10, &textureTympansStations[numeroTexture - 61] );
            glBindTexture( GL_TEXTURE_2D, textureTympansStations[numeroTexture - 61]);

        }

        else if (numeroTexture < 81) {

            glGenTextures( 10, &textureIDD1Stations[numeroTexture - 71] );
            glBindTexture( GL_TEXTURE_2D, textureIDD1Stations[numeroTexture - 71]);

        }

        else if (numeroTexture < 91) {

            glGenTextures( 10, &textureIDD2Stations[numeroTexture - 81] );
            glBindTexture( GL_TEXTURE_2D, textureIDD2Stations[numeroTexture - 81]);

        }

        else if (numeroTexture < 96) {

            glGenTextures( 5, &texture6[numeroTexture - 91] );
            glBindTexture( GL_TEXTURE_2D, texture6[numeroTexture - 91]);

        }

        else if (numeroTexture < 99) {

            glGenTextures( 3, &texture7[numeroTexture - 96] );
            glBindTexture( GL_TEXTURE_2D, texture7[numeroTexture - 96]);

        }

        else if (numeroTexture < 104) {

            glGenTextures( 5, &textureIQ1[numeroTexture - 99] );
            glBindTexture( GL_TEXTURE_2D, textureIQ1[numeroTexture - 99]);

        }

        else if (numeroTexture < 109) {

            glGenTextures( 5, &textureIQ2[numeroTexture - 104] );
            glBindTexture( GL_TEXTURE_2D, textureIQ2[numeroTexture - 104]);

        }

        else if (numeroTexture < 112) {

            glGenTextures( 3, &textureIQ3[numeroTexture - 109] );
            glBindTexture( GL_TEXTURE_2D, textureIQ3[numeroTexture - 109]);

        }

        else if (numeroTexture < 118) {

            glGenTextures(6, &texturePlaceDesRemparts[numeroTexture - 112]);
            glBindTexture( GL_TEXTURE_2D, texturePlaceDesRemparts[numeroTexture - 112]);

        }

        else if (numeroTexture < 133) {

            glGenTextures( 15, &textureCouleurs[numeroTexture - 118]);
            glBindTexture( GL_TEXTURE_2D, textureCouleurs[numeroTexture - 118]);

        }

        else if (numeroTexture < 140) {

            glGenTextures( 7, &texturePersonnages1[numeroTexture - 133]);
            glBindTexture( GL_TEXTURE_2D, texturePersonnages1[numeroTexture - 133]);

        }

        emit emettreMaxTextures(137);

        satellite = b;

        glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        emit emettreTextureChargee(numeroTexture);

    }

    else {

        QString s;
        s.setNum(numeroTexture);

        QMessageBox::information(this, "Erreur", "Impossible de charger la texture n°" + s + ".");

    }

    numeroTexture++;

}

double maFenetre::x(double d) {

    double r = 0;

    if (d <= 356) {

        return -562 + d;

    }

    else if (d <= 356 + 153 * PI/4) {

        r = (d - 356) / 153;
        return -206.0 + 153 * sin(r);

    }

    else if (d <= 356 + 153 * PI/4 + 50 * V2) {

        r = (d - 356 - 153 * PI/4) / V2;
        return -97.812662 + r;

    }

    else if (d <= 356 + 306 * PI/4 + 50 * V2) {

        r = (d - 356 - 153 * PI/4 - 50 * V2) / 153;
        return -156.0 + 153 * sin(r + PI/4);

    }

    else if (d <= 896 + 306 * PI/4 + 50 * V2) {

        return -3.0;

    }

    else if (d <= 916 + 306 * PI/4 + 50 * V2) {

        return -3.0 + 0.3 * (d - 896 - 306 * PI/4 - 50 * V2);

    }

    else if (d <= 2116 + 306 * PI/4 + 50 * V2) {

        return 3.0;

    }

    else if (d <= 2116 + 403 * PI/4 + 50 * V2) {

        r = (d - 2116 - 306 * PI/4 - 50 * V2) / 97; //rot compris entre 0 et PI/4, dans la réalité entre 16PI/16 et 12PI/16
        return 100.0 - 97 * cos(r);

    }

    else if (d <= 2891 + 403 * PI/4 + 50 * V2) {

        r = d - 2116 - 403 * PI/4 - 50 * V2;
        return 100.0 - 97 * V2 / 2 + r * V2 / 2;

    }

    else if (d <= 2891 + 1081 * PI/4 + 50 * V2) {

        double R = d - 2891 - 403 * PI/4 - 50 * V2;
        r = R / 678; //Compris entre 0 et PI/4 (de 0 à PI/4)
        r = PI/4 - r; //Compris entre 0 et PI/4 (de PI/4 à 0)
        return 100.0 + 678 * cos(r);

    }

    else if (d <= 3441 + 1081 * PI/4 + 50 * V2) {

        return 778.0;

    }

    else if (d <= 3441 + 1225 * PI/4 + 50 * V2) {

        double R = d - 3441 - 1081 * PI/4 - 50 * V2;

        r = R / 72; //Compris entre 0 et PI/2 (de 0 à PI/2)
        r = PI - r; //Compris entre PI/2 et PI (de PI à PI/2)
        return 850 + 72 * cos(r);

    }

    else if (d <= 3541 + 1225 * PI/4 + 50 * V2) {

        return 850 + d - 3441 - 1225 * PI/4 - 50 * V2;

    }

    else if (d <= 3541 + 1225 * PI/4 + 103 * PI/3 + 50 * V2) {

        double R = d - 3541 - 1225 * PI/4 - 50 * V2;
        r = R / 103; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = PI/2 - r; //Compris entre PI/6 et PI/2 (de PI/2 à PI/6)
        return 950.0 + 103 * cos(r);

    }

    else if (d <= 3541 + 1225 * PI/4 + 50 * V2 + 200 * PI/3) {

        double R = d - 3541 - 1225 * PI/4 - 103 * PI/3 - 50 * V2;

        r = R / 97; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = 5*PI/6 - r; //Compris entre PI/2 et 5PI/6 (de 5PI/6 à PI/2)
        return 1123.205081 + 97 * cos(r);

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        return 1123.205081 + d - 3541 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 97 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;

        r = R / 97; //Compris entre 0 et PI/6 (de 0 à PI/6)
        r = PI/2 - r; //Compris entre PI/3 et PI/2 (de PI/2 à PI/3)
        return 2000.0 + 97 * cos(r);

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 97 * PI/6 - 50 * V2;

        r = R / 103; //Compris entre 0 et PI/6 (de 0 à PI/6)
        r = -2*PI/3 + r; //Compris entre -2PI/3 et -PI/2 (de -2PI/3 à -PI/2)

        return 2100.0 + 103 * cos(r);

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 200 * PI/6 - 50 * V2;
        return 2100.0 + R;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 260 * PI/6 + 50 * V2) {

        double R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 200 * PI/6 - 50 * V2;

        r = R / 60; //Compris entre 0 et PI/6 (de 0 à PI/6)
        r = PI/2 - r; //Compris entre PI/3 et PI/2 (de PI/2 à PI/3)
        return 2325.0 + 60 * cos(r);

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        double R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 260 * PI/6 - 50 * V2;

        r = R / 60; //Compris entre 0 et 9PI/6 (de 0 à 9PI/6)
        r = -2*PI/3 + r; //Compris entre -2PI/3 et 5PI/6 (de -2PI/3 à 5PI/6)
        return 2385.0 + 60 * cos(r);

    }

    else if (d <= 4655.644576 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        double  R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 800 * PI/6 - 50 * V2;
        return 2333.038477 - 0.5 * R;

    }

    else if (d <= 4655.644576 + 1225 * PI/4 + 260 * PI/3 + 800 * PI/6 + 50 * V2) {

        double R = d - 4655.644576 - 1225 * PI/4 - 200 * PI/3 - 800 * PI/6 - 50 * V2;
        r = R / 60; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = -PI/6 - r; //Compris entre -PI/6 et -3PI/6 (de -PI/6 à -3PI/6)
        return 2274.652123 + 60 * cos(r);

    }

    else if (d <= 6500) {

        double R =  d - 4655.644576 - 1225 * PI/4 - 260 * PI/3 - 800 * PI/6 - 50 * V2;
        return 2274.652123 - R;

    }

    else if (d <= 25000) {

        return d - 22000;

    }

    else if (d <= 99000) {

        double R =  d - 4655.644576 - 1225 * PI/4 - 260 * PI/3 - 800 * PI/6 - 50 * V2;
        return 2274.652123 - R;

    }


    else if (d <= 101076.447042) {

        double R = d - 100000;
        return 2199.652123 - R;

    }

    else if (d <= 101076.447042 + 103 * PI/3) {

        double R = d - 101076.4470427;

        r = R / 103; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = PI/2 + r; //Compris entre PI/2 et 5PI/6 (de PI/2 à 5PI/6)
        return 1123.205081 + 103 * cos(r);

    }

    else if (d <= 101076.447042 + 200 * PI/3) {

        double R = d - 101076.447042 - 103 * PI/3;
        r = R / 97; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = -PI/6 - r; //Compris entre -PI/6 et -PI/2 (de -PI/6 à -PI/2)
        return 950.0 + 97*cos(r);

    }

    else if (d <= 101176.447042 + 200 * PI/3) {

        double R = d - 101076.447042 - 200 * PI/3;
        return 950.0 - R;

    }

    else if (d <= 101176.447042 + 200 * PI/3 + 78 * PI/2) {

        double R = d - 101176.447042 - 200 * PI/3;

        r = R / 78; //Compris entre 0 et PI/2 (de 0 à PI/2)
        r = PI/2 + r; //Compris entre PI/2 et PI (de PI à PI/2)
        return 850.0 + 78 * cos(r);

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2) {

        return 772.0;

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2 + 672 * PI/4) {

        double R = d - 101726.431531 - 200 * PI/3 - 78 * PI/2;
        r = R / 672; //Compris entre 0 et PI/4
        r = -r; //de 0 à -PI/4
        return 100.0 + 672 * cos(r);

    }

    else if (d <= 102501.431531 + 200 * PI/3 + 828 * PI/4) {

        double R = d - 101726.431531 - 200 * PI/3 - 828 * PI/4;
        return 100.0 + 103*cos(12*PI/16) + (775 - R) * sin(4*PI/16);

    }

    else if (d <= 102501.431531 + 200 * PI/3 + 931 * PI/4) {

        double R = d - 102501.431531 - 200 * PI/3 - 828 * PI/4;
        r = R / 103; //Compris entre 0 et PI/4
        r = 3*PI/4 + r; //de 3PI/4 à PI
        return 100.0 + 103 * cos(r);

    }

    else if (d <= 103681.431531 + 931 * PI/4 + 200 * PI/3) {

        return -3.0;

    }

    else if (d <= 103701.431531 + 931 * PI/4 + 200 * PI/3) {

        return -3.0 - 9 * (d - 103681.431531 - 931 * PI/4 - 200 * PI/3) / 20;

    }

    else if (d <= 103951.431531 + 931 * PI/4 + 200 * PI/3) {

        return -12.0;

    }

    else if (d <= 103951.431531 + 931 * PI/4 + 200 * PI/3 + 100 * 0.173422) {

        double R = d - 103951.431531 - 931 * PI/4 - 200 * PI/3; //compris entre 0 et 100 * 0.173422
        r = R / 100; //compris entre 0 et 0.173422
        return 88 - 100 * cos(r);

    }

    else if (d <= 103951.431531 + 931 * PI/4 + 200 * PI/3 + 200 * 0.173422) {

        double R = d - 103951.431531 - 931 * PI/4 - 200 * PI/3 - 100 * 0.173422; //compris entre 0 et 100 * 0.173422
        r = 0.173422 - R / 100; //compris entre 0 et 0.173422
        return -109 + 100 * cos(r);

    }

    else if (d <= 104261.431531 + 931 * PI/4 + 200 * PI/3) {

        return -9.0;

    }

    else if (d <= 104261.431531 + 1078 * PI/4 + 200 * PI/3) {

        r = (d - 104261.431531 - 931 * PI/4 - 200 * PI/3) / 147; //Compris entre 0 et PI/4
        return -156 + 147 * cos(r);

    }

    else if (d <= 104261.431531 + 1078 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104261.431531 - 1078 * PI/4 - 200 * PI/3) / V2;
        return -52.055303 - r;

    }

    else if (d <= 104261.431531 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104261.431531 - 931 * PI/4 - 200 * PI/3 - 50 * V2) / 147; //Compris entre PI/4 et PI/2, pour éviter de déphaser
        return -206 + 147 * cos(r);

    }

    else if (d <= 104525.431531 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        return -206 - (d - 104261.431531 - 1225 * PI/4 - 200 * PI/3 - 50 * V2);

    }

    else if (d <= 104525.431531 + 1431 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104525.431531 - 1225 * PI/4 - 200 * PI/3 - 50 * V2) / 103; //compris entre 0 et PI/2
        return -470 - 103 * sin(r);

    }

    else if (d <= 104525.431531 + 1719 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104525.431531 - 1431 * PI/4 - 200 * PI/3 - 50 * V2) / 72; //compris entre 0 et PI
        return -645 + 72 * cos(r);

    }

    else if (d <= 104562.431531 + 1719 * PI/4 + 200 * PI/3 + 50 * V2) {

        return -717.0;

    }

    else if (d <= 104562.431531 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104562.431531 - 1719 * PI/4 - 200 * PI/3 - 50 * V2) / 72; //compris entre 0 et PI/2
        return -645 - 72 * cos(r);

    }

    else {

        return d - 104562.431531 - 1863 * PI/4 - 200 * PI/3 - 50 * V2 - 645;

    }

}

double maFenetre::y(double d) {

    double r = 0;

    if (d <= 356) {

        return -513;

    }

    else if (d <= 356 + 153 * PI/4) {

        r = (d - 356) / 153;
        return -360 - 153 * cos(r);

    }

    else if (d <= 356 + 153 * PI/4 + 50 * V2) {

        r = (d - 356 - 153 * PI/4) / V2;
        return -468.187338 + r;

    }

    else if (d <= 356 + 306 * PI/4 + 50 * V2) {

        r = (d - 356 - 153 * PI/4 - 50 * V2) / 153;
        return -310 - 153 * cos(r + PI/4);

    }

    else if (d <= 2116 + 306 * PI/4 + 50 * V2) {

        return d - 666 - 306 * PI/4 - 50 * V2;

    }

    else if (d <= 2116 + 403 * PI/4 + 50 * V2) {

        r = (d - 2116 - 306 * PI/4 - 50 * V2) / 97; //compris entre 0 et PI/4
        return 1450 + 97 * sin(r);

    }

    else if (d <= 2891 + 403 * PI/4 + 50 * V2) {

        double R = d - 2116 - 403 * PI/4 - 50 * V2;
        return 1450 + 97 * V2 / 2 + R * V2 / 2;

    }

    else if (d <= 2891 + 1081 * PI/4 + 50 * V2) {

        double R = d - 2891 - 403 * PI/4 - 50 * V2;
        r = R / 678; //Compris entre 0 et PI/4 (de 0 à PI/4)
        r = PI/4 - r; //Compris entre 0 et PI/4 (de PI/4 à 0)

        return 2546.015511 - 678 * sin(r);

    }

    else if (d <= 3441 + 1081 * PI/4 + 50 * V2) {

        double R = d - 2891 - 1081 * PI/4 - 50 * V2;

        return 2546.015511 + R;

    }

    else if (d <= 3441 + 1225 * PI/4 + 50 * V2) {

        double R = d - 3441 - 1081 * PI/4 - 50 * V2;

        r = R / 72; //Compris entre 0 et PI/2 (de 0 à PI/2);
        r = PI - r;

        return 3096.0 + 72 * sin(r);

    }

    else if (d <= 3541 + 1225 * PI/4 + 50 * V2) {

        return 3168.0;

    }

    else if (d <= 3541 + 1225 * PI/4 + 103 * PI/3 + 50 * V2) {

        double R = d - 3541 - 1225 * PI/4 - 50 * V2;
        r = R / 103; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = PI/2 - r; //Compris entre PI/6 et PI/2 (de PI/2 à PI/6)

        return 3271.0 + 103*sin(-r);

    }

    else if (d <= 3541 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        double R = d - 3541 - 1225 * PI/4 - 103 * PI/3 - 50 * V2;

        r = R / 97; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = 5*PI/6 - r; //Compris entre PI/2 et 5PI/6 (de 5PI/6 à PI/2)

        return 3171.0 + 97 * sin(r);

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        return 3268.0;

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 97 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;

        r = R / 97; //Compris entre 0 et PI/6 (de 0 à PI/6)
        r = PI/2 - r; //Compris entre PI/3 et PI/2 (de PI/2 à PI/3)

        return 3171.0 + 97 * sin(r);

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 97 * PI/6 - 50 * V2;

        r = R / 103; //Compris entre 0 et PI/6 (de 0 à PI/6)
        r = -2*PI/3 + r; //Compris entre -PI/3 et -PI/2 (de -2PI/3 à -PI/2)

        return 3344.205081 + 103 * sin(r);

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        return 3241.205081;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 260 * PI/6 + 50 * V2) {

        double R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 200 * PI/6 - 50 * V2;

        r = R / 60; //Compris entre 0 et PI/6 (de 0 à PI/6)
        r = PI/2 - r; //Compris entre PI/3 et PI/2 (de PI/2 à PI/3)

        return 3181.205081 + 60 * sin(r);

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        double R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 260 * PI/6 - 50 * V2;

        r = R / 60; //Compris entre 0 et 9PI/6 (de 0 à 9PI/6)
        r = -2*PI/3 + r; //Compris entre -2PI/3 et 5PI/6 (de -2PI/3 à 5PI/6)

        return 3285.128129 + 60 * sin(r);

    }

    else if (d <= 4655.644576 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        double  R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 800 * PI/6 - 50 * V2;
        return 3315.128129 - 0.866025 * R;

    }

    else if (d <= 4655.644576 + 1225 * PI/4 + 260 * PI/3 + 800 * PI/6 + 50 * V2) {

        double R = d - 4655.644576 - 1225 * PI/4 - 200 * PI/3 - 800 * PI/6 - 50 * V2;

        r = R / 60; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = -PI/6 - r; //Compris entre -PI/6 et -3PI/6 (de -PI/6 à -3PI/6)

        return 3334.0 + 60 * sin(r);

    }

    else if (d <= 6500) {

        return 3274.0;

    }

    else if (d <= 25000) {

        return 3250.205081;

    }


    else if (d <= 101076.447042) {

        return 3274.0;

    }

    else if (d <= 101076.447042 + 103 * PI/3) {

        double R = d - 101076.447042;

        r = R / 103; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = PI/2 + r; //Compris entre PI/2 et 5PI/6 (de PI/2 à 5PI/6)

        return 3171.0 + 103 * sin(r);

    }

    else if (d <= 101076.447042 + 200 * PI/3) {

        double R = d - 101076.447042 - 103 * PI/3;
        r = R / 97; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = -PI/6 - r; //Compris entre -PI/6 et -PI/2 (de -PI/6 à -PI/2)

        return 3271.0 + 97*sin(r);

    }

    else if (d <= 101176.447042 + 200 * PI/3) {

        return 3174.0;

    }

    else if (d <= 101176.447042 + 200 * PI/3 + 78 * PI/2) {

        double R = d - 101176.447042 - 200 * PI/3;

        r = R / 78; //Compris entre 0 et PI/2 (de 0 à PI/2)
        r = PI/2 + r; //Compris entre PI/2 et PI (de PI à PI/2)

        return 3096.0 + 78 * sin(r);

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2) {

        double R = d - 101176.447042 - 200 * PI/3 - 78 * PI/2;
        return 3096.0 - R; //2546.015511 -> 449.984489

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2 + 672 * PI/4) {

        double R = d - 101726.431531 - 200 * PI/3 - 78 * PI/2;
        r = R / 672; //Compris entre 0 et PI/4
        r = -r; //de 0 à -PI/4

        return 2546.015511 + 672 * sin(r);

    }

    else if (d <= 102501.431531 + 200 * PI/3 + 828 * PI/4) {

        double R = d - 101726.431531 - 200 *PI/3 - 78 * PI/2 - 672 * PI/4;

        return 1450 + 103 * sin(12*PI/16) + (775 - R) * cos(4*PI/16);

    }

    else if (d <= 102501.431531 + 200 * PI/3 + 931 * PI/4) {

        double R = d - 102501.431531 - 200 * PI/3 - 828 * PI/4;
        r = R / 103; //Compris entre 0 et PI/4
        r = 3*PI/4 + r; //de 3PI/4 à PI

        return 1450.0 + 103 * sin(r);

    }

    else if (d <= 103951.431531 + 200 * PI/3 + 931 * PI/4) {

        double R = d - 102501.431531 - 200 * PI/3 - 931 * PI/4;
        return 1450.0 - R;

    }

    else if (d <= 103951.431531 + 931 * PI/4 + 200 * PI/3 + 100 * 0.173422) {

        double R = d - 103951.431531 - 931 * PI/4 - 200 * PI/3; //compris entre 0 et 100 * 0.173422
        r = R / 100; //compris entre 0 et 0.173422
        return -100 * sin(r);

    }

    else if (d <= 103951.431531 + 931 * PI/4 + 200 * PI/3 + 200 * 0.173422) {

        double R = d - 103951.431531 - 931 * PI/4 - 200 * PI/3 - 100 * 0.173422; //compris entre 0 et 100 * 0.173422
        r = 0.173422 - R / 100; //compris entre 0 et 0.173422
        return -34.510867 + 100 * sin(r);

    }

    else if (d <= 104261.431531 + 931 * PI/4 + 200 * PI/3) {

        r = d - 103951.431531 - 931 * PI/4 - 200 * PI/3 - 200 * 0.173422;
        return -34.510867 - r;

    }

    else if (d <= 104261.431531 + 1078 * PI/4 + 200 * PI/3) {

        r = (d - 104261.431531 - 931 * PI/4 - 200 * PI/3) / 147; //Compris entre 0 et PI/4
        return -310 - 147 * sin(r);

    }

    else if (d <= 104261.431531 + 1078 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104261.431531 - 1078 * PI/4 - 200 * PI/3) / V2;
        return -413.944697 - r;

    }

    else if (d <= 104261.431531 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104261.431531 - 931 * PI/4 - 200 * PI/3 - 50 * V2) / 147; //Compris entre PI/4 et PI/2, pour éviter de déphaser
        return -360 - 147 * sin(r);

    }

    else if (d <= 104525.431531 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        return -507;

    }

    else if (d <= 104525.431531 + 1431 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104525.431531 - 1225 * PI/4 - 200 * PI/3 - 50 * V2) / 103; //compris entre 0 et PI/2
        return -404 - 103 * cos(r);

    }

    else if (d <= 104525.431531 + 1719 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104525.431531 - 1431 * PI/4 - 200 * PI/3 - 50 * V2) / 72; //compris entre 0 et PI
        return -404 + 72 * sin(r);

    }

    else if (d <= 104562.431531 + 1719 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = d - 104525.431531 - 1719 * PI/4 - 200 * PI/3 - 50 * V2;
        return -404 - r;

    }

    else if (d <= 104562.431531 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        r = (d - 104562.431531 - 1719 * PI/4 - 200 * PI/3 - 50 * V2) / 72; //compris entre 0 et PI/2
        return -441 - 72 * sin(r);

    }

    else {

        return -513;

    }

}

double maFenetre::z(double d) {

    if (d <= 566 + 306 * PI/4 + 50 * V2) {

        return 5.5;

    }

    else if (d <= 766 + 306 * PI/4 + 50 * V2) {

        return 3.5 + 2 * sin((666 + 306 * PI/4 + 50 * V2 - d) * PI / 200);

    }

    else if (d <= 1416 + 306 * PI/4 + 50 * V2) {

        return 1.5;

    }

    else if (d <= 2016 + 306 * PI/4 + 50 * V2) {

        return 1.5 - 0.01 * (d - 1416 - 306 * PI/4 - 50 * V2);

    }

    else if (d <= 2116 + 403 * PI/4 + 50 * V2) {

        return -4.5;

    }

    else if (d <= 2416 + 403 * PI/4 + 50 * V2) {

        return -4.5 + 0.01 * (d - 2116 - 403 * PI/4 - 50 * V2);

    }

    else if (d <= 2528.5 + 403 * PI/4 + 50 * V2) {

        return -1.5;

    }

    else if (d <= 2891 + 403 * PI/4 + 675 * PI/4 * 118/128 + 50 * V2) {

        return -1.5 + 0.04 * (d - 2528.5 - 403 * PI/4 - 50 * V2);

    }

    else if (d <= 2891 + 403 * PI/4 + 675 * PI/4 * 119/128 + 50 * V2) {

        double r = d - (2891 + 403 * PI/4 + 675 * PI/4 * 118/128 + 50 * V2); //compris entre 0 et PI/4 * 675/128
        r *= 128 * 4 / (675 * PI); //compris entre 0 et 1
        return 13.0 + 0.04 * 118 * 675 * PI/512 + r * (11.5 + 6.75 * PI - 13.0 - 0.04 * 118 * 675 * PI/512);

    }

    else if (d <= (-9/128) * 675 * PI/4 + 3657.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) { //en effet la hauteur est stable un peu avant l'arrivée à CSF

        return 11.5 + 6.75 * PI;

    }

    else if (d <= 3677.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {  //1

        double r = d - 3657.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;
        return 11.5 + 6.75 * PI - 0.01 * r;

    }

    else if (d <= 3827.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) { //2

        double r = d - 3677.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;
        return 11.3 + 6.75 * PI - 0.02 * r;

    }

    else if (d <= 3977.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) { //3

        double r = d - 3827.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;
        return 8.3 + 6.75 * PI - 0.03 * r;

    }

    else if (d <= 4131.128252 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) { //1.5

        double r = d - 3977.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;
        return 3.8 + 6.75 * PI - 0.015 * r;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        return 1.5 + 6.75 * PI;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 410 * PI/6 + 50 * V2) {

        double r = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 200 * PI/6 - 50 * V2;
        return 1.5 + 6.75 * PI - 0.05 * r;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 590 * PI/6 + 50 * V2) {

        return 1.5 + 5.00 * PI;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        double r = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 590 * PI/6 - 50 * V2;
        return 1.5 + 5.00 * PI + 0.05 * r;

    }

    else if (d <= 6500) {

        return 1.5 + 6.75 * PI;

    }

    else if (d <= 25000) {

        return 1.5 + 6.75 * PI;

    }


    else if (d <= 100474.347877) {

        return 1.5 + 6.75 * PI;

    }

    else if (d <= 100627.681210) { //1.5

        double r = d - 100474.347877;
        return 1.5 + 6.75 * PI + 0.015 * r;

    }

    else if (d <= 100777.681210) { //3

        double r = d - 100627.681210;
        return 3.8 + 6.75 * PI + 0.03 * r;

    }

    else if (d <= 100927.681210) { //2

        double r = d - 100777.681210;
        return 8.3 + 6.75 * PI + 0.02 * r;

    }

    else if (d <= 100947.681210) {  //1

        double r = d - 100927.681210;
        return 11.3 + 6.75 * PI + 0.01 * r;

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2 + 9 * 672 * PI/512) {

        return 11.5 + 6.75 * PI;

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2 + 10 * 672 * PI/512) {

        double r = d - (101726.431531 + 200 * PI/3 + 78 * PI/2 + 9 * 672 * PI/512);
        r *= 512 / (672 * PI); //compris entre 0 et 1, 0 correspond à 9, 1 à 10
       // return (13.0 + 0.04 * 118 * 675 * PI/512) + (1 - r) * (11.5 + 6.75 * PI - 13.0 - 0.04 * 118 * 675 * PI/512);
        return 11.5 + 6.75 * PI + r * (13.0 + 0.04 * 118 * 672 * PI/512 - 11.5 - 6.75 * PI);

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2 + 128 * 672 * PI/512) {

        double r = d - 101726.431531 - 200 * PI/3 - 78 * PI/2 - 10 * 672 * PI/512; //compris entre 0 et 118 * 672 * PI/512
        double s = 118 * 672 * PI/512 - r; //Compris entre 0 et 118 * PI/512, mais dans l'autre sens
        return 13.0 + 0.04 * s;

    }

    else if (d <= 102088.931531 + 200 * PI/3 + 78 * PI/2 + 128 * 672 * PI/512) {

        double r = d - 101726.431531 - 200 * PI/3 - 78 * PI/2 - 128 * 672 * PI/512; //compris entre 0 et 362.5
        double s = 362.5 - r;
        return -1.5 + 0.04 * s;

    }

    else if (d <= 102201.431531 + 200 * PI/3 + 78 * PI/2 + 672 * PI/4) {

        return -1.5;

    }

    else if (d <= 102501.431531 + 200 * PI/3 + 828 * PI/4) {

        double r = d - 102201.431531 - 200 * PI/3 - 828 * PI/4;
        return -1.5 - 0.01 * r;

    }

    else if (d <= 102576.431531 + 200 * PI/3 + 931 * PI/4) {

        return -4.5;

    }

    else if (d <= 103176.431531 + 200 * PI/3 + 931 * PI/4) {

        double r = d - 102576.431531 - 200 * PI/3 - 931 * PI/4;
        return -4.5 + 0.01 * r;

    }

    else if (d <= 103826.431531 + 200 * PI/3 + 931 * PI/4) {

        return 1.5;

    }

    else if (d <= 103946.431531 + 200 * PI/3 + 931 * PI/4) {

        double r = d - 103826.431531 - 200 * PI/3 - 931 * PI/4;
        return 0.3 - 1.2 * sin(r * PI/120 - PI/2);

    }

    else if (d <= 104071.431531 + 200 * PI/3 + 931 * PI/4) {

        return -0.9;

    }

    else if (d <= 104071.431531 + 200 * PI/3 + 971 * PI/4) {

        double r = d - 104071.431531 - 200 * PI/3 - 931 * PI/4;
        return -0.1 + 0.8 * sin(r / 20 - PI/2);

    }

    else if (d <= 104191.431531 + 200 * PI/3 + 971 * PI/4) {

        return -0.1 + 0.04 * (d - 104071.431531 - 200 * PI/3 - 971 * PI/4);

    }

    else if (d <= 104191.431531 + 200 * PI/3 + 1011 * PI/4) {

        double r = d - 104191.431531 - 200 * PI/3 - 971 * PI/4;
        return 4.7 + 0.8 * sin(r / 20);

    }

    else {

        return 5.5;

    }

}

double maFenetre::r(double d) {

    double r = 0;

    if (d <= 356) {

        return PI/2;

    }

    else if (d <= 356 + 153 * PI/4) {

        return PI/2 - (d - 356) / 153;

    }

    else if (d <= 356 + 153 * PI/4 + 50 * V2) {

        return PI/4;

    }

    else if (d <= 356 + 306 * PI/4 + 50 * V2) {

        return PI/2 - (d - 356 -50 * V2) / 153;

    }

    else if (d <= 896 + 306 * PI/4 + 50 * V2) {

        return 0;

    }

    else if (d <= 899 + 306 * PI/4 + 50 * V2) {

        return 0.291457 * (d - 896 - 306 * PI/4 - 50 * V2) / 3;

    }

    else if (d <= 913 + 306 * PI/4 + 50 * V2) {

        return 0.291457; //sin(angle) = 6/sqrt(436)

    }

    else if (d <= 916 + 306 * PI/4 + 50 * V2) {

        return 0.291457 * (916 + 306 * PI/4 + 50 * V2 - d) / 3;

    }

    else if (d <= 2116 + 306 * PI/4 + 50 * V2) {

        return 0;

    }

    else if (d <= 2116 + 403 * PI/4 + 50 * V2) {

        return (d - 2116 - 306 * PI/4 - 50 * V2) / 97; //rot compris entre 0 et PI/4, dans la réalité entre 16PI/16 et 12PI/16

    }

    else if (d <= 2891 + 403 * PI/4 + 50 * V2) {

        return PI/4;

    }

    else if (d <= 2891 + 1081 * PI/4 + 50 * V2) {

        double R = d - 2891 - 403 * PI/4 - 50 * V2;
        r = R / 678; //Compris entre 0 et PI/4 (de 0 à PI/4)
        r = PI/4 - r; //Compris entre 0 et PI/4 (de PI/4 à 0)
        return r;

    }

    else if (d <= 3441 + 1081 * PI/4 + 50 * V2) {

        return 0;


    }

    else if (d <= 3441 + 1225 * PI/4 + 50 * V2) {

        double R = d - 3441 - 1081 * PI/4 - 50 * V2;
        return R / 72; //Compris entre 0 et PI/2 (de 0 à PI/2)

    }

    else if (d <= 3541 + 1225 * PI/4 + 50 * V2) {

        return PI/2;

    }

    else if (d <= 3541 + 1225 * PI/4 + 103 * PI/3 + 50 * V2) {

        double R = d - 3541 - 1225 * PI/4 - 50 * V2;
        r = R / 103; //Compris entre 0 et PI/3 (de 0 à PI/3)
        r = PI/2 - r; //Compris entre PI/6 et PI/2 (de PI/2 à PI/6)
        return r;

    }

    else if (d <= 3541 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        double R = d - 3541 - 1225 * PI/4 - 103 * PI/3 - 50 * V2;
        r = R / 97; //Compris entre 0 et PI/3 (de 0 à PI/3)
        return PI/6 + r; //de PI/6 à PI/2

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        return PI/2;

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 97 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 50 * V2;
        r = R / 97; //Compris entre 0 et PI/6 (de 0 à PI/6)
        return PI/2 + r; //de PI/2 à 2PI/3

    }

    else if (d <= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        double R = d - 4417.794919 - 1225 * PI/4 - 200 * PI/3 - 97 * PI/6 - 50 * V2;
        r = R / 103; //Compris entre 0 et PI/6 (de 0 à PI/6)
        return 2*PI/3 - r; //de PI/2 à 2PI/3 (de 2PI/3 à PI/2)

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2) {

        return PI/2;

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 260 * PI/6 + 50 * V2) {

        double R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 200 * PI/6 - 50 * V2;
        r = R / 60; //Compris entre 0 et PI/6 (de 0 à PI/6)
        return PI/2 + r; //de PI/2 à 2PI/3 (de 2PI/3 à PI/2)

    }

    else if (d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        double R = d - 4642.794919 - 1225 * PI/4 - 200 * PI/3 - 260 * PI/6 - 50 * V2;
        r = R / 60; //Compris entre 0 et 9PI/6 (de 0 à 9PI/6)
        return 2*PI/3 - r; //de PI/2 à 2PI/3 (de 2PI/3 à -5PI/6)

    }

    else if (d <= 4657.644576 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) {

        return -5*PI/6;

    }

    else if (d <= 4657.644576 + 1225 * PI/4 + 260 * PI/3 + 800 * PI/6 + 50 * V2) {

        double R = d - 4657.644576 - 1225 * PI/4 - 200 * PI/3 - 800 * PI/6 - 50 * V2;
        r = R / 60; //Compris entre 0 et PI/3 (de 0 à PI/3)
        return -5*PI/6 + r; //de -5PI/6 à -PI/2 (de -5PI/6 à -PI/2)

    }

    else if (d <= 6500) {

        return -PI/2;

    }

    else if (d <= 25000) {

        return PI/2;

    }


    else if (d <= 101076.447042) {

        return -PI/2;

    }

    else if (d <= 101076.447042 + 103 * PI/3) {

        double R = d - 101076.4470427;
        r = R / 103; //Compris entre 0 et PI/3 (de 0 à PI/3)
        return -PI/2 - r; //de -PI/2 à -5PI/6

    }

    else if (d <= 101076.447042 + 200 * PI/3) {

        double R = d - 101076.447042 - 103 * PI/3;
        r = R / 97; //Compris entre 0 et PI/3 (de 0 à PI/3)
        return -5*PI/6 + r; //de -5PI/6 à -PI/2

    }

    else if (d <= 101176.447042 + 200 * PI/3) {

        return -PI/2;

    }

    else if (d <= 101176.447042 + 200 * PI/3 + 78 * PI/2) {

        double R = d - 101176.447042 - 200 * PI/3;
        r = R / 78; //Compris entre 0 et PI/2 (de 0 à PI/2)
        return -PI/2 - r;

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2) {

        return -PI;

    }

    else if (d <= 101726.431531 + 200 * PI/3 + 78 * PI/2 + 672 * PI/4) {

        double R = d - 101726.431531 - 200 * PI/3 - 78 * PI/2;
        r = R / 672; //Compris entre 0 et PI/4
        r = -r; //de 0 à -PI/4
        return -PI - r;

    }

    else if (d <= 102501.431531 + 200 *PI/3 + 78 * PI/2 + 672 * PI/4) {

        return -3*PI/4;

    }

    else if (d <= 102501.431531 + 200 * PI/3 + 78 * PI/2 + 672 * PI/4 + 103 * PI/4) {

        double R = d - 102501.431531 - 200 * PI/3 - 78 * PI/2 - 672 * PI/4;
        r = R / 103; //Compris entre 0 et PI/4
        r = 3*PI/4 + r; //de 3PI/4 à PI
        return - r;

    }

    else if (d <= 103681.431531 + 200 * PI/3 + 931 * PI/4) {

        return PI;

    }

    else if (d <= 103686.431531 + 200 * PI/3 + 931 * PI/4) {

        return PI + 0.422854 * (d - 103681.431531 - 200 * PI/3 - 931 * PI/4) / 5;

    }

    else if (d <= 103696.431531 + 200 * PI/3 + 931 * PI/4) {

        return PI + 0.422854; //sin(angle) = 9/sqrt(481)

    }

    else if (d <= 103701.431531 + 200 * PI/3 + 931 * PI/4) {

        return PI + 0.422854 * (103701.431531 + 200 * PI/3 + 931 * PI/4 - d) / 5;

    }

    else if (d <= 103951.431531 + 200 * PI/3 + 931 * PI/4) {

        return PI;

    }

    else if (d <= 103951.431531 + 200 * PI/3 + 931 * PI/4 + 100 * 0.173422) {

        return PI - (d - 103951.431531 - 200 * PI/3 - 931 * PI/4) / 100;

    }

    else if (d <= 103951.431531 + 200 * PI/3 + 931 * PI/4 + 200 * 0.173422) {

        return PI - 0.173422 + (d - 103951.431531 - 200 * PI/3 - 931 * PI/4 - 100 * 0.173422) / 100;

    }

    else if (d <= 104261.431531 + 200 * PI/3 + 931 * PI/4) {

        return PI;

    }

    else if (d <= 104261.431531 + 200 * PI/3 + 1078 * PI/4) {

        return PI + (d - 104261.431531 - 200 * PI/3 - 931 * PI/4) / 147;

    }

    else if (d <= 104261.431531 + 200 * PI/3 + 1078 * PI/4 + 50 * V2) {

        return 5 * PI/4;

    }

    else if (d <= 104261.431531 + 200 * PI/3 + 1225 * PI/4 + 50 * V2) {

        return 5 * PI/4 + (d - 104261.431531 - 200 * PI/3 - 1078 * PI/4 - 50 * V2) / 147;

    }

    else if (d <= 104525.431531 + 200 * PI/3 + 1225 * PI/4 + 50 * V2) {

        return 3 * PI/2;

    }

    else if (d <= 104525.431531 + 200 * PI/3 + 1431 * PI/4 + 50 * V2) {

        r = (d - 104525.431531 - 200 * PI/3 - 1225 * PI/4 - 50 * V2) / 103; //compris entre 0 et PI/2
        return 3 * PI/2 + r;

    }

    else if (d <= 104525.431531 + 200 * PI/3 + 1719 * PI/4 + 50 * V2) {

        r = (d - 104525.431531 - 200 * PI/3 - 1431 * PI/4 - 50 * V2) / 72; //compris entre 0 et PI
        return -r;

    }

    else if (d <= 104562.431531 + 200 * PI/3 + 1719 * PI/4 + 50 * V2) {

        return -PI;

    }

    else if (d <= 104562.431531 + 200 * PI/3 + 1863 * PI/4 + 50 * V2) {

        r = (d - 104562.431531 - 200 * PI/3 - 1719 * PI/4 - 50 * V2) / 72; //compris entre 0 et PI/2
        return -PI - r;

    }

    else {

        return PI/2; //ou -3*PI/2

    }

}

void maFenetre::setVitesse(double v) {

    vitesse = v;

}

void maFenetre::setVitesseAutres(int n, double v) {

    vitesseAutres[n] = v;

}

double maFenetre::distTotale() {

    return distanceTotale;

}

double maFenetre::distTotaleAutres(int n) {

    return distanceTotaleAutres[n];

}

void maFenetre::setDistTotale(double d) {

    distanceTotale = d;

}

void maFenetre::setDistTotaleAutres(int n, double d) {

    distanceTotaleAutres[n] = d;

}

void maFenetre::arriveeEnStation(int s) {

    Station = s;

    switch(s) {

    case 1:

    {

        retard = int(temps - 53);
        emit emettreRetard(int(temps - 53));
        heureArriveeStation = QTime::currentTime().msecsSinceStartOfDay() / 1000;
        heureDepartStation = (heureArriveeStation - retard + 60) % 86400; //Donne l'heure de départ théorique du terminus
        break;

    }

    case 2:

        retard = int(temps - 197);
        emit emettreRetard(int(temps - 197));
        heureArriveeStation = QTime::currentTime().msecsSinceStartOfDay() / 1000;
        heureDepartStation = (heureArriveeStation - retard / 15 + 23) % 86400; //Donne l'heure de départ théorique, adaptée au retard
        break;

    case 3:

        retard = int(temps - 274);
        emit emettreRetard(int(temps - 274));
        break;

    case 4:

        retard = int(temps - 345);
        emit emettreRetard(int(temps - 345));
        break;

    case 5:

        retard = int(temps - 411);
        emit emettreRetard(int(temps - 411));
        break;

    case 6:

        retard = int(temps - 505);
        emit emettreRetard(int(temps - 505));
        break;

    case 7:

        retard = int(temps - 579);
        emit emettreRetard(int(temps - 579));
        heureArriveeStation = QTime::currentTime().msecsSinceStartOfDay() / 1000;
        heureDepartStation = (heureArriveeStation - retard / 15 + 17) % 86400; //Donne l'heure de départ théorique, adaptée au retard
        break;

    case 8:

        retard = int(temps - 659);
        emit emettreRetard(int(temps - 659));
        break;

    case 9:

        retard = int(temps - 728);
        emit emettreRetard(int(temps - 728));
        break;

    case 10:

        retard = int(temps - 813);
        emit emettreRetard(int(temps - 813));
        break;

    case 101:

        retard = int(temps - 921);
        emit emettreRetard(int(temps - 921));
        heureArriveeStation = QTime::currentTime().msecsSinceStartOfDay() / 1000;
        heureDepartStation = (heureArriveeStation - retard + 60) % 86400; //Donne l'heure de départ théorique du terminus
        break;

    case 102:

        retard = int(temps - 1028);
        emit emettreRetard(int(temps - 1028));
        break;

    case 103:

        retard = int(temps - 1098);
        emit emettreRetard(int(temps - 1098));
        break;

    case 104:

        retard = int(temps - 1175);
        emit emettreRetard(int(temps - 1175));
        heureArriveeStation = QTime::currentTime().msecsSinceStartOfDay() / 1000;
        heureDepartStation = (heureArriveeStation - retard / 15 + 17) % 86400; //Donne l'heure de départ théorique, adaptée au retard
        break;

    case 105:

        retard = int(temps - 1247);
        emit emettreRetard(int(temps - 1247));
        break;

    case 106:

        retard = int(temps - 1339);
        emit emettreRetard(int(temps - 1339));
        break;

    case 107:

        retard = int(temps - 1403);
        emit emettreRetard(int(temps - 1403));
        break;

    case 108:

        retard = int(temps - 1476);
        emit emettreRetard(int(temps - 1476));
        break;

    case 109:

        retard = int(temps - 1555);
        emit emettreRetard(int(temps - 1555));
        heureArriveeStation = QTime::currentTime().msecsSinceStartOfDay() / 1000;
        heureDepartStation = (heureArriveeStation - retard / 15 + 23) % 86400; //Donne l'heure de départ théorique, adaptée au retard
        break;

    case 110:

        retard = int(temps - 1662);
        emit emettreRetard(int(temps - 1662));
        break;

    default:

        emit emettreRetard(int(temps));

    }

    if (Station == 6 || Station == 10 || Station == 101 || Station == 105) {

        QTimer::singleShot(26000, this, SLOT(surstationnement())); //6000 + durée théorique

    }

    else if (Station == 2 ||Station == 109) {

        QTimer::singleShot(29000, this, SLOT(surstationnement()));

    }


    else if (Station == 108) {

        QTimer::singleShot(28000, this, SLOT(surstationnement()));

    }

    else {

        QTimer::singleShot(23000, this, SLOT(surstationnement())); //6000 + durée théorique

    }

}

void maFenetre::augmenterRot(double r) {

    rot2 = r;

}

bool maFenetre::ramePresente(double min, double max) {

    bool b = false;

    if (distanceTotale > min && distanceTotale < max) {

        b = true;

    }

    else {

        for (int n = 0; n < nombreRames; n++) {

            if (distanceTotaleAutres[n] >= min && distanceTotaleAutres[n] <= max) {

                b = true;
                n = nombreRames;

            }

        }

    }

    return b;

}

int maFenetre::numeroFeu(double d) {

    if (d < 332)
        return 0;

    if (d < 356 + 153 * PI/4)
        return 1;

    if (d < 356 + 153 * PI/4 + 50 * V2)
        return 2;

    if (d < 441 + 306 * PI/4 + 50 * V2)
        return 3;

    if (d < 611 + 306 * PI/4 + 50 * V2)
        return 4;

    if (d < 776 + 306 * PI/4 + 50 * V2)
        return 5;

    if (d < 806 + 306 * PI/4 + 50 * V2)
        return 6;

    if (d < 901 + 306 * PI/4 + 50 * V2)
        return 7;

    if (d < 996 + 306 * PI/4 + 50 * V2)
        return 8;

    if (d < 1331 + 306 * PI/4 + 50 * V2)
        return 9;

    if (d < 1426 + 306 * PI/4 + 50 * V2)
        return 10;

    if (d < 1666 + 306 * PI/4 + 50 * V2)
        return 11;

    if (d < 1906 + 306 * PI/4 + 50 * V2)
        return 12;

    if (d < 2006 + 306 * PI/4 + 50 * V2)
        return 13;

    if (d < 2101 + 306 * PI/4 + 50 * V2)
        return 14;

    if (d < 2201 + 403 * PI/4 + 50 * V2)
        return 15;

    if (d < 2406 + 403 * PI/4 + 50 * V2)
        return 16;

    if (d < 2501 + 403 * PI/4 + 50 * V2)
        return 17;

    if (d < 2696 + 403 * PI/4 + 50 * V2)
        return 18;

    if (d < 2891 + 403 * PI/4 + 50 * V2)
        return 19;

    if (d < 2886 + 742 * PI/4 + 50 * V2)
        return 20;

    if (d < 2881 + 1081 * PI/4 + 50 * V2)
        return 21;

    if (d < 2976 + 1081 * PI/4 + 50 * V2)
        return 22;

    if (d < 3166 + 1081 * PI/4 + 50 * V2)
        return 23;

    if (d < 3356 + 1081 * PI/4 + 50 * V2)
        return 24;

    if (d < 3451 + 1081 * PI/4 + 50 * V2)
        return 25;

    if (d < 3536 + 1225 * PI/4 + 50 * V2)
        return 26;

    if (d < 3551 + 1225 * PI/4 + 103 * PI/3 + 50 * V2)
        return 27;

    if (d < 3548 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 28;

    if (d < 3643 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 29;

    if (d < 3808 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 30;

    if (d < 3973 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 31;

    if (d < 4133 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 32;

    if (d < 4228 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 33;

    if (d < 4418 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 34;

    if (d < 4418 + 1225 * PI/4 + 248.5 * PI/3 + 50 * V2)
        return 35;

    if (d < 4500 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return 36;

    if (d < 4558 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return 37;

    if (d < 4653 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return 38;

    if (d < 4653 + 1305 * PI/4 + 300 * PI/3 + 50 * V2)
        return 39;

    if (d < 4653 + 1425 * PI/4 + 300 * PI/3 + 50 * V2)
        return 40;

    if (d < 4653 + 1545 * PI/4 + 300 * PI/3 + 50 * V2)
        return 41;

    if (d < 4665.85 + 1625 * PI/4 + 300 * PI/3 + 50 * V2)
        return 42;

    if (d < 4665.85 + 1705 * PI/4 + 300 * PI/3 + 50 * V2)
        return 43;


    if (d < 100010)
        return 0;

    if (d < 100105)
        return 1;

    if (d < 100300)
        return 2;

    if (d < 100390)
        return 3;

    if (d < 100485)
        return 4;

    if (d < 100645)
        return 5;

    if (d < 100810)
        return 6;

    if (d < 100975)
        return 7;

    if (d < 101069.35)
        return 8;

    if (d < 101076.35 + 100 * PI/3)
        return 9;

    if (d < 101076.35 + 200 * PI/3)
        return 10;

    if (d < 101176.35 + 200 * PI/3)
        return 11;

    if (d < 101166.35 + 156 * PI/4 + 200 * PI/3)
        return 12;

    if (d < 101261.35 + 156 * PI/4 + 200 * PI/3)
        return 13;

    if (d < 101451.35 + 156 * PI/4 + 200 * PI/3)
        return 14;

    if (d < 101641.35 + 156 * PI/4 + 200 * PI/3)
        return 15;

    if (d < 101736.35 + 156 * PI/4 + 200 * PI/3)
        return 16;

    if (d < 101726.35 + 490.5 * PI/4 + 200 * PI/3)
        return 17;

    if (d < 101726.35 + 825 * PI/4 + 200 * PI/3)
        return 18;

    if (d < 102116.35 + 825 * PI/4 + 200 * PI/3)
        return 19;

    if (d < 102211.35 + 825 * PI/4 + 200 * PI/3)
        return 20;

    if (d < 102356.35 + 825 * PI/4 + 200 * PI/3)
        return 21;

    if (d < 102501.35 + 825 * PI/4 + 200 * PI/3)
        return 22;

    if (d < 102516.35 + 931 * PI/4 + 200 * PI/3)
        return 23;

    if (d < 102611.35 + 931 * PI/4 + 200 * PI/3)
        return 24;

    if (d < 102801.35 + 931 * PI/4 + 200 * PI/3)
        return 25;

    if (d < 103001.35 + 931 * PI/4 + 200 * PI/3)
        return 26;

    if (d < 103191.35 + 931 * PI/4 + 200 * PI/3)
        return 27;

    if (d < 103286.35 + 931 * PI/4 + 200 * PI/3)
        return 28;

    if (d < 103676.35 + 931 * PI/4 + 200 * PI/3)
        return 29;

    if (d < 103716.35 + 931 * PI/4 + 200 * PI/3)
        return 30;

    if (d < 103811.35 + 931 * PI/4 + 200 * PI/3)
        return 31;

    if (d < 103951.35 + 931 * PI/4 + 200 * PI/3)
        return 32;

    if (d < 104081.35 + 931 * PI/4 + 200 * PI/3)
        return 33;

    if (d < 104261.35 + 931 * PI/4 + 200 * PI/3)
        return 34;

    if (d < 104261.35 + 1078 * PI/4 + 200 * PI/3)
        return 35;

    if (d < 104261.35 + 1078 * PI/4 + 200 * PI/3 + 50 * V2)
        return 36;

    if (d < 104261.35 + 1151.5 * PI/4 + 200 * PI/3 + 50 * V2)
        return 37;

    if (d < 104261.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 38;

    if (d < 104285.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 39;

    if (d < 104395.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 40;

    return 99;

}

double maFenetre::positionFeu(int n) {

    switch(n) {

    case 0:
        return 332;

    case 1:
        return 356 + 153 * PI/4;

    case 2:
        return 356 + 153 * PI/4 + 50 * V2;

    case 3:
        return 441 + 306 * PI/4 + 50 * V2;

    case 4:
        return 611 + 306 * PI/4 + 50 * V2;

    case 5:
        return 776 + 306 * PI/4 + 50 * V2;

    case 6:
        return 806 + 306 * PI/4 + 50 * V2;

    case 7:
        return 901 + 306 * PI/4 + 50 * V2;

    case 8:
        return 996 + 306 * PI/4 + 50 * V2;

    case 9:
        return 1331 + 306 * PI/4 + 50 * V2;

    case 10:
        return 1426 + 306 * PI/4 + 50 * V2;

    case 11:
        return 1666 + 306 * PI/4 + 50 * V2;

    case 12:
        return 1906 + 306 * PI/4 + 50 * V2;

    case 13:
        return 2006 + 306 * PI/4 + 50 * V2;

    case 14:
        return 2101 + 306 * PI/4 + 50 * V2;

    case 15:
        return 2201 + 403 * PI/4 + 50 * V2;

    case 16:
        return 2406 + 403 * PI/4 + 50 * V2;

    case 17:
        return 2501 + 403 * PI/4 + 50 * V2;

    case 18:
        return 2696 + 403 * PI/4 + 50 * V2;

    case 19:
        return 2891 + 403 * PI/4 + 50 * V2;

    case 20:
        return 2886 + 742 * PI/4 + 50 * V2;

    case 21:
        return 2881 + 1081 * PI/4 + 50 * V2;

    case 22:
        return 2976 + 1081 * PI/4 + 50 * V2;

    case 23:
        return 3166 + 1081 * PI/4 + 50 * V2;

    case 24:
        return 3356 + 1081 * PI/4 + 50 * V2;

    case 25:
        return 3451 + 1081 * PI/4 + 50 * V2;

    case 26:
        return 3536 + 1225 * PI/4 + 50 * V2;

    case 27:
        return 3551 + 1225 * PI/4 + 103 * PI/3 + 50 * V2;

    case 28:
        return 3548 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 29:
        return 3643 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 30:
        return 3808 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 31:
        return 3973 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 32:
        return 4133 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 33:
        return 4228 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 34:
        return 4418 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 35:
        return 4418 + 1225 * PI/4 + 248.5 * PI/3 + 50 * V2;

    case 36:
        return 4500 + 1225 * PI/4 + 300 * PI/3 + 50 * V2;

    case 37:
        return 4558 + 1225 * PI/4 + 300 * PI/3 + 50 * V2;

    case 38:
        return 4653 + 1225 * PI/4 + 300 * PI/3 + 50 * V2;

    case 39:
        return 4653 + 1305 * PI/4 + 300 * PI/3 + 50 * V2;

    case 40:
        return 4653 + 1425 * PI/4 + 300 * PI/3 + 50 * V2;

    case 41:
        return 4653 + 1545 * PI/4 + 300 * PI/3 + 50 * V2;

    case 42:
        return 4665.85 + 1625 * PI/4 + 300 * PI/3 + 50 * V2;

    case 43:
        return 4665.85 + 1705 * PI/4 + 300 * PI/3 + 50 * V2;

    case 100:
        return 100010;

    case 101:
        return 100105;

    case 102:
        return 100300;

    case 103:
        return 100390;

    case 104:
        return 100485;

    case 105:
        return 100645;

    case 106:
        return 100810;

    case 107:
        return 100975;

    case 108:
        return 101069.35;

    case 109:
        return 101076.35 + 100 * PI/3;

    case 110:
        return 101076.35 + 200 * PI/3;

    case 111:
        return 101176.35 + 200 * PI/3;

    case 112:
        return 101166.35 + 156 * PI/4 + 200 * PI/3;

    case 113:
        return 101261.35 + 156 * PI/4 + 200 * PI/3;

    case 114:
        return 101451.35 + 156 * PI/4 + 200 * PI/3;

    case 115:
        return 101641.35 + 156 * PI/4 + 200 * PI/3;

    case 116:
        return 101736.35 + 156 * PI/4 + 200 * PI/3;

    case 117:
        return 101726.35 + 409.5 * PI/4 + 200 * PI/3;

    case 118:
        return 101726.35 + 825 * PI/4 + 200 * PI/3;

    case 119:
        return 102116.35 + 825 * PI/4 + 200 * PI/3;

    case 120:
        return 102211.35 + 825 * PI/4 + 200 * PI/3;

    case 121:
        return 102356.35 + 825 * PI/4 + 200 * PI/3;

    case 122:
        return 102501.35 + 825 * PI/4 + 200 * PI/3;

    case 123:
        return 102516.35 + 931 * PI/4 + 200 * PI/3;

    case 124:
        return 102611.35 + 931 * PI/4 + 200 * PI/3;

    case 125:
        return 102801.35 + 931 * PI/4 + 200 * PI/3;

    case 126:
        return 103001.35 + 931 * PI/4 + 200 * PI/3;

    case 127:
        return 103191.35 + 931 * PI/4 + 200 * PI/3;

    case 128:
        return 103286.35 + 931 * PI/4 + 200 * PI/3;

    case 129:
        return 103676.35 + 931 * PI/4 + 200 * PI/3;

    case 130:
        return 103716.35 + 931 * PI/4 + 200 * PI/3;

    case 131:
        return 103811.35 + 931 * PI/4 + 200 * PI/3;

    case 132:
        return 103951.35 + 931 * PI/4 + 200 * PI/3;

    case 133:
        return 104081.35 + 931 * PI/4 + 200 * PI/3;

    case 134:
        return 104261.35 + 931 * PI/4 + 200 * PI/3;

    case 135:
        return 104261.35 + 1078 * PI/4 + 200 * PI/3;

    case 136:
        return 104261.35 + 1078 * PI/4 + 200 * PI/3 + 50 * V2;

    case 137:
        return 104261.35 + 1151.5 * PI/4 + 200 * PI/3 + 50 * V2;

    case 138:
        return 104261.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 139:
        return 104285.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 140:
        return 104395.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2;

    case 199:
        return 200000; //valeur en principe sans importance

    default:
        return 0;

    }

}

int maFenetre::couleurFeu(int n) {

    if (n < 100) {

        return feuAller[n];

    }

    else {

        return feuRetour[n - 100];

    }

}

double maFenetre::position_temps(int t) {

    t = 53 + t % 1752;

    switch(t) {

    case 1:

        return 3.39392;

    case 2:

        return 7.13064;

    case 3:

        return 11.2262;

    case 4:

        return 15.56;

    case 5:

        return 20.2831;

    case 6:

        return 25.2622;

    case 7:

        return 30.6729;

    case 8:

        return 36.3212;

    case 9:

        return 42.2744;

    case 10:

        return 48.2914;

    case 11:

        return 54.5302;

    case 12:

        return 60.7658;

    case 13:

        return 67.1787;

    case 14:

        return 73.5493;

    case 15:

        return 80.0708;

    case 16:

        return 86.5263;

    case 17:

        return 93.1051;

    case 18:

        return 99.4693;

    case 19:

        return 105.287;

    case 20:

        return 110.825;

    case 21:

        return 116.077;

    case 22:

        return 121.396;

    case 23:

        return 126.631;

    case 24:

        return 131.949;

    case 25:

        return 137.184;

    case 26:

        return 142.503;

    case 27:

        return 147.737;

    case 28:

        return 153.056;

    case 29:

        return 158.29;

    case 30:

        return 163.609;

    case 31:

        return 168.843;

    case 32:

        return 174.162;

    case 33:

        return 179.396;

    case 34:

        return 184.715;

    case 35:

        return 189.949;

    case 36:

        return 195.268;

    case 37:

        return 200.503;

    case 38:

        return 205.821;

    case 39:

        return 210.892;

    case 40:

        return 215.386;

    case 41:

        return 218.847;

    case 42:

        return 221.236;

    case 43:

        return 222.348;

    case 44:

        return 232;

    case 45:

        return 242;

    case 46:

        return 252;

    case 47:

        return 262;

    case 48:

        return 272;

    case 49:

        return 282;

    case 50:

        return 292;

    case 51:

        return 302;

    case 52:

        return 305;

    case 53:

        return 307;

    case 54:

        return 307;

    case 55:

        return 307;

    case 56:

        return 307;

    case 57:

        return 307;

    case 58:

        return 307;

    case 59:

        return 307;

    case 60:

        return 307;

    case 61:

        return 307;

    case 62:

        return 307;

    case 63:

        return 307;

    case 64:

        return 307;

    case 65:

        return 307;

    case 66:

        return 307;

    case 67:

        return 307;

    case 68:

        return 307;

    case 69:

        return 307;

    case 70:

        return 307;

    case 71:

        return 307;

    case 72:

        return 307;

    case 73:

        return 307;

    case 74:

        return 307;

    case 75:

        return 307;

    case 76:

        return 307;

    case 77:

        return 307;

    case 78:

        return 307;

    case 79:

        return 307;

    case 80:

        return 307;

    case 81:

        return 307;

    case 82:

        return 307;

    case 83:

        return 307;

    case 84:

        return 307;

    case 85:

        return 307;

    case 86:

        return 307;

    case 87:

        return 307;

    case 88:

        return 307;

    case 89:

        return 307;

    case 90:

        return 307;

    case 91:

        return 307;

    case 92:

        return 307;

    case 93:

        return 307;

    case 94:

        return 307;

    case 95:

        return 307;

    case 96:

        return 307;

    case 97:

        return 307;

    case 98:

        return 307;

    case 99:

        return 307;

    case 100:

        return 307;

    case 101:

        return 307;

    case 102:

        return 307;

    case 103:

        return 307;

    case 104:

        return 307;

    case 105:

        return 307;

    case 106:

        return 307;

    case 107:

        return 307;

    case 108:

        return 307;

    case 109:

        return 307;

    case 110:

        return 307;

    case 111:

        return 307;

    case 112:

        return 307;

    case 113:

        return 307;

    case 114:

        return 311.926;

    case 115:

        return 317.811;

    case 116:

        return 324.32;

    case 117:

        return 331.537;

    case 118:

        return 339.096;

    case 119:

        return 347.141;

    case 120:

        return 355.343;

    case 121:

        return 363.913;

    case 122:

        return 372.537;

    case 123:

        return 381.46;

    case 124:

        return 390.371;

    case 125:

        return 399.537;

    case 126:

        return 408.649;

    case 127:

        return 417.987;

    case 128:

        return 427.242;

    case 129:

        return 436.702;

    case 130:

        return 446.06;

    case 131:

        return 455.61;

    case 132:

        return 465.042;

    case 133:

        return 474.656;

    case 134:

        return 484.161;

    case 135:

        return 493.961;

    case 136:

        return 503.845;

    case 137:

        return 514.202;

    case 138:

        return 524.68;

    case 139:

        return 535.572;

    case 140:

        return 546.497;

    case 141:

        return 557.803;

    case 142:

        return 569.197;

    case 143:

        return 581.049;

    case 144:

        return 592.947;

    case 145:

        return 605.242;

    case 146:

        return 617.519;

    case 147:

        return 630.151;

    case 148:

        return 642.719;

    case 149:

        return 655.613;

    case 150:

        return 668.409;

    case 151:

        return 681.51;

    case 152:

        return 694.487;

    case 153:

        return 707.752;

    case 154:

        return 720.874;

    case 155:

        return 734.27;

    case 156:

        return 747.511;

    case 157:

        return 761.077;

    case 158:

        return 774.634;

    case 159:

        return 788.671;

    case 160:

        return 802.72;

    case 161:

        return 817.207;

    case 162:

        return 831.65;

    case 163:

        return 846.494;

    case 164:

        return 861.206;

    case 165:

        return 876.171;

    case 166:

        return 890.77;

    case 167:

        return 905.367;

    case 168:

        return 919.468;

    case 169:

        return 933.508;

    case 170:

        return 947.031;

    case 171:

        return 960.467;

    case 172:

        return 973.388;

    case 173:

        return 986.214;

    case 174:

        return 998.547;

    case 175:

        return 1010.78;

    case 176:

        return 1022.49;

    case 177:

        return 1033.91;

    case 178:

        return 1044.56;

    case 179:

        return 1054.89;

    case 180:

        return 1064.8;

    case 181:

        return 1074.73;

    case 182:

        return 1084.45;

    case 183:

        return 1094.28;

    case 184:

        return 1103.92;

    case 185:

        return 1113.72;

    case 186:

        return 1123.37;

    case 187:

        return 1133.17;

    case 188:

        return 1142.81;

    case 189:

        return 1152.61;

    case 190:

        return 1162.07;

    case 191:

        return 1171.28;

    case 192:

        return 1179.78;

    case 193:

        return 1187.65;

    case 194:

        return 1194.09;

    case 195:

        return 1199;

    case 196:

        return 1202.09;

    case 197:

        return 1203.17;

    case 198:

        return 1203.17;

    case 199:

        return 1203.17;

    case 200:

        return 1203.17;

    case 201:

        return 1203.17;

    case 202:

        return 1203.17;

    case 203:

        return 1203.17;

    case 204:

        return 1203.17;

    case 205:

        return 1203.17;

    case 206:

        return 1203.17;

    case 207:

        return 1203.17;

    case 208:

        return 1203.17;

    case 209:

        return 1203.17;

    case 210:

        return 1203.17;

    case 211:

        return 1203.17;

    case 212:

        return 1203.17;

    case 213:

        return 1203.17;

    case 214:

        return 1203.17;

    case 215:

        return 1203.17;

    case 216:

        return 1203.17;

    case 217:

        return 1203.17;

    case 218:

        return 1203.17;

    case 219:

        return 1203.17;

    case 220:

        return 1203.22;

    case 221:

        return 1205.19;

    case 222:

        return 1208.83;

    case 223:

        return 1213.74;

    case 224:

        return 1219.37;

    case 225:

        return 1225.65;

    case 226:

        return 1232.26;

    case 227:

        return 1239.29;

    case 228:

        return 1246.46;

    case 229:

        return 1253.95;

    case 230:

        return 1261.47;

    case 231:

        return 1269.25;

    case 232:

        return 1277.01;

    case 233:

        return 1284.98;

    case 234:

        return 1292.89;

    case 235:

        return 1301;

    case 236:

        return 1309.12;

    case 237:

        return 1317.62;

    case 238:

        return 1326.29;

    case 239:

        return 1335.48;

    case 240:

        return 1344.95;

    case 241:

        return 1355.06;

    case 242:

        return 1365.51;

    case 243:

        return 1376.66;

    case 244:

        return 1388.13;

    case 245:

        return 1400.2;

    case 246:

        return 1412.43;

    case 247:

        return 1425.17;

    case 248:

        return 1437.96;

    case 249:

        return 1451.2;

    case 250:

        return 1464.42;

    case 251:

        return 1478.04;

    case 252:

        return 1491.61;

    case 253:

        return 1505.53;

    case 254:

        return 1519.35;

    case 255:

        return 1533.46;

    case 256:

        return 1547.37;

    case 257:

        return 1561.45;

    case 258:

        return 1575.18;

    case 259:

        return 1588.98;

    case 260:

        return 1602.4;

    case 261:

        return 1615.83;

    case 262:

        return 1628.82;

    case 263:

        return 1641.76;

    case 264:

        return 1654.17;

    case 265:

        return 1666.39;

    case 266:

        return 1677.89;

    case 267:

        return 1688.87;

    case 268:

        return 1698.76;

    case 269:

        return 1707.52;

    case 270:

        return 1714.64;

    case 271:

        return 1720.04;

    case 272:

        return 1723.59;

    case 273:

        return 1725.15;

    case 274:

        return 1725.21;

    case 275:

        return 1725.21;

    case 276:

        return 1725.21;

    case 277:

        return 1725.21;

    case 278:

        return 1725.21;

    case 279:

        return 1725.21;

    case 280:

        return 1725.21;

    case 281:

        return 1725.21;

    case 282:

        return 1725.21;

    case 283:

        return 1725.21;

    case 284:

        return 1725.21;

    case 285:

        return 1725.21;

    case 286:

        return 1725.21;

    case 287:

        return 1725.21;

    case 288:

        return 1725.21;

    case 289:

        return 1725.21;

    case 290:

        return 1725.21;

    case 291:

        return 1726.41;

    case 292:

        return 1729.61;

    case 293:

        return 1734.23;

    case 294:

        return 1739.82;

    case 295:

        return 1746.33;

    case 296:

        return 1753.45;

    case 297:

        return 1761.35;

    case 298:

        return 1769.75;

    case 299:

        return 1778.91;

    case 300:

        return 1788.54;

    case 301:

        return 1798.96;

    case 302:

        return 1809.81;

    case 303:

        return 1821.43;

    case 304:

        return 1833.4;

    case 305:

        return 1846.08;

    case 306:

        return 1859.05;

    case 307:

        return 1872.7;

    case 308:

        return 1886.56;

    case 309:

        return 1901.05;

    case 310:

        return 1915.65;

    case 311:

        return 1930.8;

    case 312:

        return 1945.99;

    case 313:

        return 1961.67;

    case 314:

        return 1977.32;

    case 315:

        return 1993.44;

    case 316:

        return 2009.49;

    case 317:

        return 2025.97;

    case 318:

        return 2042.35;

    case 319:

        return 2059.13;

    case 320:

        return 2075.79;

    case 321:

        return 2092.84;

    case 322:

        return 2109.74;

    case 323:

        return 2126.99;

    case 324:

        return 2144;

    case 325:

        return 2161.23;

    case 326:

        return 2178.08;

    case 327:

        return 2195.02;

    case 328:

        return 2211.47;

    case 329:

        return 2227.9;

    case 330:

        return 2243.75;

    case 331:

        return 2259.5;

    case 332:

        return 2274.63;

    case 333:

        return 2289.57;

    case 334:

        return 2303.82;

    case 335:

        return 2317.82;

    case 336:

        return 2331.09;

    case 337:

        return 2343.99;

    case 338:

        return 2355.91;

    case 339:

        return 2367.01;

    case 340:

        return 2376.76;

    case 341:

        return 2385.3;

    case 342:

        return 2392.08;

    case 343:

        return 2396.94;

    case 344:

        return 2399.74;

    case 345:

        return 2400.4;

    case 346:

        return 2400.4;

    case 347:

        return 2400.4;

    case 348:

        return 2400.4;

    case 349:

        return 2400.4;

    case 350:

        return 2400.4;

    case 351:

        return 2400.4;

    case 352:

        return 2400.4;

    case 353:

        return 2400.4;

    case 354:

        return 2400.4;

    case 355:

        return 2400.4;

    case 356:

        return 2400.4;

    case 357:

        return 2400.4;

    case 358:

        return 2400.4;

    case 359:

        return 2400.4;

    case 360:

        return 2400.4;

    case 361:

        return 2400.4;

    case 362:

        return 2401.49;

    case 363:

        return 2404.68;

    case 364:

        return 2409.18;

    case 365:

        return 2414.81;

    case 366:

        return 2421.19;

    case 367:

        return 2428.38;

    case 368:

        return 2436.11;

    case 369:

        return 2444.51;

    case 370:

        return 2453.22;

    case 371:

        return 2462.42;

    case 372:

        return 2471.77;

    case 373:

        return 2481.5;

    case 374:

        return 2491.28;

    case 375:

        return 2501.39;

    case 376:

        return 2511.49;

    case 377:

        return 2521.86;

    case 378:

        return 2532.16;

    case 379:

        return 2542.71;

    case 380:

        return 2553.15;

    case 381:

        return 2563.8;

    case 382:

        return 2574.31;

    case 383:

        return 2585.02;

    case 384:

        return 2595.59;

    case 385:

        return 2606.46;

    case 386:

        return 2617.38;

    case 387:

        return 2628.77;

    case 388:

        return 2640.24;

    case 389:

        return 2652.11;

    case 390:

        return 2663.97;

    case 391:

        return 2676.2;

    case 392:

        return 2688.37;

    case 393:

        return 2700.86;

    case 394:

        return 2713.26;

    case 395:

        return 2725.96;

    case 396:

        return 2738.55;

    case 397:

        return 2751.4;

    case 398:

        return 2764.07;

    case 399:

        return 2776.85;

    case 400:

        return 2789.25;

    case 401:

        return 2801.6;

    case 402:

        return 2813.45;

    case 403:

        return 2825.08;

    case 404:

        return 2835.96;

    case 405:

        return 2846.3;

    case 406:

        return 2855.53;

    case 407:

        return 2863.66;

    case 408:

        return 2870.03;

    case 409:

        return 2874.7;

    case 410:

        return 2877.49;

    case 411:

        return 2878.22;

    case 412:

        return 2878.22;

    case 413:

        return 2878.22;

    case 414:

        return 2878.22;

    case 415:

        return 2878.22;

    case 416:

        return 2878.22;

    case 417:

        return 2878.22;

    case 418:

        return 2878.22;

    case 419:

        return 2878.22;

    case 420:

        return 2878.22;

    case 421:

        return 2878.22;

    case 422:

        return 2878.22;

    case 423:

        return 2878.22;

    case 424:

        return 2878.22;

    case 425:

        return 2878.22;

    case 426:

        return 2878.22;

    case 427:

        return 2878.22;

    case 428:

        return 2878.7;

    case 429:

        return 2881.37;

    case 430:

        return 2885.49;

    case 431:

        return 2890.86;

    case 432:

        return 2897.04;

    case 433:

        return 2904.1;

    case 434:

        return 2911.72;

    case 435:

        return 2920.1;

    case 436:

        return 2928.94;

    case 437:

        return 2938.52;

    case 438:

        return 2948.5;

    case 439:

        return 2959.14;

    case 440:

        return 2970.06;

    case 441:

        return 2981.56;

    case 442:

        return 2993.21;

    case 443:

        return 3005.36;

    case 444:

        return 3017.6;

    case 445:

        return 3030.31;

    case 446:

        return 3043.07;

    case 447:

        return 3056.27;

    case 448:

        return 3069.5;

    case 449:

        return 3083.15;

    case 450:

        return 3096.8;

    case 451:

        return 3110.86;

    case 452:

        return 3124.88;

    case 453:

        return 3139.31;

    case 454:

        return 3153.67;

    case 455:

        return 3168.43;

    case 456:

        return 3183.11;

    case 457:

        return 3198.17;

    case 458:

        return 3213.13;

    case 459:

        return 3228.47;

    case 460:

        return 3243.69;

    case 461:

        return 3259.28;

    case 462:

        return 3274.71;

    case 463:

        return 3290.48;

    case 464:

        return 3306.08;

    case 465:

        return 3322.01;

    case 466:

        return 3337.74;

    case 467:

        return 3353.79;

    case 468:

        return 3369.63;

    case 469:

        return 3385.78;

    case 470:

        return 3401.71;

    case 471:

        return 3417.94;

    case 472:

        return 3433.94;

    case 473:

        return 3450.23;

    case 474:

        return 3466.29;

    case 475:

        return 3482.63;

    case 476:

        return 3498.74;

    case 477:

        return 3515.13;

    case 478:

        return 3531.28;

    case 479:

        return 3547.7;

    case 480:

        return 3563.88;

    case 481:

        return 3580.33;

    case 482:

        return 3596.53;

    case 483:

        return 3612.99;

    case 484:

        return 3629.12;

    case 485:

        return 3645.38;

    case 486:

        return 3661.22;

    case 487:

        return 3677.03;

    case 488:

        return 3692.31;

    case 489:

        return 3707.52;

    case 490:

        return 3722.18;

    case 491:

        return 3736.75;

    case 492:

        return 3750.77;

    case 493:

        return 3764.69;

    case 494:

        return 3778.06;

    case 495:

        return 3791.3;

    case 496:

        return 3804.02;

    case 497:

        return 3816.63;

    case 498:

        return 3828.74;

    case 499:

        return 3840.69;

    case 500:

        return 3851.73;

    case 501:

        return 3861.76;

    case 502:

        return 3870.29;

    case 503:

        return 3877.32;

    case 504:

        return 3882.2;

    case 505:

        return 3884.16;

    case 506:

        return 3884.16;

    case 507:

        return 3884.16;

    case 508:

        return 3884.16;

    case 509:

        return 3884.16;

    case 510:

        return 3884.16;

    case 511:

        return 3884.16;

    case 512:

        return 3884.16;

    case 513:

        return 3884.16;

    case 514:

        return 3884.16;

    case 515:

        return 3884.16;

    case 516:

        return 3884.16;

    case 517:

        return 3884.16;

    case 518:

        return 3884.16;

    case 519:

        return 3884.16;

    case 520:

        return 3884.16;

    case 521:

        return 3884.16;

    case 522:

        return 3884.16;

    case 523:

        return 3884.16;

    case 524:

        return 3884.53;

    case 525:

        return 3887.09;

    case 526:

        return 3891.12;

    case 527:

        return 3896.41;

    case 528:

        return 3902.52;

    case 529:

        return 3909.49;

    case 530:

        return 3917.01;

    case 531:

        return 3925.26;

    case 532:

        return 3933.84;

    case 533:

        return 3942.94;

    case 534:

        return 3952.2;

    case 535:

        return 3961.87;

    case 536:

        return 3971.59;

    case 537:

        return 3981.65;

    case 538:

        return 3991.7;

    case 539:

        return 4002.05;

    case 540:

        return 4012.33;

    case 541:

        return 4022.88;

    case 542:

        return 4033.35;

    case 543:

        return 4044.05;

    case 544:

        return 4054.65;

    case 545:

        return 4065.47;

    case 546:

        return 4076.16;

    case 547:

        return 4087.07;

    case 548:

        return 4097.84;

    case 549:

        return 4108.81;

    case 550:

        return 4119.63;

    case 551:

        return 4130.66;

    case 552:

        return 4141.53;

    case 553:

        return 4152.53;

    case 554:

        return 4163.23;

    case 555:

        return 4173.92;

    case 556:

        return 4184.23;

    case 557:

        return 4194.45;

    case 558:

        return 4204.26;

    case 559:

        return 4213.96;

    case 560:

        return 4223.23;

    case 561:

        return 4232.42;

    case 562:

        return 4241.3;

    case 563:

        return 4250.27;

    case 564:

        return 4259.13;

    case 565:

        return 4268.18;

    case 566:

        return 4277.14;

    case 567:

        return 4286.31;

    case 568:

        return 4295.41;

    case 569:

        return 4304.74;

    case 570:

        return 4313.95;

    case 571:

        return 4323.1;

    case 572:

        return 4331.64;

    case 573:

        return 4339.59;

    case 574:

        return 4346.42;

    case 575:

        return 4352;

    case 576:

        return 4356.08;

    case 577:

        return 4358.58;

    case 578:

        return 4359.53;

    case 579:

        return 4359.54;

    case 580:

        return 4359.54;

    case 581:

        return 4359.54;

    case 582:

        return 4359.54;

    case 583:

        return 4359.54;

    case 584:

        return 4359.54;

    case 585:

        return 4359.54;

    case 586:

        return 4359.54;

    case 587:

        return 4359.54;

    case 588:

        return 4359.54;

    case 589:

        return 4359.54;

    case 590:

        return 4359.54;

    case 591:

        return 4359.54;

    case 592:

        return 4359.54;

    case 593:

        return 4359.54;

    case 594:

        return 4359.54;

    case 595:

        return 4359.89;

    case 596:

        return 4362.36;

    case 597:

        return 4366.43;

    case 598:

        return 4371.5;

    case 599:

        return 4377.39;

    case 600:

        return 4383.69;

    case 601:

        return 4390.5;

    case 602:

        return 4397.49;

    case 603:

        return 4404.83;

    case 604:

        return 4412.25;

    case 605:

        return 4419.93;

    case 606:

        return 4427.61;

    case 607:

        return 4435.52;

    case 608:

        return 4443.39;

    case 609:

        return 4451.45;

    case 610:

        return 4459.43;

    case 611:

        return 4467.59;

    case 612:

        return 4475.66;

    case 613:

        return 4483.9;

    case 614:

        return 4492.02;

    case 615:

        return 4500.29;

    case 616:

        return 4508.45;

    case 617:

        return 4516.74;

    case 618:

        return 4524.92;

    case 619:

        return 4533.24;

    case 620:

        return 4541.45;

    case 621:

        return 4549.79;

    case 622:

        return 4558.08;

    case 623:

        return 4566.67;

    case 624:

        return 4575.31;

    case 625:

        return 4584.25;

    case 626:

        return 4593.17;

    case 627:

        return 4602.35;

    case 628:

        return 4611.47;

    case 629:

        return 4620.81;

    case 630:

        return 4630.07;

    case 631:

        return 4639.54;

    case 632:

        return 4648.9;

    case 633:

        return 4658.46;

    case 634:

        return 4667.89;

    case 635:

        return 4677.51;

    case 636:

        return 4687;

    case 637:

        return 4696.66;

    case 638:

        return 4706.18;

    case 639:

        return 4715.86;

    case 640:

        return 4725.4;

    case 641:

        return 4735.11;

    case 642:

        return 4744.67;

    case 643:

        return 4754.39;

    case 644:

        return 4763.97;

    case 645:

        return 4773.72;

    case 646:

        return 4783.32;

    case 647:

        return 4793.09;

    case 648:

        return 4802.72;

    case 649:

        return 4812.5;

    case 650:

        return 4822.14;

    case 651:

        return 4831.78;

    case 652:

        return 4840.85;

    case 653:

        return 4849.42;

    case 654:

        return 4856.98;

    case 655:

        return 4863.51;

    case 656:

        return 4868.48;

    case 657:

        return 4871.81;

    case 658:

        return 4873.11;

    case 659:

        return 4873.13;

    case 660:

        return 4873.13;

    case 661:

        return 4873.13;

    case 662:

        return 4873.13;

    case 663:

        return 4873.13;

    case 664:

        return 4873.13;

    case 665:

        return 4873.13;

    case 666:

        return 4873.13;

    case 667:

        return 4873.13;

    case 668:

        return 4873.13;

    case 669:

        return 4873.13;

    case 670:

        return 4873.13;

    case 671:

        return 4873.13;

    case 672:

        return 4873.13;

    case 673:

        return 4873.13;

    case 674:

        return 4873.13;

    case 675:

        return 4873.69;

    case 676:

        return 4876.39;

    case 677:

        return 4880.63;

    case 678:

        return 4885.93;

    case 679:

        return 4892.19;

    case 680:

        return 4899.08;

    case 681:

        return 4906.75;

    case 682:

        return 4914.91;

    case 683:

        return 4923.83;

    case 684:

        return 4933.22;

    case 685:

        return 4943.37;

    case 686:

        return 4953.9;

    case 687:

        return 4965.06;

    case 688:

        return 4976.45;

    case 689:

        return 4988.38;

    case 690:

        return 5000.43;

    case 691:

        return 5012.97;

    case 692:

        return 5025.55;

    case 693:

        return 5038.55;

    case 694:

        return 5051.54;

    case 695:

        return 5064.92;

    case 696:

        return 5078.24;

    case 697:

        return 5091.92;

    case 698:

        return 5105.53;

    case 699:

        return 5119.49;

    case 700:

        return 5133.32;

    case 701:

        return 5147.45;

    case 702:

        return 5161.41;

    case 703:

        return 5175.66;

    case 704:

        return 5189.72;

    case 705:

        return 5204.04;

    case 706:

        return 5218.17;

    case 707:

        return 5232.54;

    case 708:

        return 5246.7;

    case 709:

        return 5261.07;

    case 710:

        return 5275.2;

    case 711:

        return 5289.46;

    case 712:

        return 5303.37;

    case 713:

        return 5317.34;

    case 714:

        return 5330.89;

    case 715:

        return 5344.43;

    case 716:

        return 5357.49;

    case 717:

        return 5370.42;

    case 718:

        return 5382.78;

    case 719:

        return 5395.01;

    case 720:

        return 5406.66;

    case 721:

        return 5417.91;

    case 722:

        return 5428.2;

    case 723:

        return 5437.48;

    case 724:

        return 5445.22;

    case 725:

        return 5451.3;

    case 726:

        return 5455.51;

    case 727:

        return 5457.85;

    case 728:

        return 5458.22;

    case 729:

        return 5458.22;

    case 730:

        return 5458.22;

    case 731:

        return 5458.22;

    case 732:

        return 5458.22;

    case 733:

        return 5458.22;

    case 734:

        return 5458.22;

    case 735:

        return 5458.22;

    case 736:

        return 5458.22;

    case 737:

        return 5458.22;

    case 738:

        return 5458.22;

    case 739:

        return 5458.22;

    case 740:

        return 5458.22;

    case 741:

        return 5458.22;

    case 742:

        return 5458.22;

    case 743:

        return 5458.22;

    case 744:

        return 5458.22;

    case 745:

        return 5459.97;

    case 746:

        return 5463.47;

    case 747:

        return 5468.34;

    case 748:

        return 5474.1;

    case 749:

        return 5480.76;

    case 750:

        return 5487.99;

    case 751:

        return 5495.99;

    case 752:

        return 5504.47;

    case 753:

        return 5513.67;

    case 754:

        return 5523.21;

    case 755:

        return 5533.31;

    case 756:

        return 5543.55;

    case 757:

        return 5554.2;

    case 758:

        return 5564.85;

    case 759:

        return 5575.78;

    case 760:

        return 5586.61;

    case 761:

        return 5597.64;

    case 762:

        return 5608.41;

    case 763:

        return 5619.21;

    case 764:

        return 5629.65;

    case 765:

        return 5639.97;

    case 766:

        return 5649.75;

    case 767:

        return 5659.55;

    case 768:

        return 5669.07;

    case 769:

        return 5678.48;

    case 770:

        return 5687.38;

    case 771:

        return 5695.97;

    case 772:

        return 5704.17;

    case 773:

        return 5712.5;

    case 774:

        return 5720.72;

    case 775:

        return 5729.08;

    case 776:

        return 5737.32;

    case 777:

        return 5745.7;

    case 778:

        return 5753.96;

    case 779:

        return 5762.36;

    case 780:

        return 5770.63;

    case 781:

        return 5779.03;

    case 782:

        return 5787.29;

    case 783:

        return 5795.59;

    case 784:

        return 5803.49;

    case 785:

        return 5811.17;

    case 786:

        return 5818.31;

    case 787:

        return 5825.11;

    case 788:

        return 5831.75;

    case 789:

        return 5838.54;

    case 790:

        return 5845.27;

    case 791:

        return 5852.13;

    case 792:

        return 5858.89;

    case 793:

        return 5865.78;

    case 794:

        return 5872.58;

    case 795:

        return 5879.5;

    case 796:

        return 5886.33;

    case 797:

        return 5893.28;

    case 798:

        return 5900.13;

    case 799:

        return 5907.11;

    case 800:

        return 5914;

    case 801:

        return 5921;

    case 802:

        return 5927.88;

    case 803:

        return 5934.88;

    case 804:

        return 5941.77;

    case 805:

        return 5948.77;

    case 806:

        return 5955.66;

    case 807:

        return 5962.65;

    case 808:

        return 5969.32;

    case 809:

        return 5975.49;

    case 810:

        return 5980.57;

    case 811:

        return 5984.45;

    case 812:

        return 5986.85;

    case 813:

        return 5987.7;

    case 814:

        return 5987.7;

    case 815:

        return 5987.7;

    case 816:

        return 5987.7;

    case 817:

        return 5987.7;

    case 818:

        return 5987.7;

    case 819:

        return 5987.7;

    case 820:

        return 5987.7;

    case 821:

        return 5987.7;

    case 822:

        return 5987.7;

    case 823:

        return 5987.7;

    case 824:

        return 5987.7;

    case 825:

        return 5987.7;

    case 826:

        return 5987.7;

    case 827:

        return 5987.7;

    case 828:

        return 5987.7;

    case 829:

        return 5987.7;

    case 830:

        return 5987.7;

    case 831:

        return 5987.7;

    case 832:

        return 5987.7;

    case 833:

        return 5988.3;

    case 834:

        return 5990.44;

    case 835:

        return 5993.28;

    case 836:

        return 5996.39;

    case 837:

        return 5999.67;

    case 838:

        return 6002.96;

    case 839:

        return 6006.32;

    case 840:

        return 6009.66;

    case 841:

        return 6013.08;

    case 842:

        return 6016.48;

    case 843:

        return 6019.98;

    case 844:

        return 6023.42;

    case 845:

        return 6026.93;

    case 846:

        return 6030.39;

    case 847:

        return 6033.92;

    case 848:

        return 6037.41;

    case 849:

        return 6040.97;

    case 850:

        return 6044.49;

    case 851:

        return 6048.08;

    case 852:

        return 6051.63;

    case 853:

        return 6055.25;

    case 854:

        return 6058.84;

    case 855:

        return 6062.49;

    case 856:

        return 6066.11;

    case 857:

        return 6069.8;

    case 858:

        return 6073.43;

    case 859:

        return 6077.13;

    case 860:

        return 6080.76;

    case 861:

        return 6084.46;

    case 862:

        return 6088.1;

    case 863:

        return 6091.79;

    case 864:

        return 6095.43;

    case 865:

        return 6099.18;

    case 866:

        return 6102.99;

    case 867:

        return 6106.92;

    case 868:

        return 6110.81;

    case 869:

        return 6114.75;

    case 870:

        return 6118.65;

    case 871:

        return 6122.73;

    case 872:

        return 6126.79;

    case 873:

        return 6130.93;

    case 874:

        return 6135.01;

    case 875:

        return 6139.19;

    case 876:

        return 6143.45;

    case 877:

        return 6148;

    case 878:

        return 6152.72;

    case 879:

        return 6157.8;

    case 880:

        return 6163.1;

    case 881:

        return 6168.8;

    case 882:

        return 6174.74;

    case 883:

        return 6181.13;

    case 884:

        return 6187.78;

    case 885:

        return 6194.85;

    case 886:

        return 6202.05;

    case 887:

        return 6209.54;

    case 888:

        return 6217.02;

    case 889:

        return 6224.71;

    case 890:

        return 6232.31;

    case 891:

        return 6240.05;

    case 892:

        return 6247.67;

    case 893:

        return 6255.38;

    case 894:

        return 6262.93;

    case 895:

        return 6270.54;

    case 896:

        return 6277.99;

    case 897:

        return 6285.51;

    case 898:

        return 6292.87;

    case 899:

        return 6300.32;

    case 900:

        return 6307.63;

    case 901:

        return 6315.05;

    case 902:

        return 6322.37;

    case 903:

        return 6329.83;

    case 904:

        return 6337.21;

    case 905:

        return 6344.76;

    case 906:

        return 6352.25;

    case 907:

        return 6359.93;

    case 908:

        return 6367.57;

    case 909:

        return 6375.4;

    case 910:

        return 6383.2;

    case 911:

        return 6391.2;

    case 912:

        return 6399.14;

    case 913:

        return 6407.26;

    case 914:

        return 6415.3;

    case 915:

        return 6423.38;

    case 916:

        return 6430.9;

    case 917:

        return 6437.83;

    case 918:

        return 6443.57;

    case 919:

        return 6448.02;

    case 920:

        return 6450.84;

    case 921:

        return 6451.89;

    case 922:

        return 6451.89;

    case 923:

        return 6451.89;

    case 924:

        return 6451.89;

    case 925:

        return 6451.89;

    case 926:

        return 6451.89;

    case 927:

        return 6451.89;

    case 928:

        return 6451.89;

    case 929:

        return 6451.89;

    case 930:

        return 6451.89;

    case 931:

        return 6451.89;

    case 932:

        return 6451.89;

    case 933:

        return 6451.89;

    case 934:

        return 6451.89;

    case 935:

        return 6451.89;

    case 936:

        return 6451.89;

    case 937:

        return 6451.89;

    case 938:

        return 6451.89;

    case 939:

        return 6451.89;

    case 940:

        return 6451.89;

    case 941:

        return 6451.89;

    case 942:

        return 6451.89;

    case 943:

        return 6451.89;

    case 944:

        return 6451.89;

    case 945:

        return 6451.89;

    case 946:

        return 6451.89;

    case 947:

        return 6451.89;

    case 948:

        return 6451.89;

    case 949:

        return 6451.89;

    case 950:

        return 6451.89;

    case 951:

        return 6451.89;

    case 952:

        return 6451.89;

    case 953:

        return 6451.89;

    case 954:

        return 6451.89;

    case 955:

        return 6451.89;

    case 956:

        return 6451.89;

    case 957:

        return 6451.89;

    case 958:

        return 6451.89;

    case 959:

        return 6451.89;

    case 960:

        return 6451.89;

    case 961:

        return 6451.89;

    case 962:

        return 6451.89;

    case 963:

        return 6451.89;

    case 964:

        return 6451.89;

    case 965:

        return 6451.89;

    case 966:

        return 6451.89;

    case 967:

        return 6451.89;

    case 968:

        return 6451.89;

    case 969:

        return 6451.89;

    case 970:

        return 6451.89;

    case 971:

        return 6451.89;

    case 972:

        return 6451.89;

    case 973:

        return 6451.89;

    case 974:

        return 6451.89;

    case 975:

        return 6451.89;

    case 976:

        return 6451.89;

    case 977:

        return 6451.89;

    case 978:

        return 6451.89;

    case 979:

        return 6451.89;

    case 980:

        return 6451.89;

    case 981:

        return 6451.89;

    case 982:

        return 99997.4;

    case 983:

        return 99999.6;

    case 984:

        return 100003;

    case 985:

        return 100008;

    case 986:

        return 100014;

    case 987:

        return 100021;

    case 988:

        return 100029;

    case 989:

        return 100036;

    case 990:

        return 100045;

    case 991:

        return 100054;

    case 992:

        return 100064;

    case 993:

        return 100074;

    case 994:

        return 100084;

    case 995:

        return 100095;

    case 996:

        return 100106;

    case 997:

        return 100117;

    case 998:

        return 100129;

    case 999:

        return 100140;

    case 1000:

        return 100153;

    case 1001:

        return 100165;

    case 1002:

        return 100178;

    case 1003:

        return 100191;

    case 1004:

        return 100204;

    case 1005:

        return 100217;

    case 1006:

        return 100231;

    case 1007:

        return 100245;

    case 1008:

        return 100259;

    case 1009:

        return 100272;

    case 1010:

        return 100286;

    case 1011:

        return 100299;

    case 1012:

        return 100313;

    case 1013:

        return 100326;

    case 1014:

        return 100340;

    case 1015:

        return 100353;

    case 1016:

        return 100366;

    case 1017:

        return 100379;

    case 1018:

        return 100392;

    case 1019:

        return 100404;

    case 1020:

        return 100416;

    case 1021:

        return 100428;

    case 1022:

        return 100438;

    case 1023:

        return 100448;

    case 1024:

        return 100457;

    case 1025:

        return 100464;

    case 1026:

        return 100469;

    case 1027:

        return 100472;

    case 1028:

        return 100473;

    case 1029:

        return 100473;

    case 1030:

        return 100473;

    case 1031:

        return 100473;

    case 1032:

        return 100473;

    case 1033:

        return 100473;

    case 1034:

        return 100473;

    case 1035:

        return 100473;

    case 1036:

        return 100473;

    case 1037:

        return 100473;

    case 1038:

        return 100473;

    case 1039:

        return 100473;

    case 1040:

        return 100473;

    case 1041:

        return 100473;

    case 1042:

        return 100473;

    case 1043:

        return 100473;

    case 1044:

        return 100473;

    case 1045:

        return 100473;

    case 1046:

        return 100476;

    case 1047:

        return 100480;

    case 1048:

        return 100485;

    case 1049:

        return 100491;

    case 1050:

        return 100498;

    case 1051:

        return 100505;

    case 1052:

        return 100513;

    case 1053:

        return 100522;

    case 1054:

        return 100531;

    case 1055:

        return 100541;

    case 1056:

        return 100551;

    case 1057:

        return 100562;

    case 1058:

        return 100573;

    case 1059:

        return 100585;

    case 1060:

        return 100597;

    case 1061:

        return 100610;

    case 1062:

        return 100623;

    case 1063:

        return 100636;

    case 1064:

        return 100650;

    case 1065:

        return 100663;

    case 1066:

        return 100677;

    case 1067:

        return 100691;

    case 1068:

        return 100705;

    case 1069:

        return 100719;

    case 1070:

        return 100733;

    case 1071:

        return 100748;

    case 1072:

        return 100762;

    case 1073:

        return 100776;

    case 1074:

        return 100791;

    case 1075:

        return 100805;

    case 1076:

        return 100820;

    case 1077:

        return 100834;

    case 1078:

        return 100848;

    case 1079:

        return 100862;

    case 1080:

        return 100876;

    case 1081:

        return 100890;

    case 1082:

        return 100904;

    case 1083:

        return 100917;

    case 1084:

        return 100930;

    case 1085:

        return 100943;

    case 1086:

        return 100955;

    case 1087:

        return 100967;

    case 1088:

        return 100979;

    case 1089:

        return 100991;

    case 1090:

        return 101002;

    case 1091:

        return 101013;

    case 1092:

        return 101023;

    case 1093:

        return 101032;

    case 1094:

        return 101041;

    case 1095:

        return 101048;

    case 1096:

        return 101053;

    case 1097:

        return 101056;

    case 1098:

        return 101057;

    case 1099:

        return 101057;

    case 1100:

        return 101057;

    case 1101:

        return 101057;

    case 1102:

        return 101057;

    case 1103:

        return 101057;

    case 1104:

        return 101057;

    case 1105:

        return 101057;

    case 1106:

        return 101057;

    case 1107:

        return 101057;

    case 1108:

        return 101057;

    case 1109:

        return 101057;

    case 1110:

        return 101057;

    case 1111:

        return 101057;

    case 1112:

        return 101057;

    case 1113:

        return 101057;

    case 1114:

        return 101057;

    case 1115:

        return 101058;

    case 1116:

        return 101062;

    case 1117:

        return 101066;

    case 1118:

        return 101072;

    case 1119:

        return 101078;

    case 1120:

        return 101086;

    case 1121:

        return 101093;

    case 1122:

        return 101101;

    case 1123:

        return 101109;

    case 1124:

        return 101118;

    case 1125:

        return 101126;

    case 1126:

        return 101135;

    case 1127:

        return 101144;

    case 1128:

        return 101153;

    case 1129:

        return 101162;

    case 1130:

        return 101172;

    case 1131:

        return 101181;

    case 1132:

        return 101190;

    case 1133:

        return 101200;

    case 1134:

        return 101209;

    case 1135:

        return 101219;

    case 1136:

        return 101228;

    case 1137:

        return 101238;

    case 1138:

        return 101248;

    case 1139:

        return 101257;

    case 1140:

        return 101267;

    case 1141:

        return 101276;

    case 1142:

        return 101286;

    case 1143:

        return 101296;

    case 1144:

        return 101305;

    case 1145:

        return 101315;

    case 1146:

        return 101325;

    case 1147:

        return 101334;

    case 1148:

        return 101344;

    case 1149:

        return 101354;

    case 1150:

        return 101363;

    case 1151:

        return 101373;

    case 1152:

        return 101383;

    case 1153:

        return 101392;

    case 1154:

        return 101402;

    case 1155:

        return 101412;

    case 1156:

        return 101422;

    case 1157:

        return 101431;

    case 1158:

        return 101441;

    case 1159:

        return 101451;

    case 1160:

        return 101461;

    case 1161:

        return 101470;

    case 1162:

        return 101480;

    case 1163:

        return 101490;

    case 1164:

        return 101499;

    case 1165:

        return 101509;

    case 1166:

        return 101519;

    case 1167:

        return 101529;

    case 1168:

        return 101538;

    case 1169:

        return 101547;

    case 1170:

        return 101556;

    case 1171:

        return 101564;

    case 1172:

        return 101571;

    case 1173:

        return 101576;

    case 1174:

        return 101580;

    case 1175:

        return 101581;

    case 1176:

        return 101581;

    case 1177:

        return 101581;

    case 1178:

        return 101581;

    case 1179:

        return 101581;

    case 1180:

        return 101581;

    case 1181:

        return 101581;

    case 1182:

        return 101581;

    case 1183:

        return 101581;

    case 1184:

        return 101581;

    case 1185:

        return 101581;

    case 1186:

        return 101581;

    case 1187:

        return 101581;

    case 1188:

        return 101581;

    case 1189:

        return 101581;

    case 1190:

        return 101581;

    case 1191:

        return 101581;

    case 1192:

        return 101582;

    case 1193:

        return 101584;

    case 1194:

        return 101588;

    case 1195:

        return 101594;

    case 1196:

        return 101600;

    case 1197:

        return 101607;

    case 1198:

        return 101614;

    case 1199:

        return 101622;

    case 1200:

        return 101631;

    case 1201:

        return 101640;

    case 1202:

        return 101649;

    case 1203:

        return 101659;

    case 1204:

        return 101669;

    case 1205:

        return 101679;

    case 1206:

        return 101689;

    case 1207:

        return 101699;

    case 1208:

        return 101710;

    case 1209:

        return 101720;

    case 1210:

        return 101731;

    case 1211:

        return 101741;

    case 1212:

        return 101752;

    case 1213:

        return 101762;

    case 1214:

        return 101773;

    case 1215:

        return 101784;

    case 1216:

        return 101795;

    case 1217:

        return 101806;

    case 1218:

        return 101817;

    case 1219:

        return 101828;

    case 1220:

        return 101839;

    case 1221:

        return 101850;

    case 1222:

        return 101860;

    case 1223:

        return 101871;

    case 1224:

        return 101881;

    case 1225:

        return 101892;

    case 1226:

        return 101902;

    case 1227:

        return 101911;

    case 1228:

        return 101921;

    case 1229:

        return 101930;

    case 1230:

        return 101939;

    case 1231:

        return 101947;

    case 1232:

        return 101956;

    case 1233:

        return 101965;

    case 1234:

        return 101974;

    case 1235:

        return 101983;

    case 1236:

        return 101993;

    case 1237:

        return 102002;

    case 1238:

        return 102011;

    case 1239:

        return 102020;

    case 1240:

        return 102029;

    case 1241:

        return 102037;

    case 1242:

        return 102044;

    case 1243:

        return 102050;

    case 1244:

        return 102054;

    case 1245:

        return 102056;

    case 1246:

        return 102057;

    case 1247:

        return 102058;

    case 1248:

        return 102058;

    case 1249:

        return 102058;

    case 1250:

        return 102058;

    case 1251:

        return 102058;

    case 1252:

        return 102058;

    case 1253:

        return 102058;

    case 1254:

        return 102058;

    case 1255:

        return 102058;

    case 1256:

        return 102058;

    case 1257:

        return 102058;

    case 1258:

        return 102058;

    case 1259:

        return 102058;

    case 1260:

        return 102058;

    case 1261:

        return 102058;

    case 1262:

        return 102058;

    case 1263:

        return 102058;

    case 1264:

        return 102058;

    case 1265:

        return 102058;

    case 1266:

        return 102058;

    case 1267:

        return 102061;

    case 1268:

        return 102066;

    case 1269:

        return 102071;

    case 1270:

        return 102077;

    case 1271:

        return 102084;

    case 1272:

        return 102092;

    case 1273:

        return 102100;

    case 1274:

        return 102109;

    case 1275:

        return 102119;

    case 1276:

        return 102129;

    case 1277:

        return 102140;

    case 1278:

        return 102152;

    case 1279:

        return 102164;

    case 1280:

        return 102177;

    case 1281:

        return 102190;

    case 1282:

        return 102204;

    case 1283:

        return 102218;

    case 1284:

        return 102232;

    case 1285:

        return 102247;

    case 1286:

        return 102262;

    case 1287:

        return 102277;

    case 1288:

        return 102292;

    case 1289:

        return 102308;

    case 1290:

        return 102323;

    case 1291:

        return 102339;

    case 1292:

        return 102355;

    case 1293:

        return 102371;

    case 1294:

        return 102387;

    case 1295:

        return 102403;

    case 1296:

        return 102420;

    case 1297:

        return 102436;

    case 1298:

        return 102452;

    case 1299:

        return 102469;

    case 1300:

        return 102486;

    case 1301:

        return 102503;

    case 1302:

        return 102520;

    case 1303:

        return 102537;

    case 1304:

        return 102555;

    case 1305:

        return 102573;

    case 1306:

        return 102591;

    case 1307:

        return 102609;

    case 1308:

        return 102628;

    case 1309:

        return 102646;

    case 1310:

        return 102665;

    case 1311:

        return 102683;

    case 1312:

        return 102702;

    case 1313:

        return 102720;

    case 1314:

        return 102739;

    case 1315:

        return 102757;

    case 1316:

        return 102774;

    case 1317:

        return 102792;

    case 1318:

        return 102809;

    case 1319:

        return 102827;

    case 1320:

        return 102844;

    case 1321:

        return 102860;

    case 1322:

        return 102877;

    case 1323:

        return 102893;

    case 1324:

        return 102908;

    case 1325:

        return 102923;

    case 1326:

        return 102938;

    case 1327:

        return 102952;

    case 1328:

        return 102965;

    case 1329:

        return 102978;

    case 1330:

        return 102990;

    case 1331:

        return 103002;

    case 1332:

        return 103014;

    case 1333:

        return 103024;

    case 1334:

        return 103034;

    case 1335:

        return 103043;

    case 1336:

        return 103050;

    case 1337:

        return 103055;

    case 1338:

        return 103058;

    case 1339:

        return 103059;

    case 1340:

        return 103059;

    case 1341:

        return 103059;

    case 1342:

        return 103059;

    case 1343:

        return 103059;

    case 1344:

        return 103059;

    case 1345:

        return 103059;

    case 1346:

        return 103059;

    case 1347:

        return 103059;

    case 1348:

        return 103059;

    case 1349:

        return 103059;

    case 1350:

        return 103059;

    case 1351:

        return 103059;

    case 1352:

        return 103059;

    case 1353:

        return 103059;

    case 1354:

        return 103059;

    case 1355:

        return 103059;

    case 1356:

        return 103060;

    case 1357:

        return 103064;

    case 1358:

        return 103068;

    case 1359:

        return 103074;

    case 1360:

        return 103080;

    case 1361:

        return 103087;

    case 1362:

        return 103095;

    case 1363:

        return 103103;

    case 1364:

        return 103112;

    case 1365:

        return 103122;

    case 1366:

        return 103132;

    case 1367:

        return 103143;

    case 1368:

        return 103155;

    case 1369:

        return 103167;

    case 1370:

        return 103179;

    case 1371:

        return 103192;

    case 1372:

        return 103205;

    case 1373:

        return 103218;

    case 1374:

        return 103232;

    case 1375:

        return 103245;

    case 1376:

        return 103259;

    case 1377:

        return 103272;

    case 1378:

        return 103286;

    case 1379:

        return 103299;

    case 1380:

        return 103312;

    case 1381:

        return 103324;

    case 1382:

        return 103335;

    case 1383:

        return 103347;

    case 1384:

        return 103358;

    case 1385:

        return 103369;

    case 1386:

        return 103380;

    case 1387:

        return 103391;

    case 1388:

        return 103402;

    case 1389:

        return 103413;

    case 1390:

        return 103425;

    case 1391:

        return 103436;

    case 1392:

        return 103447;

    case 1393:

        return 103458;

    case 1394:

        return 103469;

    case 1395:

        return 103480;

    case 1396:

        return 103491;

    case 1397:

        return 103501;

    case 1398:

        return 103511;

    case 1399:

        return 103520;

    case 1400:

        return 103527;

    case 1401:

        return 103533;

    case 1402:

        return 103537;

    case 1403:

        return 103540;

    case 1404:

        return 103540;

    case 1405:

        return 103540;

    case 1406:

        return 103540;

    case 1407:

        return 103540;

    case 1408:

        return 103540;

    case 1409:

        return 103540;

    case 1410:

        return 103540;

    case 1411:

        return 103540;

    case 1412:

        return 103540;

    case 1413:

        return 103540;

    case 1414:

        return 103540;

    case 1415:

        return 103540;

    case 1416:

        return 103540;

    case 1417:

        return 103540;

    case 1418:

        return 103540;

    case 1419:

        return 103540;

    case 1420:

        return 103540;

    case 1421:

        return 103542;

    case 1422:

        return 103546;

    case 1423:

        return 103551;

    case 1424:

        return 103557;

    case 1425:

        return 103563;

    case 1426:

        return 103571;

    case 1427:

        return 103579;

    case 1428:

        return 103587;

    case 1429:

        return 103596;

    case 1430:

        return 103606;

    case 1431:

        return 103616;

    case 1432:

        return 103627;

    case 1433:

        return 103638;

    case 1434:

        return 103650;

    case 1435:

        return 103662;

    case 1436:

        return 103675;

    case 1437:

        return 103688;

    case 1438:

        return 103701;

    case 1439:

        return 103715;

    case 1440:

        return 103729;

    case 1441:

        return 103743;

    case 1442:

        return 103758;

    case 1443:

        return 103773;

    case 1444:

        return 103788;

    case 1445:

        return 103803;

    case 1446:

        return 103819;

    case 1447:

        return 103835;

    case 1448:

        return 103851;

    case 1449:

        return 103867;

    case 1450:

        return 103884;

    case 1451:

        return 103900;

    case 1452:

        return 103917;

    case 1453:

        return 103933;

    case 1454:

        return 103950;

    case 1455:

        return 103967;

    case 1456:

        return 103983;

    case 1457:

        return 103999;

    case 1458:

        return 104015;

    case 1459:

        return 104031;

    case 1460:

        return 104047;

    case 1461:

        return 104062;

    case 1462:

        return 104077;

    case 1463:

        return 104091;

    case 1464:

        return 104105;

    case 1465:

        return 104119;

    case 1466:

        return 104132;

    case 1467:

        return 104145;

    case 1468:

        return 104157;

    case 1469:

        return 104168;

    case 1470:

        return 104179;

    case 1471:

        return 104189;

    case 1472:

        return 104198;

    case 1473:

        return 104205;

    case 1474:

        return 104210;

    case 1475:

        return 104214;

    case 1476:

        return 104215;

    case 1477:

        return 104215;

    case 1478:

        return 104215;

    case 1479:

        return 104215;

    case 1480:

        return 104215;

    case 1481:

        return 104215;

    case 1482:

        return 104215;

    case 1483:

        return 104215;

    case 1484:

        return 104215;

    case 1485:

        return 104215;

    case 1486:

        return 104215;

    case 1487:

        return 104215;

    case 1488:

        return 104215;

    case 1489:

        return 104215;

    case 1490:

        return 104215;

    case 1491:

        return 104215;

    case 1492:

        return 104215;

    case 1493:

        return 104215;

    case 1494:

        return 104216;

    case 1495:

        return 104220;

    case 1496:

        return 104224;

    case 1497:

        return 104230;

    case 1498:

        return 104236;

    case 1499:

        return 104244;

    case 1500:

        return 104251;

    case 1501:

        return 104260;

    case 1502:

        return 104269;

    case 1503:

        return 104278;

    case 1504:

        return 104288;

    case 1505:

        return 104299;

    case 1506:

        return 104309;

    case 1507:

        return 104320;

    case 1508:

        return 104331;

    case 1509:

        return 104342;

    case 1510:

        return 104354;

    case 1511:

        return 104365;

    case 1512:

        return 104377;

    case 1513:

        return 104388;

    case 1514:

        return 104400;

    case 1515:

        return 104412;

    case 1516:

        return 104424;

    case 1517:

        return 104436;

    case 1518:

        return 104448;

    case 1519:

        return 104460;

    case 1520:

        return 104472;

    case 1521:

        return 104484;

    case 1522:

        return 104497;

    case 1523:

        return 104509;

    case 1524:

        return 104521;

    case 1525:

        return 104532;

    case 1526:

        return 104544;

    case 1527:

        return 104554;

    case 1528:

        return 104564;

    case 1529:

        return 104574;

    case 1530:

        return 104583;

    case 1531:

        return 104591;

    case 1532:

        return 104599;

    case 1533:

        return 104606;

    case 1534:

        return 104613;

    case 1535:

        return 104620;

    case 1536:

        return 104627;

    case 1537:

        return 104634;

    case 1538:

        return 104641;

    case 1539:

        return 104648;

    case 1540:

        return 104655;

    case 1541:

        return 104661;

    case 1542:

        return 104668;

    case 1543:

        return 104675;

    case 1544:

        return 104682;

    case 1545:

        return 104689;

    case 1546:

        return 104696;

    case 1547:

        return 104703;

    case 1548:

        return 104710;

    case 1549:

        return 104717;

    case 1550:

        return 104724;

    case 1551:

        return 104729;

    case 1552:

        return 104734;

    case 1553:

        return 104737;

    case 1554:

        return 104739;

    case 1555:

        return 104740;

    case 1556:

        return 104740;

    case 1557:

        return 104740;

    case 1558:

        return 104740;

    case 1559:

        return 104740;

    case 1560:

        return 104740;

    case 1561:

        return 104740;

    case 1562:

        return 104740;

    case 1563:

        return 104740;

    case 1564:

        return 104740;

    case 1565:

        return 104740;

    case 1566:

        return 104740;

    case 1567:

        return 104740;

    case 1568:

        return 104740;

    case 1569:

        return 104740;

    case 1570:

        return 104740;

    case 1571:

        return 104740;

    case 1572:

        return 104740;

    case 1573:

        return 104740;

    case 1574:

        return 104740;

    case 1575:

        return 104740;

    case 1576:

        return 104740;

    case 1577:

        return 104740;

    case 1578:

        return 104742;

    case 1579:

        return 104746;

    case 1580:

        return 104751;

    case 1581:

        return 104757;

    case 1582:

        return 104763;

    case 1583:

        return 104771;

    case 1584:

        return 104779;

    case 1585:

        return 104787;

    case 1586:

        return 104797;

    case 1587:

        return 104806;

    case 1588:

        return 104817;

    case 1589:

        return 104827;

    case 1590:

        return 104838;

    case 1591:

        return 104849;

    case 1592:

        return 104861;

    case 1593:

        return 104872;

    case 1594:

        return 104883;

    case 1595:

        return 104894;

    case 1596:

        return 104905;

    case 1597:

        return 104916;

    case 1598:

        return 104927;

    case 1599:

        return 104938;

    case 1600:

        return 104948;

    case 1601:

        return 104959;

    case 1602:

        return 104970;

    case 1603:

        return 104981;

    case 1604:

        return 104992;

    case 1605:

        return 105003;

    case 1606:

        return 105013;

    case 1607:

        return 105024;

    case 1608:

        return 105035;

    case 1609:

        return 105046;

    case 1610:

        return 105057;

    case 1611:

        return 105069;

    case 1612:

        return 105081;

    case 1613:

        return 105093;

    case 1614:

        return 105105;

    case 1615:

        return 105118;

    case 1616:

        return 105131;

    case 1617:

        return 105144;

    case 1618:

        return 105157;

    case 1619:

        return 105170;

    case 1620:

        return 105183;

    case 1621:

        return 105196;

    case 1622:

        return 105208;

    case 1623:

        return 105221;

    case 1624:

        return 105233;

    case 1625:

        return 105245;

    case 1626:

        return 105258;

    case 1627:

        return 105270;

    case 1628:

        return 105282;

    case 1629:

        return 105294;

    case 1630:

        return 105307;

    case 1631:

        return 105319;

    case 1632:

        return 105332;

    case 1633:

        return 105344;

    case 1634:

        return 105356;

    case 1635:

        return 105368;

    case 1636:

        return 105379;

    case 1637:

        return 105390;

    case 1638:

        return 105401;

    case 1639:

        return 105412;

    case 1640:

        return 105422;

    case 1641:

        return 105432;

    case 1642:

        return 105442;

    case 1643:

        return 105452;

    case 1644:

        return 105461;

    case 1645:

        return 105471;

    case 1646:

        return 105481;

    case 1647:

        return 105490;

    case 1648:

        return 105500;

    case 1649:

        return 105510;

    case 1650:

        return 105520;

    case 1651:

        return 105529;

    case 1652:

        return 105539;

    case 1653:

        return 105549;

    case 1654:

        return 105559;

    case 1655:

        return 105568;

    case 1656:

        return 105578;

    case 1657:

        return 105586;

    case 1658:

        return 105594;

    case 1659:

        return 105601;

    case 1660:

        return 105607;

    case 1661:

        return 105610;

    case 1662:

        return 105612;

    case 1663:

        return 105612;

    case 1664:

        return 105612;

    case 1665:

        return 105612;

    case 1666:

        return 105612;

    case 1667:

        return 105612;

    case 1668:

        return 105612;

    case 1669:

        return 105612;

    case 1670:

        return 105612;

    case 1671:

        return 105612;

    case 1672:

        return 105612;

    case 1673:

        return 105612;

    case 1674:

        return 105612;

    case 1675:

        return 105612;

    case 1676:

        return 105612;

    case 1677:

        return 105612;

    case 1678:

        return 105612;

    case 1679:

        return 105612;

    case 1680:

        return 105612;

    case 1681:

        return 105612;

    case 1682:

        return 105613;

    case 1683:

        return 105615;

    case 1684:

        return 105619;

    case 1685:

        return 105624;

    case 1686:

        return 105631;

    case 1687:

        return 105638;

    case 1688:

        return 105645;

    case 1689:

        return 105653;

    case 1690:

        return 105662;

    case 1691:

        return 105672;

    case 1692:

        return 105681;

    case 1693:

        return 105692;

    case 1694:

        return 105702;

    case 1695:

        return 105713;

    case 1696:

        return 105724;

    case 1697:

        return 105735;

    case 1698:

        return 105746;

    case 1699:

        return 105756;

    case 1700:

        return 105767;

    case 1701:

        return 105778;

    case 1702:

        return 105789;

    case 1703:

        return 105800;

    case 1704:

        return 105811;

    case 1705:

        return 105822;

    case 1706:

        return 105833;

    case 1707:

        return 105844;

    case 1708:

        return 105855;

    case 1709:

        return 105866;

    case 1710:

        return 105877;

    case 1711:

        return 105887;

    case 1712:

        return 105897;

    case 1713:

        return 105906;

    case 1714:

        return 105915;

    case 1715:

        return 105923;

    case 1716:

        return 105931;

    case 1717:

        return 105939;

    case 1718:

        return 105947;

    case 1719:

        return 105955;

    case 1720:

        return 105963;

    case 1721:

        return 105971;

    case 1722:

        return 105979;

    case 1723:

        return 105987;

    case 1724:

        return 105995;

    case 1725:

        return 106003;

    case 1726:

        return 106011;

    case 1727:

        return 106019;

    case 1728:

        return 106027;

    case 1729:

        return 106035;

    case 1730:

        return 106043;

    case 1731:

        return 106051;

    case 1732:

        return 106059;

    case 1733:

        return 106067;

    case 1734:

        return 106075;

    case 1735:

        return 106082;

    case 1736:

        return 106089;

    case 1737:

        return 106096;

    case 1738:

        return 106103;

    case 1739:

        return 106110;

    case 1740:

        return 106117;

    case 1741:

        return 106123;

    case 1742:

        return 106130;

    case 1743:

        return 106137;

    case 1744:

        return 106144;

    case 1745:

        return 106151;

    case 1746:

        return 106158;

    case 1747:

        return 106165;

    case 1748:

        return 106172;

    case 1749:

        return 106179;

    case 1750:

        return 106186;

    case 1751:

        return 106193;

    case 1752:

        return 106200;

    case 1753:

        return 106208;

    case 1754:

        return 106215;

    case 1755:

        return 106223;

    case 1756:

        return 106231;

    case 1757:

        return 106239;

    case 1758:

        return 106247;

    case 1759:

        return 106255;

    case 1760:

        return 106263;

    case 1761:

        return 106271;

    case 1762:

        return 106279;

    case 1763:

        return 106287;

    case 1764:

        return 106296;

    case 1765:

        return 106304;

    case 1766:

        return 106312;

    case 1767:

        return 106320;

    case 1768:

        return 106329;

    case 1769:

        return 106337;

    case 1770:

        return 106345;

    case 1771:

        return 106353;

    case 1772:

        return 106362;

    case 1773:

        return 106370;

    case 1774:

        return 106378;

    case 1775:

        return 106387;

    case 1776:

        return 106395;

    case 1777:

        return 106403;

    case 1778:

        return 106412;

    case 1779:

        return 106420;

    case 1780:

        return 106430;

    case 1781:

        return 106439;

    case 1782:

        return 106449;

    case 1783:

        return 106459;

    case 1784:

        return 106470;

    case 1785:

        return 106481;

    case 1786:

        return 106493;

    case 1787:

        return 106505;

    case 1788:

        return 106518;

    case 1789:

        return 106531;

    case 1790:

        return 106544;

    case 1791:

        return 106557;

    case 1792:

        return 106571;

    case 1793:

        return 106584;

    case 1794:

        return 106598;

    case 1795:

        return 106612;

    case 1796:

        return 106625;

    case 1797:

        return 106638;

    case 1798:

        return 106650;

    case 1799:

        return 106661;

    case 1800:

        return 106670;

    case 1801:

        return 106679;

    case 1802:

        return 106686;

    case 1803:

        return 106691;

    case 1804:

        return 106694;

    case 1805:

        return 106695;

    case 1806:

        return 106695;

    case 1807:

        return 106695;

    case 1808:

        return 106695;

    case 1809:

        return 106695;

    case 1810:

        return 106695;

    case 1811:

        return 106695;

    case 1812:

        return 106695;

    case 1813:

        return 106695;

    case 1814:

        return 106695;

    case 1815:

        return 106695;

    case 1816:

        return 106695;

    case 1817:

        return 106695;

    case 1818:

        return 106695;

    case 1819:

        return 106695;

    case 1820:

        return 106695;

    case 1821:

        return 106695;

    case 1822:

        return 106695;

    case 1823:

        return 106695;

    case 1824:

        return 106695;

    case 1825:

        return 106695;

    case 1826:

        return 106695;

    case 1827:

        return 106695;

    case 1828:

        return 106695;

    case 1829:

        return 106695;

    case 1830:

        return 106695;

    case 1831:

        return 106695;

    case 1832:

        return 106695;

    case 1833:

        return 106695;

    case 1834:

        return 106695;

    case 1835:

        return 106695;

    case 1836:

        return 106695;

    case 1837:

        return 106695;

    case 1838:

        return 106695;

    case 1839:

        return 106695;

    case 1840:

        return 106695;

    case 1841:

        return 106695;

    case 1842:

        return 106695;

    case 1843:

        return 106695;

    case 1844:

        return 106695;

    case 1845:

        return 106695;

    case 1846:

        return 106695;

    case 1847:

        return 106695;

    case 1848:

        return 106695;

    case 1849:

        return 106695;

    case 1850:

        return 106695;

    case 1851:

        return 106695;

    case 1852:

        return 106695;

    case 1853:

        return 106695;

    case 1854:

        return 106695;

    case 1855:

        return 106695;

    case 1856:

        return 106695;

    case 1857:

        return 106695;

    case 1858:

        return 106695;

    case 1859:

        return 106695;

    case 1860:

        return 106695;

    case 1861:

        return 106695;

    case 1862:

        return 106695;

    case 1863:

        return 106695;

    case 1864:

        return 106695;

    case 1865:

        return 106695;

    default:

        return 0;

    }

}

int maFenetre::temps_position(double p) {

    p -= 2;
    int t = 1;

    if (p > 99000) {

        t = 855;

    }

    while (position_temps(t) < p && t < 1865) {

        t++;

    }

    return t;

}

int maFenetre::avanceRame(int n) {

    return avance[n];

}

void maFenetre::setBiaisAutres(int n, double augmentation) {

    biais_temps[n] += augmentation;

}

int maFenetre::numeroStation(double d) {

    if ( 304 < d && d <= 888 + 306 * PI/4 + 50 * V2)
        return 1;
    if ( 888 + 306 * PI/4 + 50 * V2 < d && d <=  894 + 306 * PI/4 + 50 * V2)
        return 2;
    if (1413 + 306 * PI/4 + 50 * V2 < d && d <= 2088 + 306 * PI/4 + 50 * V2)
        return 3;
    if (2088 + 306 * PI/4 + 50 * V2 < d && d <= 2488 + 403 * PI/4 + 50 * V2)
        return 4;
    if (2488 + 403 * PI/4 + 50 * V2 < d && d <= 2963 + 1081 * PI/4 + 50 * V2)
        return 5;
    if (2963 + 1081 * PI/4 + 50 * V2 < d && d <= 3438 + 1081 * PI/4 + 50 * V2)
        return 6;
    if (3438 + 1081 * PI/4 + 50 * V2 < d && d <= 3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 7;
    if (3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 < d && d <= 4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 8;
    if (4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 < d && d <= 4640 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 9;
    if (4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2 < d && d <= 4727.95 + 1705 * PI/4 + 300 * PI/3 + 50 * V2)
        return 10;
    if (4727.95 + 1705 * PI/4 + 300 * PI/3 + 50 * V2 < d && d <= 4827.95 + 1705 * PI/4 + 300 * PI/3 + 50 * V2) //2ème borne arbitraire, correspond à la station Bourse départ
        return 11;
    if ( 99997 < d && d <= 100471.35)
        return 101;
    if (100471.35 < d && d <= 101056.35)
        return 102;
    if (101056.35 < d && d <= 101248.35 + 156 * PI/4 + 200 * PI/3)
        return 103;
    if (101248.35 + 156 * PI/4 + 200 * PI/3 < d && d <= 101723.35 + 156 * PI/4 + 200 * PI/3)
        return 104;
    if (101723.35 + 156 * PI/4 + 200 * PI/3 < d && d <= 102198.35 + 828 * PI/4 + 200 * PI/3)
        return 105;
    if (102198.35 + 828 * PI/4 + 200 * PI/3 < d && d <= 102598.35 + 931 * PI/4 + 200 * PI/3)
        return 106;
    if (102598.35 + 931 * PI/4 + 200 * PI/3 < d && d <= 103273.35 + 931 * PI/4 + 200 * PI/3)
        return 107;
    if (103273.35 + 931 * PI/4 + 200 * PI/3 < d && d <= 103792.35 + 931 * PI/4 + 200 * PI/3)
        return 108;
    if (103798.35 + 931 * PI/4 + 200 * PI/3 < d && d <= 104367.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 109;
    if ((104367.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 < d && d <= 104955.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) || (d <= 304))
        return 110;
    return 0; //erreur

}

void maFenetre::dessinerPetitsBoutsTunnel() {

    glBindTexture(GL_TEXTURE_2D, texture2[1]);

    glBegin(GL_QUADS);

    //petits bouts de tunnel, utilisés notamment dans les courbes

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -504.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -345.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -345.0f, -504.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -345.0f, -504.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -345.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -357.5f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -357.5f, -504.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -357.5f, -504.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -357.5f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -370.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -370.0f, -504.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -330.0f, -516.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -330.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -345.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -345.0f, -516.0f, 4.0f);

    for (int c = 0; c < 5; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-370 - 20 * c), -498.0f, 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-370 - 20 * c), -498.0f, 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-390 - 20 * c), -498.0f, 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-390 - 20 * c), -498.0f, 4.0f);

    }

    for (int c = 0; c < 24; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-470 - 106 * sin(c       * PI / 48)), float(-404 - 106 * cos(c       * PI/48)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-470 - 106 * sin((c + 1) * PI / 48)), float(-404 - 106 * cos((c + 1) * PI/48)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-470 - 106 * sin((c + 1) * PI / 48)), float(-404 - 106 * cos((c + 1) * PI/48)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-470 - 106 * sin(c       * PI / 48)), float(-404 - 106 * cos(c       * PI/48)), 9.1f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-470 - 94 * sin(c       * PI / 48)), float(-404 - 94 * cos(c       * PI/48)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-470 - 94 * sin((c + 1) * PI / 48)), float(-404 - 94 * cos((c + 1) * PI/48)), 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-470 - 94 * sin((c + 1) * PI / 48)), float(-404 - 94 * cos((c + 1) * PI/48)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-470 - 94 * sin(c       * PI / 48)), float(-404 - 94 * cos(c       * PI/48)), 9.1f);

    }

    for (int c = 0; c < 30; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 + 81 * cos(c       * PI / 30)), float(-404 + 81 * sin(c       * PI/30)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 + 81 * cos(c       * PI / 30)), float(-404 + 81 * sin(c       * PI/30)), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 + 81 * cos((c + 1) * PI / 30)), float(-404 + 81 * sin((c + 1) * PI/30)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 + 81 * cos((c + 1) * PI / 30)), float(-404 + 81 * sin((c + 1) * PI/30)), 4.0f);

        if (c >= 15) {

            glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 + 69 * cos(c       * PI / 30)), float(-404 + 69 * sin(c       * PI/30)), 4.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 + 69 * cos(c       * PI / 30)), float(-404 + 69 * sin(c       * PI/30)), 9.1f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 + 69 * cos((c + 1) * PI / 30)), float(-404 + 69 * sin((c + 1) * PI/30)), 9.1f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 + 69 * cos((c + 1) * PI / 30)), float(-404 + 69 * sin((c + 1) * PI/30)), 4.0f);

            if (c >= 22) {

                glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 + 75 * cos(c       * PI / 30)), float(-404 + 75 * sin(c       * PI/30)), 4.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 + 75 * cos(c       * PI / 30)), float(-404 + 75 * sin(c       * PI/30)), 9.1f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 + 75 * cos((c + 1) * PI / 30)), float(-404 + 75 * sin((c + 1) * PI/30)), 9.1f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 + 75 * cos((c + 1) * PI / 30)), float(-404 + 75 * sin((c + 1) * PI/30)), 4.0f);

            }

        }

    }

    for (int c = 0; c < 10; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-576.0 - (c / 10.0) * 69), float(-404.0 + (c / 10.0) * 69), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-576.0 - (c / 10.0) * 69), float(-404.0 + (c / 10.0) * 69), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-582.9 - (c / 10.0) * 69), float(-397.1 + (c / 10.0) * 69), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-582.9 - (c / 10.0) * 69), float(-397.1 + (c / 10.0) * 69), 4.0f);

    }

    for (int c = 0; c < 4; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -714.0f, float(-404.00 -9.25 * c), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -714.0f, float(-404.00 -9.25 * c), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -714.0f, float(-413.25 -9.25 * c), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -714.0f, float(-413.25 -9.25 * c), 4.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -726.0f, float(-404.00 -9.25 * c), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -726.0f, float(-404.00 -9.25 * c), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -726.0f, float(-413.25 -9.25 * c), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -726.0f, float(-413.25 -9.25 * c), 4.0f);

        if (c < 2) {

            glTexCoord2f(0.0f, 0.0f); glVertex3f( -720.0f, float(-404.00 -9.25 * c), 4.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( -720.0f, float(-404.00 -9.25 * c), 9.1f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( -720.0f, float(-413.25 -9.25 * c), 9.1f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( -720.0f, float(-413.25 -9.25 * c), 4.0f);

        }

    }

    for (int c = 0; c < 18; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 - 69 * cos(c       * PI / 36)), float(-441 - 69 * sin(c       * PI/36)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 - 69 * cos(c       * PI / 36)), float(-441 - 69 * sin(c       * PI/36)), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 - 69 * cos((c + 1) * PI / 36)), float(-441 - 69 * sin((c + 1) * PI/36)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 - 69 * cos((c + 1) * PI / 36)), float(-441 - 69 * sin((c + 1) * PI/36)), 4.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-645 - 81 * cos(c       * PI / 36)), float(-441 - 81 * sin(c       * PI/36)), 4.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-645 - 81 * cos(c       * PI / 36)), float(-441 - 81 * sin(c       * PI/36)), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-645 - 81 * cos((c + 1) * PI / 36)), float(-441 - 81 * sin((c + 1) * PI/36)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-645 - 81 * cos((c + 1) * PI / 36)), float(-441 - 81 * sin((c + 1) * PI/36)), 4.0f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -206.0f, -504.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -223.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -223.0f, -504.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -223.0f, -504.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -223.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -504.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -504.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -206.0f, -516.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -223.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -223.0f, -516.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -223.0f, -516.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -223.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -240.0f, -516.0f, 9.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -240.0f, -516.0f, 4.0f);

    for (int c = 16; c < 32; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-206 + 144 * cos(c       * PI / 64)), float(-360 - 144 * sin(c       * PI/64)), 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-206 + 144 * cos(c       * PI / 64)), float(-360 - 144 * sin(c       * PI/64)), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-206 + 144 * cos((c + 1) * PI / 64)), float(-360 - 144 * sin((c + 1) * PI/64)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-206 + 144 * cos((c + 1) * PI / 64)), float(-360 - 144 * sin((c + 1) * PI/64)), 3.9f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-206 + 156 * cos(c       * PI / 64)), float(-360 - 156 * sin(c       * PI/64)), 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-206 + 156 * cos(c       * PI / 64)), float(-360 - 156 * sin(c       * PI/64)), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-206 + 156 * cos((c + 1) * PI / 64)), float(-360 - 156 * sin((c + 1) * PI/64)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-206 + 156 * cos((c + 1) * PI / 64)), float(-360 - 156 * sin((c + 1) * PI/64)), 3.9f);

    }

    for (int c = 0; c < 4; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-54.176624 - 12.5 * c), float(-411.823376 - 12.5 * c), 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-54.176624 - 12.5 * c), float(-411.823376 - 12.5 * c), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-66.676624 - 12.5 * c), float(-424.323376 - 12.5 * c), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-66.676624 - 12.5 * c), float(-424.323376 - 12.5 * c), 3.9f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-45.691342 - 12.5 * c), float(-420.308658 - 12.5 * c), 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-45.691342 - 12.5 * c), float(-420.308658 - 12.5 * c), 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-58.191342 - 12.5 * c), float(-432.808658 - 12.5 * c), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-58.191342 - 12.5 * c), float(-432.808658 - 12.5 * c), 3.9f);


    }

    for (int c = 0; c < 16; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-156 + 144 * cos(c       * PI / 64)), float(-310 - 144 * sin(c       * PI/64)), 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-156 + 144 * cos((c + 1) * PI / 64)), float(-310 - 144 * sin((c + 1) * PI/64)), 3.9f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-156 + 144 * cos((c + 1) * PI / 64)), float(-310 - 144 * sin((c + 1) * PI/64)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-156 + 144 * cos(c       * PI / 64)), float(-310 - 144 * sin(c       * PI/64)), 9.1f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(-156 + 156 * cos(c       * PI / 64)), float(-310 - 156 * sin(c       * PI/64)), 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(-156 + 156 * cos((c + 1) * PI / 64)), float(-310 - 156 * sin((c + 1) * PI/64)), 3.9f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(-156 + 156 * cos((c + 1) * PI / 64)), float(-310 - 156 * sin((c + 1) * PI/64)), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(-156 + 156 * cos(c       * PI / 64)), float(-310 - 156 * sin(c       * PI/64)), 9.1f);

    }

    for (double f = -310.0; f < -150.0; f += 20) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, float(f)     , float(-3.0 + 0.02 * (-f - 150)));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, float(f)     , 5.0f                           );
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, float(f + 20), 5.0f                           );
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, float(f + 20), float(-3.0 + 0.02 * (-f - 170)));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, float(f)     , -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, float(f)     ,  1.95f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f, float(f + 20),  1.95f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, float(f + 20), -2.5f );

    }

    for (double f = -150.0; f < -100.0; f += 12.5) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, float(f)       , 1.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, float(f)       , 4.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, float(f + 12.5), 4.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, float(f + 12.5), 1.9f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, float(f)       , 3.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, float(f)       , 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, float(f + 12.5), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, float(f + 12.5), 3.9f);

    }

    for (double f = -150.0; f < -75.0; f += 25) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, float(f)     , -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, float(f)     ,  1.95f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f, float(f + 25),  1.95f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, float(f + 25), -2.5f );

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, float(f)     , -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, float(f)     ,  1.95f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, float(f + 25),  1.95f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, float(f + 25), -2.5f );

    }

    for (double f = -150.0; f < -100.0; f += 12.5) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, float(f)       , 1.9f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, float(f)       , 9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, float(f + 12.5), 9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, float(f + 12.5), 1.9f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  0.0f, -75.0f, -2.5f );
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  0.0f, -75.0f,  1.95f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, -75.0f,  1.95f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, -75.0f, -2.5f );

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 0.0f, -2.5f );
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 0.0f,  1.95f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, 0.0f,  1.95f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, 0.0f, -2.5f );

    for (double f = -75.0; f < 0.0; f += 25) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, float(f)     ,  -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, float(f)     ,   1.95f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, float(f + 25),   1.95f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, float(f + 25),  -2.5f );

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, float(f)     ,  -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, float(f)     ,   1.95f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, float(f + 25),   1.95f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, float(f + 25),  -2.5f );

    }

    for (double f = 0.0; f < 150.0; f += 25) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, float(f)     , -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, float(f)     , -0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f, float(f + 25), -0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, float(f + 25), -2.5f );

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f, float(f)     ,  -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f, float(f)     ,   0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, float(f + 25),   0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, float(f + 25),  -2.5f );

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, float(f)     , -2.5f );
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, float(f)     , -0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f, float(f + 25), -0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f, float(f + 25), -2.5f );

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f, 345.0f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f, 345.0f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  -9.0f, 345.0f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  -9.0f, 345.0f, -0.1f);

    for (double y = 120.0; y < 150.0; y += 15) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, float(y)     , -0.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, float(y)     ,  5.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.0f, float(y + 15),  5.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f, float(y + 15), -0.1f);

    }

    for (double y = 0.0; y < 120.0; y += 20) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, float(y)     , -2.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, float(y)     ,  1.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.0f, float(y + 20),  1.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f, float(y + 20), -2.5f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, float(y)     ,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, float(y)     ,  5.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.0f, float(y + 20),  5.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f, float(y + 20),  1.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, float(y)     ,  5.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, float(y)     ,  9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.0f, float(y + 20),  9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f, float(y + 20),  5.0f);

    }

    for (double y = 0.0; y < 100.0; y += 20) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, float(y)     , -0.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, float(y)     ,  9.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f, float(y + 20),  9.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, float(y + 20), -0.1f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f,  90.0f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f,  90.0f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 100.0f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 100.0f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 100.0f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 100.0f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 111.5f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 111.5f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 111.1f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 111.1f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 125.0f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 125.0f, -0.1f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 140.0f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 140.0f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 150.0f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 150.0f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 225.0f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 225.0f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.0f, 237.5f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f, 237.5f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 225.0f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 225.0f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 237.5f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 237.5f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -9.0f, 237.5f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -9.0f, 237.5f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -9.0f, 250.0f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -9.0f, 250.0f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 237.5f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 237.5f,  5.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 250.0f,  5.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 250.0f, -0.1f);

    for (int c = 24; c < 32; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 +  94*cos(c      *PI/32)), float(1450.0 +  94*sin(c      *PI/32)),  -6.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 +  94*cos(c      *PI/32)), float(1450.0 +  94*sin(c      *PI/32)),  -0.7f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 +  94*cos((c + 1)*PI/32)), float(1450.0 +  94*sin((c + 1)*PI/32)),  -0.7f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 +  94*cos((c + 1)*PI/32)), float(1450.0 +  94*sin((c + 1)*PI/32)),  -6.1f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 106*cos(c      *PI/32)), float(1450.0 + 106*sin(c      *PI/32)),  -6.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 106*cos(c      *PI/32)), float(1450.0 + 106*sin(c      *PI/32)),  -0.7f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 106*cos((c + 1)*PI/32)), float(1450.0 + 106*sin((c + 1)*PI/32)),  -0.7f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 106*cos((c + 1)*PI/32)), float(1450.0 + 106*sin((c + 1)*PI/32)),  -6.1f);

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1425.0f,  -6.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1425.0f,  -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1437.5f,  -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1437.5f,  -6.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1437.5f,  -6.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1437.5f,  -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1450.0f,  -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1450.0f,  -6.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1425.0f,  -6.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1425.0f,  -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1437.5f,  -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1437.5f,  -6.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  6.0f, 1437.5f,  -6.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  6.0f, 1437.5f,  -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  6.0f, 1450.0f,  -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  6.0f, 1450.0f,  -6.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  298.697006f, 1781.633080f,  -3.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  298.697006f, 1781.633080f,   2.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  311.955258f, 1794.891332f,   2.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  311.955258f, 1794.891332f,  -3.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  311.955258f, 1794.891332f,  -3.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  311.955258f, 1794.891332f,   2.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  325.213510f, 1808.149584f,   2.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  325.213510f, 1808.149584f,  -3.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  290.211724f, 1790.118362f,  -3.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  290.211724f, 1790.118362f,   2.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  303.469976f, 1803.376614f,   2.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  303.469976f, 1803.376614f,  -3.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  303.469976f, 1803.376614f,  -3.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  303.469976f, 1803.376614f,   2.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  316.728228f, 1816.634866f,   2.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  316.728228f, 1816.634866f,  -3.1f);



    for (int c = 3; c < 32; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-c      *PI/128)), float(2546.015511 + 669*sin(-c      *PI/128)),  float(11.0 + 0.04 * (-c + 32) * 675 * PI/ 128));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-c      *PI/128)), float(2546.015511 + 669*sin(-c      *PI/128)),  float(16.8 + 0.04 * (-c + 32) * 675 * PI/ 128));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-(c + 1)*PI/128)), float(2546.015511 + 669*sin(-(c + 1)*PI/128)),  float(16.8 + 0.04 * (-c + 31) * 675 * PI/ 128));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(c + 1)*PI/128)), float(2546.015511 + 669*sin(-(c + 1)*PI/128)),  float(11.0 + 0.04 * (-c + 31) * 675 * PI/ 128));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-c      *PI/128)), float(2546.015511 + 681*sin(-c      *PI/128)),  float(11.0 + 0.04 * (-c + 32) * 675 * PI/ 128));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-c      *PI/128)), float(2546.015511 + 681*sin(-c      *PI/128)),  float(16.8 + 0.04 * (-c + 32) * 675 * PI/ 128));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-(c + 1)*PI/128)), float(2546.015511 + 681*sin(-(c + 1)*PI/128)),  float(16.8 + 0.04 * (-c + 31) * 675 * PI/ 128));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(c + 1)*PI/128)), float(2546.015511 + 681*sin(-(c + 1)*PI/128)),  float(11.0 + 0.04 * (-c + 31) * 675 * PI/ 128)); //en principe 11.5 -> 16.5

        //attention la courbe est tracée dans le mauvais sens

    }

    for (int c = 0; c < 2; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-c      *PI/128)), float(2546.015511 + 669*sin(-c      *PI/128)),  float( 9.5 + 6.75 * PI)); //normalement 10.0 -> 15.0
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-c      *PI/128)), float(2546.015511 + 669*sin(-c      *PI/128)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-(c + 1)*PI/128)), float(2546.015511 + 669*sin(-(c + 1)*PI/128)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(c + 1)*PI/128)), float(2546.015511 + 669*sin(-(c + 1)*PI/128)),  float( 9.5 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-c      *PI/128)), float(2546.015511 + 681*sin(-c      *PI/128)),  float( 9.5 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-c      *PI/128)), float(2546.015511 + 681*sin(-c      *PI/128)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-(c + 1)*PI/128)), float(2546.015511 + 681*sin(-(c + 1)*PI/128)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(c + 1)*PI/128)), float(2546.015511 + 681*sin(-(c + 1)*PI/128)),  float( 9.5 + 6.75 * PI));

        //le cas c = 2 correspond à une pente intermédiaire

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-2      *PI/128)), float(2546.015511 + 669*sin(-2      *PI/128)),  float( 9.5 + 6.75 * PI)); //normalement 10.0 -> 15.0
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-2      *PI/128)), float(2546.015511 + 669*sin(-2      *PI/128)),  float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 669*cos(-(2 + 1)*PI/128)), float(2546.015511 + 669*sin(-(2 + 1)*PI/128)),  float(16.8 + 0.04 * (-2 + 31) * 675 * PI / 128));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 669*cos(-(2 + 1)*PI/128)), float(2546.015511 + 669*sin(-(2 + 1)*PI/128)),  float(11.0 + 0.04 * (-2 + 31) * 675 * PI / 128));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-2      *PI/128)), float(2546.015511 + 681*sin(-2      *PI/128)),  float( 9.5 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-2      *PI/128)), float(2546.015511 + 681*sin(-2      *PI/128)),  float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(100.0 + 681*cos(-(2 + 1)*PI/128)), float(2546.015511 + 681*sin(-(2 + 1)*PI/128)),  float(16.8 + 0.04 * (-2 + 31) * 675 * PI / 128));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(100.0 + 681*cos(-(2 + 1)*PI/128)), float(2546.015511 + 681*sin(-(2 + 1)*PI/128)),  float(11.0 + 0.04 * (-2 + 31) * 675 * PI / 128));


    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 2621.0f, float( 9.5   + 6.75 * PI)); //spécial passage en aérien
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 2621.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  769.0f, 2641.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  769.0f, 2641.0f, float( 9.5   + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  781.0f, 2621.0f, float( 9.5   + 6.75 * PI)); //spécial passage en aérien
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  781.0f, 2621.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 2641.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 2641.0f, float( 9.5   + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  769.0f, 3001.0f, float( 9.5   + 6.75 * PI)); //spécial passage en aérien
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  769.0f, 3001.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  769.0f, 3021.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  769.0f, 3021.0f, float( 9.5   + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  781.0f, 3001.0f, float( 9.5   + 6.75 * PI)); //spécial passage en aérien
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  781.0f, 3001.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  781.0f, 3021.0f, float(15.001 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  781.0f, 3021.0f, float( 9.5   + 6.75 * PI));

    for (int c = 32; c > 16; c--) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 +  69*cos(c      *PI/32)), float(3096.0 +  69*sin(c      *PI/32)),  float( 9.7 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 +  69*cos(c      *PI/32)), float(3096.0 +  69*sin(c      *PI/32)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 +  69*cos((c - 1)*PI/32)), float(3096.0 +  69*sin((c - 1)*PI/32)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 +  69*cos((c - 1)*PI/32)), float(3096.0 +  69*sin((c - 1)*PI/32)),  float( 9.7 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 +  81*cos(c      *PI/32)), float(3096.0 +  81*sin(c      *PI/32)),  float( 9.7 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 +  81*cos(c      *PI/32)), float(3096.0 +  81*sin(c      *PI/32)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 +  81*cos((c - 1)*PI/32)), float(3096.0 +  81*sin((c - 1)*PI/32)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 +  81*cos((c - 1)*PI/32)), float(3096.0 +  81*sin((c - 1)*PI/32)),  float( 9.7 + 6.75 * PI));

    }

    for (int d = 0; d < 8; d++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 12.5*d      ), 3165.0f, float( 9.9 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 12.5*d      ), 3165.0f, float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 12.5*(d + 1)), 3165.0f, float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 12.5*(d + 1)), 3165.0f, float( 9.9 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(850.0 + 12.5*d      ), 3177.0f, float( 9.9 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(850.0 + 12.5*d      ), 3177.0f, float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(850.0 + 12.5*(d + 1)), 3177.0f, float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(850.0 + 12.5*(d + 1)), 3177.0f, float( 9.9 + 6.75 * PI)); //n'est pas une courbe mais une ligne droite

    }

    for (int c = -36; c < -12; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(950.0 +  94*cos(c      *PI/ 72)), float(3271.0 +  94*sin(c      *PI/ 72)),  float( 9.7 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(950.0 +  94*cos(c      *PI/ 72)), float(3271.0 +  94*sin(c      *PI/ 72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(950.0 +  94*cos((c + 1)*PI/ 72)), float(3271.0 +  94*sin((c + 1)*PI/ 72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(950.0 +  94*cos((c + 1)*PI/ 72)), float(3271.0 +  94*sin((c + 1)*PI/ 72)),  float( 9.7 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(950.0 + 106*cos(c      *PI/ 72)), float(3271.0 + 106*sin(c      *PI/ 72)),  float( 9.7 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(950.0 + 106*cos(c      *PI/ 72)), float(3271.0 + 106*sin(c      *PI/ 72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(950.0 + 106*cos((c + 1)*PI/ 72)), float(3271.0 + 106*sin((c + 1)*PI/ 72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(950.0 + 106*cos((c + 1)*PI/ 72)), float(3271.0 + 106*sin((c + 1)*PI/ 72)),  float( 9.7 + 6.75 * PI));

        //attention la courbe est tracée dans le mauvais sens

    }

    for (int c = 36; c < 60; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(1123.205081 +  94*cos(c      *PI/72)), float(3171.0 +  94*sin(c      *PI/72)),  float( 9.7 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(1123.205081 +  94*cos(c      *PI/72)), float(3171.0 +  94*sin(c      *PI/72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(1123.205081 +  94*cos((c + 1)*PI/72)), float(3171.0 +  94*sin((c + 1)*PI/72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(1123.205081 +  94*cos((c + 1)*PI/72)), float(3171.0 +  94*sin((c + 1)*PI/72)),  float( 9.7 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(1123.205081 + 106*cos(c      *PI/72)), float(3171.0 + 106*sin(c      *PI/72)),  float( 9.7 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(1123.205081 + 106*cos(c      *PI/72)), float(3171.0 + 106*sin(c      *PI/72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(1123.205081 + 106*cos((c + 1)*PI/72)), float(3171.0 + 106*sin((c + 1)*PI/72)),  float(15.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(1123.205081 + 106*cos((c + 1)*PI/72)), float(3171.0 + 106*sin((c + 1)*PI/72)),  float( 9.7 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1123.205081f, 3265.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1123.205081f, 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f     , 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f     , 3265.0f, float( 9.9 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1123.205081f, 3277.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1123.205081f, 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1140.0f     , 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1140.0f     , 3277.0f, float( 9.9 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3265.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1227.5f, 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1227.5f, 3265.0f, float( 9.9 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1215.0f, 3277.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1215.0f, 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1227.5f, 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1227.5f, 3277.0f, float( 9.9 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1227.5f, 3265.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1227.5f, 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1240.0f, 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1240.0f, 3265.0f, float( 9.9 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1227.5f, 3277.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1227.5f, 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1240.0f, 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1240.0f, 3277.0f, float( 9.9 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1240.0f, 3265.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1240.0f, 3265.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1250.0f, 3265.0f, float(14.9 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1250.0f, 3265.0f, float( 9.8 + 6.75 * PI)); //pente de 1%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1240.0f, 3277.0f, float( 9.9 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1240.0f, 3277.0f, float(15.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1250.0f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1250.0f, 3277.0f, float( 9.8 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1250.0f, 3265.0f, float( 9.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1250.0f, 3265.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1260.0f, 3265.0f, float(14.9 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1260.0f, 3265.0f, float( 9.7 + 6.75 * PI)); //pente de 1%

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1250.0f, 3277.0f, float( 9.8 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1250.0f, 3277.0f, float(15.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1260.0f, 3277.0f, float(14.9 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1260.0f, 3277.0f, float( 9.7 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1713.333333f, 3265.0f,  float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1713.333333f, 3265.0f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f     , 3265.0f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f     , 3265.0f,  float(-0.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1713.333333f, 3277.0f,  float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1713.333333f, 3277.0f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1725.0f     , 3277.0f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1725.0f     , 3277.0f,  float(-0.1 + 6.75 * PI));


    for (int compteur = 0; compteur < 18; compteur++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(1990.0 + 10 * compteur), 3271.0f,  float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(1990.0 + 10 * compteur), 3271.0f,  float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2000.0 + 10 * compteur), 3271.0f,  float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2000.0 + 10 * compteur), 3271.0f,  float(-0.3 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.0f, 3244.205081f,  float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.0f, 3244.205081f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3244.205081f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3244.205081f,  float(-0.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.0f, 3247.205081f,  float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.0f, 3247.205081f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2250.0f, 3247.205081f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2250.0f, 3247.205081f,  float(-0.1 + 6.75 * PI));

    for (int c = 24; c < 36; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2000.0 +  94*cos(c      *PI/72)), float(3171.0 +  94*sin(c      *PI/72)),   float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2000.0 +  94*cos(c      *PI/72)), float(3171.0 +  94*sin(c      *PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2000.0 +  94*cos((c + 1)*PI/72)), float(3171.0 +  94*sin((c + 1)*PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2000.0 +  94*cos((c + 1)*PI/72)), float(3171.0 +  94*sin((c + 1)*PI/72)),   float(-0.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2000.0 + 100*cos(c      *PI/72)), float(3171.0 + 100*sin(c      *PI/72)),   float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2000.0 + 100*cos(c      *PI/72)), float(3171.0 + 100*sin(c      *PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2000.0 + 100*cos((c + 1)*PI/72)), float(3171.0 + 100*sin((c + 1)*PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2000.0 + 100*cos((c + 1)*PI/72)), float(3171.0 + 100*sin((c + 1)*PI/72)),   float(-0.3 + 6.75 * PI)); //voie unique

    }

    for (int c = -48; c < -36; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2100.0 + 100*cos(c      *PI/72)), float(3344.205081 + 100*sin(c      *PI/72)),   float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2100.0 + 100*cos(c      *PI/72)), float(3344.205081 + 100*sin(c      *PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2100.0 + 100*cos((c + 1)*PI/72)), float(3344.205081 + 100*sin((c + 1)*PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2100.0 + 100*cos((c + 1)*PI/72)), float(3344.205081 + 100*sin((c + 1)*PI/72)),   float(-0.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2100.0 + 106*cos(c      *PI/72)), float(3344.205081 + 106*sin(c      *PI/72)),   float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2100.0 + 106*cos(c      *PI/72)), float(3344.205081 + 106*sin(c      *PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2100.0 + 106*cos((c + 1)*PI/72)), float(3344.205081 + 106*sin((c + 1)*PI/72)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2100.0 + 106*cos((c + 1)*PI/72)), float(3344.205081 + 106*sin((c + 1)*PI/72)),   float(-0.3 + 6.75 * PI));//voie unique

    }

    for (int c = 0; c < 6; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2100.0 + 12 * c), 3244.205081f,  float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2100.0 + 12 * c), 3244.205081f,  float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2112.0 + 12 * c), 3244.205081f,  float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2112.0 + 12 * c), 3244.205081f,  float(-0.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f( float(2100.0 + 12 * c), 3247.205081f,  float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f( float(2100.0 + 12 * c), 3247.205081f,  float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( float(2112.0 + 12 * c), 3247.205081f,  float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f( float(2112.0 + 12 * c), 3247.205081f,  float(-0.3 + 6.75 * PI));

    }

    for (int c = 12; c < 18; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 57*cos(c      *PI/36)), float(3181.205081 + 57*sin(c      *PI/36)),  float(-0.3 + (-18 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 57*cos(c      *PI/36)), float(3181.205081 + 57*sin(c      *PI/36)),  float( 5.1 + 6.75 * PI)); // (-18 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 57*cos((c + 1)*PI/36)), float(3181.205081 + 57*sin((c + 1)*PI/36)),  float( 5.1 + 6.75 * PI)); // (-17 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 57*cos((c + 1)*PI/36)), float(3181.205081 + 57*sin((c + 1)*PI/36)),  float(-0.3 + (-17 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));

    }

    for (int c = 12; c < 14; c++) { //on enlève les 4 premiers pans de mur à cause de la sortie de Bourse-arrivée

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2325.0 + 63*cos(c      *PI/36)), float(3181.205081 + 63*sin(c      *PI/36)),  float(-0.3 + (-18 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2325.0 + 63*cos(c      *PI/36)), float(3181.205081 + 63*sin(c      *PI/36)),  float( 5.1 + 6.75 * PI)); // (-18 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2325.0 + 63*cos((c + 1)*PI/36)), float(3181.205081 + 63*sin((c + 1)*PI/36)),  float( 5.1 + 6.75 * PI)); // (-17 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2325.0 + 63*cos((c + 1)*PI/36)), float(3181.205081 + 63*sin((c + 1)*PI/36)),  float(-0.3 + (-17 + c) * 0.05 * 60 * PI/36 + 6.75 * PI));

    }

    for (int c = -24; c < -9; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/36)), float(3285.128129 + 57*sin(c      *PI/36)),  float(-0.3 + (-24 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/36)), float(3285.128129 + 57*sin(c      *PI/36)),  float( 5.1 + (-24 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/36)), float(3285.128129 + 57*sin((c + 1)*PI/36)),  float( 5.1 + (-25 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/36)), float(3285.128129 + 57*sin((c + 1)*PI/36)),  float(-0.3 + (-25 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/36)), float(3285.128129 + 63*sin(c      *PI/36)),  float(-0.3 + (-24 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/36)), float(3285.128129 + 63*sin(c      *PI/36)),  float( 5.1 + (-24 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/36)), float(3285.128129 + 63*sin((c + 1)*PI/36)),  float( 5.1 + (-25 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/36)), float(3285.128129 + 63*sin((c + 1)*PI/36)),  float(-0.3 + (-25 - c) * 0.05 * 60 * PI/36 + 6.25 * PI));

    }

    for (int c = -9; c < 9; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/36)), float(3285.128129 + 57*sin(c      *PI/36)),  float(-0.3 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/36)), float(3285.128129 + 57*sin(c      *PI/36)),  float( 5.1 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/36)), float(3285.128129 + 57*sin((c + 1)*PI/36)),  float( 5.1 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/36)), float(3285.128129 + 57*sin((c + 1)*PI/36)),  float(-0.3 + 5.00 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/36)), float(3285.128129 + 63*sin(c      *PI/36)),  float(-0.3 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/36)), float(3285.128129 + 63*sin(c      *PI/36)),  float( 5.1 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/36)), float(3285.128129 + 63*sin((c + 1)*PI/36)),  float( 5.1 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/36)), float(3285.128129 + 63*sin((c + 1)*PI/36)),  float(-0.3 + 5.00 * PI));

    }

    for (int c = 9; c < 30; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/36)), float(3285.128129 + 57*sin(c      *PI/36)),  float(-0.3 + (-9 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos(c      *PI/36)), float(3285.128129 + 57*sin(c      *PI/36)),  float( 5.1 + (-9 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/36)), float(3285.128129 + 57*sin((c + 1)*PI/36)),  float( 5.1 + (-8 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 57*cos((c + 1)*PI/36)), float(3285.128129 + 57*sin((c + 1)*PI/36)),  float(-0.3 + (-8 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/36)), float(3285.128129 + 63*sin(c      *PI/36)),  float(-0.3 + (-9 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos(c      *PI/36)), float(3285.128129 + 63*sin(c      *PI/36)),  float( 5.1 + (-9 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/36)), float(3285.128129 + 63*sin((c + 1)*PI/36)),  float( 5.1 + (-8 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2385.0 + 63*cos((c + 1)*PI/36)), float(3285.128129 + 63*sin((c + 1)*PI/36)),  float(-0.3 + (-8 + c) * 0.05 * 60 * PI/36 + 5.00 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2330.435387f, 3316.628129f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2324.017898f, 3305.5f     , float(-0.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2324.017898f, 3305.5f     , float( 5.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2330.435387f, 3316.628129f, float( 5.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2329.209818f, 3302.5f     , float(-0.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2335.636844f, 3313.628129f, float(-0.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2335.636844f, 3313.628129f, float( 5.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2329.209818f, 3302.5f     , float( 5.1 + 6.75 * PI));

    for (int c = -18; c < -6; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 57*cos(c      *PI/36)), float(3334.0 + 57*sin(c      *PI/36)),   float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 57*cos(c      *PI/36)), float(3334.0 + 57*sin(c      *PI/36)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 57*cos((c + 1)*PI/36)), float(3334.0 + 57*sin((c + 1)*PI/36)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 57*cos((c + 1)*PI/36)), float(3334.0 + 57*sin((c + 1)*PI/36)),   float(-0.3 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(  float(2274.652123 + 63*cos(c      *PI/36)), float(3334.0 + 63*sin(c      *PI/36)),   float(-0.3 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  float(2274.652123 + 63*cos(c      *PI/36)), float(3334.0 + 63*sin(c      *PI/36)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(  float(2274.652123 + 63*cos((c + 1)*PI/36)), float(3334.0 + 63*sin((c + 1)*PI/36)),   float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(  float(2274.652123 + 63*cos((c + 1)*PI/36)), float(3334.0 + 63*sin((c + 1)*PI/36)),   float(-0.3 + 6.75 * PI));

    }

    for (int c = 0; c < 30; c++) {

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(2350.0 + 10*c), 3247.205081f, float(-0.2 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(2350.0 + 10*c), 3247.205081f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(2360.0 + 10*c), 3247.205081f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(2360.0 + 10*c), 3247.205081f, float(-0.2 + 6.75 * PI)); //décalage lié à la sortie Bourse-arrivée (configuration particulière)

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(2325.0 + 10*c), 3253.205081f, float(-0.2 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(2325.0 + 10*c), 3253.205081f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(2335.0 + 10*c), 3253.205081f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(2335.0 + 10*c), 3253.205081f, float(-0.2 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(2274.652123 + 10*c), 3265.0f, float(-0.2 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(2274.652123 + 10*c), 3265.0f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(2284.652123 + 10*c), 3265.0f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(2284.652123 + 10*c), 3265.0f, float(-0.2 + 6.75 * PI));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(2274.652123 + 10*c), 3271.0f, float(-0.2 + 6.75 * PI));
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(2274.652123 + 10*c), 3271.0f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(2284.652123 + 10*c), 3271.0f, float( 5.1 + 6.75 * PI));
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(2284.652123 + 10*c), 3271.0f, float(-0.2 + 6.75 * PI));

    }

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2090.0f, 3265.0f,  float(-0.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2090.0f, 3265.0f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2090.0f, 3271.0f,  float( 5.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2090.0f, 3271.0f,  float(-0.1 + 6.75 * PI));

    glEnd();

}

void maFenetre::ajoutTIV(int valeur, double xCen, double yCen, double zCen, double angle, bool fermetureDroite, bool fermetureGauche, bool fermetureHaut) {

    int numeroTexture = (valeur - 10) / 5;
    angle *= PI / 180;
    double sinAngle = sin(angle), cosAngle = cos(angle);

    glBindTexture(GL_TEXTURE_2D, textureTIV[numeroTexture]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle, yCen - 0.5 * cosAngle, zCen - 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle, yCen - 0.5 * cosAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle, yCen + 0.5 * cosAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle, yCen + 0.5 * cosAngle, zCen - 0.5);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture2[4]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen - 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen - 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    if (fermetureHaut) {

        glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);

    }

    if (fermetureGauche) {

        glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen - 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    }

    if (fermetureDroite) {

        glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen - 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    }

    glEnd();

}

void maFenetre::ajoutFeu(int valeur, double xCen, double yCen, double zCen, double angle, bool fermetureDroite, bool fermetureGauche, bool fermetureHaut) {

    //valeur vaut 1 pour vert, 2 pour jaune, 3 pour rouge
    angle *= PI / 180;
    double sinAngle = sin(angle), cosAngle = cos(angle);

    glBindTexture(GL_TEXTURE_2D, texture5[valeur - 1]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle, yCen - 0.5 * cosAngle, zCen - 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle, yCen - 0.5 * cosAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle, yCen + 0.5 * cosAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle, yCen + 0.5 * cosAngle, zCen - 0.5);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture2[4]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen - 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen - 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    if (fermetureHaut) {

        glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);

    }

    if (fermetureGauche) {

        glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen - 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle                 , yCen - 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen - 0.5 * sinAngle - 0.1 * cosAngle, yCen - 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    }

    if (fermetureDroite) {

        glTexCoord2f(0.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen - 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle                 , yCen + 0.5 * cosAngle                 , zCen + 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen + 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3d( xCen + 0.5 * sinAngle - 0.1 * cosAngle, yCen + 0.5 * cosAngle + 0.1 * sinAngle, zCen - 0.5);

    }

    glEnd();

}

void maFenetre::afficherTIV() {

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[1]);

    glBegin(GL_QUADS);

    //TIV 15

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2334.621976f,  3238.401688f,  float(2.5 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2334.621976f,  3238.401688f,  float(3.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2334.456080f,  3237.415245f,  float(3.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2334.456080f,  3237.415245f,  float(2.5 + 6.75 * PI));

    glEnd();*/

    ajoutTIV(15, 2334.539028, 3237.908467, 3 + 6.75 * PI, 189.549297);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[2]);

    glBegin(GL_QUADS);

    //TIV 20
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  110.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  110.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  110.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  110.0f,  3.0f);



    glEnd();*/

    ajoutTIV(20, 5.5, 110, 3.5, 90);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[3]);

    glBegin(GL_QUADS);

    //TIV 25

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -695.204581f, -353.795419f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -695.204581f, -353.795419f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -695.911688f, -353.088312f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -695.911688f, -353.088312f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2182.0f,  3239.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2182.0f,  3239.205081f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2182.0f,  3238.205081f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2182.0f,  3238.205081f,  float(3.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -3.5f,  275.0f,   4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -3.5f,  275.0f,   5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.5f,  275.0f,   5.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.5f,  275.0f,   4.0f);



    glEnd();

    */

    ajoutTIV(25, -695.558135, -353.441866, 8.5, 315, true, true, false);
    ajoutTIV(25, 2182, 3238.705081, 3.5 + 6.75 * PI, 180);
    ajoutTIV(25, -2.5, 275, 4.5, 270, true, true, false);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[4]);

    glBegin(GL_QUADS);

    //TIV 30

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  235.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  235.0f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -3.0f,  235.0f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -3.0f,  235.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.727267f,  3106.046174f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.727267f,  3106.046174f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.724638f,  3105.973710f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.724638f,  3105.973710f,  float(13.0 + 6.75 * PI));



    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2047.5f,  3257.602540f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2047.5f,  3257.602540f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2047.0f,  3256.736515f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2047.0f,  3256.736515f,  float(3.0 + 6.75 * PI)); //même limite que la precédente

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2047.5f,  3253.272413f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2047.5f,  3253.272413f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2047.0f,  3252.406388f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2047.0f,  3252.406388f,  float(3.0 + 6.75 * PI)); //la limitation prend effet 10m avant

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2447.0f,  3285.128129f,  float(3.0 + 5.00 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2447.0f,  3285.128129f,  float(4.0 + 5.00 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2448.0f,  3285.128129f,  float(4.0 + 5.00 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2448.0f,  3285.128129f,  float(3.0 + 5.00 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -574.0f, -404.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -574.0f, -404.0f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -573.0f, -404.0f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -573.0f, -404.0f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -716.0f, -441.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -716.0f, -441.0f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -717.0f, -441.0f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -717.0f, -441.0f, 8.0f);

    glEnd();*/

    ajoutTIV(30, -3.5, 235, 4.5, 90, true, true, false);
    //ajoutTIV(30, 781.225953, 3106.009942, 13.5 + 6.75 * PI, 98.243997, true); //douteux... plutôt 94.156 ?
    ajoutTIV(30, 781.218184, 3105.965531, 13.5 + 6.75 * PI, 98.243997, true);
    ajoutTIV(30, 2047.25, 3257.169528, 3.5 + 6.75 * PI, 210, true, false);
    ajoutTIV(30, 2047.25, 3252.839401, 3.5 + 6.75 * PI, 210);
    ajoutTIV(30, 2447.5, 3285.128129, 3.5 + 5 * PI, 90, true);
    ajoutTIV(30, -573.5, -404, 8.5, 90, true, true, false);
    ajoutTIV(30, -716.5, -441, 8.5, 270, true, true, false);

    glBindTexture(GL_TEXTURE_2D, textureTIV[5]);

    glBegin(GL_QUADS);

    //TIV 35

    /*

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.0f,  -515.0f,  7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -230.0f,  -515.0f,  8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -230.0f,  -516.0f,  8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.0f,  -516.0f,  7.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  110.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  110.0f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -3.0f,  110.0f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -3.0f,  110.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  10.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  10.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  10.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  10.0f,  3.0f); //ancien panneau de début de ligne, actuellement dans la voie de garage de République (avant le 20)

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.0f,  3166.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.0f,  3165.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.0f,  3165.0f,  float(13.0 + 6.75 * PI)); //ne pas oublier que le panneau est 10m plus loin!

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2000.0f,  3266.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2000.0f,  3266.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2000.0f,  3265.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2000.0f,  3265.0f,  float(3.0 + 6.75 * PI)); //la limitation prend effet 10m avant

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.0f,  3276.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1130.0f,  3276.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1130.0f,  3277.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.0f,  3277.0f,  float(13.0 + 6.75 * PI));*/

    glColor3d(0.7 + 0.3 * bleuCiel(), 0.55 + 0.45 * bleuCiel(), 0.1 + 0.9 * bleuCiel());

    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2960.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  2960.0f,  float(14.0 + 6.75 * PI));
    glColor3d(bleuCiel(), bleuCiel(), bleuCiel());
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  2960.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2960.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  2682.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2682.0f,  float(13.0 + 6.75 * PI));
    glColor3d(0.7 + 0.3 * bleuCiel(), 0.55 + 0.45 * bleuCiel(), 0.1 + 0.9 * bleuCiel());
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2682.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  2682.0f,  float(14.0 + 6.75 * PI));


    glColor3d(1, 1, 1);

    /*

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -149.362852f,  -496.734171f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -149.362852f,  -496.734171f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -149.745535f,  -495.810291f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -149.745535f,  -495.810291f,  8.0f);

    */

    glEnd();

    ajoutTIV(35, -230, -515.5, 7.5, 180);
    ajoutTIV(35, -3.5, 110, 4.5, 90, true, true, false);
    ajoutTIV(35, 5.5, 10, 3.5, 90);
    ajoutTIV(35, 935, 3165.5, 13.5 + 6.75 * PI, 180);
    ajoutTIV(35, 2000, 3265.5, 3.5 + 6.75 * PI, 180);
    ajoutTIV(35, 1130, 3276.5, 13.5 + 6.75 * PI, 0);

    ajoutTIV(35, -149.554194, -496.272231, 8.5, 337.5, true, true, false);

    glBindTexture(GL_TEXTURE_2D, textureTIV[6]);

    glBegin(GL_QUADS);

    //TIV 40
/*
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1435.0f,  -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1435.0f,  -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  1435.0f,  -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  1435.0f,  -3.0f);*/

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2631.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2631.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  2631.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  2631.0f,  float(13.0 + 6.75 * PI)); //correspond à la partie aérienne

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  3011.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  3011.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  3011.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  3011.0f,  float(13.0 + 6.75 * PI)); //correspond à la partie aérienne

    /*

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 32.824856f,  1531.317280f,  -2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 32.824856f,  1531.317280f,  -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 32.117749f,  1532.024387f,  -1.9f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  0.0f,   0.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  0.0f,   1.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  0.0f,   1.6f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  0.0f,   0.6f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -103.469517f,  -462.530483f,   7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -103.469517f,  -462.530483f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -104.176624f,  -461.823376f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -104.176624f,  -461.823376f,   7.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -470.0f,  -508.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -470.0f,  -508.0f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -470.0f,  -507.0f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.0f,  -507.0f,   8.0f);*/


    glEnd();

    ajoutTIV(40, 5.5, 1435, -2.5, 90);
    ajoutTIV(40, 32.471303, 1531.670834, -2.4, 315);
    ajoutTIV(40, -14.5, 0, 1.1, 270);
    ajoutTIV(40, -103.823071, -462.176930, 7.5, 315);
    ajoutTIV(40, -470, -507.5, 8.5, 0, true, true, false);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[7]);

    glBegin(GL_QUADS);

    //TIV 45

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -96.398449f,  -469.601551f,   7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -96.398449f,  -469.601551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -95.691342f,  -470.308658f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -95.691342f,  -470.308658f,   7.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.0f,  3266.0f,   float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1810.0f,  3266.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1810.0f,  3265.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.0f,  3265.0f,   float(3.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2189.652123f,  3276.0f,   float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2189.652123f,  3276.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2189.652123f,  3277.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2189.652123f,  3277.0f,   float(3.0 + 6.75 * PI));

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  665.0f,   3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  665.0f,   4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  665.0f,   4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  665.0f,   3.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  140.0f,   3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  140.0f,   4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  140.0f,   4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  140.0f,   3.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f,  -310.0f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f,  -310.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -11.0f,  -310.0f,   8.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -11.0f,  -310.0f,   7.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -340.0f,  -505.0f,   7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.0f,  -505.0f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.0f,  -504.0f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -340.0f,  -504.0f,   7.0f);

    glEnd(); */

    ajoutTIV(45, -96.044896, -469.955105, 7.5, 135);
    ajoutTIV(45, 1810, 3265.5, 3.5 + 6.75 * PI, 180);
    ajoutTIV(45, 2189.652123, 3276.5, 3.5 + 6.75 * PI, 0);
    ajoutTIV(45, -5.5, 665, 3.5, 270);
    ajoutTIV(45, -14.5, 140, 3.5, 270);
    ajoutTIV(45, -11.5, -310, 7.5, 270);
    ajoutTIV(45, -340, -504.5, 7.5, 0);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[8]);

    glBegin(GL_QUADS);


    //TIV 50    

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -46.398449f,  -419.601551f,   7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -46.398449f,  -419.601551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -45.691342f,  -420.308658f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -45.691342f,  -420.308658f,   7.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 92.928933f,  1577.279221f,  -2.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 92.928933f,  1577.279221f,  -1.15f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 93.636039f,  1576.572113f,  -1.15f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 93.636039f,  1576.572113f,  -2.15f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1900.0f,  3276.0f,   float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1900.0f,  3276.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1900.0f,  3277.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1900.0f,  3277.0f,   float(3.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 297.989898f, 1796.482323f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 297.989898f, 1796.482323f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 297.282791f, 1797.189430f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 297.282791f, 1797.189430f, 0.0f);

    glEnd();

    */

    ajoutTIV(50, -46.044896, -419.955105, 7.5, 135);
    ajoutTIV(50, 93.282486, 1576.925667, -1.65, 135);
    ajoutTIV(50, 1900, 3276.5, 3.5 + 6.75 * PI, 0);
    ajoutTIV(50, 297.636345, 1796.835877, 0.5, 315);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[9]);

    glBegin(GL_QUADS);

    //TIV 55


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  335.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  335.0f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  335.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  335.0f,  3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.0f,  3266.0f,  float(10.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1390.0f,  3266.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1390.0f,  3265.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3265.0f,  float(10.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2095.0f,  3276.0f,   float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2095.0f,  3276.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2095.0f,  3277.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2095.0f,  3277.0f,   float(3.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 230.814754f,  1729.307178f,  -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 230.814754f,  1729.307178f,   0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 230.107647f,  1730.014285f,   0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 230.107647f,  1730.014285f,  -0.1f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -552.0f,  -512.0f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -552.0f,  -512.0f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -552.0f,  -513.0f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -552.0f,  -513.0f,  8.0f);

    glEnd();

    */

    ajoutTIV(55, 5.5, 335, 3.5, 90);
    ajoutTIV(55, 1390, 3265.5, 10.95 + 6.75 * PI, 180);
    ajoutTIV(55, 2095, 3276.5, 3.5 + 6.75 * PI, 0);
    ajoutTIV(55, 230.461201, 1729.660732, 0.4, 315);
    ajoutTIV(55, -552, -512.5, 8.5, 180, true, true, false);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[10]);

    glBegin(GL_QUADS);

    //TIV 60


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -225.0f, 7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -225.0f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -225.0f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -225.0f, 7.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f, 1240.0f, -1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f, 1240.0f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 1240.0f, -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 1240.0f, -1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.0f,  3266.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1225.0f,  3266.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1225.0f,  3265.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.0f,  3265.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.0f,  3276.0f,  float(3.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1715.0f,  3276.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1715.0f,  3277.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.0f,  3277.0f,  float(3.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.925151f,  2536.000936f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.925151f,  2536.000936f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 768.925263f,  2536.015883f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 768.925263f,  2536.015883f,  float(13.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -130.0f, 0.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, -130.0f, 1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -13.0f, -130.0f, 1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -13.0f, -130.0f, 0.9f);

    glEnd();*/

    ajoutTIV(60, -0.5, -225, 7.5, 90);
    ajoutTIV(60, 5.5, 1240, -1.4, 90);
    ajoutTIV(60, 1225, 3265.5, 13.5 + 6.75 * PI, 180);
    ajoutTIV(60, 1715, 3276.5, 3.6 + 6.75 * PI, 0);
    ajoutTIV(60, 769.425206, 2536.008420, 13.5 + 6.75 * PI, 270.857);
    ajoutTIV(60, -12.5, -130, 1.4, 270, true, true, true);

/*
    glBindTexture(GL_TEXTURE_2D, textureTIV[11]);



    glBegin(GL_QUADS);

    //TIV 65


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  760.0f,  2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  760.0f,  3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  760.0f,  3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  760.0f,  2.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 718.999286f,  2289.617611f,  float(14.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 718.999286f,  2289.617611f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 718.075407f,  2290.000295f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 718.075407f,  2290.000295f,  float(14.5 + 3.375 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 950.0f,   1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 950.0f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 950.0f,   2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 950.0f,   1.0f);

    glEnd();*/

    ajoutTIV(65, 5.5, 760, 3.4, 90);
    ajoutTIV(65, 718.537347, 2289.808953, 15 + 3.375 * PI, 292.5, true);
    ajoutTIV(65, -5.5, 950, 1.5, 270);

    /*

    glBindTexture(GL_TEXTURE_2D, textureTIV[12]);

    glBegin(GL_QUADS);

    //TIV 70
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 305.060967f,  1789.411255f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 305.060967f,  1789.411255f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 305.768074f,  1788.704148f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 305.768074f,  1788.704148f,   0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 573.761543f, 2072.253968f, 14.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 573.761543f, 2072.253968f, 15.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 573.054437f, 2072.961074f, 15.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 573.054437f, 2072.961074f, 14.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 1340.0f,  -2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 1340.0f,  -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1340.0f,  -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1340.0f,  -2.9f);

    glEnd();

    */

    ajoutTIV(70, -5.5, 1340, -2.4, 270);
    ajoutTIV(70, 305.414521, 1789.157702, 0.5, 135);
    ajoutTIV(70, 573.407990, 2072.607521, 15, 315);

}

void maFenetre::afficherFeux() {

    glBindTexture(GL_TEXTURE_2D, texture5[0]);

    glBegin(GL_QUADS);

    const double dAller[100] = {332.,
                                356 + 153 * PI/4,
                                356 + 153 * PI/4 + 50 * V2,
                                441 + 306 * PI/4 + 50 * V2,
                                611 + 306 * PI/4 + 50 * V2,
                                776 + 306 * PI/4 + 50 * V2,
                                806 + 306 * PI/4 + 50 * V2,
                                901 + 306 * PI/4 + 50 * V2,
                                1001 + 306 * PI/4 + 50 * V2,
                                1331 + 306 * PI/4 + 50 * V2,
                                1426 + 306 * PI/4 + 50 * V2,
                                1666 + 306 * PI/4 + 50 * V2,
                                1906 + 306 * PI/4 + 50 * V2,
                                2006 + 306 * PI/4 + 50 * V2,
                                2101 + 306 * PI/4 + 50 * V2,
                                2201 + 403 * PI/4 + 50 * V2,
                                2406 + 403 * PI/4 + 50 * V2,
                                2501 + 403 * PI/4 + 50 * V2,
                                2696 + 403 * PI/4 + 50 * V2,
                                2891 + 403 * PI/4 + 50 * V2,
                                2886 + 742 * PI/4 + 50 * V2,
                                2881 + 1081 * PI/4 + 50 * V2,
                                2976 + 1081 * PI/4 + 50 * V2,
                                3166 + 1081 * PI/4 + 50 * V2,
                                3356 + 1081 * PI/4 + 50 * V2,
                                3451 + 1081 * PI/4 + 50 * V2,
                                3536 + 1225 * PI/4 + 50 * V2,
                                3551 + 1225 * PI/4 + 103 * PI/3 + 50 * V2,
                                3548 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                3643 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                3808 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                3973 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                4133 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                4228 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                4418 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                4418 + 1225 * PI/4 + 248.5 * PI/3 + 50 * V2,
                                4500 + 1225 * PI/4 + 300 * PI/3 + 50 * V2,
                                4558 + 1225 * PI/4 + 300 * PI/3 + 50 * V2,
                                4653 + 1225 * PI/4 + 300 * PI/3 + 50 * V2,
                                4653 + 1305 * PI/4 + 300 * PI/3 + 50 * V2,
                                4653 + 1425 * PI/4 + 300 * PI/3 + 50 * V2,
                                4653 + 1545 * PI/4 + 300 * PI/3 + 50 * V2,
                                4665.85 + 1625 * PI/4 + 300 * PI/3 + 50 * V2,
                                4665.85 + 1705 * PI/4 + 300 * PI/3 + 50 * V2
                                };
    const double dRetour[100] = {100010.,
                                 100105.,
                                 100300.,
                                 100390.,
                                 100485.,
                                 100645.,
                                 100810.,
                                 100975.,
                                 101069.35,
                                 101076.35 + 100 * PI/3,
                                 101076.35 + 200 * PI/3,
                                 101176.35 + 200 * PI/3,
                                 101166.35 + 156 * PI/4 + 200 * PI/3,
                                 101261.35 + 156 * PI/4 + 200 * PI/3,
                                 101451.35 + 156 * PI/4 + 200 * PI/3,
                                 101641.35 + 156 * PI/4 + 200 * PI/3,
                                 101736.35 + 156 * PI/4 + 200 * PI/3,
                                 101726.35 + 490.5 * PI/4 + 200 * PI/3,
                                 101726.35 + 825 * PI/4 + 200 * PI/3,
                                 102116.35 + 825 * PI/4 + 200 * PI/3,
                                 102211.35 + 825 * PI/4 + 200 * PI/3,
                                 102356.35 + 825 * PI/4 + 200 * PI/3,
                                 102501.35 + 825 * PI/4 + 200 * PI/3,
                                 102516.35 + 931 * PI/4 + 200 * PI/3,
                                 102611.35 + 931 * PI/4 + 200 * PI/3,
                                 102801.35 + 931 * PI/4 + 200 * PI/3,
                                 103001.35 + 931 * PI/4 + 200 * PI/3,
                                 103191.35 + 931 * PI/4 + 200 * PI/3,
                                 103286.35 + 931 * PI/4 + 200 * PI/3,
                                 103676.35 + 931 * PI/4 + 200 * PI/3,
                                 103716.35 + 931 * PI/4 + 200 * PI/3,
                                 103811.35 + 931 * PI/4 + 200 * PI/3,
                                 103951.35 + 931 * PI/4 + 200 * PI/3,
                                 104081.35 + 931 * PI/4 + 200 * PI/3,
                                 104261.35 + 931 * PI/4 + 200 * PI/3,
                                 104261.35 + 1078 * PI/4 + 200 * PI/3,
                                 104261.35 + 1078 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104261.35 + 1151.1 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104261.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104285.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104395.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104525.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104525.35 + 1431 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104525.35 + 1647 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104562.35 + 1719 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104655.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2,
                                 104867.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2
                                 };

    if (!ramePresente(dAller[0], dAller[3] + 75)) feuAller[0] = 1;
    if (!ramePresente(dAller[1], dAller[4] + 75)) feuAller[1] = 1;
    if (!ramePresente(dAller[2], dAller[5] + 75)) feuAller[2] = 1;
    if (!ramePresente(dAller[3], dAller[6] + 75)) feuAller[3] = 1;
    if (!ramePresente(dAller[4], dAller[7] + 75)) feuAller[4] = 1;
    if (!ramePresente(dAller[5], dAller[8] + 75)) feuAller[5] = 1;
    if (!ramePresente(dAller[6], dAller[8] + 75)) feuAller[6] = 1;
    if (!ramePresente(dAller[7], dAller[10] + 75)) feuAller[7] = 1;
    if (!ramePresente(dAller[8], dAller[11] + 75)) feuAller[8] = 1;
    if (!ramePresente(dAller[9], dAller[11] + 75)) feuAller[9] = 1;
    if (!ramePresente(dAller[10], dAller[13] + 75)) feuAller[10] = 1;
    if (!ramePresente(dAller[11], dAller[14] + 75)) feuAller[11] = 1;
    if (!ramePresente(dAller[12], dAller[15] + 75)) feuAller[12] = 1;
    if (!ramePresente(dAller[13], dAller[15] + 75)) feuAller[13] = 1;
    if (!ramePresente(dAller[14], dAller[17] + 75)) feuAller[14] = 1;
    if (!ramePresente(dAller[15], dAller[18] + 75)) feuAller[15] = 1;
    if (!ramePresente(dAller[16], dAller[18] + 75)) feuAller[16] = 1;
    if (!ramePresente(dAller[17], dAller[20] + 75)) feuAller[17] = 1;
    if (!ramePresente(dAller[18], dAller[21] + 75)) feuAller[18] = 1;
    if (!ramePresente(dAller[19], dAller[22] + 75)) feuAller[19] = 1;
    if (!ramePresente(dAller[20], dAller[23] + 75)) feuAller[20] = 1;
    if (!ramePresente(dAller[21], dAller[23] + 75)) feuAller[21] = 1;
    if (!ramePresente(dAller[22], dAller[25] + 75)) feuAller[22] = 1;
    if (!ramePresente(dAller[23], dAller[26] + 75)) {

        glColor3d(0.8 + 0.2 * bleuCiel(), 0.65 + 0.35 * bleuCiel(), 0.2 + 0.8 * bleuCiel()); //on triche par rapport à un TIV (+0.1 par composante la nuit) car le fond du feu est gris foncé et non blanc!
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glColor3d(bleuCiel(), bleuCiel(), bleuCiel());
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glColor3d(1, 1, 1);
        feuAller[23] = 1;

    }

    if (!ramePresente(dAller[24], dAller[26] + 75)) feuAller[24] = 1;
    if (!ramePresente(dAller[25], dAller[28] + 75)) feuAller[25] = 1;
    if (!ramePresente(dAller[26], dAller[29] + 75)) feuAller[26] = 1;
    if (!ramePresente(dAller[27], dAller[29] + 75)) feuAller[27] = 1;
    if (!ramePresente(dAller[28], dAller[29] + 75)) feuAller[28] = 1;
    if (!ramePresente(dAller[29], dAller[32] + 75)) feuAller[29] = 1;
    if (!ramePresente(dAller[30], dAller[33] + 75)) feuAller[30] = 1;
    if (!ramePresente(dAller[31], dAller[34] + 75)) feuAller[31] = 1;
    if (!ramePresente(dAller[32], dAller[34] + 75)) feuAller[32] = 1;
    if (!ramePresente(dAller[33], dAller[37] + 75)) feuAller[33] = 1;
    if (!ramePresente(dAller[34], dAller[37] + 75)) feuAller[34] = 1;
    if (!ramePresente(dAller[35], dAller[38] + 75)) feuAller[35] = 1;
    if (!ramePresente(dAller[36], dAller[39] + 75)) feuAller[36] = 1;
    if (!ramePresente(dAller[37], dAller[39] + 75)) feuAller[37] = 1;
    if (!ramePresente(dAller[38], dAller[41] + 75)) feuAller[38] = 1;
    if (!ramePresente(dAller[39], dAller[42] + 75)) feuAller[39] = 1;
    if (!ramePresente(dAller[40], dAller[43] + 75)) feuAller[40] = 1;
    if (!ramePresente(dAller[41], dAller[43] + 75)) feuAller[41] = 1;
    if (!ramePresente(dAller[42], 7000)) feuAller[42] = 1;
    if (!ramePresente(dAller[43], 7000) && !ramePresente(99000, 100010 + 75)) feuAller[43] = 1;

    if (!ramePresente(dRetour[0], dRetour[2] + 75)) feuRetour[0] = 1;
    if (!ramePresente(dRetour[1], dRetour[4] + 75)) feuRetour[1] = 1;
    if (!ramePresente(dRetour[2], dRetour[5] + 75)) feuRetour[2] = 1;
    if (!ramePresente(dRetour[3], dRetour[5] + 75)) feuRetour[3] = 1;
    if (!ramePresente(dRetour[4], dRetour[7] + 75)) feuRetour[4] = 1;
    if (!ramePresente(dRetour[5], dRetour[8] + 75)) feuRetour[5] = 1;
    if (!ramePresente(dRetour[6], dRetour[9] + 75)) feuRetour[6] = 1;
    if (!ramePresente(dRetour[7], dRetour[10] + 75)) feuRetour[7] = 1;
    if (!ramePresente(dRetour[8], dRetour[11] + 75)) feuRetour[8] = 1;
    if (!ramePresente(dRetour[9], dRetour[12] + 75)) feuRetour[9] = 1;
    if (!ramePresente(dRetour[10], dRetour[12] + 75)) feuRetour[10] = 1;
    if (!ramePresente(dRetour[11], dRetour[14] + 75)) feuRetour[11] = 1;
    if (!ramePresente(dRetour[12], dRetour[14] + 75)) feuRetour[12] = 1;
    if (!ramePresente(dRetour[13], dRetour[16] + 75)) feuRetour[13] = 1;
    if (!ramePresente(dRetour[14], dRetour[16] + 75)) {

        glColor3d(0.8 + 0.2 * bleuCiel(), 0.65 + 0.35 * bleuCiel(), 0.2 + 0.8 * bleuCiel());
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glColor3d(bleuCiel(), bleuCiel(), bleuCiel());
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glColor3d(1, 1, 1);
        feuRetour[14] = 1;

    }

    if (!ramePresente(dRetour[15], dRetour[17] + 75)) feuRetour[15] = 1;
    if (!ramePresente(dRetour[16], dRetour[18] + 75)) feuRetour[16] = 1;
    if (!ramePresente(dRetour[17], dRetour[19] + 75)) feuRetour[17] = 1;
    if (!ramePresente(dRetour[18], dRetour[20] + 75)) feuRetour[18] = 1;
    if (!ramePresente(dRetour[19], dRetour[21] + 75)) feuRetour[19] = 1;
    if (!ramePresente(dRetour[20], dRetour[23] + 75)) feuRetour[20] = 1;
    if (!ramePresente(dRetour[21], dRetour[24] + 75)) feuRetour[21] = 1;
    if (!ramePresente(dRetour[22], dRetour[25] + 75)) feuRetour[22] = 1;
    if (!ramePresente(dRetour[23], dRetour[25] + 75)) feuRetour[23] = 1;
    if (!ramePresente(dRetour[24], dRetour[27] + 75)) feuRetour[24] = 1;
    if (!ramePresente(dRetour[25], dRetour[28] + 75)) feuRetour[25] = 1;
    if (!ramePresente(dRetour[26], dRetour[29] + 75)) feuRetour[26] = 1;
    if (!ramePresente(dRetour[27], dRetour[29] + 75)) feuRetour[27] = 1;
    if (!ramePresente(dRetour[28], dRetour[31] + 75)) feuRetour[28] = 1;
    if (!ramePresente(dRetour[29], dRetour[32] + 75)) feuRetour[29] = 1;
    if (!ramePresente(dRetour[30], dRetour[33] + 75)) feuRetour[30] = 1;
    if (!ramePresente(dRetour[31], dRetour[34] + 75)) feuRetour[31] = 1;
    if (!ramePresente(dRetour[32], dRetour[35] + 75)) feuRetour[32] = 1;
    if (!ramePresente(dRetour[33], dRetour[36] + 75)) feuRetour[33] = 1;
    if (!ramePresente(dRetour[34], dRetour[37] + 75)) feuRetour[34] = 1;
    if (!ramePresente(dRetour[35], dRetour[37] + 75)) feuRetour[35] = 1;
    if (!ramePresente(dRetour[36], dRetour[39] + 75)) feuRetour[36] = 1;
    if (!ramePresente(dRetour[37], dRetour[41] + 75)) feuRetour[37] = 1;
    if (!ramePresente(dRetour[38], dRetour[41] + 75)) feuRetour[38] = 1;
    if (!ramePresente(dRetour[39], dRetour[41] + 75)) feuRetour[39] = 1;
    if (!ramePresente(dRetour[40], dRetour[43] + 75)) feuRetour[40] = 1;
    if (!ramePresente(dRetour[41], dRetour[44] + 75)) feuRetour[41] = 1;
    if (!ramePresente(dRetour[42], dRetour[45] + 75)) feuRetour[42] = 1;
    if (!ramePresente(dRetour[43], dRetour[46] + 75)) feuRetour[43] = 1;
    if (!ramePresente(dRetour[44], dRetour[46] + 75)) feuRetour[44] = 1;
    if (!ramePresente(dRetour[45], dRetour[46] + 75)) feuRetour[45] = 1;
    if (!ramePresente(232, dAller[0] + 75)) feuRetour[46] = 1;

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture5[1]);

    glBegin(GL_QUADS);

    //feu jaune

    if (!ramePresente(dAller[0], dAller[2] + 75) && ramePresente(dAller[2], dAller[3] + 75)) feuAller[0] = 2;
    if (!ramePresente(dAller[1], dAller[3] + 75) && ramePresente(dAller[3], dAller[4] + 75)) feuAller[1] = 2;
    if (!ramePresente(dAller[2], dAller[4] + 75) && ramePresente(dAller[4], dAller[5] + 75)) feuAller[2] = 2;
    if (!ramePresente(dAller[3], dAller[5] + 75) && ramePresente(dAller[5], dAller[6] + 75)) feuAller[3] = 2;
    if (!ramePresente(dAller[7], dAller[9] + 75) && ramePresente(dAller[9], dAller[10] + 75)) feuAller[7] = 2;
    if (!ramePresente(dAller[8], dAller[10] + 75) && ramePresente(dAller[10], dAller[11] + 75)) feuAller[8] = 2;
    if (!ramePresente(dAller[10], dAller[12] + 75) && ramePresente(dAller[12], dAller[13] + 75)) feuAller[10] = 2;
    if (!ramePresente(dAller[11], dAller[13] + 75) && ramePresente(dAller[13], dAller[14] + 75)) feuAller[11] = 2;
    if (!ramePresente(dAller[12], dAller[14] + 75) && ramePresente(dAller[14], dAller[15] + 75)) feuAller[12] = 2;
    if (!ramePresente(dAller[14], dAller[16] + 75) && ramePresente(dAller[16], dAller[17] + 75)) feuAller[14] = 2;
    if (!ramePresente(dAller[15], dAller[17] + 75) && ramePresente(dAller[17], dAller[18] + 75)) feuAller[15] = 2;
    if (!ramePresente(dAller[17], dAller[19] + 75) && ramePresente(dAller[19], dAller[20] + 75)) feuAller[17] = 2;
    if (!ramePresente(dAller[18], dAller[20] + 75) && ramePresente(dAller[20], dAller[21] + 75)) feuAller[18] = 2;
    if (!ramePresente(dAller[19], dAller[21] + 75) && ramePresente(dAller[21], dAller[22] + 75)) feuAller[19] = 2;
    if (!ramePresente(dAller[20], dAller[22] + 75) && ramePresente(dAller[22], dAller[23] + 75)) feuAller[20] = 2;
    if (!ramePresente(dAller[22], dAller[24] + 75) && ramePresente(dAller[24], dAller[25] + 75)) feuAller[22] = 2;
    if (!ramePresente(dAller[23], dAller[25] + 75) && ramePresente(dAller[25], dAller[26] + 75)) {

        glColor3d(0.8 + 0.2 * bleuCiel(), 0.65 + 0.35 * bleuCiel(), 0.2 + 0.8 * bleuCiel());
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glColor3d(bleuCiel(), bleuCiel(), bleuCiel());
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glColor3d(1, 1, 1);
        feuAller[23] = 2;

    }

    if (!ramePresente(dAller[25], dAller[27] + 75) && ramePresente(dAller[27], dAller[28] + 75)) feuAller[25] = 2;
    if (!ramePresente(dAller[26], dAller[28] + 75) && ramePresente(dAller[28], dAller[29] + 75)) feuAller[26] = 2;
    if (!ramePresente(dAller[29], dAller[31] + 75) && ramePresente(dAller[31], dAller[32] + 75)) feuAller[29] = 2;
    if (!ramePresente(dAller[30], dAller[32] + 75) && ramePresente(dAller[32], dAller[33] + 75)) feuAller[30] = 2;
    if (!ramePresente(dAller[31], dAller[33] + 75) && ramePresente(dAller[33], dAller[34] + 75)) feuAller[31] = 2;
    if (!ramePresente(dAller[33], dAller[36] + 75) && ramePresente(dAller[36], dAller[37] + 75)) feuAller[33] = 2;
    if (!ramePresente(dAller[36], dAller[38] + 75) && ramePresente(dAller[38], dAller[39] + 75)) feuAller[36] = 2;
    if (!ramePresente(dAller[38], dAller[40] + 75) && ramePresente(dAller[40], dAller[41] + 75)) feuAller[38] = 2;
    if (!ramePresente(dAller[39], dAller[41] + 75) && ramePresente(dAller[41], dAller[42] + 75)) feuAller[39] = 2;
    if (!ramePresente(dAller[40], dAller[42] + 75) && ramePresente(dAller[42], dAller[43] + 75)) feuAller[40] = 2;

    if (!ramePresente(dRetour[1], dRetour[3] + 75) && ramePresente(dRetour[3], dRetour[4] + 75)) feuRetour[1] = 2;
    if (!ramePresente(dRetour[2], dRetour[4] + 75) && ramePresente(dRetour[4], dRetour[5] + 75)) feuRetour[2] = 2;
    if (!ramePresente(dRetour[4], dRetour[6] + 75) && ramePresente(dRetour[6], dRetour[7] + 75)) feuRetour[4] = 2;
    if (!ramePresente(dRetour[5], dRetour[7] + 75) && ramePresente(dRetour[7], dRetour[8] + 75)) feuRetour[5] = 2;
    if (!ramePresente(dRetour[6], dRetour[8] + 75) && ramePresente(dRetour[8], dRetour[9] + 75)) feuRetour[6] = 2;
    if (!ramePresente(dRetour[8], dRetour[10] + 75) && ramePresente(dRetour[10], dRetour[11] + 75)) feuRetour[8] = 2;
    if (!ramePresente(dRetour[9], dRetour[11] + 75) && ramePresente(dRetour[11], dRetour[12] + 75)) feuRetour[9] = 2;
    if (!ramePresente(dRetour[11], dRetour[13] + 75) && ramePresente(dRetour[13], dRetour[14] + 75)) feuRetour[11] = 2;
    if (!ramePresente(dRetour[13], dRetour[15] + 75) && ramePresente(dRetour[15], dRetour[16] + 75)) feuRetour[13] = 2;
    if (!ramePresente(dRetour[17], dRetour[18] + 75) && ramePresente(dRetour[18], dRetour[19] + 75)) feuRetour[17] = 2;
    if (!ramePresente(dRetour[18], dRetour[19] + 75) && ramePresente(dRetour[19], dRetour[20] + 75)) feuRetour[18] = 2;
    if (!ramePresente(dRetour[20], dRetour[22] + 75) && ramePresente(dRetour[22], dRetour[23] + 75)) feuRetour[20] = 2;
    if (!ramePresente(dRetour[21], dRetour[23] + 75) && ramePresente(dRetour[23], dRetour[24] + 75)) feuRetour[21] = 2;
    if (!ramePresente(dRetour[22], dRetour[24] + 75) && ramePresente(dRetour[24], dRetour[25] + 75)) feuRetour[22] = 2;
    if (!ramePresente(dRetour[24], dRetour[26] + 75) && ramePresente(dRetour[26], dRetour[27] + 75)) feuRetour[24] = 2;
    if (!ramePresente(dRetour[25], dRetour[27] + 75) && ramePresente(dRetour[27], dRetour[28] + 75)) feuRetour[25] = 2;
    if (!ramePresente(dRetour[26], dRetour[28] + 75) && ramePresente(dRetour[28], dRetour[29] + 75)) feuRetour[26] = 2;
    if (!ramePresente(dRetour[28], dRetour[30] + 75) && ramePresente(dRetour[30], dRetour[31] + 75)) feuRetour[28] = 2;
    if (!ramePresente(dRetour[30], dRetour[32] + 75) && ramePresente(dRetour[32], dRetour[33] + 75)) feuRetour[30] = 2;
    if (!ramePresente(dRetour[31], dRetour[33] + 75) && ramePresente(dRetour[33], dRetour[34] + 75)) feuRetour[31] = 2;
    if (!ramePresente(dRetour[32], dRetour[34] + 75) && ramePresente(dRetour[34], dRetour[35] + 75)) feuRetour[32] = 2;
    if (!ramePresente(dRetour[33], dRetour[35] + 75) && ramePresente(dRetour[35], dRetour[36] + 75)) feuRetour[33] = 2;
    if (!ramePresente(dRetour[34], dRetour[36] + 75) && ramePresente(dRetour[36], dRetour[37] + 75)) feuRetour[34] = 2;
    if (!ramePresente(dRetour[37], dRetour[40] + 75) && ramePresente(dRetour[40], dRetour[41] + 75)) feuRetour[37] = 2;
    if (!ramePresente(dRetour[40], dRetour[42] + 75) && ramePresente(dRetour[42], dRetour[43] + 75)) feuRetour[40] = 2;
    if (!ramePresente(dRetour[41], dRetour[43] + 75) && ramePresente(dRetour[43], dRetour[44] + 75)) feuRetour[41] = 2;
    if (!ramePresente(dRetour[42], dRetour[44] + 75) && ramePresente(dRetour[44], dRetour[45] + 75)) feuRetour[42] = 2;
    if (!ramePresente(dRetour[43], dRetour[45] + 75) && ramePresente(dRetour[45], dRetour[46] + 75)) feuRetour[43] = 2;

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture5[2]);

    glBegin(GL_QUADS);

    //feu rouge

    if (ramePresente(dAller[0], dAller[2] + 75)) feuAller[0] = 3;
    if (ramePresente(dAller[1], dAller[3] + 75)) feuAller[1] = 3;
    if (ramePresente(dAller[2], dAller[4] + 75)) feuAller[2] = 3;
    if (ramePresente(dAller[3], dAller[5] + 75)) feuAller[3] = 3;
    if (ramePresente(dAller[4], dAller[7] + 75)) feuAller[4] = 3;
    if (ramePresente(dAller[5], dAller[8] + 75)) feuAller[5] = 3;
    if (ramePresente(dAller[6], dAller[8] + 75)) feuAller[6] = 3;
    if (ramePresente(dAller[7], dAller[9] + 75)) feuAller[7] = 3;
    if (ramePresente(dAller[8], dAller[10] + 75)) feuAller[8] = 3;
    if (ramePresente(dAller[9], dAller[11] + 75)) feuAller[9] = 3;
    if (ramePresente(dAller[10], dAller[12] + 75)) feuAller[10] = 3;
    if (ramePresente(dAller[11], dAller[13] + 75)) feuAller[11] = 3;
    if (ramePresente(dAller[12], dAller[14] + 75)) feuAller[12] = 3;
    if (ramePresente(dAller[13], dAller[15] + 75)) feuAller[13] = 3;
    if (ramePresente(dAller[14], dAller[16] + 75)) feuAller[14] = 3;
    if (ramePresente(dAller[15], dAller[17] + 75)) feuAller[15] = 3;
    if (ramePresente(dAller[16], dAller[18] + 75)) feuAller[16] = 3;
    if (ramePresente(dAller[17], dAller[19] + 75)) feuAller[17] = 3;
    if (ramePresente(dAller[18], dAller[20] + 75)) feuAller[18] = 3;
    if (ramePresente(dAller[19], dAller[21] + 75)) feuAller[19] = 3;
    if (ramePresente(dAller[20], dAller[22] + 75)) feuAller[20] = 3;
    if (ramePresente(dAller[21], dAller[23] + 75)) feuAller[21] = 3;
    if (ramePresente(dAller[22], dAller[24] + 75)) feuAller[22] = 3;
    if (ramePresente(dAller[23], dAller[25] + 75)) {

        glColor3d(0.8 + 0.2 * bleuCiel(), 0.65 + 0.35 * bleuCiel(), 0.2 + 0.8 * bleuCiel());
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glColor3d(bleuCiel(), bleuCiel(), bleuCiel());
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glColor3d(1, 1, 1);
        feuAller[23] = 3;

    }

    if (ramePresente(dAller[24], dAller[26] + 75)) feuAller[24] = 3;
    if (ramePresente(dAller[25], dAller[27] + 75)) feuAller[25] = 3;
    if (ramePresente(dAller[26], dAller[28] + 75)) feuAller[26] = 3;
    if (ramePresente(dAller[27], dAller[29] + 75)) feuAller[27] = 3;
    if (ramePresente(dAller[28], dAller[29] + 75)) feuAller[28] = 3;
    if (ramePresente(dAller[29], dAller[31] + 75)) feuAller[29] = 3;
    if (ramePresente(dAller[30], dAller[32] + 75)) feuAller[30] = 3;
    if (ramePresente(dAller[31], dAller[33] + 75)) feuAller[31] = 3;
    if (ramePresente(dAller[32], dAller[34] + 75)) feuAller[32] = 3;
    if (ramePresente(dAller[33], dAller[36] + 75)) feuAller[33] = 3;
    if (ramePresente(dAller[34], dAller[37] + 75)) feuAller[34] = 3;
    if (ramePresente(dAller[35], dAller[38] + 75)) feuAller[35] = 3;
    if (ramePresente(dAller[36], dAller[38] + 75)) feuAller[36] = 3;
    if (ramePresente(dAller[37], dAller[39] + 75)) feuAller[37] = 3;
    if (ramePresente(dAller[38], dAller[40] + 75)) feuAller[38] = 3;
    if (ramePresente(dAller[39], dAller[41] + 75)) feuAller[39] = 3;
    if (ramePresente(dAller[40], dAller[42] + 75)) feuAller[40] = 3;
    if (ramePresente(dAller[41], dAller[43] + 75)) feuAller[41] = 3;
    if (ramePresente(dAller[42], 7000)) feuAller[42] = 3;
    if (ramePresente(dAller[43], 7000) || ramePresente(99000, 100010 + 75)) feuAller[43] = 3;


    if (ramePresente(dRetour[0], dRetour[2] + 75)) feuRetour[0] = 3;
    if (ramePresente(dRetour[1], dRetour[3] + 75)) feuRetour[1] = 3;
    if (ramePresente(dRetour[2], dRetour[4] + 75)) feuRetour[2] = 3;
    if (ramePresente(dRetour[3], dRetour[5] + 75)) feuRetour[3] = 3;
    if (ramePresente(dRetour[4], dRetour[6] + 75)) feuRetour[4] = 3;
    if (ramePresente(dRetour[5], dRetour[7] + 75)) feuRetour[5] = 3;
    if (ramePresente(dRetour[6], dRetour[8] + 75)) feuRetour[6] = 3;
    if (ramePresente(dRetour[7], dRetour[10] + 75)) feuRetour[7] = 3;
    if (ramePresente(dRetour[8], dRetour[10] + 75)) feuRetour[8] = 3;
    if (ramePresente(dRetour[9], dRetour[11] + 75)) feuRetour[9] = 3;
    if (ramePresente(dRetour[10], dRetour[12] + 75)) feuRetour[10] = 3;
    if (ramePresente(dRetour[11], dRetour[13] + 75)) feuRetour[11] = 3;
    if (ramePresente(dRetour[12], dRetour[14] + 75)) feuRetour[12] = 3;
    if (ramePresente(dRetour[13], dRetour[15] + 75)) feuRetour[13] = 3;
    if (ramePresente(dRetour[14], dRetour[16] + 75)) {

        glColor3d(0.8 + 0.2 * bleuCiel(), 0.65 + 0.35 * bleuCiel(), 0.2 + 0.8 * bleuCiel());
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  2821.0f,  float(14.0 + 6.75 * PI));
        glColor3d(bleuCiel(), bleuCiel(), bleuCiel());
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2821.0f,  float(13.0 + 6.75 * PI));
        glColor3d(1, 1, 1);
        feuRetour[14] = 3;

    }

    if (ramePresente(dRetour[15], dRetour[17] + 75)) feuRetour[15] = 3;
    if (ramePresente(dRetour[16], dRetour[18] + 75)) feuRetour[16] = 3;
    if (ramePresente(dRetour[17], dRetour[18] + 75)) feuRetour[17] = 3;
    if (ramePresente(dRetour[18], dRetour[19] + 75)) feuRetour[18] = 3;
    if (ramePresente(dRetour[19], dRetour[21] + 75)) feuRetour[19] = 3;
    if (ramePresente(dRetour[20], dRetour[22] + 75)) feuRetour[20] = 3;
    if (ramePresente(dRetour[21], dRetour[23] + 75)) feuRetour[21] = 3;
    if (ramePresente(dRetour[22], dRetour[24] + 75)) feuRetour[22] = 3;
    if (ramePresente(dRetour[23], dRetour[25] + 75)) feuRetour[23] = 3;
    if (ramePresente(dRetour[24], dRetour[26] + 75)) feuRetour[24] = 3;
    if (ramePresente(dRetour[25], dRetour[27] + 75)) feuRetour[25] = 3;
    if (ramePresente(dRetour[26], dRetour[28] + 75)) feuRetour[26] = 3;
    if (ramePresente(dRetour[27], dRetour[29] + 75)) feuRetour[27] = 3;
    if (ramePresente(dRetour[28], dRetour[30] + 75)) feuRetour[28] = 3;
    if (ramePresente(dRetour[29], dRetour[32] + 75)) feuRetour[29] = 3;
    if (ramePresente(dRetour[30], dRetour[32] + 75)) feuRetour[30] = 3;
    if (ramePresente(dRetour[31], dRetour[33] + 75)) feuRetour[31] = 3;
    if (ramePresente(dRetour[32], dRetour[34] + 75)) feuRetour[32] = 3;
    if (ramePresente(dRetour[33], dRetour[35] + 75)) feuRetour[33] = 3;
    if (ramePresente(dRetour[34], dRetour[36] + 75)) feuRetour[34] = 3;
    if (ramePresente(dRetour[35], dRetour[37] + 75)) feuRetour[35] = 3;
    if (ramePresente(dRetour[36], dRetour[39] + 75)) feuRetour[36] = 3;
    if (ramePresente(dRetour[37], dRetour[40] + 75)) feuRetour[37] = 3;
    if (ramePresente(dRetour[38], dRetour[41] + 75)) feuRetour[38] = 3;
    if (ramePresente(dRetour[39], dRetour[41] + 75)) feuRetour[39] = 3;
    if (ramePresente(dRetour[40], dRetour[42] + 75)) feuRetour[40] = 3;
    if (ramePresente(dRetour[41], dRetour[43] + 75)) feuRetour[41] = 3;
    if (ramePresente(dRetour[42], dRetour[44] + 75)) feuRetour[42] = 3;
    if (ramePresente(dRetour[43], dRetour[45] + 75)) feuRetour[43] = 3;
    if (ramePresente(dRetour[44], dRetour[46] + 75)) feuRetour[44] = 3;
    if (ramePresente(dRetour[45], dRetour[46] + 75)) feuRetour[45] = 3;
    if (ramePresente(232, dAller[0] + 75)) feuRetour[46] = 3;

    glEnd(); //feu retour [29] et [30]

    ajoutFeu(feuAller[0], -230, -515.5, 6.5, 180);
    ajoutFeu(feuAller[1], -96.044896, -469.955105, 6.5, 135);
    ajoutFeu(feuAller[2], -46.044896, -419.955105, 6.5, 135);
    ajoutFeu(feuAller[3], -0.5, -225, 6.5, 90);
    ajoutFeu(feuAller[4], -0.5, -55, 6.5, 90);
    ajoutFeu(feuAller[5], -2.5, 110, 4.5, 90);
    ajoutFeu(feuAller[6], -5.5, 139.9, 2.5, 90, true, false);
    ajoutFeu(feuAller[7], -2.5, 235, 4.5, 90, true, true, false);
    ajoutFeu(feuAller[8], 5.5, 335, 2.5, 90);
    ajoutFeu(feuAller[9], 5.5, 665, 2.5, 90);
    ajoutFeu(feuAller[10], 5.5, 760, 2.4, 90);
    ajoutFeu(feuAller[11], 5.5, 1000, 0.0, 90);
    ajoutFeu(feuAller[12], 5.5, 1240, -2.4, 90);
    ajoutFeu(feuAller[13], 5.5, 1340, -3.4, 90);
    ajoutFeu(feuAller[14], 5.5, 1435, -3.5, 90);
    ajoutFeu(feuAller[15], 93.282486, 1576.925667, -2.65, 135);
    ajoutFeu(feuAller[16], 238.239376, 1721.882567, -0.6, 135);
    ajoutFeu(feuAller[17], 305.414520, 1789.057711, -0.5, 135);
    ajoutFeu(feuAller[18], 443.300343, 1926.943525, 6, 135);
    ajoutFeu(feuAller[19], 581.186165, 2064.829347, 14, 135);
    ajoutFeu(feuAller[20], 726.771064, 2280.990449, 14 + 3.375 * PI, 112.921);
    ajoutFeu(feuAller[21], 780.426734, 2536.023123, 12.5 + 6.75 * PI, 90.838);
    ajoutFeu(feuAller[22], 780.5, 2631.0, 12.5 + 6.75 * PI, 90);
    ajoutFeu(feuAller[24], 780.5, 3011, 12.5 + 6.75 * PI, 90);
    ajoutFeu(feuAller[25], 781.218184, 3105.965531, 12.5 + 6.75 * PI, 98.243997, true);
    ajoutFeu(feuAller[26],935, 3165.5, 12.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[27], 1041.365680, 3218.25, 12.5 + 6.75 * PI, 150);
    ajoutFeu(feuAller[28], 1130, 3265.5, 12.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[29], 1225, 3265.5, 12.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[30], 1390, 3265.5, 9.95 + 6.75 * PI, 180);
    ajoutFeu(feuAller[31], 1555, 3265.5, 5.41 + 6.75 * PI, 180);
    ajoutFeu(feuAller[32], 1715, 3265.5, 2.6 + 6.75 * PI, 180);
    ajoutFeu(feuAller[33], 1810, 3265.5, 2.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[34], 2000, 3265.5, 2.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[35], 2047.25, 3252.839401, 2.5 + 6.75 * PI, 210);
    ajoutFeu(feuAller[36], 2182, 3238.705081, 2.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[37], 2240, 3238.705081, 2.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[38], 2334.539028, 3237.908467, 2 + 6.75 * PI, 189.549297);
    ajoutFeu(feuAller[39], 2385, 3222.628129, 2.5 + 6.75 * PI, 180);
    ajoutFeu(feuAller[40], 2447.5, 3285.128129, 2.5 + 5 * PI, 90);
    ajoutFeu(feuAller[41], 2385, 3347.628129, 2.5 + 5.75 * PI, 0);
    ajoutFeu(feuAller[42], 232.448584, 3305.25, 2.5 + 6.75 * PI, 300);
    ajoutFeu(feuAller[43], 2284.202123, 3277.298612, 2.5 + 6.75 * PI, 350.440);


    ajoutFeu(feuRetour[0], 2189.652123, 3276.5, 2.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[1], 2095, 3276.5, 2.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[2], 1900, 3276.5, 2.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[3], 1810, 3276.5, 2.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[4], 1715, 3276.5, 2.6 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[5], 1555, 3276.5, 5.41 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[6], 1390, 3276.5, 9.95 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[7], 1225, 3276.5, 12.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[8], 1130, 3276.5, 12.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[9], 1031.839401, 3223.75, 12.5 + 6.75 * PI, 330);
    ajoutFeu(feuRetour[10], 950, 3176.5, 12.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[11], 850, 3176.5, 12.5 + 6.75 * PI, 0);
    ajoutFeu(feuRetour[12], 770.113474, 3105.919319, 12.5 + 6.75 * PI, 277.078);
    ajoutFeu(feuRetour[13], 769.5, 3011, 12.5 + 6.75 * PI, 270);
    ajoutFeu(feuRetour[15], 769.5, 2631, 12.5 + 6.75 * PI, 270);
    ajoutFeu(feuRetour[16], 769.425206, 2536.008420, 12.5 + 6.75 * PI, 270.857);
    ajoutFeu(feuRetour[17], 718.537347, 2289.808953, 14 + 3.375 * PI, 292.5);
    ajoutFeu(feuRetour[18], 573.407990, 2072.607521, 14, 315);
    ajoutFeu(feuRetour[19], 297.636345, 1796.835877, -0.5, 315);
    ajoutFeu(feuRetour[20], 230.461201, 1729.660732, -0.6, 315);
    ajoutFeu(feuRetour[21], 131.471252, 1630.660783, -2, 315);
    ajoutFeu(feuRetour[22], 32.471303, 1531.670834, -3.4, 315);
    ajoutFeu(feuRetour[23], -5.5, 1435, -3.5, 270);
    ajoutFeu(feuRetour[24], -5.5, 1340, -3.4, 270);
    ajoutFeu(feuRetour[25], -5.5, 1150, -1.5, 270);
    ajoutFeu(feuRetour[26], -5.5, 950, 0.5, 270);
    ajoutFeu(feuRetour[27], -5.5, 760, 2.4, 270);
    ajoutFeu(feuRetour[28], -5.5, 665, 2.5, 270);
    ajoutFeu(feuRetour[29], -3.5, 275, 4.5, 270, true, true, false); //feu déplacé, coord. de départ : xmoy = -5.5, ymoy = 275, zmoy = 2.5
    ajoutFeu(feuRetour[30], -14.5, 235, 2.5, 270); //feu déplacé, coord. de départ : xmoy = -5.5
    ajoutFeu(feuRetour[31], -14.5, 140, 2.5, 270);
    ajoutFeu(feuRetour[32], -14.5, 0.0, 0.1, 270);
    ajoutFeu(feuRetour[33], -12.5, -130, 0.4, 270);
    ajoutFeu(feuRetour[34], -11.5, -310, 6.5, 270);
    ajoutFeu(feuRetour[35], -53.823071, -412.176930, 6.5, 315);
    ajoutFeu(feuRetour[36], -103.823071, -462.176930, 6.5, 315);
    ajoutFeu(feuRetour[37], -149.936877, -495.348352, 8.5, 337.5, true, true, false);
    ajoutFeu(feuRetour[38], -206, -504.5, 6.5, 0);
    ajoutFeu(feuRetour[39], -230, -504.5, 6.5, 0);
    ajoutFeu(feuRetour[40], -340, -504.5, 6.5, 0);
    ajoutFeu(feuRetour[41], -470, -506.5, 8.5, 0, true, true, false);
    ajoutFeu(feuRetour[42], -572.5, -404, 8.5, 90, true, true, false);
    ajoutFeu(feuRetour[43], -696.265242, -352.734759, 8.5, 315, true, true, false);
    ajoutFeu(feuRetour[44], -717.5, -441, 8.5, 270, true, true, false);
    ajoutFeu(feuRetour[45], -552, -513.5, 8.5, 180, true, true, false);
    ajoutFeu(feuRetour[46], -340, -515.5, 6.5, 180);


    //feu retour[30] à l'entrée de République à déplacer, feu retour[29] au positionnement bizarre



}

void maFenetre::afficherCachesTIV() {

    glBindTexture(GL_TEXTURE_2D, texture2[4]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -339.9f,  -515.0f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -339.9f,  -515.0f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -339.9f,  -516.0f,   7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -339.9f,  -516.0f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -339.9f,  -515.0f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -339.9f,  -515.0f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.0f,  -515.0f,   7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -340.0f,  -515.0f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -339.9f,  -515.0f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.0f,  -515.0f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.0f,  -516.0f,   6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -339.9f,  -516.0f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -339.9f,  -515.0f,   7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.0f,  -515.0f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.0f,  -516.0f,   7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -339.9f,  -516.0f,   7.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1555.1f,  3266.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1555.1f,  3266.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1555.1f,  3265.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.1f,  3265.0f,   float(4.91 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1555.1f,  3266.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1555.1f,  3266.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1555.0f,  3266.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.0f,  3266.0f,   float(4.91 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1555.0f,  3266.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1555.1f,  3266.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1555.1f,  3265.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.0f,  3265.0f,   float(4.91 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1555.0f,  3266.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1555.1f,  3266.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1555.1f,  3265.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.0f,  3265.0f,   float(5.91 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 726.410541f, 2281.185177f, float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 726.410541f, 2281.185177f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 727.331586f, 2280.795720f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 727.331586f, 2280.795720f, float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 726.310541f, 2281.185177f, float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 726.310541f, 2281.185177f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 726.431586f, 2281.185177f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 726.431586f, 2281.185177f, float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 727.331586f, 2280.795720f, float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 727.331586f, 2280.795720f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 727.231586f, 2280.795720f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 727.231586f, 2280.795720f, float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 726.310541f, 2281.185177f, float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 726.410541f, 2281.185177f, float(13.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 727.331586f, 2280.795720f, float(13.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 727.231586f, 2280.795720f, float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 726.310541f, 2281.185177f, float(14.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 726.410541f, 2281.185177f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 727.331586f, 2280.795720f, float(14.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 727.231586f, 2280.795720f, float(14.5 + 3.375 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 443.046789f,  1927.397078f,  5.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 443.046789f,  1927.397078f,  6.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 443.753896f,  1926.689971f,  6.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 443.753896f,  1926.689971f,  5.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 442.946789f,  1927.297078f,  5.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 442.946789f,  1927.297078f,  6.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 443.046789f,  1927.397078f,  6.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 443.046789f,  1927.397078f,  5.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 443.753896f,  1926.689971f,  5.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 443.753896f,  1926.689971f,  6.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 443.653896f,  1926.589971f,  6.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 443.653896f,  1926.589971f,  5.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 442.946789f,  1927.297078f,  5.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 443.046789f,  1927.397078f,  5.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 443.753896f,  1926.689971f,  5.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 443.653896f,  1926.589971f,  5.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 442.946789f,  1927.297078f,  6.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 443.046789f,  1927.397078f,  6.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 443.753896f,  1926.689971f,  6.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 443.653896f,  1926.589971f,  6.5f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1340.1f,  -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1340.1f,  -2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  1340.1f,  -2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  1340.1f,  -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1340.1f,  -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1340.1f,  -2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1340.0f,  -2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1340.0f,  -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1340.0f,  -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1340.1f,  -3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  1340.1f,  -3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  1340.0f,  -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1340.0f,  -2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1340.1f,  -2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f,  1340.1f,  -2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f,  1340.0f,  -2.9f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f, 1000.1f,  -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f, 1000.1f,   0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 1000.1f,   0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 1000.1f,  -0.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f, 1000.1f,  -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f, 1000.1f,   0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f, 1000.0f,   0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f, 1000.0f,  -0.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f, 1000.0f,  -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f, 1000.1f,  -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 1000.1f,  -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 1000.0f,  -0.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f, 1000.0f,   0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f, 1000.1f,   0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 6.0f, 1000.1f,   0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 6.0f, 1000.0f,   0.5f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -54.9f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -54.9f, 7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -54.9f, 7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -54.9f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -54.9f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -54.9f, 7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -1.0f, -55.0f, 7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.0f, -55.0f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -55.0f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -54.9f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -54.9f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -55.0f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -55.0f, 7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -54.9f, 7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -54.9f, 7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -55.0f, 7.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 131.729805f,  1630.202229f,  -2.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 131.729805f,  1630.202229f,  -1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 131.012698f,  1630.919336f,  -1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 131.012698f,  1630.919336f,  -2.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 131.112698f,  1631.019336f,  -2.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 131.112698f,  1631.019336f,  -1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 131.012698f,  1630.919336f,  -1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 131.012698f,  1630.919336f,  -2.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 131.729805f,  1630.202229f,  -2.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 131.829805f,  1630.302229f,  -2.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 131.112698f,  1631.019336f,  -2.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 131.012698f,  1630.919336f,  -2.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 131.729805f,  1630.202229f,  -1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 131.829805f,  1630.302229f,  -1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 131.112698f,  1631.019336f,  -1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 131.012698f,  1630.919336f,  -1.5f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 949.9f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 949.9f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 949.9f,  3177.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 949.9f,  3177.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 949.9f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 949.9f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 950.0f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 950.0f,  3176.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 949.9f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 950.0f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 950.0f,  3177.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 949.9f,  3177.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 949.9f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 950.0f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 950.0f,  3177.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 949.9f,  3177.0f,  float(13.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1554.9f,  3276.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1554.9f,  3276.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1554.9f,  3277.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1554.9f,  3277.0f,   float(4.91 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1554.9f,  3276.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1554.9f,  3276.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1555.0f,  3276.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.0f,  3276.0f,   float(4.91 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1555.0f,  3276.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1554.9f,  3276.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1554.9f,  3277.0f,   float(4.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.0f,  3277.0f,   float(4.91 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1555.0f,  3276.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1554.9f,  3276.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1554.9f,  3277.0f,   float(5.91 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1555.0f,  3277.0f,   float(5.91 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1389.9f,  3276.0f,   float( 9.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1389.9f,  3276.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1389.9f,  3277.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1389.9f,  3277.0f,   float( 9.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1389.9f,  3276.0f,   float( 9.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1389.9f,  3276.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1390.0f,  3276.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3276.0f,   float( 9.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.0f,  3276.0f,   float(9.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1389.9f,  3276.0f,   float(9.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1389.9f,  3277.0f,   float(9.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3277.0f,   float(9.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.0f,  3276.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1389.9f,  3276.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1389.9f,  3277.0f,   float(10.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3277.0f,   float(10.45 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  110.1f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  110.1f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  110.1f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.0f,  110.1f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  110.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  110.0f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -4.0f,  110.1f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -4.0f,  110.1f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.0f,  110.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.0f,  110.0f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  110.1f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.0f,  110.1f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  110.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -3.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -3.0f,  110.0f,  4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -149.462852f,  -496.734171f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -149.462852f,  -496.734171f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -150.228219f,  -494.886412f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -150.228219f,  -494.886412f,  8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -149.362852f,  -496.734171f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -149.362852f,  -496.734171f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -149.462852f,  -496.734171f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -149.462852f,  -496.734171f,  8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -150.228219f,  -494.886412f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -150.228219f,  -494.886412f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -150.128219f,  -494.886412f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -150.128219f,  -494.886412f,  8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -149.362852f,  -496.734171f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -149.462852f,  -496.734171f,  8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -150.228219f,  -494.886412f,  8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -150.128219f,  -494.886412f,  8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -551.9f,  -512.0f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -551.9f,  -512.0f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -551.9f,  -514.0f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -551.9f,  -514.0f,  8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -551.9f,  -512.0f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -551.9f,  -512.0f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -552.0f,  -512.0f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -552.0f,  -512.0f,  8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -551.9f,  -514.0f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -551.9f,  -514.0f,  9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -552.0f,  -514.0f,  9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -552.0f,  -514.0f,  8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -551.9f,  -512.0f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -552.0f,  -512.0f,  8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -552.0f,  -514.0f,  8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -551.9f,  -514.0f,  8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -716.0f, -441.1f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -716.0f, -441.1f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -718.0f, -441.1f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -718.0f, -441.1f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -716.0f, -441.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -716.0f, -441.0f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -716.0f, -441.1f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -716.0f, -441.1f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -718.0f, -441.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -718.0f, -441.0f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -718.0f, -441.1f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -718.0f, -441.1f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -716.0f, -441.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -716.0f, -441.1f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -718.0f, -441.1f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -718.0f, -441.0f, 8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -695.304581f, -353.895419f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -695.304581f, -353.895419f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -696.718795f, -352.481205f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -696.718795f, -352.481205f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -695.204581f, -353.795419f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -695.204581f, -353.795419f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -695.304581f, -353.895419f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -695.304581f, -353.895419f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -696.718795f, -352.481205f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -696.718795f, -352.481205f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -696.618795f, -352.381205f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -696.618795f, -352.381205f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -695.204581f, -353.795419f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -695.304581f, -353.895419f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -696.718795f, -352.481205f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -696.618795f, -352.381205f, 8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -574.0f, -403.9f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -574.0f, -403.9f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -572.0f, -403.9f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -572.0f, -403.9f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -574.0f, -404.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -574.0f, -404.0f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -574.0f, -403.9f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -574.0f, -403.9f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -572.0f, -404.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -572.0f, -404.0f, 9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -572.0f, -403.9f, 9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -572.0f, -403.9f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -574.0f, -403.9f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -574.0f, -404.0f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -572.0f, -404.0f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -572.0f, -403.9f, 8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -470.1f,  -508.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -470.1f,  -508.0f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -470.1f,  -506.0f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.1f,  -506.0f,   8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -470.0f,  -508.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -470.1f,  -508.0f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -470.1f,  -506.0f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.0f,  -506.0f,   8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -470.0f,  -508.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -470.0f,  -508.0f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -470.1f,  -508.0f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.1f,  -508.0f,   8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -470.0f,  -506.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -470.0f,  -506.0f,   9.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -470.1f,  -506.0f,   9.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -470.1f,  -506.0f,   8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -130.1f, -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, -130.1f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -13.0f, -130.1f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -13.0f, -130.1f, -4.0f); //spécial, on donne l'illusion d'un ancrage au sol

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -130.1f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -12.0f, -130.1f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f, -130.0f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -130.0f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -13.0f, -130.1f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -13.0f, -130.1f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -13.0f, -130.0f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -13.0f, -130.0f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -130.1f, -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -13.0f, -130.1f, -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -13.0f, -130.0f, -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -130.0f, -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -12.0f, -130.1f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -13.0f, -130.1f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -13.0f, -130.0f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f, -130.0f,  1.9f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -340.1f,  -505.0f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.1f,  -505.0f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.1f,  -504.0f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -340.1f,  -504.0f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -340.1f,  -505.0f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.1f,  -505.0f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.0f,  -505.0f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -340.0f,  -505.0f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -340.0f,  -505.0f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.1f,  -505.0f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.1f,  -504.0f,   6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -340.0f,  -504.0f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -340.0f,  -505.0f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -340.1f,  -505.0f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -340.1f,  -504.0f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -340.0f,  -504.0f,   8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -103.569517f,  -462.630483f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -103.569517f,  -462.630483f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -104.276624f,  -461.923376f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -104.276624f,  -461.923376f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -103.469517f,  -462.530483f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -103.469517f,  -462.530483f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -103.569517f,  -462.630483f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -103.569517f,  -462.630483f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -103.469517f,  -462.530483f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -103.569517f,  -462.630483f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -104.276624f,  -461.923376f,   6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -104.176624f,  -461.823376f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -103.469517f,  -462.530483f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -103.569517f,  -462.630483f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -104.276624f,  -461.923376f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -104.176624f,  -461.823376f,   8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -53.569517f,  -412.630483f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -53.569517f,  -412.630483f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -54.276624f,  -411.923376f,   7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -54.276624f,  -411.923376f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -53.469517f,  -412.530483f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -53.469517f,  -412.530483f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -53.569517f,  -412.630483f,   7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -53.569517f,  -412.630483f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -53.469517f,  -412.530483f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -53.569517f,  -412.630483f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -54.276624f,  -411.923376f,   6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -54.176624f,  -411.823376f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -53.469517f,  -412.530483f,   7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -53.569517f,  -412.630483f,   7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -54.276624f,  -411.923376f,   7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -54.176624f,  -411.823376f,   7.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -96.298449f,  -469.501551f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -96.298449f,  -469.501551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -95.591342f,  -470.208658f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -95.591342f,  -470.208658f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -96.398449f,  -469.601551f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -96.398449f,  -469.601551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -96.298449f,  -469.501551f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -96.298449f,  -469.501551f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -96.398449f,  -469.601551f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -96.298449f,  -469.501551f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -95.591342f,  -470.208658f,   6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -95.691342f,  -470.308658f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -96.398449f,  -469.601551f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -96.298449f,  -469.501551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -95.591342f,  -470.208658f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -95.691342f,  -470.308658f,   8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -46.298449f,  -419.501551f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -46.298449f,  -419.501551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -45.591342f,  -420.208658f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -45.591342f,  -420.208658f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -46.398449f,  -419.601551f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -46.398449f,  -419.601551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -46.298449f,  -419.501551f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -46.298449f,  -419.501551f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -46.398449f,  -419.601551f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -46.298449f,  -419.501551f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -45.591342f,  -420.208658f,   6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -45.691342f,  -420.308658f,   6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -46.398449f,  -419.601551f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -46.298449f,  -419.501551f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -45.591342f,  -420.208658f,   8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -45.691342f,  -420.308658f,   8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -206.1f,  -504.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.1f,  -504.0f,  7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -206.1f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -206.1f,  -505.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -206.0f,  -505.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.0f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -206.1f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -206.1f,  -505.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -206.0f,  -504.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.1f,  -504.0f,  6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -206.1f,  -505.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -206.0f,  -505.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -206.0f,  -504.0f,  7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -206.1f,  -504.0f,  7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -206.1f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -206.0f,  -505.0f,  7.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.1f,  -504.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -230.1f,  -504.0f,  7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -230.1f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.1f,  -505.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.0f,  -505.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -230.0f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -230.1f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.1f,  -505.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.0f,  -504.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -230.1f,  -504.0f,  6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -230.1f,  -505.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.0f,  -505.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.0f,  -504.0f,  7.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -230.1f,  -504.0f,  7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -230.1f,  -505.0f,  7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.0f,  -505.0f,  7.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -229.9f,  -515.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -229.9f,  -515.0f,  8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -229.9f,  -516.0f,  8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -229.9f,  -516.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -229.9f,  -515.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -229.9f,  -515.0f,  8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -230.0f,  -515.0f,  8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.0f,  -515.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.0f,  -515.0f,  6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -229.9f,  -515.0f,  6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -229.9f,  -516.0f,  6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.0f,  -516.0f,  6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -230.0f,  -515.0f,  8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -229.9f,  -515.0f,  8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -229.9f,  -516.0f,  8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -230.0f,  -516.0f,  8.0f);


    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f,  -310.1f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f,  -310.1f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -11.0f,  -310.1f,   8.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -11.0f,  -310.1f,   6.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -11.0f,  -310.1f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -11.0f,  -310.1f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -11.0f,  -310.0f,   8.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -11.0f,  -310.0f,   6.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f,  -310.0f,   6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f,  -310.1f,   6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -11.0f,  -310.1f,   6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -11.0f,  -310.0f,   6.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -12.0f,  -310.0f,   8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -12.0f,  -310.1f,   8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -11.0f,  -310.1f,   8.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -11.0f,  -310.0f,   8.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,  -0.1f,   -0.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  -0.1f,    1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -14.0f,  -0.1f,    1.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -14.0f,  -0.1f,   -0.4f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  -0.1f,   -0.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  -0.1f,    1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -14.0f,   0.0f,    1.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -14.0f,   0.0f,   -0.4f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,   0.0f,   -0.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  -0.1f,   -0.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -14.0f,  -0.1f,   -0.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -14.0f,   0.0f,   -0.4f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -15.0f,   0.0f,   1.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -15.0f,  -0.1f,   1.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -14.0f,  -0.1f,   1.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -14.0f,   0.0f,   1.6f);


    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  139.9f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  139.9f,   4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  139.9f,   4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  139.9f,   2.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -14.0f,  139.9f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -14.0f,  139.9f,   4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  140.0f,   4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  140.0f,   2.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  140.0f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  139.9f,   2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  139.9f,   2.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  140.0f,   2.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f( -15.0f,  140.0f,   4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -15.0f,  139.9f,   4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -14.0f,  139.9f,   4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -14.0f,  140.0f,   4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -224.9f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -224.9f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -224.9f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -224.9f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -225.0f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -225.0f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -1.0f, -224.9f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.0f, -224.9f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -225.0f, 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -224.9f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -224.9f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -225.0f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -225.0f, 8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f, -224.9f, 8.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, -224.9f, 8.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, -225.0f, 8.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 93.028933f,  1577.379221f,  -3.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 93.028933f,  1577.379221f,  -1.15f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 93.736039f,  1576.672113f,  -1.15f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 93.736039f,  1576.672113f,  -3.15f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 93.028933f,  1577.379221f,  -1.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 92.928933f,  1577.279221f,  -1.15f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 93.636039f,  1576.572113f,  -1.15f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 93.736039f,  1576.672113f,  -1.15f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 93.028933f,  1577.379221f,  -3.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 92.928933f,  1577.279221f,  -3.15f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 93.636039f,  1576.572113f,  -3.15f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 93.736039f,  1576.672113f,  -3.15f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 92.928933f,  1577.279221f,  -3.15f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 92.928933f,  1577.279221f,  -1.15f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 93.028933f,  1577.379221f,  -1.15f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 93.028933f,  1577.379221f,  -3.15f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 305.160967f,  1789.511255f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 305.160967f,  1789.511255f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 305.868074f,  1788.804148f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 305.868074f,  1788.804148f,  -1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 305.160967f,  1789.511255f,   1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 305.060967f,  1789.411255f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 305.768074f,  1788.704148f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 305.868074f,  1788.804148f,   1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 305.160967f,  1789.511255f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 305.060967f,  1789.411255f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 305.768074f,  1788.704148f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 305.868074f,  1788.804148f,  -1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 305.060967f,  1789.411255f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 305.060967f,  1789.411255f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 305.160967f,  1789.511225f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 305.160967f,  1789.511225f,  -1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1714.9f,  3276.0f,  float(2.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1714.9f,  3276.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1714.9f,  3277.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1714.9f,  3277.0f,  float(2.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1714.9f,  3276.0f,  float(2.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1714.9f,  3276.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1715.0f,  3276.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.0f,  3276.0f,  float(2.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.0f,  3276.0f,  float(2.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1714.9f,  3276.0f,  float(2.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1714.9f,  3277.0f,  float(2.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.0f,  3277.0f,  float(2.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.0f,  3276.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1714.9f,  3276.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1714.9f,  3277.0f,  float(4.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.0f,  3277.0f,  float(4.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1899.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1899.9f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1899.9f,  3277.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1899.9f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1899.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1899.9f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1900.0f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1900.0f,  3276.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1900.0f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1899.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1899.9f,  3277.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1900.0f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1900.0f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1899.9f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1899.9f,  3277.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1900.0f,  3277.0f,  float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2094.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2094.9f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2094.9f,  3277.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2094.9f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2094.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2094.9f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2095.0f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2095.0f,  3276.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2095.0f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2094.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2094.9f,  3277.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2095.0f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2095.0f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2094.9f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2094.9f,  3277.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2095.0f,  3277.0f,  float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2189.532123f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2189.532123f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2189.532123f,  3277.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2189.532123f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2189.532123f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2189.532123f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2189.652123f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2189.652123f,  3276.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2189.652123f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2189.532123f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2189.532123f,  3277.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2189.652123f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2189.652123f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2189.532123f,  3276.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2189.532123f,  3277.0f,  float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2189.652123f,  3277.0f,  float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.1f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1225.1f,  3266.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1225.1f,  3265.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.1f,  3265.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.1f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1225.1f,  3266.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1225.0f,  3266.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.0f,  3266.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.0f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1225.1f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1225.1f,  3265.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.0f,  3265.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.0f,  3266.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1225.1f,  3266.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1225.1f,  3265.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.0f,  3265.0f,  float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.1f,  3266.0f,  float( 9.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1390.1f,  3266.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1390.1f,  3265.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.1f,  3265.0f,  float( 9.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.1f,  3266.0f,  float( 9.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1390.1f,  3266.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1390.0f,  3266.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3266.0f,  float( 9.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.0f,  3266.0f,  float( 9.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1390.1f,  3266.0f,  float( 9.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1390.1f,  3265.0f,  float( 9.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3265.0f,  float( 9.45 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1390.0f,  3266.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1390.1f,  3266.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1390.1f,  3265.0f,  float(11.45 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1390.0f,  3265.0f,  float(11.45 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.1f,  3266.0f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1810.1f,  3266.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1810.1f,  3265.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.1f,  3265.0f,   float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.1f,  3266.0f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1810.1f,  3266.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1810.0f,  3266.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.0f,  3266.0f,   float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.0f,  3266.0f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1810.1f,  3266.0f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1810.1f,  3265.0f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.0f,  3265.0f,   float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.0f,  3266.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1810.1f,  3266.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1810.1f,  3265.0f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.0f,  3265.0f,   float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2182.1f,  3239.205081f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2182.1f,  3239.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2182.1f,  3238.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2182.1f,  3238.205081f,   float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2182.1f,  3239.205081f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2182.1f,  3239.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2182.0f,  3239.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2182.0f,  3239.205081f,   float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2182.0f,  3239.205081f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2182.1f,  3239.205081f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2182.1f,  3238.205081f,   float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2182.0f,  3238.205081f,   float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2182.0f,  3239.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2182.1f,  3239.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2182.1f,  3238.205081f,   float(4.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2182.0f,  3238.205081f,   float(4.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.1f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3165.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.1f,  3165.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.1f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.0f,  3166.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3165.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.0f,  3165.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3165.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.0f,  3165.0f,  float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f,  2631.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f,  2631.1f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f,  2631.1f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f,  2631.1f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2631.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2631.1f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f,  2631.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f,  2631.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f,  2631.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f,  2631.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f,  2631.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f,  2631.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.0f,  2631.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.0f,  2631.1f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f,  2631.1f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f,  2631.0f,  float(14.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  1435.1f,  -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  1435.1f,  -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1435.1f,  -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1435.1f,  -4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1435.1f,  -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1435.1f,  -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1435.0f,  -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1435.0f,  -4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  1435.0f,  -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  1435.1f,  -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1435.1f,  -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1435.0f,  -4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  1435.0f,  -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  1435.1f,  -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1435.1f,  -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1435.0f,  -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  10.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  10.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  10.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  10.1f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  10.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  10.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  10.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  10.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  10.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  10.1f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  10.1f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  10.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  10.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  10.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  10.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  10.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  110.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  110.1f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  110.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  110.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  110.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  110.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  110.1f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  110.1f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  110.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  110.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  110.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  110.0f,  4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  235.1f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  235.1f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  235.1f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.0f,  235.1f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  235.1f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.0f,  235.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  235.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -4.0f,  235.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.0f,  235.1f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.0f,  235.1f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.0f,  235.0f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.0f,  235.0f,  4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -4.0f,  235.1f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -4.0f,  235.1f,  5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -4.0f,  235.0f,  5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -4.0f,  235.0f,  4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  335.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  335.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  335.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  335.1f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  335.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  335.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  335.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  335.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  335.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  335.1f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  335.1f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  335.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  335.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  335.1f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  335.1f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  335.0f,  4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  760.1f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  760.1f,  3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  760.1f,  3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  760.1f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  760.1f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  760.1f,  3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  760.0f,  3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  760.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  760.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  760.1f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  760.1f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  760.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  760.0f,  3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  760.1f,  3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  760.1f,  3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  760.0f,  3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  665.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  665.1f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  665.1f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  665.1f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  665.1f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  665.1f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  665.0f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  665.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  665.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  665.1f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  665.1f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  665.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  665.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  665.1f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  665.1f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  665.0f,  3.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  139.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  139.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  140.0f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  140.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  140.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  139.9f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  139.9f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  140.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  140.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  139.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  139.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  140.0f,  3.0f); //il manque la face arrière car cette dernière est occupée par un autre feu!


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  234.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  234.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  234.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  234.9f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  234.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  234.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  235.0f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  235.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  235.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  234.9f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  234.9f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  235.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  235.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  234.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  234.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  235.0f,  3.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  274.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  274.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  274.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  274.9f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  274.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  274.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  275.0f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  275.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  275.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  274.9f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  274.9f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  275.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  275.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  274.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  274.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  275.0f,  3.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  759.9f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  759.9f,  2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  759.9f,  2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  759.9f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  759.9f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  759.9f,  2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  760.0f,  2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  760.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  760.0f,  1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  759.9f,  1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  759.9f,  1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  760.0f,  1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  760.0f,  2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  759.9f,  2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  759.9f,  2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  760.0f,  2.9f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  1339.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  1339.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1339.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1339.9f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  1339.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  1339.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1340.0f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1340.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  1340.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  1339.9f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1339.9f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1340.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  1340.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  1339.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1339.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1340.0f,  3.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  1434.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  1434.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1434.9f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  1434.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1435.0f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1435.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  1435.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  1434.9f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1435.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  1435.0f,  3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  1434.9f,  3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1435.0f,  3.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  664.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  664.9f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  664.9f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  664.9f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  664.9f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  664.9f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  665.0f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  665.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  665.0f,  2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  664.9f,  2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  664.9f,  2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  665.0f,  2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f,  665.0f,  4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f,  664.9f,  4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  664.9f,  4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  665.0f,  4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  1240.1f, -2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  1240.1f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1240.1f, -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1240.1f, -2.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 5.0f,  1240.1f, -2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 5.0f,  1240.1f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1240.0f, -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1240.0f, -2.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  1240.0f, -2.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  1240.1f, -2.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1240.1f, -2.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1240.0f, -2.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 6.0f,  1240.0f, -0.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 6.0f,  1240.1f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  1240.1f, -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f,  1240.0f, -0.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1129.9f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1129.9f,  3276.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1129.9f,  3277.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1129.9f,  3277.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1129.9f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1129.9f,  3276.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1130.0f,  3276.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.0f,  3276.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.0f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1129.9f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1129.9f,  3277.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.0f,  3277.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.0f,  3276.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1129.9f,  3276.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1129.9f,  3277.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.0f,  3277.0f,  float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1419.9f,  3276.0f,  float(8.5 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1419.9f,  3276.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1419.9f,  3277.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1419.9f,  3277.0f,  float(8.5 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1419.9f,  3276.0f,  float(8.5 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1419.9f,  3276.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1420.0f,  3276.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1420.0f,  3276.0f,  float(8.5 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1420.0f,  3276.0f,  float(8.5 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1419.9f,  3276.0f,  float(8.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1419.9f,  3277.0f,  float(8.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1420.0f,  3277.0f,  float(8.5 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1420.0f,  3276.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1419.9f,  3276.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1419.9f,  3277.0f,  float(9.5 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1420.0f,  3277.0f,  float(9.5 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f,  3010.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f,  3010.9f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f,  3010.9f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f,  3010.9f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  3010.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  3010.9f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f,  3011.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f,  3011.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f,  3011.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f,  3010.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f,  3010.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f,  3011.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.0f,  3011.0f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.0f,  3010.9f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f,  3010.9f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f,  3011.0f,  float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.925263f,  2535.915883f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.925263f,  2535.915883f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.925151f,  2535.900936f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.925151f,  2535.900936f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 769.925151f,  2535.900936f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 769.925151f,  2535.900936f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.925151f,  2536.000936f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.925151f,  2536.000936f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.925263f,  2536.015883f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.925263f,  2535.915883f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.925151f,  2535.900936f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.925151f,  2536.000936f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 768.925263f,  2536.015883f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 768.925263f,  2535.915883f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.925151f,  2535.900936f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.925151f,  2536.000936f,  float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 230.714754f,  1729.207178f,  -1.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 230.714754f,  1729.207178f,   0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 230.007647f,  1729.914285f,   0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 230.007647f,  1729.914285f,  -1.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 230.714754f,  1729.207178f,  -1.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 230.814754f,  1729.307178f,  -1.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 230.107647f,  1730.014285f,  -1.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 230.007647f,  1729.914285f,  -1.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 230.714754f,  1729.207178f,   0.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 230.814754f,  1729.307178f,   0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 230.107647f,  1730.014285f,   0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 230.007647f,  1729.914285f,   0.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 230.814754f,  1729.307178f,  -1.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 230.814754f,  1729.307178f,   0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 230.714754f,  1729.207178f,   0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 230.714754f,  1729.207178f,  -1.1f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 32.724856f,  1531.217280f,  -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 32.724856f,  1531.217280f,  -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 32.017749f,  1531.924387f,  -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 32.017749f,  1531.924387f,  -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 32.724856f,  1531.217280f,  -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 32.824856f,  1531.317280f,  -3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 32.117749f,  1532.024387f,  -3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 32.017749f,  1531.924387f,  -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 32.724856f,  1531.217280f,  -1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 32.824856f,  1531.317280f,  -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 32.117749f,  1532.024387f,  -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 32.017749f,  1531.924387f,  -1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 32.824856f,  1531.317280f,  -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 32.824856f,  1531.317280f,  -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 32.724856f,  1531.217280f,  -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 32.724856f,  1531.217280f,  -3.9f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1339.9f, -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1339.9f, -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f, 1339.9f, -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f, 1339.9f, -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 1339.9f, -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 1339.9f, -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f, 1340.0f, -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f, 1340.0f, -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1340.0f, -3.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1339.9f, -3.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f, 1339.9f, -3.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f, 1340.0f, -3.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -6.0f, 1340.0f, -1.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -6.0f, 1339.9f, -1.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f, 1339.9f, -1.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f, 1340.0f, -1.9f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -3.5f, 274.9f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -3.5f, 274.9f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.5f, 274.9f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.5f, 274.9f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -2.5f, 274.9f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -2.5f, 274.9f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.5f, 275.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.5f, 275.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -3.5f, 275.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -3.5f, 274.9f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -2.5f, 274.9f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -2.5f, 275.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -3.5f, 274.9f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -3.5f, 274.9f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -3.5f, 275.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -3.5f, 275.0f, 4.0f); //exception, le TIV est accroché en haut du tunnel donc on couvre la droite et non le haut


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.724638f,  3106.073710f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.724638f,  3106.073710f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.727267f,  3106.146174f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.727267f,  3106.146174f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.727267f,  3106.146174f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.727267f,  3106.146174f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.727267f,  3106.046174f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.727267f,  3106.046174f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.724638f,  3105.973710f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.724638f,  3106.073710f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.727267f,  3106.146174f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.727267f,  3106.046174f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 781.724638f,  3105.973710f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 781.724638f,  3106.073710f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.727267f,  3106.146174f,  float(14.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.727267f,  3106.046174f,  float(14.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.985822f,  1722.336110f,  -1.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 237.985822f,  1722.336110f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 238.692929f,  1721.629003f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 238.692929f,  1721.629003f,  -1.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.885822f,  1722.236110f,  -1.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 237.985822f,  1722.336110f,  -1.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 238.692929f,  1721.629003f,  -1.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 238.592929f,  1721.529003f,  -1.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.885822f,  1722.236110f,  -0.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 237.985822f,  1722.336110f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 238.692929f,  1721.629003f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 238.592929f,  1721.529003f,  -0.1f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 237.885822f,  1722.236110f,  -1.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 237.885822f,  1722.236110f,  -0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 237.985822f,  1722.336110f,  -0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 237.985822f,  1722.336110f,  -1.1f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 580.932611f,  2065.282900f,  13.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 580.932611f,  2065.282900f,  14.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 581.639718f,  2064.575793f,  14.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 581.639718f,  2064.575793f,  13.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 580.832611f,  2065.182900f,  13.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 580.932611f,  2065.282900f,  13.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 581.639718f,  2064.575793f,  13.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 581.539718f,  2064.475793f,  13.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 580.832611f,  2065.182900f,  14.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 580.932611f,  2065.282900f,  14.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 581.639718f,  2064.575793f,  14.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 581.539718f,  2064.475793f,  14.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 580.832611f,  2065.182900f,  13.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 580.832611f,  2065.182900f,  14.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 580.932611f,  2065.282900f,  14.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 580.932611f,  2065.282900f,  13.5f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  2536.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  2536.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  2536.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  2536.1f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 779.926686f,  2536.030464f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f     ,  2536.1f     ,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f     ,  2536.1f     ,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.926579f,  2536.015781f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 779.926686f,  2536.030464f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f     ,  2536.1f     ,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f     ,  2536.1f     ,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.926579f,  2536.015781f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 779.926686f,  2536.030464f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 779.926886f,  2536.030464f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f     ,  2536.1f     ,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f     ,  2536.1f     ,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  3011.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  3011.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  3011.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  3011.1f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  3011.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  3011.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  3011.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  3011.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  3011.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  3011.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 781.0f,  3011.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 781.0f,  3011.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 780.0f,  3011.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 780.0f,  3011.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 780.0f,  3011.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 780.0f,  3011.1f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.1f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3165.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.1f,  3165.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3165.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.0f,  3165.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3165.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.0f,  3165.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 935.0f,  3166.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 935.0f,  3166.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 935.1f,  3166.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 935.1f,  3166.0f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1041.032667f,  3218.6f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1041.032667f,  3218.6f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1041.898693f,  3218.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1041.898693f,  3218.1f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1040.932667f,  3218.5f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1041.032667f,  3218.6f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1041.898693f,  3218.1f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1041.798693f,  3218.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1040.932667f,  3218.5f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1041.032667f,  3218.6f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1041.898693f,  3218.1f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1041.798693f,  3218.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1040.932667f,  3218.5f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1040.932667f,  3218.5f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1041.032667f,  3218.6f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1041.032667f,  3218.6f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.1f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1130.1f,  3266.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1130.1f,  3265.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.1f,  3265.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.0f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1130.1f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1130.1f,  3265.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.0f,  3265.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.0f,  3266.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1130.1f,  3266.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1130.1f,  3265.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.0f,  3265.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1130.0f,  3266.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1130.0f,  3266.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1130.1f,  3266.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1130.1f,  3266.0f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.1f,  3266.0f,   float(2.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1715.1f,  3266.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1715.1f,  3265.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.1f,  3265.0f,   float(2.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.0f,  3266.0f,   float(2.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1715.1f,  3266.0f,   float(2.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1715.1f,  3265.0f,   float(2.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.0f,  3265.0f,   float(2.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.0f,  3266.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1715.1f,  3266.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1715.1f,  3265.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.0f,  3265.0f,   float(3.1 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1715.0f,  3266.0f,   float(2.1 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1715.0f,  3266.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1715.1f,  3266.0f,   float(3.1 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1715.1f,  3266.0f,   float(2.1 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.1f,  3239.205081f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.1f,  3239.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2240.1f,  3238.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2240.1f,  3238.205081f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.0f,  3239.205081f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.1f,  3239.205081f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2240.1f,  3238.205081f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2240.0f,  3238.205081f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.0f,  3239.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.1f,  3239.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2240.1f,  3238.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2240.0f,  3238.205081f,  float(3.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2240.0f,  3239.205081f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2240.0f,  3239.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2240.1f,  3239.205081f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2240.1f,  3239.205081f,  float(2.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2284.185166f,  3276.805556f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2284.185166f,  3276.805556f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2284.019079f,  3277.791667f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2284.019079f,  3277.791667f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2284.285166f,  3276.805556f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2284.185166f,  3276.805556f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2284.019079f,  3277.791667f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2284.119079f,  3277.791667f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2284.285166f,  3276.805556f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2284.185166f,  3276.805556f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2284.019079f,  3277.791667f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2284.119079f,  3277.791667f,  float(3.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 2284.285166f,  3276.805556f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 2284.285166f,  3276.805556f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2284.185166f,  3276.805556f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2284.185166f,  3276.805556f,  float(2.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1809.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1809.9f,  3276.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1809.9f,  3277.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1809.9f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.0f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1809.9f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1809.9f,  3277.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.0f,  3277.0f,  float(2.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.0f,  3276.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1809.9f,  3276.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1809.9f,  3277.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1810.0f,  3277.0f,  float(3.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1810.0f,  3276.0f,  float(2.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1810.0f,  3276.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1809.9f,  3276.0f,  float(3.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1809.9f,  3276.0f,  float(2.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1224.9f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1224.9f,  3276.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1224.9f,  3277.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1224.9f,  3277.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.0f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1224.9f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1224.9f,  3277.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.0f,  3277.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.0f,  3276.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1224.9f,  3276.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1224.9f,  3277.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1225.0f,  3277.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1225.0f,  3276.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1225.0f,  3276.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1224.9f,  3276.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1224.9f,  3276.0f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1032.172413f,  3223.4f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1032.172413f,  3223.4f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1031.306388f,  3223.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1031.306388f,  3223.9f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1032.272413f,  3223.5f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1032.172413f,  3223.4f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1031.306388f,  3223.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1031.406388f,  3224.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1032.272413f,  3223.5f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1032.172413f,  3223.4f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1031.306388f,  3223.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1031.406388f,  3224.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1032.272413f,  3223.5f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1032.272413f,  3223.5f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1032.172413f,  3223.4f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1032.172413f,  3223.4f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f(  849.9f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  849.9f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  849.9f,  3177.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  849.9f,  3177.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  850.0f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  849.9f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  849.9f,  3177.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  840.0f,  3177.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  850.0f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  849.9f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  849.9f,  3177.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  850.0f,  3177.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f(  850.0f,  3176.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f(  850.0f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  849.9f,  3176.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  849.9f,  3176.0f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.609663f,  3105.757708f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.609663f,  3105.757708f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.617284f,  3105.880930f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.617284f,  3105.880930f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.609663f,  3105.857708f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.609663f,  3105.757708f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.617284f,  3105.880930f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.617284f,  3105.980930f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.609663f,  3105.857708f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.609663f,  3105.757708f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.617284f,  3105.880930f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.617284f,  3105.980930f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.609663f,  3105.857708f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.609663f,  3105.857708f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.609663f,  3105.757708f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.609063f,  3105.757708f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2630.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2630.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  2630.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  2630.9f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2631.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2630.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  2630.9f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  2631.0f,  float(12.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2631.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2630.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 769.0f,  2630.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 769.0f,  2631.0f,  float(13.0 + 6.75 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 770.0f,  2631.0f,  float(12.0 + 6.75 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 770.0f,  2631.0f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 770.0f,  2630.9f,  float(13.0 + 6.75 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 770.0f,  2630.9f,  float(12.0 + 6.75 * PI));


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 573.661543f,  2072.153968f,  13.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 573.661543f,  2072.153968f,  15.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 572.954437f,  2072.861074f,  15.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 572.954437f,  2072.861074f,  13.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 573.761543f,  2072.253968f,  13.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 573.661543f,  2072.153968f,  13.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 572.954437f,  2072.861074f,  13.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 573.054437f,  2072.961074f,  13.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 573.761543f,  2072.253968f,  15.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 573.661543f,  2072.153968f,  15.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 572.954437f,  2072.861074f,  15.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 573.054437f,  2072.961074f,  15.5f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 573.761543f,  2072.253968f,  13.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 573.761543f,  2072.253968f,  15.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 573.661543f,  2072.153968f,  15.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 573.661543f,  2072.153968f,  13.5f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 297.889898f,  1796.382323f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 297.889898f,  1796.382323f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 297.182791f,  1797.089430f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 297.182791f,  1797.089430f,  -1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 297.989898f,  1796.482323f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 297.889898f,  1796.382323f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 297.182791f,  1797.089430f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 297.282791f,  1797.189430f,  -1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 297.989898f,  1796.482323f,   1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 297.889898f,  1796.382323f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 297.182791f,  1797.089430f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 297.282791f,  1797.189430f,   1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 297.989898f,  1796.482323f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 297.989898f,  1796.482323f,   1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 297.889898f,  1796.382323f,   1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 297.889898f,  1796.382323f,  -1.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  1434.9f,  -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  -3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  1434.9f,  -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  1434.9f,  -4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  1435.0f,  -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  1434.9f,  -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  1435.0f,  -4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  1435.0f,  -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  -3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f,  1434.9f,  -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f,  1435.0f,  -3.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f,  1435.0f,  -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f,  1435.0f,  -3.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f,  1434.9f,  -3.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f,  1434.9f,  -4.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 1149.9f,  -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 1149.9f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1149.9f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1149.9f,  -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 1150.0f,  -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 1149.9f,  -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1149.9f,  -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1150.0f,  -2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 1150.0f,  -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 1149.9f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 1149.9f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 1150.0f,  -1.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 1150.0f,  -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 1150.0f,  -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f, 1149.9f,  -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f, 1149.9f,  -2.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 949.9f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 949.9f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 949.9f,   2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 949.9f,   0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 950.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 949.9f,   0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 949.9f,   0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 950.0f,   0.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 950.0f,   2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 949.9f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -6.0f, 949.9f,   2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -6.0f, 950.0f,   2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( -5.0f, 950.0f,   0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -5.0f, 950.0f,   2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -5.0f, 949.9f,   2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -5.0f, 949.9f,   0.0f);


    glTexCoord2f(0.0f, 0.0f); glVertex3f( 718.075407f,  2289.900295f,  float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 718.075407f,  2289.900295f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 718.999286f,  2289.517611f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 718.999286f,  2289.517611f,  float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 718.075407f,  2290.000295f,  float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 718.075407f,  2289.900295f,  float(13.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 718.999286f,  2289.517611f,  float(13.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 718.999286f,  2289.617611f,  float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 718.075407f,  2290.000295f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 718.075407f,  2289.900295f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 718.999286f,  2289.517611f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 718.999286f,  2289.617611f,  float(15.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 718.075407f,  2290.000295f,  float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 718.075407f,  2290.000295f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 718.075407f,  2289.900295f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 718.075407f,  2289.900295f,  float(13.5 + 3.375 * PI));

    glTexCoord2f(0.0f, 0.0f); glVertex3f( 718.999286f,  2289.517611f,  float(13.5 + 3.375 * PI));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 718.999286f,  2289.517611f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 718.999286f,  2289.617611f,  float(15.5 + 3.375 * PI));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 718.999286f,  2289.617611f,  float(13.5 + 3.375 * PI));

    glEnd();

}

bool maFenetre::estEnStation(double d) {

    return     ( 304                                      < d && d <  310                                     ) //le quai va de 232 à 322, arrêt 15m avant
            || ( 888 +  306 * PI/4              + 50 * V2 < d && d <  894 +  306 * PI/4              + 50 * V2)
            || (1413 +  306 * PI/4              + 50 * V2 < d && d < 1419 +  306 * PI/4              + 50 * V2)
            || (2088 +  306 * PI/4              + 50 * V2 < d && d < 2094 +  306 * PI/4              + 50 * V2)
            || (2488 +  403 * PI/4              + 50 * V2 < d && d < 2494 +  403 * PI/4              + 50 * V2)
            || (2963 + 1081 * PI/4              + 50 * V2 < d && d < 2969 + 1081 * PI/4              + 50 * V2)
            || (3438 + 1081 * PI/4              + 50 * V2 < d && d < 3444 + 1081 * PI/4              + 50 * V2)
            || (3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 < d && d < 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
            || (4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 < d && d < 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
            || (4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2 < d && d < 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
            || (4727 + 1705 * PI/4 + 300 * PI/3 + 50 * V2 < d && d < 4734 + 1705 * PI/4 + 300 * PI/3 + 50 * V2) //4727.95 et 4733.95
            || ( 99997 < d && d < 100003)
            || (100471.35 < d && d < 100477.35)
            || (101056.35 < d && d < 101062.35)
            || (101248.35 +  156 * PI/4 + 200 * PI/3           < d && d < 101254.35 +  156 * PI/4 + 200 * PI/3          )
            || (101723.35 +  156 * PI/4 + 200 * PI/3           < d && d < 101729.35 +  156 * PI/4 + 200 * PI/3          )
            || (102198.35 +  828 * PI/4 + 200 * PI/3           < d && d < 102204.35 +  828 * PI/4 + 200 * PI/3          )
            || (102598.35 +  931 * PI/4 + 200 * PI/3           < d && d < 102604.35 +  931 * PI/4 + 200 * PI/3          )
            || (103273.35 +  931 * PI/4 + 200 * PI/3           < d && d < 103279.35 +  931 * PI/4 + 200 * PI/3          )
            || (103798.35 +  931 * PI/4 + 200 * PI/3           < d && d < 103804.35 +  931 * PI/4 + 200 * PI/3          )
            || (104367.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 < d && d < 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) //le quai va de 104295.35 + ... à 104385.35 + ..., arrêt 15m avant
            || (104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2 < d && d < 104955.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2);

}

void maFenetre::avancerC() {

    c_x += 0.05 * sin(rot + rotc);
    c_y += 0.05 * cos(rot + rotc);

}

void maFenetre::tournerHC() {

    rotc -= PI/4;

}

void maFenetre::tournerTC() {

    rotc += PI/4;

}

double maFenetre::abs(double x) {

    if (x >= 0)
        return x;
    return -x;

}

bool maFenetre::estEnCabine() {

    return pow(pow(c_x, 2) + pow(c_y, 2), 0.5) <= 0.5;

}

void maFenetre::remettreEnCabine() {

    c_x = 0.0;
    c_y = 0.0;
    rotc = 0.0;

}

double maFenetre::exp(double x) {

    return pow(2.718, x);

}

void maFenetre::dessinerPersonnages() {

    int npm = 6; //probabilités à modifier dans le sens retour

    double minStationAller[10] = {304., 888 + 306 * PI/4 + 50 * V2, 1413 + 306 * PI/4 + 50 * V2, 2088 + 306 * PI/4 + 50 * V2, 2488 + 403 * PI/4 + 50 * V2, 2963 + 1081 * PI/4 + 50 * V2, 3438 + 1081 * PI/4 + 50 * V2, 3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2};
    double maxStationAller[10] = {310., 894 + 306 * PI/4 + 50 * V2, 1419 + 306 * PI/4 + 50 * V2, 2094 + 306 * PI/4 + 50 * V2, 2494 + 403 * PI/4 + 50 * V2, 2969 + 1081 * PI/4 + 50 * V2, 3444 + 1081 * PI/4 + 50 * V2, 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2};
    int orientationStationAller[10] = {180, 90, 270, 270, 225, 270, 270, 180, 180, 0}; //attention, montée à gauche à République après le réaménagement
    double hauteurQuaiAller[10] = {4.8, 0.8, 0.8, -5.2, -2.2, 10.8 + 6.75 * PI, 10.8 + 6.75 * PI, 10.8 + 6.75 * PI, 0.8 + 6.75 * PI, 0.8 + 6.75 * PI};
    int facteursAller[10] = {130, 145, 105, 90, 80, 130, 60, 50, 25, 0};

    double minStationRetour[10] = {99997. , 100471.35, 101056.35, 101248.35 + 156 * PI/4 + 200 * PI/3, 101723.35 + 156 * PI/4 + 200 * PI/3, 102198.35 + 828 * PI/4 + 200 * PI/3, 102598.35 + 931 * PI/4 + 200 * PI/3, 103273.35 + 931 * PI/4 + 200 * PI/3, 103798.35 + 931 * PI/4 + 200 * PI/3, 104367.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2};
    double maxStationRetour[10] = {100003., 100477.35, 101062.35, 101254.35 + 156 * PI/4 + 200 * PI/3, 101729.35 + 156 * PI/4 + 200 * PI/3, 102204.35 + 828 * PI/4 + 200 * PI/3, 102604.35 + 931 * PI/4 + 200 * PI/3, 103279.35 + 931 * PI/4 + 200 * PI/3, 103804.35 + 931 * PI/4 + 200 * PI/3, 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2};
    int orientationStationRetour[10] = {0, 0, 0, 90, 90, 45, 90, 90, 90, 0};
    double hauteurQuaiRetour[10] = {0.8 + 6.75 * PI, 0.8 + 6.75 * PI, 10.8 + 6.75 * PI, 10.8 + 6.75 * PI, 10.8 + 6.75 * PI, -2.2, -5.2, 0.8, 0.8, 4.8};
    int facteursRetour[10] = {165, 100, 90, 80, 125, 70, 50, 25, 65, 0};

    int probabiliteMontee = 20; //sur 1000

    //ajout de personnages

    for (int stationAller = 0; stationAller < 10; stationAller++) {

        double p = facteurMonde * facteursAller[stationAller] * 0.000002 * (1 - exp(3 * (totalPersonnagesAller[stationAller] * 0.01 - 1)));
        int a = rand() % 1000000;

        if (a < 1000000 * p) {

            ajouterPersonnage(stationAller);

        }

    }

    for (int stationRetour = 0; stationRetour < 10; stationRetour++) {

        double p = facteurMonde * facteursRetour[stationRetour] * 0.000002 * (1 - exp(3 * (totalPersonnagesRetour[stationRetour] * 0.01 - 1)));
        int a = rand() % 1000000;

        if (a < 1000000 * p) {

            ajouterPersonnage(100 + stationRetour);

        }

    }

    if (distanceTotale <= 310) {

        emit modifierMonde(totalPersonnagesAller[0]);

    }

    else if (distanceTotale < 894 + 306 * PI/4 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[1]);

    }

    else if (distanceTotale < 1419 + 306 * PI/4 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[2]);

    }

    else if (distanceTotale < 2094 + 306 * PI/4 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[3]);

    }

    else if (distanceTotale < 2494 + 403 * PI/4 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[4]);

    }

    else if (distanceTotale < 2969 + 1081 * PI/4 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[5]);

    }

    else if (distanceTotale < 3444 + 1081 * PI/4 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[6]);

    }

    else if (distanceTotale < 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[7]);

    }

    else if (distanceTotale < 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[8]);

    }

    else if (distanceTotale < 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) {

        emit modifierMonde(totalPersonnagesAller[9]);

    }


    else if (distanceTotale < 100003) {

        emit modifierMonde(totalPersonnagesRetour[0]);

    }

    else if (distanceTotale < 100477.35) {

        emit modifierMonde(totalPersonnagesRetour[1]);

    }

    else if (distanceTotale < 101062.35) {

        emit modifierMonde(totalPersonnagesRetour[2]);

    }

    else if (distanceTotale < 101254.35 + 156 * PI/4 + 200 * PI/3) {

        emit modifierMonde(totalPersonnagesRetour[3]);

    }

    else if (distanceTotale < 101729.35 + 156 * PI/4 + 200 * PI/3) {

        emit modifierMonde(totalPersonnagesRetour[4]);

    }

    else if (distanceTotale < 102204.35 + 828 * PI/4 + 200 * PI/3) {

        emit modifierMonde(totalPersonnagesRetour[5]);

    }

    else if (distanceTotale < 102604.35 + 931 * PI/4 + 200 * PI/3) {

        emit modifierMonde(totalPersonnagesRetour[6]);

    }

    else if (distanceTotale < 103279.35 + 931 * PI/4 + 200 * PI/3) {

        emit modifierMonde(totalPersonnagesRetour[7]);

    }

    else if (distanceTotale < 103804.35 + 931 * PI/4 + 200 * PI/3) {

        emit modifierMonde(totalPersonnagesRetour[8]);

    }

    else if (distanceTotale < 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

        emit modifierMonde(totalPersonnagesRetour[9]);

    }

    //descente des passagers

    if (passagersDescendants > 0 && vitesse < 0.000001 && estEnStation(distanceTotale) && portesOuvertes) {

        if (rand() % 1000 < 215) {

            passagersDescendants--;
            capacite++;
            emit modifierCapacite(capacite);

        }

    }

    else if (frameCount % 2 == 0) {

        int facteurProba = 2;

        //déplacement des personnages montants

        for (int stationAller = 0; stationAller < 10; stationAller++) {

            if (distanceTotale >= minStationAller[stationAller] && distanceTotale <= maxStationAller[stationAller]) {

                for (int i = 0; i < 100; i++) {

                    if (abs(xAller[stationAller][i]) > 0.000001 || abs(yAller[stationAller][i]) > 0.000001) {

                        if (abs(orientationAller[stationAller][i] - orientationStationAller[stationAller]) > 0.55 * facteurProba) {

                            if (orientationAller[stationAller][i] >= 180) { //on a un intervalle de la forme [but - 180, but[

                                if (orientationAller[stationAller][i] >= orientationStationAller[stationAller] - 180 && orientationAller[stationAller][i] - orientationStationAller[stationAller] < -0.55 * facteurProba) {

                                    orientationAller[stationAller][i] = (orientationAller[stationAller][i] + facteurProba) % 360;

                                }

                                else {

                                    orientationAller[stationAller][i] = orientationAller[stationAller][i] - facteurProba;

                                    if (orientationAller[stationAller][i] < 0) {

                                        orientationAller[stationAller][i] += 360;

                                    }

                                }

                            }

                            else { //on a un ensemble de la forme [0, but[ u [but + 180, 360[

                                if (orientationAller[stationAller][i] - orientationStationAller[stationAller] < -0.55 * facteurProba || orientationAller[stationAller][i] >= orientationStationAller[stationAller] + 180) {

                                    orientationAller[stationAller][i] = (orientationAller[stationAller][i] + facteurProba) % 360;

                                }

                                else {

                                    orientationAller[stationAller][i] = orientationAller[stationAller][i] - facteurProba;

                                    if (orientationAller[stationAller][i] < -0.55 * facteurProba) {

                                        orientationAller[stationAller][i] += 360;

                                    }

                                }

                            }

                        }

                        else if (portesOuvertes) {

                            bool monteeTentee = false;

                            switch(stationAller) { //impossible de faire autrement puisque par exemple le quai de la station Boulevard de l'Hôpital est incliné de 45°

                            case 0:

                                if (yAller[stationAller][i] > -516) {

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                        emit modifierMonde(totalPersonnagesAller[0]);
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 1:

                                if (xAller[stationAller][i] > -6) {

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                        emit modifierMonde(totalPersonnagesAller[1]);
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 2:
                            case 3:

                                if (xAller[stationAller][i] < 6) { //condition de fin de quai à Bouchers, Rue des Chenilles

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);


                                        if (distanceTotale <= 1419 + 306 * PI/4 + 50 * V2) {

                                            emit modifierMonde(totalPersonnagesAller[2]);

                                        }

                                        else {

                                            emit modifierMonde(totalPersonnagesAller[3]);

                                        }

                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 4:

                                if (yAller[stationAller][i] - xAller[stationAller][i] > 1482.936074) { //condition de fin de quai à Boulevard de l'Hôpital

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                        emit modifierMonde(totalPersonnagesAller[4]); //le case 3 ne concerne que Boulevard de l'Hôpital aller
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 5:
                            case 6:

                                if (xAller[stationAller][i] < 781) {

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);

                                        if (distanceTotale <= 2969 + 1081 * PI/4 + 50 * V2) {

                                            emit modifierMonde(totalPersonnagesAller[5]);
                                            emit modifierCapacite(capacite);

                                        }

                                        else {

                                            emit modifierMonde(totalPersonnagesAller[6]);
                                            emit modifierCapacite(capacite);

                                        }

                                    }

                                }

                                break;

                            case 7:
                            case 8:

                                if (yAller[stationAller][i] > 3265) {

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        monteeTentee = true;

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);

                                        if (distanceTotale <= 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

                                            emit modifierMonde(totalPersonnagesAller[7]);
                                            emit modifierCapacite(capacite);

                                        }

                                        else {

                                            emit modifierMonde(totalPersonnagesAller[8]);
                                            emit modifierCapacite(capacite);

                                        }

                                    }

                                }

                                break;

                            case 9:

                                if (yAller[stationAller][i] < 3244.205081) { //rare + arrivée sur la voie de droite donc mouvement inversé

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                        emit modifierMonde(totalPersonnagesAller[9]);
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            }

                            if (!monteeTentee) {

                                switch(stationAller) {

                                case 1:

                                    xAller[stationAller][i] += 0.01;
                                    break;

                                case 2:
                                case 3:
                                case 5:
                                case 6:

                                    xAller[stationAller][i] -= 0.01;
                                    break;

                                case 4:

                                    xAller[stationAller][i] -= 0.005;
                                    yAller[stationAller][i] += 0.005; //et non pas 0.00707107 pour conserver la même vitesse (on se rapproche à la même vitesse du bord du quai)
                                    break;

                                case 0:
                                case 7:
                                case 8:

                                    yAller[stationAller][i] += 0.01;
                                    break;

                                case 9:

                                    yAller[stationAller][i] -= 0.01; //en principe inutile car personne ne monte au terminus
                                    break;

                                }

                            }

                        }

                    }

                }

            }

            else {

                for (int rame = 0; rame < nombreRamesMax; rame++) {

                    if (distanceTotaleAutres[rame] >= minStationAller[stationAller] && distanceTotaleAutres[rame] <= maxStationAller[stationAller]) {

                        for (int i = 0; i < 100; i++) {

                            if (abs(xAller[stationAller][i]) > 0.000001 || abs(yAller[stationAller][i]) > 0.000001) {

                                if (abs(orientationAller[stationAller][i] - orientationStationAller[stationAller]) > 0.55 * facteurProba) {

                                    if (orientationAller[stationAller][i] >= 180) { //on a un intervalle de la forme [but - 180, but[

                                        if (orientationAller[stationAller][i] >= orientationStationAller[stationAller] - 180 && orientationAller[stationAller][i] - orientationStationAller[stationAller] < -0.55 * facteurProba) {

                                            orientationAller[stationAller][i] = (orientationAller[stationAller][i] + facteurProba) % 360;

                                        }

                                        else {

                                            orientationAller[stationAller][i] = orientationAller[stationAller][i] - facteurProba;

                                            if (orientationAller[stationAller][i] < -0.55 * facteurProba) {

                                                orientationAller[stationAller][i] += 360; //12 indentations!

                                            }

                                        }

                                    }

                                    else { //on a un ensemble de la forme [0, but[ u [but + 180, 360[

                                        if (abs(orientationAller[stationAller][i] - orientationStationAller[stationAller]) > 0.55 * facteurProba || orientationAller[stationAller][i] >= orientationStationAller[stationAller] + 180) {

                                            orientationAller[stationAller][i] = (orientationAller[stationAller][i] + facteurProba) % 360;

                                        }

                                        else {

                                            orientationAller[stationAller][i] = orientationAller[stationAller][i] - facteurProba;

                                            if (orientationAller[stationAller][i] < 0) {

                                                orientationAller[stationAller][i] += 360;

                                            }

                                        }

                                    }

                                }

                                else if (rand() % 1000 < 185) {

                                    bool monteeTentee = false;

                                    switch(stationAller) { //impossible de faire autrement puisque par exemple le quai de la station Boulevard de l'Hôpital est incliné de 45°

                                    case 0:

                                        if (yAller[stationAller][i] > -516) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 1:

                                        if (xAller[stationAller][i] > -6) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 2:
                                    case 3:

                                        if (xAller[stationAller][i] < 6) { //condition de fin de quai à Bouchers, Rue des Chenilles

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 4:

                                        if (yAller[stationAller][i] - xAller[stationAller][i] > 1482.936074) { //condition de fin de quai à Boulevard de l'Hôpital

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 5:
                                    case 6:

                                        if (xAller[stationAller][i] < 781) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 7:
                                    case 8:

                                        if (yAller[stationAller][i] > 3265) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 9:

                                        if (yAller[stationAller][i] < 3244.205081) { //rare + arrivée sur la voie de droite donc mouvement inversé

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(stationAller, xAller[stationAller][i], yAller[stationAller][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    }

                                    if (!monteeTentee) {

                                        switch(stationAller) {

                                        case 1:

                                            xAller[stationAller][i] += facteurProba * 0.01; //montée à gauche à République après réaménagement
                                            break;

                                        case 2:
                                        case 3:
                                        case 5:
                                        case 6:

                                            xAller[stationAller][i] -= facteurProba * 0.01;
                                            break;

                                        case 4:

                                            xAller[stationAller][i] -= facteurProba * 0.005;
                                            yAller[stationAller][i] += facteurProba * 0.005; //et non pas 0.00707107 pour conserver la même vitesse (on se rapproche à la même vitesse du bord du quai)
                                            break;

                                        case 0:
                                        case 7:
                                        case 8:

                                            yAller[stationAller][i] += facteurProba * 0.01;
                                            break;

                                        case 9:

                                            yAller[stationAller][i] -= facteurProba * 0.01; //en principe inutile car personne ne monte au terminus
                                            break;

                                        }

                                    }

                                }

                            }

                        }

                    }

                }

            }

        }


        for (int stationRetour = 0; stationRetour < 10; stationRetour++) {

            if (distanceTotale >= minStationRetour[stationRetour] && distanceTotale <= maxStationRetour[stationRetour]) {

                for (int i = 0; i < 100; i++) {

                    if (abs(xRetour[stationRetour][i]) > 0.000001 || abs(yRetour[stationRetour][i]) > 0.000001) {

                        if (abs(orientationRetour[stationRetour][i] - orientationStationRetour[stationRetour]) > 0.55 * facteurProba) {

                            if (orientationRetour[stationRetour][i] >= 180) { //on a un intervalle de la forme [but - 180, but[

                                if (orientationRetour[stationRetour][i] >= orientationStationRetour[stationRetour] - 180 && orientationRetour[stationRetour][i] - orientationStationRetour[stationRetour] < -0.55 * facteurProba) {

                                    orientationRetour[stationRetour][i] = (orientationRetour[stationRetour][i] + facteurProba) % 360;

                                }

                                else {

                                    orientationRetour[stationRetour][i] = orientationRetour[stationRetour][i] - facteurProba;

                                    if (orientationRetour[stationRetour][i] < 0) {

                                        orientationRetour[stationRetour][i] += 360;

                                    }

                                }

                            }

                            else { //on a un ensemble de la forme [0, but[ u [but + 180, 360[

                                if (orientationRetour[stationRetour][i] - orientationStationRetour[stationRetour] < -0.55 * facteurProba || orientationRetour[stationRetour][i] >= orientationStationRetour[stationRetour] + 180) {

                                    orientationRetour[stationRetour][i] = (orientationRetour[stationRetour][i] + facteurProba) % 360;

                                }

                                else {

                                    orientationRetour[stationRetour][i] = orientationRetour[stationRetour][i] - facteurProba;

                                    if (orientationRetour[stationRetour][i] < 0) {

                                        orientationRetour[stationRetour][i] += 360;

                                    }

                                }

                            }

                        }

                        else if (portesOuvertes) {

                            bool monteeTentee = false;

                            switch(stationRetour) { //impossible de faire autrement puisque par exemple le quai de la station Boulevard de l'Hôpital est incliné de 45°

                            case 0:
                            case 1:
                            case 2:

                                if (yRetour[stationRetour][i] < 3277) {

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);

                                        if (distanceTotale <= 100003) {

                                            emit modifierMonde(totalPersonnagesRetour[0]);
                                            emit modifierCapacite(capacite);

                                        }

                                        else if (distanceTotale <= 100477.35) {

                                            emit modifierMonde(totalPersonnagesRetour[1]);
                                            emit modifierCapacite(capacite);

                                        }

                                        else {

                                            emit modifierMonde(totalPersonnagesRetour[2]);
                                            emit modifierCapacite(capacite);

                                        }

                                    }

                                }

                                break;

                            case 3:
                            case 4:

                                if (xRetour[stationRetour][i] > 769) {

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);

                                        if (distanceTotale <= 101254.35 + 156 * PI/4 + 200 * PI/3) {

                                            emit modifierMonde(totalPersonnagesRetour[3]);
                                            emit modifierCapacite(capacite);

                                        }

                                        else {

                                            emit modifierMonde(totalPersonnagesRetour[4]);
                                            emit modifierCapacite(capacite);

                                        }

                                    }

                                }

                                break;

                            case 5:

                                if (yRetour[stationRetour][i] - xRetour[stationRetour][i] < 1499.906637) { //condition de fin de quai à Boulevard de l'Hôpital

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);

                                        emit modifierMonde(totalPersonnagesRetour[5]); //le case 5 ne concerne que Boulevard de l'Hôpital retour
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 6:
                            case 7:

                                if (xRetour[stationRetour][i] > -6) { //condition de fin de quai à Bouchers, Rue des Chenilles

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);


                                        if (distanceTotale <= 102604.35 + 931 * PI/4 + 200 * PI/3) {

                                            emit modifierMonde(totalPersonnagesRetour[6]);

                                        }

                                        else {

                                            emit modifierMonde(totalPersonnagesRetour[7]);

                                        }

                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 8:

                                if (xRetour[stationRetour][i] > -15) { //condition de fin de quai à République

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);

                                        emit modifierMonde(totalPersonnagesRetour[8]);
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            case 9:

                                if (yRetour[stationRetour][i] < -504) {

                                    monteeTentee = true;

                                    if (rand() % 1000 < facteurProba * probabiliteMontee) {

                                        retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);

                                        emit modifierMonde(totalPersonnagesRetour[9]);
                                        emit modifierCapacite(capacite);

                                    }

                                }

                                break;

                            }

                            if (!monteeTentee) {

                                switch(stationRetour) {

                                case 0:
                                case 1:
                                case 2:
                                case 9:

                                    yRetour[stationRetour][i] -= 0.01;
                                    break;

                                case 3:
                                case 4:
                                case 6:
                                case 7:
                                case 8:

                                    xRetour[stationRetour][i] += 0.01; //en principe le case 8 est inutile car personne ne monte au terminus
                                    break;

                                case 5:

                                    xRetour[stationRetour][i] += 0.005;
                                    yRetour[stationRetour][i] -= 0.005; //et non pas 0.00707107 pour conserver la même vitesse (on se rapproche à la même vitesse du bord du quai)
                                    break;

                                }

                            }

                        }

                    }

                }

            }

            else {

                for (int rame = 0; rame < nombreRamesMax; rame++) {

                    if ((distanceTotaleAutres[rame] >= minStationRetour[stationRetour] && distanceTotaleAutres[rame] <= maxStationRetour[stationRetour]) || (stationRetour == 0 && distanceTotaleAutres[rame] >= 6448.61838802 && distanceTotaleAutres[rame] <= 6454.61838802)) {

                        for (int i = 0; i < 100; i++) {

                            if (abs(xRetour[stationRetour][i]) > 0.000001 || abs(yRetour[stationRetour][i]) > 0.000001) {

                                if (abs(orientationRetour[stationRetour][i] - orientationStationRetour[stationRetour]) > 0.55 * facteurProba) {

                                    if (orientationRetour[stationRetour][i] >= 180) { //on a un intervalle de la forme [but - 180, but[

                                        if (orientationRetour[stationRetour][i] >= orientationStationRetour[stationRetour] - 180 && orientationRetour[stationRetour][i] - orientationStationRetour[stationRetour] < -0.55 * facteurProba) {

                                            orientationRetour[stationRetour][i] = (orientationRetour[stationRetour][i] + facteurProba) % 360;

                                        }

                                        else {

                                            orientationRetour[stationRetour][i] = orientationRetour[stationRetour][i] - facteurProba;

                                            if (orientationRetour[stationRetour][i] < 0) {

                                                orientationRetour[stationRetour][i] += 360; //12 indentations!

                                            }

                                        }

                                    }

                                    else { //on a un ensemble de la forme [0, but[ u [but + 180, 360[

                                        if (orientationRetour[stationRetour][i] - orientationStationRetour[stationRetour] < -0.55 * facteurProba || orientationRetour[stationRetour][i] >= orientationStationRetour[stationRetour] + 180) {

                                            orientationRetour[stationRetour][i] = (orientationRetour[stationRetour][i] + facteurProba) % 360;

                                        }

                                        else {

                                            orientationRetour[stationRetour][i] = orientationRetour[stationRetour][i] - facteurProba;

                                            if (orientationRetour[stationRetour][i] < 0) {

                                                orientationRetour[stationRetour][i] += 360;

                                            }

                                        }

                                    }

                                }

                                else {

                                    bool monteeTentee = false;

                                    switch(stationRetour) { //impossible de faire autrement puisque par exemple le quai de la station Boulevard de l'Hôpital est incliné de 45°

                                    case 0:
                                    case 1:
                                    case 2:

                                        if (yRetour[stationRetour][i] < 3277) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 3:
                                    case 4:

                                        if (xRetour[stationRetour][i] > 769) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 5:

                                        if (yRetour[stationRetour][i] - xRetour[stationRetour][i] < 1499.906637) { //condition de fin de quai à Boulevard de l'Hôpital

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 6:
                                    case 7:

                                        if (xRetour[stationRetour][i] > -6) { //condition de fin de quai à Bouchers, Rue des Chenilles

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    case 8:

                                        if (xRetour[stationRetour][i] > -15) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);
                                            monteeTentee = true;

                                        }


                                        break;

                                    case 9:

                                        if (yRetour[stationRetour][i] < -504) {

                                            if (rand() % 1000 < facteurProba * probabiliteMontee) retirerPersonnageStation(100 + stationRetour, xRetour[stationRetour][i], yRetour[stationRetour][i]);
                                            monteeTentee = true;

                                        }

                                        break;

                                    }

                                    if (!monteeTentee) {

                                        switch(stationRetour) {

                                        case 0:
                                        case 1:
                                        case 2:
                                        case 9:

                                            yRetour[stationRetour][i] -= facteurProba * 0.01;
                                            break;

                                        case 3:
                                        case 4:
                                        case 6:
                                        case 7:
                                        case 8:

                                            xRetour[stationRetour][i] += facteurProba * 0.01; //en principe le case 8 est inutile car personne ne monte au terminus
                                            break;

                                        case 5:

                                            xRetour[stationRetour][i] += facteurProba * 0.005;
                                            yRetour[stationRetour][i] -= facteurProba * 0.005; //et non pas 0.00707107 pour conserver la même vitesse (on se rapproche à la même vitesse du bord du quai)
                                            break;

                                        }

                                    }

                                }

                            }

                        }

                    }

                }

            }

        }

    }

    //dessin en lui-même

    for (int stationAller = 0; stationAller < 10; stationAller++) {

        double h = hauteurQuaiAller[stationAller];

        for (int t = 0; t < 15; t++) {

            glBindTexture(GL_TEXTURE_2D, textureCouleurs[t]);

            glBegin(GL_QUADS);

            for (int i = 0; i < 100; i++) {

                if ((abs(xAller[stationAller][i]) >= 0.000001 || abs(yAller[stationAller][i]) >= 0.000001) && (textAller[stationAller][i] / 100) % 10 == t) {

                    double cosAngle = cos(orientationAller[stationAller][i] * PI / 180);
                    double sinAngle = sin(orientationAller[stationAller][i] * PI / 180);

                    double x1 = xAller[stationAller][i] - 0.2  * cosAngle;
                    double x2 = xAller[stationAller][i] - 0.03 * cosAngle;

                    double y1 = yAller[stationAller][i] - 0.2  * sinAngle;
                    double y2 = yAller[stationAller][i] - 0.03 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h      ));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 0.7));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 0.7));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h      )); //jambe "gauche"

                    x1 += 0.23 * cosAngle;
                    x2 += 0.23 * cosAngle;

                    y1 += 0.23 * sinAngle;
                    y2 += 0.23 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h      ));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 0.7));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 0.7));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h      )); //jambe "droite"

                    x1 -= 0.23 * cosAngle;
                    y1 -= 0.23 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 0.7 ));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 0.75));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 0.75));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 0.7 )); //haut de pantalon

                }

                if ((abs(xAller[stationAller][i]) >= 0.000001 || abs(yAller[stationAller][i]) >= 0.000001) && textAller[stationAller][i] % 100 == t) {

                    double cosAngle = cos(orientationAller[stationAller][i] * PI / 180);
                    double sinAngle = sin(orientationAller[stationAller][i] * PI / 180);

                    double x1 = xAller[stationAller][i] - 0.2 * cosAngle;
                    double x2 = xAller[stationAller][i] + 0.2 * cosAngle;

                    double y1 = yAller[stationAller][i] - 0.2 * sinAngle;
                    double y2 = yAller[stationAller][i] + 0.2 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 0.75));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.6 ));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.6 ));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 0.75)); //bas de chemise

                    x1 -= 0.15 * cosAngle;
                    x2 -= 0.4  * cosAngle;
                    y1 -= 0.15 * sinAngle;
                    y2 -= 0.4  * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.45));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.55));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.60));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.50)); //manche gauche

                    x1 += 0.55 * cosAngle;
                    x2 += 0.55 * cosAngle;
                    y1 += 0.55 * sinAngle;
                    y2 += 0.55 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.50));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.60));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.55));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.45)); //manche droite

                }

                if ((abs(xAller[stationAller][i]) >= 0.000001 || abs(yAller[stationAller][i]) >= 0.000001) && t == 14) {

                    double cosAngle = cos(orientationAller[stationAller][i] * PI / 180);
                    double sinAngle = sin(orientationAller[stationAller][i] * PI / 180);

                    double x1 = xAller[stationAller][i] - 0.5  * cosAngle;
                    double x2 = xAller[stationAller][i] - 0.35 * cosAngle;

                    double y1 = yAller[stationAller][i] - 0.5  * sinAngle;
                    double y2 = yAller[stationAller][i] - 0.35 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.40));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.50));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.55));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.45)); //main gauche

                    x1 += 0.85 * cosAngle;
                    x2 += 0.85 * cosAngle;
                    y1 += 0.85 * sinAngle;
                    y2 += 0.85 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.45));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.55));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.50));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.40)); //main droite

                }

            }

        glEnd();

        }

        for (int np = 0; np < npm; np++) {

            glBindTexture(GL_TEXTURE_2D, texturePersonnages1[np]);

            glBegin(GL_QUADS);

            for (int i = 0; i < 100; i++) {

                if ((abs(xAller[stationAller][i]) >= 0.000001 || abs(yAller[stationAller][i]) >= 0.000001) && textAller[stationAller][i] / 1000 == np) {

                    double cosAngle = cos(orientationAller[stationAller][i] * PI / 180);
                    double sinAngle = sin(orientationAller[stationAller][i] * PI / 180);

                    double x1 = xAller[stationAller][i] - 0.12 * cosAngle;
                    double x2 = xAller[stationAller][i] + 0.12 * cosAngle;

                    double y1 = yAller[stationAller][i] - 0.12 * sinAngle;
                    double y2 = yAller[stationAller][i] + 0.12 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.60));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.78));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.78));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.60)); //tête

                }

            }

            glEnd();

        }

    }

    for (int stationRetour = 0; stationRetour < 10; stationRetour++) {

        double h = hauteurQuaiRetour[stationRetour];

        for (int t = 0; t < 15; t++) {

            glBindTexture(GL_TEXTURE_2D, textureCouleurs[t]);

            glBegin(GL_QUADS);

            for (int i = 0; i < 100; i++) {

                if ((abs(xRetour[stationRetour][i]) >= 0.000001 || abs(yRetour[stationRetour][i]) >= 0.000001) && (textRetour[stationRetour][i] / 100) % 10 == t) {

                    double cosAngle = cos(orientationRetour[stationRetour][i] * PI / 180);
                    double sinAngle = sin(orientationRetour[stationRetour][i] * PI / 180);

                    double x1 = xRetour[stationRetour][i] - 0.2  * cosAngle;
                    double x2 = xRetour[stationRetour][i] - 0.03 * cosAngle;

                    double y1 = yRetour[stationRetour][i] - 0.2  * sinAngle;
                    double y2 = yRetour[stationRetour][i] - 0.03 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h      ));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 0.7));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 0.7));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h      )); //jambe "gauche"

                    x1 += 0.23 * cosAngle;
                    x2 += 0.23 * cosAngle;

                    y1 += 0.23 * sinAngle;
                    y2 += 0.23 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h      ));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 0.7));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 0.7));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h      )); //jambe "droite"

                    x1 -= 0.23 * cosAngle;
                    y1 -= 0.23 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 0.7 ));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 0.75));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 0.75));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 0.7 )); //haut de pantalon

                }

                if ((abs(xRetour[stationRetour][i]) >= 0.000001 || abs(yRetour[stationRetour][i]) >= 0.000001) && textRetour[stationRetour][i] % 100 == t) {

                    double cosAngle = cos(orientationRetour[stationRetour][i] * PI / 180);
                    double sinAngle = sin(orientationRetour[stationRetour][i] * PI / 180);

                    double x1 = xRetour[stationRetour][i] - 0.2 * cosAngle;
                    double x2 = xRetour[stationRetour][i] + 0.2 * cosAngle;

                    double y1 = yRetour[stationRetour][i] - 0.2 * sinAngle;
                    double y2 = yRetour[stationRetour][i] + 0.2 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 0.75));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.6 ));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.6 ));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 0.75)); //bas de chemise

                    x1 -= 0.15 * cosAngle;
                    x2 -= 0.4  * cosAngle;
                    y1 -= 0.15 * sinAngle;
                    y2 -= 0.4  * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.45));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.55));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.60));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.50)); //manche gauche

                    x1 += 0.55 * cosAngle;
                    x2 += 0.55 * cosAngle;
                    y1 += 0.55 * sinAngle;
                    y2 += 0.55 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.50));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.60));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.55));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.45)); //manche droite

                }

                if ((abs(xRetour[stationRetour][i]) >= 0.000001 || abs(yRetour[stationRetour][i]) >= 0.000001) && t == 14) {

                    double cosAngle = cos(orientationRetour[stationRetour][i] * PI / 180);
                    double sinAngle = sin(orientationRetour[stationRetour][i] * PI / 180);

                    double x1 = xRetour[stationRetour][i] - 0.5  * cosAngle;
                    double x2 = xRetour[stationRetour][i] - 0.35 * cosAngle;

                    double y1 = yRetour[stationRetour][i] - 0.5  * sinAngle;
                    double y2 = yRetour[stationRetour][i] - 0.35 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.40));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.50));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.55));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.45)); //main gauche

                    x1 += 0.85 * cosAngle;
                    x2 += 0.85 * cosAngle;
                    y1 += 0.85 * sinAngle;
                    y2 += 0.85 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.45));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.55));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.50));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.40)); //main droite

                }

            }

        glEnd();

        }

        for (int np = 0; np < npm; np++) {

            glBindTexture(GL_TEXTURE_2D, texturePersonnages1[np]);

            glBegin(GL_QUADS);

            for (int i = 0; i < 100; i++) {

                if ((abs(xRetour[stationRetour][i]) >= 0.000001 ||abs (yRetour[stationRetour][i]) >= 0.000001) && textRetour[stationRetour][i] / 1000 == np) {

                    double cosAngle = cos(orientationRetour[stationRetour][i] * PI / 180);
                    double sinAngle = sin(orientationRetour[stationRetour][i] * PI / 180);

                    double x1 = xRetour[stationRetour][i] - 0.12 * cosAngle;
                    double x2 = xRetour[stationRetour][i] + 0.12 * cosAngle;

                    double y1 = yRetour[stationRetour][i] - 0.12 * sinAngle;
                    double y2 = yRetour[stationRetour][i] + 0.12 * sinAngle;

                    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(x1), float(y1), float(h + 1.60));
                    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(x1), float(y1), float(h + 1.78));
                    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(x2), float(y2), float(h + 1.78));
                    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(x2), float(y2), float(h + 1.60)); //tête

                }

            }

            glEnd();

        }

    }

}

void maFenetre::ajouterPersonnage(int station) {

    int npm = 6;

    if (station < 100) {

        int numero = 0;
        QString sStation = "";

        do {

            numero = rand() % 100;

        } while (abs(xAller[station][numero]) > 0.000001 || abs(yAller[station][numero]) > 0.000001); //(0.0, 0.0) correspondant à un personnage non défini

        switch(station) {

        case 0: //Place des Remparts

        {

            xAller[station][numero] = 0.01 * (-32950 + rand() % 7400);
            yAller[station][numero] = 0.01 * (-52300 + rand() % 650); //quai élargi à 7.5m
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % 6) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Place des Remparts v1";
            break;


        }

        case 1: //République

        {

            xAller[station][numero] = 0.01 * (-850 + rand() % 200); //quai étroit et à gauche de la voie
            yAller[station][numero] = 0.01 * (15050 + rand() % 7400);
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % 6) + 100 * (rand() % 4) + rand() % 14;
            sStation = "République v1";
            break;

        }

        case 2: //Bouchers

        {

            xAller[station][numero] = 0.01 * (650 + rand() % 300);
            yAller[station][numero] = 0.01 * (67550 + rand() % 7400);
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14; //on évite le beige
            sStation = "Bouchers v1";
            break;

        }

        case 3: //Rue des Chenilles

        {

            xAller[station][numero] = 0.01 * (650 + rand() % 300);
            yAller[station][numero] = 0.01 * (135050 + rand() % 7400);
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Rue des Chenilles v1";
            break;

        }

        case 4: //Boulevard de l'Hôpital

        {

            double x = 0.01 * (50 + rand() % 300); //compris entre 0.5 et 3.5
            double y = 0.01 * (50 + rand() % 7400); //compris entre 0.5 et 74.5
            xAller[station][numero] = 245.663997 + 0.707107 * x + 0.707107 * y; //changement de repère
            yAller[station][numero] = 1728.600071 - 0.707107 * x + 0.707107 * y; //repère à l'entrée de la station (tympan République), haut du quai de droite côté voie
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Boulevard de l'Hôpital v1";
            break;

        }

        case 5: //Cerisiers - Framboisiers (avec carrossage)

        {

            xAller[station][numero] = 0.01 * (78150 + rand() % 200); //carrossage
            yAller[station][numero] = 0.01 * (254650 + rand() % 7400); //on néglige le fait que le quai ne fasse que 74.984489 m
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Cerisiers - Framboisiers v1";
            break;

        }

        case 6: //Blainville

        {

            xAller[station][numero] = 0.01 * (78150 + rand() % 300);
            yAller[station][numero] = 0.01 * (302150 + rand() % 7400);
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Blainville v1";
            break;

        }

        case 7: //Mirla (avec carrossage)

        {

            xAller[station][numero] = 0.01 * (114050 + rand() % 7400);
            yAller[station][numero] = 0.01 * (326250 + rand() % 200); //inversion abscisse/ordonnée + carrossage
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Mirla v1";
            break;

        }

        case 8: //Histiaux

        {

            xAller[station][numero] = 0.01 * (172550 + rand() % 7400);
            yAller[station][numero] = 0.01 * (326150 + rand() % 300);
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Histiaux v1";
            break;

        }

        case 9: //Bourse station arrivée - normalement inutile - largeur de quai réduite

        {

            xAller[station][numero] = 0.01 * (225050 + rand() % 7400); //on ne considère que la partie utile du quai
            yAller[station][numero] = 0.01 * (324470.5081 + rand() % 200);
            orientationAller[station][numero] = rand() % 360;
            textAller[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Bourse v1";
            break;

        }

        }

        totalPersonnagesAller[station]++;

        if (totalPersonnagesAller[station] == 90) {

            emit addInfo(sStation + " : saturation extrême");

        }

        else if (totalPersonnagesAller[station] == 80) {

            emit addInfo(sStation + " : saturation");

        }

        else if (totalPersonnagesAller[station] == 70) {

            emit addInfo(sStation + " : proche de la saturation");

        }

        else if (totalPersonnagesAller[station] == 60) {

            emit addInfo(sStation + " : quais très chargés");

        }

        else if (totalPersonnagesAller[station] == 50) {

            emit addInfo(sStation + " : quais chargés");

        }

    }

    else {

        station -= 100;

        int numero = 0;
        QString sStation = "";

        do {

            numero = rand() % 100;

        } while (abs(xRetour[station][numero]) > 0.000001 || abs(yRetour[station][numero]) > 0.000001); //(0.0, 0.0) correspondant à un personnage non défini

        switch(station) {

        case 0: //Bourse

        {

            xRetour[station][numero] = 0.01 * (220015.2123 + rand() % 7400);
            yRetour[station][numero] = 0.01 * (327750 + rand() % 300);
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Bourse v2";
            break;

        }

        case 1: //Histiaux

        {

            xRetour[station][numero] = 0.01 * (172550 + rand() % 7400);
            yRetour[station][numero] = 0.01 * (327750 + rand() % 300);
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14; //on évite le beige
            sStation = "Histiaux v2";
            break;

        }

        case 2: //Mirla (avec carrossage)

        {

            xRetour[station][numero] = 0.01 * (114050 + rand() % 7400);
            yRetour[station][numero] = 0.01 * (327750 + rand() % 200); //carrossage
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Mirla v2";
            break;

        }

        case 3: //Blainville

        {

            xRetour[station][numero] = 0.01 * (76550 + rand() % 300);
            yRetour[station][numero] = 0.01 * (302150 + rand() % 7400);
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Blainville v2";
            break;

        }

        case 4: //Cerisiers - Framboisiers (avec carrossage)

        {

            xRetour[station][numero] = 0.01 * (76650 + rand() % 200); //carrossage
            yRetour[station][numero] = 0.01 * (254650 + rand() % 7400); //on néglige le fait que le quai ne fasse que 74.984489 m
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Cerisiers - Framboisiers v2";
            break;

        }


        case 5: //Boulevard de l'Hôpital

        {

            double x = 0.01 * (50 + rand() % 300); //compris entre 0.5 et 3.5
            double y = 0.01 * (50 + rand() % 7400); //compris entre 0.5 et 74.5
            xRetour[station][numero] = 234.350288 + 0.707107 * x + 0.707107 * y; //changement de repère
            yRetour[station][numero] = 1739.913780 - 0.707107 * x + 0.707107 * y; //repère dans le coin du quai gauche entre le tympan République et le piédroit
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Boulevard de l'Hôpital v2";
            break;

        }

        case 6: //Rue des Chenilles

        {

            xRetour[station][numero] = 0.01 * (-950 + rand() % 300);
            yRetour[station][numero] = 0.01 * (135050 + rand() % 7400);
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Rue des Chenilles v2";
            break;

        }

        case 7: //Bouchers

        {

            xRetour[station][numero] = 0.01 * (-950 + rand() % 300);
            yRetour[station][numero] = 0.01 * (67550 + rand() % 7400);
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Bouchers v2";
            break;

        }

        case 8: //République

        {

            xRetour[station][numero] = 0.01 * (-1850 + rand() % 300);
            yRetour[station][numero] = 0.01 * (15050 + rand() % 7400);
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "République v2";
            break;

        }

        case 9: //Place des Remparts station arrivée - normalement inutile

        {

            xRetour[station][numero] = 0.01 * (-31450 + rand() % 7400);
            yRetour[station][numero] = 0.01 * (-50350 + rand() % 650); //quai élargi
            orientationRetour[station][numero] = rand() % 360;
            textRetour[station][numero] = 1000 * (rand() % npm) + 100 * (rand() % 4) + rand() % 14;
            sStation = "Place des Remparts v2";
            break;

        }

        }

        totalPersonnagesRetour[station]++;

        if (totalPersonnagesRetour[station] == 90) {

            emit addInfo(sStation + " : saturation extrême");

        }

        else if (totalPersonnagesRetour[station] == 80) {

            emit addInfo(sStation + " : saturation");

        }

        else if (totalPersonnagesRetour[station] == 70) {

            emit addInfo(sStation + " : proche de la saturation");

        }

        else if (totalPersonnagesRetour[station] == 60) {

            emit addInfo(sStation + " : quais très chargés");

        }

        else if (totalPersonnagesRetour[station] == 50) {

            emit addInfo(sStation + " : quais chargés");

        }

    }

}

void maFenetre::retirerPersonnageStation(int station, double x, double y) {

    double minStationAller[10] = {304., 888 + 306 * PI/4 + 50 * V2, 1413 + 306 * PI/4 + 50 * V2, 2088 + 306 * PI/4 + 50 * V2, 2488 + 403 * PI/4 + 50 * V2, 2963 + 1081 * PI/4 + 50 * V2, 3438 + 1081 * PI/4 + 50 * V2, 3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2};
    double maxStationAller[10] = {310., 894 + 306 * PI/4 + 50 * V2, 1419 + 306 * PI/4 + 50 * V2, 2094 + 306 * PI/4 + 50 * V2, 2494 + 403 * PI/4 + 50 * V2, 2969 + 1081 * PI/4 + 50 * V2, 3444 + 1081 * PI/4 + 50 * V2, 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2, 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2};

    double minStationRetour[10] = {99997. , 100471.35, 101056.35, 101248.35 + 156 * PI/4 + 200 * PI/3, 101723.35 + 156 * PI/4 + 200 * PI/3, 102198.35 + 828 * PI/4 + 200 * PI/3, 102598.35 + 931 * PI/4 + 200 * PI/3, 103273.35 + 931 * PI/4 + 200 * PI/3, 103798.35 + 931 * PI/4 + 200 * PI/3, 104367.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2};
    double maxStationRetour[10] = {100003., 100477.35, 101062.35, 101254.35 + 156 * PI/4 + 200 * PI/3, 101729.35 + 156 * PI/4 + 200 * PI/3, 102204.35 + 828 * PI/4 + 200 * PI/3, 102604.35 + 931 * PI/4 + 200 * PI/3, 103279.35 + 931 * PI/4 + 200 * PI/3, 103804.35 + 931 * PI/4 + 200 * PI/3, 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2};

    bool confirmation = true;

    if (station < 100) {

        int i = 0;

        for (int j = 0; j < 100; j++) { //Détermination du numéro i

            if (abs(xAller[station][j] - x) < 0.000001 && abs(yAller[station][j] - y) < 0.000001) {

                i = j;

            }

        }

        if (distanceTotale >= minStationAller[station] && distanceTotale <= maxStationAller[station]) {

            if (capacite > rand() % 550) {

                capacite--;

            }

            else {

                confirmation = false;

            }

        }

        else {

            for (int compteur = 0; compteur < nombreRamesMax; compteur++) {

                if (distanceTotaleAutres[compteur] >= minStationAller[station] && distanceTotaleAutres[compteur] <= maxStationAller[station]) {

                    if (capaciteAutres[compteur] > rand() % 100) {

                        capaciteAutres[compteur]--;

                    }

                    else {

                        confirmation = false;

                    }

                }

            }

        }

        if (confirmation) {

            xAller[station][i] = 0.;
            yAller[station][i] = 0.;
            textAller[station][i] = 0.;
            orientationAller[station][i] = 0.;
            totalPersonnagesAller[station]--;

        }

    }

    else {

        station -= 100;

        int i = 0;

        for (int j = 0; j < 100; j++) {

            if (abs(xRetour[station][j] - x) < 0.000001 && abs(yRetour[station][j] - y) < 0.000001) {

                i = j;

            }

        }

        if (distanceTotale >= minStationRetour[station] && distanceTotale <= maxStationRetour[station]) {

            if (capacite > rand() % 550) {

                capacite--; //Condition nécessaire, en effet il pourrait s'agir d'une rame "autre"

            }

            else {

                confirmation = false;

            }

        }

        else {

            for (int compteur = 0; compteur < nombreRamesMax; compteur++) {

                if (distanceTotaleAutres[compteur] >= minStationRetour[station] && distanceTotaleAutres[compteur] <= maxStationRetour[station]) {

                    if (capaciteAutres[compteur] > rand() % 100) {

                        capaciteAutres[compteur]--;

                    }

                    else {

                        confirmation = false;

                    }

                }

            }

        }

        if (confirmation) {

            xRetour[station][i] = 0.;
            yRetour[station][i] = 0.;
            textRetour[station][i] = 0.;
            orientationRetour[station][i] = 0.;
            totalPersonnagesRetour[station]--;

        }

    }

}


void maFenetre::augmenterxS() {

    xS += 0.01;

}

void maFenetre::diminuerxS() {

    xS -= 0.01;

}

void maFenetre::augmenteryS() {

    yS += 0.01;

}

void maFenetre::diminueryS() {

    yS -= 0.01;

}

void maFenetre::augmenterxS2() {

    xS += 0.05 * zS;

}

void maFenetre::diminuerxS2() {

    xS -= 0.05 * zS;

}

void maFenetre::augmenteryS2() {

    yS += 0.05 * zS;

}

void maFenetre::diminueryS2() {

    yS -= 0.05 * zS;

}

void maFenetre::augmenterzS() {

    zS += 0.2;

}

void maFenetre::diminuerzS() {

    zS -= 0.2;

}

void maFenetre::modifierPassagersDescendants(int p) {

    passagersDescendants = p;

}

bool maFenetre::passagersD() {

    return passagersDescendants == 0;

}

void maFenetre::surstationnement(int delta) {

    retard += delta;

    if (estEnStation(distanceTotale)) {

        QTimer::singleShot(1000 * delta, this, SLOT(surstationnement()));

    }

}

QString maFenetre::codePosition(double p) {

    if (p < 304)
        return "RM1-";

    if (p < 310)
        return "RM1 ";

    if (p < 325 + 306 * PI/4 + 50 * V2)
        return "RM1+";

    if (p < 888 + 306 * PI/4 + 50 * V2)
        return "REP-";

    if (p < 894 + 306 * PI/4 + 50 * V2)
        return "REP ";

    if (p < 1166 + 306 * PI/4 + 50 * V2)
        return "REP+";

    if (p < 1413 + 306 * PI/4 + 50 * V2)
        return "BCH-";

    if (p < 1419 + 306 * PI/4 + 50 * V2)
        return "BCH ";

    if (p < 1666 + 306 * PI/4 + 50 * V2)
        return "BCH+";

    if (p < 2088 + 306 * PI/4 + 50 * V2)
        return "CHE-";

    if (p < 2094 + 306 * PI/4 + 50 * V2)
        return "CHE ";

    if (p < 2266 + 356 * PI/4 + 50 * V2)
        return "CHE+";

    if (p < 2488 + 403 * PI/4 + 50 * V2)
        return "HOP-";

    if (p < 2494 + 403 * PI/4 + 50 * V2)
        return "HOP ";

    if (p < 2616 + 706 * PI/4 + 50 * V2)
        return "HOP+";

    if (p < 2963 + 1081 * PI/4 + 50 * V2)
        return "CSF-";

    if (p < 2969 + 1081 * PI/4 + 50 * V2)
        return "CSF ";

    if (p < 3166 + 1081 * PI/4 + 50 * V2)
        return "CSF+";

    if (p < 3438 + 1081 * PI/4 + 50 * V2)
        return "BLV-";

    if (p < 3444 + 1081 * PI/4 + 50 * V2)
        return "BLV ";

    if (p < 3516 + 1156 * PI/4 + 100 * PI/3 + 50 * V2)
        return "BLV+";

    if (p < 3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "MIR-";

    if (p < 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "MIR ";

    if (p < 3916 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "MIR+";

    if (p < 4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "HIS-";

    if (p < 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "HIS ";

    if (p < 4416 + 1225 * PI/4 + 250 * PI/3 + 50 * V2)
        return "HIS+";

    if (p < 4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return "BS1-";

    if (p < 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return "BS1 ";

    if (p < 4727.95 + 1705 * PI/4 + 300 * PI/3 + 50 * V2)
        return "BCLE";

    if (p < 100003)
        return "BS2 ";

    if (p < 100250)
        return "BS2+";

    if (p < 100471.35)
        return "HIS-";

    if (p < 100477.35)
        return "HIS ";

    if (p < 100750)
        return "HIS+";

    if (p < 101056.35)
        return "MIR-";

    if (p < 101062.35)
        return "MIR ";

    if (p < 101150 + 78 * PI/4 + 100 * PI/3)
        return "MIR+";

    if (p < 101248.35 + 156 * PI/4 + 200 * PI/3)
        return "BLV-";

    if (p < 101254.35 + 156 * PI/4 + 200 * PI/3)
        return "BLV ";

    if (p < 101500 + 156 * PI/4 + 200 * PI/3)
        return "BLV+";

    if (p < 101723.35 + 156 * PI/4 + 200 * PI/3)
        return "CSF-";

    if (p < 101729.35 + 156 * PI/4 + 200 * PI/3)
        return "CSF ";

    if (p < 102000 + 450 * PI/4 + 200 * PI/3)
        return "CSF+";

    if (p < 102198.35 + 828 * PI/4 + 200 * PI/3)
        return "HOP-";

    if (p < 102204.35 + 828 * PI/4 + 200 * PI/3)
        return "HOP ";

    if (p < 102400 + 875 * PI/4 + 200 * PI/3)
        return "HOP+";

    if (p < 102598.35 + 931 * PI/4 + 200 * PI/3)
        return "CHE-";

    if (p < 102604.35 + 931 * PI/4 + 200 * PI/3)
        return "CHE ";

    if (p < 102900 + 931 * PI/4 + 200 * PI/3)
        return "CHE+";

    if (p < 103273.35 + 931 * PI/4 + 200 * PI/3)
        return "BCH-";

    if (p < 103279.35 + 931 * PI/4 + 200 * PI/3)
        return "BCH ";

    if (p < 103550 + 931 * PI/4 + 200 * PI/3)
        return "BCH+";

    if (p < 103798.35 + 931 * PI/4 + 200 * PI/3)
        return "REP-";

    if (p < 103804.35 + 931 * PI/4 + 200 * PI/3)
        return "REP ";

    if (p < 104297.35 + 931 * PI/4 + 200 * PI/3)
        return "REP+";

    if (p < 104367.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "RM2-";

    if (p < 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return "RM2 ";

    return "RMB ";

}

void maFenetre::majTemps(int t) {

    temps += t;

}

int maFenetre::limiteVitesse(double d) {

    if (d < 304)
        return 55;
    if (d < 346 + 153 * PI/4)
        return 35;
    if (d < 346 + 153 * PI/4 + 50 * V2)
        return 45;
    if (d < 431 + 306 * PI/4 + 50 * V2)
        return 50;
    if (d < 766 + 306 * PI/4 + 50 * V2)
        return 60;
    if (d < 888 + 306 * PI/4 + 50 * V2)
        return 35;
    if (d < 991 + 306 * PI/4 + 50 * V2)
        return 30;
    if (d < 1413 + 306 * PI/4 + 50 * V2)
        return 55;
    if (d < 1881 + 306 * PI/4 + 50 * V2)
        return 65;
    if (d < 2088 + 306 * PI/4 + 50 * V2)
        return 60;
    if (d < 2176 + 403 * PI/4 + 50 * V2)
        return 40;
    if (d < 2488 + 403 * PI/4 + 50 * V2)
        return 50;
    if (d < 2963 + 1081 * PI/4 + 50 * V2)
        return 70;
    if (d < 3295 + 1081 * PI/4 + 50 * V2)
        return 40;
    if (d < 3438 + 1081 * PI/4 + 50 * V2)
        return 35;
    if (d < 3516 + 1225 * PI/4 + 50 * V2)
        return 30;
    if (d < 3630 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 35;
    if (d < 3788 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 60;
    if (d < 4215 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 55;
    if (d < 4398 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
        return 45;
    if (d < 4398 + 1225 * PI/4 + 247.5 * PI/3 + 50 * V2)
        return 35;
    if (d < 4489 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return 30;
    if (d < 4640 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
        return 25;
    if (d < 4630 + 1425 * PI/4 + 300 * PI/3 + 50 * V2)
        return 15;
    if (d < 99997)
        return 30;
    if (d < 100085.35)
        return 45;
    if (d < 100280.35)
        return 55;
    if (d < 100471.35)
        return 50;
    if (d < 101056.35)
        return 60;
    if (d < 101248.35 +  156   * PI/4 + 200 * PI/3)
        return 35;
    if (d < 101580.35 +  156   * PI/4 + 200 * PI/3)
        return 40;
    if (d < 101723.35 +  156   * PI/4 + 200 * PI/3)
        return 35;
    if (d < 101713.35 +  492   * PI/4 + 200 * PI/3)
        return 60;
    if (d < 101713.35 +  828   * PI/4 + 200 * PI/3)
        return 65;
    if (d < 102106.35 +  828   * PI/4 + 200 * PI/3)
        return 70;
    if (d < 102198.35 +  828   * PI/4 + 200 * PI/3)
        return 50;
    if (d < 102481.35 +  828   * PI/4 + 200 * PI/3)
        return 55;
    if (d < 102598.35 +  931   * PI/4 + 200 * PI/3)
        return 40;
    if (d < 102991.35 +  931   * PI/4 + 200 * PI/3)
        return 70;
    if (d < 103273.35 +  931   * PI/4 + 200 * PI/3)
        return 65;
    if (d < 103666.35 +  931   * PI/4 + 200 * PI/3)
        return 45;
    if (d < 103798.35 +  931   * PI/4 + 200 * PI/3)
        return 25;
    if (d < 103941.35 +  931   * PI/4 + 200 * PI/3)
        return 45;
    if (d < 104071.35 +  931   * PI/4 + 200 * PI/3)
        return 40;
    if (d < 104251.35 +  931   * PI/4 + 200 * PI/3)
        return 60;
    if (d < 104251.35 + 1078   * PI/4 + 200 * PI/3 + 50 * V2)
        return 45;
    if (d < 104251.35 + 1151.5 * PI/4 + 200 * PI/3 + 50 * V2)
        return 40;
    if (d < 104367.35 + 1225   * PI/4 + 200 * PI/3 + 50 * V2)
        return 35;
    if (d < 104515.35 + 1225   * PI/4 + 200 * PI/3 + 50 * V2)
        return 45;
    if (d < 104515.35 + 1431   * PI/4 + 200 * PI/3 + 50 * V2)
        return 40;
    if (d < 104515.35 + 1647   * PI/4 + 200 * PI/3 + 50 * V2)
        return 30;
    if (d < 104552.35 + 1719   * PI/4 + 200 * PI/3 + 50 * V2)
        return 25;
    if (d < 104645.35 + 1863   * PI/4 + 200 * PI/3 + 50 * V2)
        return 30;
    if (d < 104945.35 + 1863   * PI/4 + 200 * PI/3 + 50 * V2)
        return 55;
    if (d < 104987.35 + 2016   * PI/4 + 200 * PI/3 + 50 * V2)
        return 35;

    //470, fin quai 315  -> 155m  tolérance 145m  arrêt en station 148m
    return 0;

}

void maFenetre::afficherHorlogeRegulation() {

    if (estEnStation(distanceTotale)) {

        QTime t = QTime::currentTime();

        switch(Station) {

        case 1:

            dessinHR(t.hour(), t.minute(), t.second(), (heureDepartStation % 3600) / 60, heureDepartStation % 60, -250.0, -516.20, 6.50, 180);
            break;

        case 2:

            dessinHR(t.hour(), t.minute(), t.second(), (heureDepartStation % 3600) / 60, heureDepartStation % 60, -5.92, 230, 2.5, 270);
            //angle de 270°, puisque l'angle de 90° est lié à la montée à gauche!
            break;

        case 7:

            dessinHR(t.hour(), t.minute(), t.second(), (heureDepartStation % 3600) / 60, heureDepartStation % 60, 780.08, 3101.0, 12.50 + 6.75 * PI, 270);
            break;

        case 101:

            dessinHR(t.hour(), t.minute(), t.second(), (heureDepartStation % 3600) / 60, heureDepartStation % 60, 2194.652123, 3276.08, 2.50 + 6.75 * PI, 0);
            break;

        case 104:

            dessinHR(t.hour(), t.minute(), t.second(), (heureDepartStation % 3600) / 60, heureDepartStation % 60, 769.92, 3016.0, 12.50 + 6.75 * PI, 90);
            break;

        case 109:

            dessinHR(t.hour(), t.minute(), t.second(), (heureDepartStation % 3600) / 60, heureDepartStation % 60, -14.08, 145.0, 2.5, 90);
            break;

        }

    }

}

void maFenetre::dessinHR(int hC, int mC, int sC, int mD, int sD, double xIni, double yIni, double zIni, double angle) {

    angle *= PI / 180.;

    int a = hC / 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni                    ), float(yIni                    ), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni                    ), float(yIni                    ), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni       ));

    glEnd();

    a = hC % 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni       ));

    glEnd();

    a = mC / 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni       ));

    glEnd();

    a = mC % 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni       ));
    glEnd();

    a = sC / 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni       ));

    glEnd();

    a = sC % 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.72 * sin(angle)), float(yIni + 0.72 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.72 * sin(angle)), float(yIni + 0.72 * cos(angle)), float(zIni       ));

    glEnd();


    a = mD / 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni - 0.15));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni - 0.15));

    glEnd();

    a = mD % 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni - 0.15));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni - 0.15));

    glEnd();

    a = sD / 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni - 0.15));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni - 0.15));

    glEnd();

    a = sD % 10;

    if (a > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[a - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[a]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni - 0.15));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.72 * sin(angle)), float(yIni + 0.72 * cos(angle)), float(zIni       ));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.72 * sin(angle)), float(yIni + 0.72 * cos(angle)), float(zIni - 0.15));

    glEnd();

}

void maFenetre::dessinIQ(bool positif, int m, int s, double xIni, double yIni, double zIni, double angle) {

    angle *= PI / 180.;

    if (positif) {

        glBindTexture(GL_TEXTURE_2D, textureIQ3[2]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ3[1]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni                    ), float(yIni                    ), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni                    ), float(yIni                    ), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni       ));

    glEnd();

    int dm = m / 10;

    if (dm > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[dm - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[dm]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.12 * sin(angle)), float(yIni + 0.12 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni       ));

    glEnd();

    m %= 10;

    if (m > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[m - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[m]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.24 * sin(angle)), float(yIni + 0.24 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni       ));

    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIQ3[0]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.36 * sin(angle)), float(yIni + 0.36 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni       ));

    glEnd();

    int ds = s / 10;

    if (ds > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[ds - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[ds]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.48 * sin(angle)), float(yIni + 0.48 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni       ));

    glEnd();

    s %= 10;

    if (s > 5) {

        glBindTexture(GL_TEXTURE_2D, textureIQ2[s - 5]);

    }

    else {

        glBindTexture(GL_TEXTURE_2D, textureIQ1[s]);

    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni       ));
    glTexCoord2f(0.0f, 1.0f); glVertex3f( float(xIni - 0.60 * sin(angle)), float(yIni + 0.60 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 1.0f); glVertex3f( float(xIni - 0.72 * sin(angle)), float(yIni + 0.72 * cos(angle)), float(zIni + 0.15));
    glTexCoord2f(1.0f, 0.0f); glVertex3f( float(xIni - 0.72 * sin(angle)), float(yIni + 0.72 * cos(angle)), float(zIni       ));

    glEnd();

}

void maFenetre::setPortes(bool ptesOuvertes) {

    portesOuvertes = ptesOuvertes;

}

double maFenetre::rougeCiel() {

    int d = QDate::currentDate().dayOfYear(); //1 correspond au 1er janvier
    d = (d + 10) % 366; //11 correspond au 1er janvier, 0 au 21 décembre
    int t = QTime::currentTime().msecsSinceStartOfDay() / 1000;

    double m = -4 * cos(d * PI / 183);

    double intensite = (t - 3600 * (7 - m/2)) * (t - 3600 * (21 + m/2)) * 8. / (-200000000 * (12 + m));

    return 0.5 * min(1, max(0, intensite));

}

double maFenetre::vertCiel() {

    int d = QDate::currentDate().dayOfYear(); //1 correspond au 1er janvier
    d = (d + 10) % 366; //11 correspond au 1er janvier, 0 au 21 décembre
    int t = QTime::currentTime().msecsSinceStartOfDay() / 1000;

    double m = -4 * cos(d * PI / 183);

    double intensite = (t - 3600 * (7 - m/2)) * (t - 3600 * (21 + m/2)) * 8. / (-200000000 * (12 + m));

    return 0.7 * min(1, max(0, intensite));

}

double maFenetre::bleuCiel() {

    int d = QDate::currentDate().dayOfYear(); //1 correspond au 1er janvier
    d = (d + 10) % 366; //11 correspond au 1er janvier, 0 au 21 décembre
    int t = QTime::currentTime().msecsSinceStartOfDay() / 1000;

    double m = -4 * cos(d * PI / 183);

    double intensite = (t - 3600 * (7 - m/2)) * (t - 3600 * (21 + m/2)) * 8. / (-200000000 * (12 + m));

    return 1.0 * min(1, max(0, intensite));

}

double maFenetre::min(double a, double b) {if (a < b) return a; else return b;}

double maFenetre::max(double a, double b) {if (a > b) return a; else return b;}