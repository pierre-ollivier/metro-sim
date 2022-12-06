#ifndef MAFENETRE_H
#define MAFENETRE_H

#include "fenetreprincipale.h"
#include "fenetrechargement.h"
#include <QtOpenGL>
#include <QGLWidget>
#include <GL/glu.h>
#include <QImage>
#include <ctime>
#include <cstdlib>


class maFenetre : public FenetrePrincipale
{
    Q_OBJECT

public:

    explicit maFenetre(QWidget *parent = nullptr, int nbRames = 0, int d = 0, int fa = 1, bool modeDemo = false, bool modeSatellite = false);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void loadTexture(QString textureName);
    void keyPressEvent(QKeyEvent *keyEvent);

public slots:

    QString codePosition(double p);

    double abs(double x);

    double x(double d);
    double y(double d);
    double z(double d);
    double r(double d);
    void setVitesse(double v);
    void setVitesseAutres(int n, double v);
    double distTotale();
    double distTotaleAutres(int n);
    void setDistTotale(double d);
    void setDistTotaleAutres(int n, double d);
    void setBiaisAutres(int n, double augmentation);
    void arriveeEnStation(int s);
    void augmenterRot(double r);
    bool ramePresente(double min, double max);
    int numeroFeu(double d);
    double positionFeu(int n);
    int couleurFeu(int n);
    int avanceRame(int n);
    double position_temps(int t);
    int temps_position(double p);
    int numeroStation(double d);
    int limiteVitesse(double d);

    bool passagersD();

    void avancerC();
    void tournerTC();
    void tournerHC();

    void afficherTempsAttente();
    void dessinerPetitsBoutsTunnel();
    void afficherFeux();
    void afficherTIV();
    void afficherCachesTIV();
    void afficherRetard();
    void dessinerPersonnages();

    bool estEnStation(double d);
    bool estEnCabine();

    void remettreEnCabine();

    void ajouterPersonnage(int station);
    void retirerPersonnageStation(int station, double x, double y);

    double exp(double x);

    void augmenterxS();
    void diminuerxS();

    void augmenteryS();
    void diminueryS();

    void augmenterzS();
    void diminuerzS();

    void augmenterxS2();
    void diminuerxS2();

    void augmenteryS2();
    void diminueryS2();
    void modifierPassagersDescendants(int p);

    void surstationnement(int delta = 6);
    void majTemps(int t);

    void afficherHorlogeRegulation();
    void dessinHR(int hC, int mC, int sC, int mD, int sD, double xIni, double yIni, double zIni, double angle);
    void dessinIQ(bool positif, int m, int s, double xIni, double yIni, double zIni, double angle);
    void ajoutTIV(int valeur, double xCen, double yCen, double zCen, double angle, bool fermetureDroite = false, bool fermetureGauche = true, bool femetureHaut = true);
    void ajoutFeu(int couleur, double xCen, double yCen, double zCen, double angle, bool fermetureDroite = false, bool fermetureGauche = true, bool femetureHaut = true);

    void setPortes(bool ptesOuvertes);

    double rougeCiel();
    double vertCiel();
    double bleuCiel();

    double min(double a, double b);
    double max(double a, double b);

signals:

    void changementVitesse();
    void changementVitesseAutres(int);
    int emettreRetard(int);
    void modifierMonde(int);
    void modifierCapacite(int);
    void addInfo(QString);
    void emettreIconePert(int);

    void emettreTextureChargee(int);
    void emettreMaxTextures(int);



private:

    GLuint textureStations[10];
    GLuint textureTIV[13];
    GLuint texture[6];
    GLuint texture2[6];
    GLuint texture3[6];
    GLuint texture4[5];
    GLuint texture5[5];
    GLuint textureChiffres1[5];
    GLuint textureChiffres2[5];
    GLuint textureTympansStations[10];
    GLuint textureIDD1Stations[10];
    GLuint textureIDD2Stations[10];
    GLuint texture6[5];
    GLuint texture7[3];
    GLuint textureIQ1[5];
    GLuint textureIQ2[5];
    GLuint textureIQ3[3];
    GLuint texturePlaceDesRemparts[6];
    GLuint textureCouleurs[15];
    GLuint texturePersonnages1[7];

    FenetreChargement *fc;


    /*Procédure d'ajout de textures:
     * Modifier la valeur par défaut de npm dans les fonctions ajouterPersonnage et dessinerPersonnage
     * Ajouter les textures -> modification de la fonction loadTexture et appel dans le constructeur
     */

    int frameCount = 0;

    bool satellite = false;

    int passagersDescendants = 0;

    double tempsImmobilisation[11] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    double f_x = 0.0;
    double f_y = 0.0;
    double f_z = -1.5;

    double c_x = 0.0;
    double c_y = 0.0;


    double rot = 0.0;
    double rot2 = 0.0;
    double rotc = 0.0;

    double distanceTotale = 0.0;
    double distanceTotaleAutres[11] = {1000.0, 2000.0, 3200.0, 4000.0, 5000.0, 5440.0, 101057.0, 102000.0, 103000.0, 104000.0, 104500.0};


    int numeroTexture = 0;

    int retard = 0;
    int Station = 0;

    double vitesse = 0.0;
    double vitesseAutres[11] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    int nombreRames = 10;
    const int nombreRamesMax = 11;

    double temps = 0.0;
    double tempsAutres = 0.0;
    double biais_temps[11] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};

    int feuAller[100];
    int feuRetour[100];
    int facteurAcceleration = 1;

    int avance[11];

    double xAller[10][100];
    double yAller[10][100];
    int textAller[10][100];
    int orientationAller[10][100]; //en °

    int totalPersonnagesAller[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    double xRetour[10][100];
    double yRetour[10][100];
    int textRetour[10][100];
    int orientationRetour[10][100]; //en °

    int totalPersonnagesRetour[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    int capacite = 550;
    int capaciteAutres[11] = {550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550};

    double facteurMonde = 1.0;

    bool demo = false;

    int voieEntreeRepublique = 3;

    double xS = 0.;
    double yS = 0.;
    double zS = 30.;

    int heureArriveeStation = 0;
    int heureDepartStation = 0;

    bool portesOuvertes = false;

    double alpha = -0.2;
    double beta = 0.08; //Compris entre 0 et PI/2 de jour

    double luminanceStations[11] = {1, 0.85, 0.75, 0.74, 0.78, 0.92, 0.86, 0.95, 0.91, 0.93, 1}; //de Place des Remparts à Bourse arrivée/Bourse départ

    //int orientationStationAller[10] = {180, 90, 270, 270, 225, 270, 270, 180, 180, 0}; //attention, montée à gauche à République après le réaménagement
    //int orientationStationRetour[10] = {0, 0, 0, 90, 90, 45, 90, 90, 90, 0};



};

#endif // MAFENETRE_H
