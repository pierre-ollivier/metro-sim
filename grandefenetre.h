#ifndef GRANDEFENETRE_H
#define GRANDEFENETRE_H

#include <QWidget>
#include <QtWidgets>
#include "fenetreprincipale.h"
#include "mafenetre.h"
#include <QGLWidget>
#include <QSound>
#include "fenetrechargement.h"

class GrandeFenetre : public QWidget
{

    Q_OBJECT

public:

    GrandeFenetre(QWidget *parent = nullptr, int nbRames = 0, int positionDepart = 0, bool modeDemo = false, bool modeSatellite = false);
    void keyPressEvent(QKeyEvent *keyEvent);

public slots:

    void ouvrirPortes();
    void finOuverturePortes();
    void sonnerPortes();
    void finSonneriePortes();
    void fermerPortes();
    void finFermeturePortes();
    void joker();
    void accelerer();
    void accelererAutres(int n);
    void relachement();
    void recevoirRetard(int r);

    void loadPA();

    int lecturePA();
    int lecturePAAutres(int n);
    void changementSliderPACM(int);
    void finArretAutres0();
    void finArretAutres1();
    void finArretAutres2();
    void finArretAutres3();
    void finArretAutres4();
    void finArretAutres5();
    void finArretAutres6();
    void finArretAutres7();
    void finArretAutres8();
    void finArretAutres9();
    void finArretAutres10();
    void majLabelInfo(QString s);

    void dessinerCarte();

    void effetsSonores();

    int loi_binomiale(int n, double p);
    QString codePosition(double p);
    bool estEnStation(double d);
    bool estEnCourbe(double d);

    void setLabelMonde(int n);
    void setLabelCapacite(int n);
    void recevoirIconePert(int i);

    void afficherLimiteVitesse();


signals:

    double vit(double v);
    double vitAutres(int n, double v);
    int pourcentageChargement(int);

private:

    maFenetre *f;

    QGraphicsScene *sceneCompteur;
    QGraphicsScene *sceneCarte;

    QPushButton *boutonOuvrir;
    QPushButton *boutonSonner;
    QPushButton *boutonFermer;
    QPushButton *boutonJoker;
    QLabel *labelPA;
    QLabel *labelCM;
    QSlider *sliderPACM;
    QLabel *labelPA2;
    QLabel *labelCM2;
    QSlider *sliderPACM2;

    QGraphicsPolygonItem *aiguille;
    QPainterPath *pathVitesse;
    QGraphicsPathItem *ancienPath;

    QGraphicsTextItem *g0, *g10, *g20, *g30, *g40, *g50, *g60, *g70, *g80;

    QTabWidget *tab;
    QWidget *page1;
    QLabel *page2;
    QWidget *page3;
    QWidget *page4;

    QGridLayout *layout;
    QGridLayout *layoutGauche;
    QGridLayout *layoutCommandes;
    QGridLayout *layoutStation;
    QGridLayout *layoutCarte;

    QGraphicsView *vueCompteur;
    QGraphicsView *vueCarte;

    QSlider *slider;

    QLabel *labelRetard;
    QGraphicsTextItem *labelHeure;

    QLabel *IQHeure;
    QLabel *IQHeureTheo;
    QLabel *IQCM;
    QLabel *labelMonde;
    QLabel *labelCapacite;

    QTimer *timer;
    QTimer *timerSonnerie;

    QSound *sonOuverture;
    QSound *sonSonnerie;
    QSound *sonFermeture;
    QSound *sonAiguillage;
    QSound *sonVitesse55;
    QSound *sonCourbe22;
    QSound *sonCourbe33;
    QSound *sonCourbe44;
    QSound *sonCourbe50;
    QSound *sonDroite31;
    QSound *sonDroite46;
    QSound *sonDroite60;
    QSound *sonDemarrage;
    QSound *sonAccelerationDroite;

    FenetreChargement *chargement;


    QGraphicsTextItem *tx0;
    QGraphicsTextItem *tx1;
    QGraphicsTextItem *tx2;
    QGraphicsTextItem *tx3;
    QGraphicsTextItem *tx4;
    QGraphicsTextItem *tx5;
    QGraphicsTextItem *tx6;
    QGraphicsTextItem *tx7;
    QGraphicsTextItem *tx8;
    QGraphicsTextItem *tx9;


    double vitesse = 0.0;
    double vitesseAutres[11] = {16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0};
    double retardAutres[11] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //en s
    int coefficientRetardAutres[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //en s
    int numeroAutres[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int arretTheo1[11] = {60000, 23000, 17000, 17000, 17000, 20000, 17000, 17000, 17000, 20000, 60000}; //dernier nombre normalement inutile
    int arretTheo2[11] = {60000, 17000, 17000, 17000, 20000, 17000, 17000, 18000, 23000, 20000, 60000}; //dernier nombre normalement inutile
    int stationAutres[11] = {2, 4, 5, 7, 8, 9, 102, 104, 105, 107, 108};

    int horairesTheoAller[11] = {144, 54, 55, 47, 78, 52, 61, 51, 67, 89, 0}; //0
    int horairesTheoRetour[11] = {47, 54, 62, 54, 76, 49, 57, 62, 85, 124, 0};//0
    bool portesOuvertes = false;
    bool enStation = false;
    bool arretEffectue = false;
    bool initPA = false;
    bool demo = false;
    bool satellite = false;
    bool passagersDescendus = false;

    bool arretEffectueAutres[11] = {true, true, true, true, true, true, true, true, true, true, true};
    bool affichageEffectueAutres[11] = {true, true, true, true, true, true, true, true, true, true, true};
    int nombreRames = 11;
    int nombreRamesMax = 11;
    int cFeu[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int facteurAcceleration = 1;

    int Station = 1;
    int retard = 0;

    int nbAiguillages = 1;
    int limiteVitesse = 0;

    bool sonVitesse55Joue = false;
    bool sonCourbe22Joue = false;
    bool sonCourbe33Joue = false;
    bool sonCourbe44Joue = false;
    bool sonCourbe50Joue = false;
    bool sonDroite31Joue = false;
    bool sonDroite46Joue = false;
    bool sonDroite60Joue = false;
    bool sonDemarrageJoue = false;
    bool sonAccelerationDroiteJoue = false;

    double xS = 0;
    double yS = 0;
    double zS = 30;

    int capacite = 550;

    FenetreChargement *g;

    int vitessesPAAller[10][1100];
    int vitessesPARetour[10][1100];

};

#endif // GRANDEFENETRE_H
