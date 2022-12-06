#include "grandefenetre.h"
#include "mafenetre.h"
#define PI 3.1415926535
#define V2 1.4142135624
#include <ctime>
#include <cstdlib>

GrandeFenetre::GrandeFenetre(QWidget *parent, int nbRames, int positionDepart, bool modeDemo, bool modeSatellite): QWidget(parent)
{

    QMessageBox::information(this, "Simulateur de métro", "Bienvenue dans le simulateur!\nDurée théorique d'un aller-retour: 30 min.\nBon jeu!");
    chargement = new FenetreChargement(100);
    QObject::connect(this, SIGNAL(pourcentageChargement(int)), chargement, SLOT(changerValeur(int)));
    chargement->move(0, 0);

    emit pourcentageChargement(1);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 1100; j++) {
            vitessesPAAller[i][j] = -400;
            vitessesPARetour[i][j] = -400;
        }
    }

    loadPA();

    emit pourcentageChargement(3);

    demo = modeDemo;
    satellite = modeSatellite;


    nombreRames = nbRames;

    sonOuverture = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Ouverture portes.wav");
    sonSonnerie = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Sonnerie métro.wav");
    sonFermeture = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Fermeture portes.wav");
    sonAiguillage = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Aiguillage 35.wav");
    sonVitesse55 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Vitesse stable 55.wav");
    sonCourbe22 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Courbe 22.wav");
    sonCourbe33 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Courbe 33.wav");
    sonCourbe44 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Courbe 44.wav");
    sonCourbe50 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Courbe 50.wav");
    sonDroite31 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Droite 31.wav");
    sonDroite46 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Droite 46.wav");
    sonDroite60 = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Droite 60.wav");
    sonDemarrage = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Son démarrage.wav");
    sonAccelerationDroite = new QSound("D:/Users/pioll/Documents/Enregistrements audio/Accélération droite.wav");

    emit pourcentageChargement(4);

    sonVitesse55->setLoops(0);
    sonCourbe22->setLoops(0);
    sonCourbe33->setLoops(0);
    sonCourbe44->setLoops(0);
    sonCourbe50->setLoops(0);
    sonDroite31->setLoops(0);
    sonDroite46->setLoops(0);
    sonDroite60->setLoops(0);

    if (demo) {

        nbRames = 0;
        nombreRames = 0;
        positionDepart = 0;
        facteurAcceleration = 5;

    }

    else if (modeSatellite) {

        nbRames = 0;
        nombreRames = 0;
        positionDepart = 0;
        facteurAcceleration = 1;
        satellite = true;

    }

    else {

        facteurAcceleration = 1;

    }

    srand(unsigned(time(nullptr)));

    tx0 = new QGraphicsTextItem("Place des Remparts");
    tx1 = new QGraphicsTextItem("République");
    tx2 = new QGraphicsTextItem("Bouchers");
    tx3 = new QGraphicsTextItem("Rue des Chenilles");
    tx4 = new QGraphicsTextItem("Boulevard de l'Hôpital");
    tx5 = new QGraphicsTextItem("Cerisiers - Framboisiers");
    tx6 = new QGraphicsTextItem("Blainville");
    tx7 = new QGraphicsTextItem("Mirla");
    tx8 = new QGraphicsTextItem("Histiaux");
    tx9 = new QGraphicsTextItem("Bourse");

    layout = new QGridLayout();

    sceneCompteur = new QGraphicsScene();

    sceneCompteur->setSceneRect(-200, -200, 400, 341);
    sceneCompteur->setBackgroundBrush(QBrush(QColor(192, 192, 192)));

    vueCompteur = new QGraphicsView(sceneCompteur);

    sceneCarte = new QGraphicsScene();

    sceneCarte->addItem(tx0);
    sceneCarte->addItem(tx1);
    sceneCarte->addItem(tx2);
    sceneCarte->addItem(tx3);
    sceneCarte->addItem(tx4);
    sceneCarte->addItem(tx5);
    sceneCarte->addItem(tx6);
    sceneCarte->addItem(tx7);
    sceneCarte->addItem(tx8);
    sceneCarte->addItem(tx9);

    vueCarte = new QGraphicsView(sceneCarte);

    QGraphicsEllipseItem *e;
    e = new QGraphicsEllipseItem();
    e->setSpanAngle(270 * 16);
    e->setStartAngle(-45 * 16);
    e->setRect(-200, -200, 400, 400);

    ancienPath = new QGraphicsPathItem();
    pathVitesse = new QPainterPath();
    pathVitesse->arcMoveTo(-200, -200, 400, 400, 216);
    pathVitesse->arcTo(-200, -200, 400, 400, 216, -306);

    ancienPath = sceneCompteur->addPath(*pathVitesse, QPen(QBrush(QColor(15, 185, 15)), 5));

    aiguille = new QGraphicsPolygonItem();
    aiguille->setBrush(QBrush(QColor(255, 10, 10)));

    QPolygonF p;

    p << QPoint(0, 0) << QPoint(12, 12) << QPoint(-200, 0) << QPoint(12, -12) << QPoint(0, 0);

    aiguille->setPolygon(p);
    aiguille->setPos(0, 0);
    aiguille->setRotation(-30);

    sceneCompteur->addItem(aiguille);
    sceneCompteur->addItem(e);

    QPen pen(QBrush(QColor(0, 0, 0)), 3);

    sceneCompteur->addLine(181 * cos(PI/6), 181*sin(PI/6), 201 * cos(PI/6), 201*sin(PI/6), pen);
    sceneCompteur->addLine(181 * cos(0*PI/6), 181*sin(0*PI/6), 201 * cos(0*PI/6), 201*sin(0*PI/6), pen);
    sceneCompteur->addLine(181 * cos(-PI/6), 181*sin(-PI/6), 201 * cos(-PI/6), 201*sin(-PI/6), pen);
    sceneCompteur->addLine(181 * cos(-2*PI/6), 181*sin(-2*PI/6), 201 * cos(-2*PI/6), 201*sin(-2*PI/6), pen);
    sceneCompteur->addLine(181 * cos(-3*PI/6), 181*sin(-3*PI/6), 201 * cos(-3*PI/6), 201*sin(-3*PI/6), pen);
    sceneCompteur->addLine(181 * cos(-4*PI/6), 181*sin(-4*PI/6), 201 * cos(-4*PI/6), 201*sin(-4*PI/6), pen);
    sceneCompteur->addLine(181 * cos(-5*PI/6), 181*sin(-5*PI/6), 201 * cos(-5*PI/6), 201*sin(-5*PI/6), pen);
    sceneCompteur->addLine(181 * cos(-6*PI/6), 181*sin(-6*PI/6), 201 * cos(-6*PI/6), 201*sin(-6*PI/6), pen);
    sceneCompteur->addLine(181 * cos(-7*PI/6), 181*sin(-7*PI/6), 201 * cos(-7*PI/6), 201*sin(-7*PI/6), pen);

    for (int compteur = -13; compteur <= 1; compteur += 2) {

        sceneCompteur->addLine(191 * cos(compteur*PI/12), 191 * sin(compteur*PI/12), 200 * cos(compteur*PI/12), 200 * sin(compteur*PI/12));

    }

    for (int compteur = -72; compteur <= 12; compteur++) {

        sceneCompteur->addLine(196 * cos(compteur*PI/60), 196 * sin(compteur*PI/60), 200 * cos(compteur*PI/60), 200*sin(compteur*PI/60));

    }

    g0 = new QGraphicsTextItem("0");
    g10 = new QGraphicsTextItem("10");
    g20 = new QGraphicsTextItem("20");
    g30 = new QGraphicsTextItem("30");
    g40 = new QGraphicsTextItem("40");
    g50 = new QGraphicsTextItem("50");
    g60 = new QGraphicsTextItem("60");
    g70 = new QGraphicsTextItem("70");
    g80 = new QGraphicsTextItem("80");

    g0->setFont(QFont("Arial", 15));
    g10->setFont(QFont("Arial", 15));
    g20->setFont(QFont("Arial", 15));
    g30->setFont(QFont("Arial", 15));
    g40->setFont(QFont("Arial", 15));
    g50->setFont(QFont("Arial", 15));
    g60->setFont(QFont("Arial", 15));
    g70->setFont(QFont("Arial", 15));
    g80->setFont(QFont("Arial", 15));

    g0->setPos(160 * cos(-7*PI/6) - 16, 160* sin(-7*PI/6) - 17);
    g10->setPos(160 * cos(-6*PI/6) - 16, 160* sin(-6*PI/6) - 17);
    g20->setPos(160 * cos(-5*PI/6) - 16, 160* sin(-5*PI/6) - 17);
    g30->setPos(160 * cos(-4*PI/6) - 16, 160* sin(-4*PI/6) - 17);
    g40->setPos(160 * cos(-3*PI/6) - 16, 160* sin(-3*PI/6) - 17);
    g50->setPos(160 * cos(-2*PI/6) - 16, 160* sin(-2*PI/6) - 17);
    g60->setPos(160 * cos(-1*PI/6) - 16, 160* sin(-1*PI/6) - 17);
    g70->setPos(160 * cos(0*PI/6) - 16, 160* sin(0*PI/6) - 17);
    g80->setPos(160 * cos(1*PI/6) - 16, 160* sin(1*PI/6) - 17);

    sceneCompteur->addItem(g0);
    sceneCompteur->addItem(g10);
    sceneCompteur->addItem(g20);
    sceneCompteur->addItem(g30);
    sceneCompteur->addItem(g40);
    sceneCompteur->addItem(g50);
    sceneCompteur->addItem(g60);
    sceneCompteur->addItem(g70);
    sceneCompteur->addItem(g80);

    page1 = new QWidget();
    page2 = new QLabel("Initialisation...\n");
    page2->setFont(QFont("Courier New", 8));

    page3 = new QWidget();

    page4 = new QWidget();

    tab = new QTabWidget();
    tab->addTab(page1, "Conduite");
    tab->addTab(page2, "Informations");
    tab->addTab(page3, "En station");
    tab->addTab(page4, "Carte");
    tab->setAutoFillBackground(true);
    tab->setTabIcon(1, QIcon("D:/Users/pioll/Mes programmes/NouveauSimMetro/Pictogrammes/Vert.png"));
    tab->setMaximumWidth(450);

    page1->setPalette(QPalette(QPalette::Base, QColor(0, 0, 0)));
    page1->setAutoFillBackground(true);

    page2->setPalette(QPalette(QPalette::Base, QColor(205, 205, 205)));
    page2->setAutoFillBackground(true);

    layout = new QGridLayout();
    layoutGauche = new QGridLayout();
    layoutCommandes = new QGridLayout();
    layoutStation = new QGridLayout();
    layoutCarte = new QGridLayout();

    slider = new QSlider();
    slider->setOrientation(Qt::Vertical);

    slider->setMinimum(-300);
    slider->setMaximum(300);
    slider->setTickInterval(100);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setFixedWidth(200);
    slider->setMinimumHeight(420);

    boutonOuvrir = new QPushButton("O");
    boutonSonner = new QPushButton("S");
    boutonFermer = new QPushButton("F");
    boutonSonner->setCheckable(true);
    boutonJoker = new QPushButton("Joker");

    boutonOuvrir->setStyleSheet("QPushButton { background-color: green; }");
    boutonSonner->setStyleSheet("QPushButton { background-color: yellow; }");
    boutonFermer->setStyleSheet("QPushButton { background-color: red; }");
    boutonJoker->setStyleSheet("QPushButton { background-color: blue; }");

    this->setStyleSheet("QTabWidget { color: black; }");

    QObject::connect(boutonOuvrir, SIGNAL(clicked()), this, SLOT(ouvrirPortes()));
    QObject::connect(boutonSonner, SIGNAL(clicked()), this, SLOT(sonnerPortes()));
    QObject::connect(boutonFermer, SIGNAL(clicked()), this, SLOT(fermerPortes()));
    QObject::connect(boutonJoker, SIGNAL(clicked()), this, SLOT(joker()));

    labelPA = new QLabel("<font color = \"#FFFFFF\">PA</font>");
    labelCM = new QLabel("<font color = \"#FFFFFF\">CM</font>");

    labelPA2 = new QLabel("<font color = \"#000000\">PA</font>");
    labelCM2 = new QLabel("<font color = \"#000000\">CM</font>");

    labelMonde = new QLabel("Monde: ---");
    labelMonde->setFont(QFont("Ebrima", 14));

    labelCapacite = new QLabel("Places restantes: 550");
    labelCapacite->setFont(QFont("Ebrima", 14));

    sliderPACM = new QSlider(Qt::Horizontal);
    sliderPACM->setMinimum(0);
    sliderPACM->setMaximum(1);
    sliderPACM->setValue(1);

    sliderPACM2 = new QSlider(Qt::Horizontal);
    sliderPACM2->setMinimum(0);
    sliderPACM2->setMaximum(1);
    sliderPACM2->setValue(1);

    boutonSonner->setDisabled(true);
    boutonFermer->setDisabled(true);

    vitesse = 0;

    labelRetard = new QLabel("<font color = \"#FF9505\">.</font>");
    labelRetard->setFont(QFont("Arial", 30));
    labelRetard->setAlignment(Qt::AlignCenter);
    labelRetard->setHidden(true);

    labelHeure = new QGraphicsTextItem();
    labelHeure->setHtml(QTime::currentTime().toString("HH:mm"));
    labelHeure->setFont(QFont("Arial", 20));

    sceneCompteur->addItem(labelHeure);
    labelHeure->setPos(-40, 107);

    IQHeure = new QLabel("<font color = \"#FF9505\">000000</font>");
    IQHeure->setFont(QFont("Arial", 20));
    IQHeure->setAlignment(Qt::AlignRight);
    IQHeure->setHidden(true);

    IQHeureTheo = new QLabel("<font color = \"#FF9505\">0018</font>");
    IQHeureTheo->setFont(QFont("Arial", 20));
    IQHeureTheo->setAlignment(Qt::AlignRight);
    IQHeureTheo->setHidden(true);

    IQCM = new QLabel();
    IQCM->setFont(QFont("Arial", 20));
    IQCM->setAlignment(Qt::AlignCenter);

    timer = new QTimer();

    timerSonnerie = new QTimer();

    f = new maFenetre(nullptr, nbRames, positionDepart, facteurAcceleration, demo, satellite);


    layoutCommandes->addWidget(boutonJoker, 1, 1, 1, 7);
    layoutCommandes->addWidget(labelPA, 2, 1);
    layoutCommandes->addWidget(sliderPACM, 2, 2, 1, 5);
    layoutCommandes->addWidget(labelCM, 2, 7);
    layoutCommandes->addWidget(slider, 3, 1, 1, 7);
    layoutCommandes->addWidget(vueCompteur, 4, 1, 1, 7);

    layoutStation->addWidget(boutonOuvrir, 1, 1, 1, 2);
    layoutStation->addWidget(boutonFermer, 1, 3, 1, 2);
    layoutStation->addWidget(boutonSonner, 1, 5, 1, 2);
    layoutStation->addWidget(labelPA2, 2, 1);
    layoutStation->addWidget(sliderPACM2, 2, 2, 1, 4);
    layoutStation->addWidget(labelCM2, 2, 6);
    layoutStation->addWidget(labelRetard, 3, 1, 2, 3);
    layoutStation->addWidget(IQHeure, 3, 4, 1, 3);
    layoutStation->addWidget(IQHeureTheo, 4, 4, 1, 3);
    layoutStation->addWidget(IQCM, 5, 1, 1, 6);
    layoutStation->addWidget(labelMonde, 6, 1, 1, 6);
    layoutStation->addWidget(labelCapacite, 7, 1, 1, 6);

    layoutCarte->addWidget(vueCarte, 1, 1);

    page1->setLayout(layoutCommandes);
    page3->setLayout(layoutStation);
    page4->setLayout(layoutCarte);

    layoutGauche->addWidget(tab, 2, 1);

    layout->addLayout(layoutGauche, 1, 1);
    layout->addWidget(f, 1, 2);

    setLayout(layout);

    emit pourcentageChargement(70);



    QObject::connect(f, SIGNAL(changementVitesse()), this, SLOT(accelerer()));
    QObject::connect(f, SIGNAL(changementVitesseAutres(int)), this, SLOT(accelererAutres(int)));
    QObject::connect(this, SIGNAL(vit(double)), f, SLOT(setVitesse(double)));
    QObject::connect(this, SIGNAL(vitAutres(int, double)), f, SLOT(setVitesseAutres(int, double)));
    QObject::connect(f, SIGNAL(emettreRetard(int)), this, SLOT(recevoirRetard(int)));

    QObject::connect(slider, SIGNAL(sliderReleased()), this, SLOT(relachement()));

    QObject::connect(sliderPACM, SIGNAL(valueChanged(int)), this, SLOT(changementSliderPACM(int)));
    QObject::connect(sliderPACM2, SIGNAL(valueChanged(int)), this, SLOT(changementSliderPACM(int)));

    QObject::connect(f, SIGNAL(modifierMonde(int)), this, SLOT(setLabelMonde(int)));
    QObject::connect(f, SIGNAL(modifierCapacite(int)), this, SLOT(setLabelCapacite(int)));
    QObject::connect(f, SIGNAL(addInfo(QString)), this, SLOT(majLabelInfo(QString)));
    QObject::connect(f, SIGNAL(emettreIconePert(int)), this, SLOT(recevoirIconePert(int)));

    emit pourcentageChargement(96);

    if (!demo) {

        numeroAutres[0] = rand() % 30 + 1;

        for (int compteur = 1; compteur < nombreRames; compteur++) {

            bool existeDeja = false;

            numeroAutres[compteur] = numeroAutres[compteur - 1] + loi_binomiale(4, 0.15) + 1;

            while (numeroAutres[compteur] > 30) {

                numeroAutres[compteur] -= 30;

            }

            do {

                for (int i = 0; i < compteur; i++) {

                    if (!existeDeja && numeroAutres[compteur] == numeroAutres[i]) {

                        numeroAutres[compteur]++;

                        while (numeroAutres[compteur] > 30) {

                            numeroAutres[compteur] -= 30;

                        }

                        existeDeja = true;

                    }

                }

            } while (existeDeja);

        }

        for (int compteur = 0; compteur < nombreRames; compteur++) {

            coefficientRetardAutres[compteur] = loi_binomiale(20, 0.5) - 10;
            stationAutres[compteur] = f->numeroStation(f->distTotaleAutres(compteur));
         /*   coefficientRetardAutres[0] = 0; //0
            stationAutres[0] = 1; //0*/

        }

        for (int compteur = 0; compteur < nombreRamesMax - nombreRames; compteur++) {

            f->setDistTotaleAutres(compteur + nombreRames, 24200 + 80 * compteur);

        }

    }

    else {

        for (int compteur = 0; compteur < nombreRamesMax; compteur++) {

            f->setDistTotaleAutres(compteur, 24200 + 80 * compteur);

        }

        f->setDistTotale(0);

    }

    afficherLimiteVitesse();

    emit pourcentageChargement(100);

}

void GrandeFenetre::keyPressEvent(QKeyEvent *keyEvent) {

    switch(keyEvent->key()) {

    case Qt::Key_Space:

        if (!satellite) {

            f->avancerC();

        }

        break;

    case Qt::Key_Down:

        if (!satellite) {

            f->tournerHC();

        }

        else {

            f->augmenteryS();

        }

        break;

    case Qt::Key_Left:

        if (satellite) {

            f->diminueryS();

        }

        else {

            f->tournerTC();

        }

        break;

    case Qt::Key_Up:

        if (satellite) {

            f->augmenterxS();

        }

        else {

            f->tournerTC();

        }

        break;

    case Qt::Key_Right:

        if (satellite) {

            f->diminuerxS();

        }

        break;

    case Qt::Key_Plus:

        if (satellite) {

            f->diminuerzS();

        }

        break;

    case Qt::Key_Minus:

        if (satellite) {

            f->augmenterzS();

        }

        break;

    case Qt::Key_4:

        if (satellite) {

            f->diminueryS2();

        }

        break;

    case Qt::Key_8:

        if (satellite) {

            f->diminuerxS2();

        }

        break;

    case Qt::Key_2:

        if (satellite) {

            f->augmenterxS2();

        }

        break;

    case Qt::Key_6:

        if (satellite) {

            f->augmenteryS2();

        }

    }

}

int GrandeFenetre::loi_binomiale(int n, double p) {

    int res = 0;
    p = int(1000*p); //compris entre 0 et 1000

    for (int compteur = 0; compteur < n; compteur++) {

        if (rand() % 1000 <= p) {

            res++;

        }

    }

    return res;

}


void GrandeFenetre::changementSliderPACM(int a) {

    if (sliderPACM->value() != a) {

        sliderPACM->setValue(a);

    }

    if (sliderPACM2->value() != a) {

        sliderPACM2->setValue(a);

    }

}

void GrandeFenetre::ouvrirPortes() {

    if (f->estEnCabine()) {

        tab->setCurrentWidget(page3);
        boutonOuvrir->setDisabled(true);
        boutonFermer->setDisabled(true);
        sonOuverture->play();
        QTimer::singleShot(4400, this, SLOT(finOuverturePortes()));
        slider->setDisabled(true);
        f->remettreEnCabine();

    }

}

void GrandeFenetre::finOuverturePortes() {

    boutonSonner->setEnabled(true);
    portesOuvertes = true;
    f->setPortes(true);

}

void GrandeFenetre::sonnerPortes() {

    if (f->estEnCabine()) {

        sonOuverture->stop();
        sonSonnerie->play();
        sonSonnerie->setLoops(QSound::Infinite);
        QTimer::singleShot(3000, this, SLOT(finSonneriePortes()));
        boutonSonner->setDisabled(true);
        initPA = true;
        f->remettreEnCabine();

    }

}

void GrandeFenetre::finSonneriePortes() {

    boutonSonner->setDisabled(true);
    boutonFermer->setEnabled(true);

}

void GrandeFenetre::fermerPortes() {

    sonSonnerie->stop();

    if (f->estEnCabine()) {

        boutonSonner->setDisabled(true);
        boutonSonner->setDown(false);
        boutonFermer->setDisabled(true);
        boutonOuvrir->setDisabled(true);
        sonFermeture->play();
        QTimer::singleShot(4200, this, SLOT(finFermeturePortes()));
        f->remettreEnCabine();

    }

}

void GrandeFenetre::finFermeturePortes() {

    boutonOuvrir->setEnabled(true);
    portesOuvertes = false;
    f->setPortes(false);
    tab->setCurrentWidget(page1);

    slider->setEnabled(true);

}

void GrandeFenetre::joker() {

    f->setDistTotale(f->distTotale() - 100);
    boutonJoker->setDisabled(true);

}

void GrandeFenetre::majLabelInfo(QString s) {

    QString s0 = page2->text();
    QTime time = QTime::currentTime();

    s0.append(time.toString(">HH:mm"));
    s0.append(" | " + s + "\n");

    if (s0.size() > 2200) {

        while (s0.size() > 2100 || s0[0] != '>') {

            s0.remove(0, 1);

        }

    }

    page2->setText(s0);

}

void GrandeFenetre::accelerer() {

    dessinerCarte();

    effetsSonores();

    labelHeure->setHtml("<font color = \"#FF7515\">" + QTime::currentTime().toString("HH:mm") + "</font>");

    if (tab->currentWidget() == page3 && vitesse >= 5) {

        tab->setCurrentWidget(page1);

    }

    double a = 0;

    if (sliderPACM->value() == 1) {

        a = (slider->value() * 0.01);

    }

    else {

        a = 0.01*lecturePA();

    }

    if (a != 0.0) {

        if (vitesse > 0.01 && a > 0) {

            vitesse += (4*a/(2*sqrt(vitesse)) * (91 - vitesse)/17.5)*0.1 / 9;

        }

        else if (vitesse > 0.01 && a < 0) {

            vitesse += (6*a/(2*sqrt(vitesse)) * (93 - vitesse)/17.5)*0.1 / 9;

        }

        else if (vitesse != 0.0) {

            vitesse = 0.000000;

        }

        else if (vitesse == 0.0 && a > 0) {

            vitesse = 2 * a / 6; //Passage de 10 images par seconde à 62.5 images par seconde + passage de m/s à km/h

            if (vitesse < 1) {

                vitesse = 0.2;

            }

        }

    }

    else {

        vitesse -= 0.6*0.1 / 6; //Passage de 10 images par seconde à 62.5 images par seconde + passage de m/s à km/h

    }

    if (vitesse < 0) {

        vitesse = 0.000000;

    }

    if (vitesse < 0.1) {

        vitesse = 0.000000;

    }

    if (f->z(f->distTotale()) > f->z(f->distTotale() - 75)) {

        double d = f->z(f->distTotale()) - f->z(f->distTotale() - 75); //d > 0
        vitesse -= d * 0.004;

    }

    if (f->z(f->distTotale()) < f->z(f->distTotale() - 75)) {

        double d = f->z(f->distTotale()) - f->z(f->distTotale() - 75); //d < 0
        vitesse -= d * 0.004;

    }

    if (vitesse >= -0.1 && vitesse <= 0.1) {

        vitesse = 0.000000;

    }

    emit vit(vitesse / 3.6);

    aiguille->setRotation(-30 + 3 * vitesse);

    if (f->limiteVitesse(f->distTotale()) != limiteVitesse) {

        limiteVitesse = f->limiteVitesse(f->distTotale());
        afficherLimiteVitesse();

    }

    if (vitesse > 0.1 && arretEffectue) {

        arretEffectue = false;
        boutonOuvrir->setDisabled(true);

    }

    if (vitesse < 0.1 && vitesse > -0.1 && estEnStation(f->distTotale())) {

        enStation = true;

    }

    else {

        enStation = false;

        int facteursAller[10] = {0, 90, 30, 50, 70, 125, 80, 90, 100, 170};
        int facteursRetour[11] = {0, 25, 50, 60, 190, 80, 90, 100, 350, 320, 0};

        int st = 0;

        if (f->distTotale() <= 310) {

            st = 0;

        }

        else if (f->distTotale() <= 894 + 310 * PI/4 + 50 * V2) {

            st = 1;

        }

        else if (f->distTotale() <= 1419 + 306 * PI/4 + 50 * V2) {

            st = 2;

        }

        else if (f->distTotale() <= 2094 + 306 * PI/4 + 50 * V2) {

            st = 3;

        }

        else if (f->distTotale() <= 2494 + 403 * PI/4 + 50 * V2) {

            st = 4;

        }

        else if (f->distTotale() <= 2969 + 1081 * PI/4 + 50 * V2) {

            st = 5;

        }

        else if (f->distTotale() <= 3444 + 1081 * PI/4 + 50 * V2) {

            st = 6;

        }

        else if (f->distTotale() <= 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

            st = 7;

        }

        else if (f->distTotale() <= 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

            st = 8;

        }

        else if (f->distTotale() <= 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) {

            st = 9;

        }

        else if (f->distTotale() <= 100003) {

            st = 100;

        }

        else if (f->distTotale() <= 100477.35) {

            st = 101;

        }

        else if (f->distTotale() <= 101062.35) {

            st = 102;

        }

        else if (f->distTotale() <= 101254.35 + 156 * PI/4 + 200 * PI/3) {

            st = 103;

        }

        else if (f->distTotale() <= 101729.35 + 156 * PI/4 + 200 * PI/3) {

            st = 104;

        }

        else if (f->distTotale() <= 102204.35 + 828 * PI/4 + 200 * PI/3) {

            st = 105;

        }

        else if (f->distTotale() <= 102604.35 + 931 * PI/4 + 200 * PI/3) {

            st = 106;

        }

        else if (f->distTotale() <= 103279.35 + 931 * PI/4 + 200 * PI/3) {

            st = 107;

        }

        else if (f->distTotale() <= 103804.35 + 931 * PI/4 + 200 * PI/3) {

            st = 108;

        }

        else if (f->distTotale() <= 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

            st = 109;

        }

        else {

            st = 110; //N'a guère de sens puisque la partie se termine

        }

        int passagersDescendants = 0;

        if (st < 100) {

            double num = facteursAller[st];
            double den = 0;

            for (int s = st; s < 10; s++) {

                den += facteursAller[s];

            }

            if (std::abs(den) < 0.01)
                den = 1;

            passagersDescendants = int((550 - capacite) * num / den);


        }

        else {

            double num = facteursRetour[st - 100];
            double den = 0;

            for (int s = st; s < 111; s++) {

                den += facteursRetour[s - 100];

            }

            if (std::abs(den) < 0.01)
                den = 1;

            passagersDescendants = int(num * (550 - capacite) / den);

        }


        if (f->passagersD() && capacite != 550 && !estEnStation(f->distTotale()) && !estEnStation(f->distTotale() + 2.5) && !estEnStation(f->distTotale() + 5) && !estEnStation(f->distTotale() + 7.5)) { //la dernière condition permet d'éviter les cas limites où la mise à jour est effectuée 1 station trop tôt (-> problème de calcul)

            f->modifierPassagersDescendants(passagersDescendants);

        }

    }

    if (enStation) {

        IQHeure->setText("<font color = \"#FF7515\">" + QTime::currentTime().toString("HHmmss") + "</font>");

        if (!arretEffectue) {

            arretEffectue = true;
            boutonOuvrir->setEnabled(true);

            if (f->distTotale() <= 310) {

                Station = 1;

            }

            else if (f->distTotale() <= 894 + 306 * PI/4 + 50 * V2) {

                Station = 2;

            }

            else if (f->distTotale() <= 1419 + 306 * PI/4 + 50 * V2) {

                Station = 3;

            }

            else if (f->distTotale() <= 2094 + 306 * PI/4 + 50 * V2) {

                Station = 4;

            }

            else if (f->distTotale() <= 2494 + 403 * PI/4 + 50 * V2) {

                Station = 5;

            }

            else if (f->distTotale() <= 2969 + 1081 * PI/4 + 50 * V2) {

                Station = 6;

            }

            else if (f->distTotale() <= 3444 + 1081 * PI/4 + 50 * V2) {

                Station = 7;

            }

            else if (f->distTotale() <= 3636 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

                Station = 8;

            }

            else if (f->distTotale() <= 4221 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

                Station = 9;

            }

            else if (f->distTotale() <= 4646 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) {

                Station = 10;

            }

            else if (f->distTotale() <= 4733.95 + 1705 * PI/4 + 300 * PI/3 + 50 * V2) {

                Station = 101;
                f->setDistTotale(f->distTotale() + 100000 - 6454.61838802); //fonctionne même si le nombre 6454.61838802 fut obtenu de manière empirique

            }

            else if (f->distTotale() <= 100003) {

                Station = 101;

            }

            else if (f->distTotale() <= 100477.35) {

                Station = 102;

            }

            else if (f->distTotale() <= 101062.35) {

                Station = 103;

            }

            else if (f->distTotale() <= 101254.35 + 156 * PI/4 + 200 * PI/3) {

                Station = 104;

            }

            else if (f->distTotale() <= 101729.35 + 156 * PI/4 + 200 * PI/3) {

                Station = 105;

            }

            else if (f->distTotale() <= 102204.35 + 828 * PI/4 + 200 * PI/3) {

                Station = 106;

            }

            else if (f->distTotale() <= 102604.35 + 931 * PI/4 + 200 * PI/3) {

                Station = 107;

            }

            else if (f->distTotale() <= 103279.35 + 931 * PI/4 + 200 * PI/3) {

                Station = 108;

            }

            else if (f->distTotale() <= 103804.35 + 931 * PI/4 + 200 * PI/3) {

                Station = 109;

            }

            else if (f->distTotale() <= 104373.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) {

                Station = 110;

            }

            else if (f->distTotale() <= 104955.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

                Station = 1;
                f->setDistTotale(f->distTotale() + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
                f->majTemps(-1752);

            }

            f->arriveeEnStation(Station);
            tab->setCurrentWidget(page3);

            if (Station == 1) {

                IQHeureTheo->setText("<font color = \"#FF7515\">" + QTime::currentTime().addMSecs(arretTheo1[0] - 1000 * retard).toString("mmss") + "</font>");

            }

            else if (Station < 90) {

                IQHeureTheo->setText("<font color = \"#FF7515\">" + QTime::currentTime().addMSecs(arretTheo1[Station - 1]).toString("mmss") + "</font>");

            }

            else if (Station != 101 && Station != 10) {

                IQHeureTheo->setText("<font color = \"#FF7515\">" + QTime::currentTime().addMSecs(arretTheo2[Station - 101]).toString("mmss") + "</font>");

            }

            else {

                IQHeureTheo->setText("<font color = \"#FF7515\">" + QTime::currentTime().addMSecs(arretTheo2[0] - 1000 * retard).toString("mmss") + "</font>");

            }

        }

        else {

        }

    }

    else {

        initPA = false;
        arretEffectue = false;

    }

}

void GrandeFenetre::finArretAutres0() {

    arretEffectueAutres[0] = true;

    if (f->distTotaleAutres(0) >= 6448.61838802 && f->distTotaleAutres(0) < 99000) {

        f->setDistTotaleAutres(0, f->distTotaleAutres(0) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(0) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(0, f->distTotaleAutres(0) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(0, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres1() {

    arretEffectueAutres[1] = true;

    if (f->distTotaleAutres(1) >= 6448.61838802 && f->distTotaleAutres(1) < 99000) {

        f->setDistTotaleAutres(1, f->distTotaleAutres(1) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(1) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(1, f->distTotaleAutres(1) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(1, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres2() {

    arretEffectueAutres[2] = true;

    if (f->distTotaleAutres(2) >= 6448.61838802 && f->distTotaleAutres(2) < 99000) {

        f->setDistTotaleAutres(2, f->distTotaleAutres(2) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(2) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(2, f->distTotaleAutres(2) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(2, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres3() {

    arretEffectueAutres[3] = true;

    if (f->distTotaleAutres(3) >= 6448.61838802 && f->distTotaleAutres(3) < 99000) {

        f->setDistTotaleAutres(3, f->distTotaleAutres(3) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(3) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(3, f->distTotaleAutres(3) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(3, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres4() {

    arretEffectueAutres[4] = true;

    if (f->distTotaleAutres(4) >= 6448.61838802 && f->distTotaleAutres(4) < 99000) {

        f->setDistTotaleAutres(4, f->distTotaleAutres(4) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(4) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(4, f->distTotaleAutres(4) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(4, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres5() {

    arretEffectueAutres[5] = true;

    if (f->distTotaleAutres(5) >= 6448.61838802 && f->distTotaleAutres(5) < 99000) {

        f->setDistTotaleAutres(5, f->distTotaleAutres(5) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(5) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(5, f->distTotaleAutres(5) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(5, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres6() {

    arretEffectueAutres[6] = true;

    if (f->distTotaleAutres(6) >= 6448.61838802 && f->distTotaleAutres(6) < 99000) {

        f->setDistTotaleAutres(6, f->distTotaleAutres(6) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(6) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(6, f->distTotaleAutres(6) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(6, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres7() {

    arretEffectueAutres[7] = true;

    if (f->distTotaleAutres(7) >= 6448.61838802 && f->distTotaleAutres(7) < 99000) {

        f->setDistTotaleAutres(7, f->distTotaleAutres(7) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(7) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(7, f->distTotaleAutres(7) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(7, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres8() {

    arretEffectueAutres[8] = true;

    if (f->distTotaleAutres(8) >= 6448.61838802 && f->distTotaleAutres(8) < 99000) {

        f->setDistTotaleAutres(8, f->distTotaleAutres(8) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(8) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(8, f->distTotaleAutres(8) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(8, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres9() {

    arretEffectueAutres[9] = true;

    if (f->distTotaleAutres(9) >= 6448.61838802 && f->distTotaleAutres(9) < 99000) {

        f->setDistTotaleAutres(9, f->distTotaleAutres(9) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(9) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(9, f->distTotaleAutres(9) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(9, -1752); //valeur provisoire

    }

}

void GrandeFenetre::finArretAutres10() {

    arretEffectueAutres[10] = true;

    if (f->distTotaleAutres(10) >= 6448.61838802 && f->distTotaleAutres(10) < 99000) {

        f->setDistTotaleAutres(10, f->distTotaleAutres(10) + 100000 - 6454.61838802);

    }

    if (f->distTotaleAutres(10) >= 104949.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2) {

        f->setDistTotaleAutres(10, f->distTotaleAutres(10) + 310 - 104955.35 - 1863 * PI/4 - 200 * PI/3 - 50 * V2);
        f->setBiaisAutres(10, -1752); //valeur provisoire

    }

}

void GrandeFenetre::accelererAutres(int n) {

    double a = 0;

    a = 0.01*lecturePAAutres(n);

    if (arretEffectueAutres[n] && vitesseAutres[n] > 14.0) {

        arretEffectueAutres[n] = false;
        affichageEffectueAutres[n] = false;

    }

    if (a != 0.0) {

        if (vitesseAutres[n] > 0.01 && a > 0) {

            vitesseAutres[n] += (4*a/(2*sqrt(vitesseAutres[n])) * (91 - vitesseAutres[n])/17.5)*0.1 / 9;

        }

        else if (vitesseAutres[n] > 0.01 && a < 0) {

            vitesseAutres[n] += (6*a/(2*sqrt(vitesseAutres[n])) * (93 - vitesseAutres[n])/17.5)*0.1 / 9;

        }

        else if (vitesseAutres[n] < 0.00001 && a > 0) {

            vitesseAutres[n] = 2 * a /6; //Passage de 10 images par seconde à 62.5 images par seconde + passage de m/s à km/h

            if (vitesseAutres[n] < 1) {

                vitesseAutres[n] = 0.2;

            }

        }

    }

    else if (vitesseAutres[n] > 0.00001) {

        vitesseAutres[n] -= 0.6*0.1 / 6; //Passage de 10 images par seconde à 62.5 images par seconde + passage de m/s à km/h

    }

    //0
/*
    if (n == 0) {

        QFile fichier("D:/Users/pioll/Mes programmes/build-SimMetro3D-Desktop_Qt_5_11_2_MinGW_32bit-Debug/debug/Sauvegarde 1.txt");

        if (fichier.open(QIODevice::Append)) {

            QTextStream flux(&fichier);
            QString s;
            s.setNum(f->distTotaleAutres(0));
            flux << s + "\n";
            fichier.close();

        }

    }*/

    if (vitesseAutres[n] < 0.00001 && !affichageEffectueAutres[n] && estEnStation(f->distTotaleAutres(n))) {

        affichageEffectueAutres[n] = true;

        vitesseAutres[n] = 0.0;

        double k = 0.0; //durée du timer

        stationAutres[n] = f->numeroStation(f->distTotaleAutres(n));

        if (stationAutres[n] == 11) {

            stationAutres[n] = 101;

        }

        if (stationAutres[n] == 111) {

            stationAutres[n] = 1;

        }

        if (stationAutres[n] - 1 < 90) {

            k = arretTheo1[stationAutres[n] - 1];

        }

        else {

            k = arretTheo2[stationAutres[n] - 101];

        }

        double r = 0.0;
  //0  ; suite à modifier
        if (stationAutres[n] == 1 || stationAutres[n] == 11 || stationAutres[n] == 101 || stationAutres[n] == 111) {

            if (retardAutres[n] < 0) {

                r = -1000*retardAutres[n];

            }

            if (retardAutres[n] > 0) { //à corriger, peut pour un retard de 5 secondes faire prendre une avance de 40 secondes!

                double R = 1000*retardAutres[n];

                if (R > k) {

                    r = 8000 - k;

                }

                else {

                    r = 8000 - R;

                }

            }

        }

        r += 1000 * coefficientRetardAutres[n];
        r += (rand() % 10000) - 5000;



        if (k + r < 8000) {

            r += 8000 - k - r;

        }

        if (rand() % 1000 <= 1) {

            r += 1000*(rand() % 400 + 500);

        }

        else if (rand() % 1000 <= 3) {

            r += 1000*(rand() % 70 + 200);

        }

        else if (rand() % 1000 <= 10) {

            r += 1000*(rand() % 40 + 50);

        }

        else if (rand() % 1000 <= 50) {

            r += 1000*(rand() % 4 + 5);

        }

        k += r;

        switch (n) {

        case 0:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres0()));
            break;

        case 1:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres1()));
            break;

        case 2:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres2()));
            break;

        case 3:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres3()));
            break;

        case 4:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres4()));
            break;

        case 5:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres5()));
            break;

        case 6:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres6()));
            break;

        case 7:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres7()));
            break;

        case 8:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres8()));
            break;

        case 9:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres9()));
            break;

        case 10:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres10()));
            break;

        default:

            QTimer::singleShot(int(k), Qt::PreciseTimer, this, SLOT(finArretAutres0()));
            break;

        }

    }

    if (f->z(f->distTotaleAutres(n)) > f->z(f->distTotaleAutres(n) - 75)) {

        double d = f->z(f->distTotaleAutres(n)) - f->z(f->distTotaleAutres(n) - 75); //d > 0
        vitesseAutres[n] -= d * 0.004;

    }

    if (f->z(f->distTotaleAutres(n)) < f->z(f->distTotaleAutres(n) - 75)) {

        double d = f->z(f->distTotaleAutres(n)) - f->z(f->distTotaleAutres(n) - 75); //d < 0
        vitesseAutres[n] -= d * 0.004;

    }

    if (vitesseAutres[n] <= 0.1) {

        vitesseAutres[n] = 0.000000;

    }

    emit vitAutres(n, vitesseAutres[n] / 3.6);

}

void GrandeFenetre::relachement() {

    slider->setValue(0);

}

void GrandeFenetre::recevoirRetard(int r) {

    retard = r;

    bool avance = false;

    if (r < 0) {

        avance = true;
        r = -r;

    }

    int M = r / 60;
    int S = r % 60;

    QString s = "", ss = "";

    if (avance) {

        ss += "-";

    }

    else {

        ss += "+";

    }

    s.setNum(M);

    if (M < 10) {

        ss += "0";

    }

    ss += s + ":";

    s.setNum(S);

    if (S < 10) {

        ss += "0";

    }

    ss += s;

    if ((f->distTotale() < 99997 || f->distTotale() > 100003) && (f->distTotale() < 222 || f->distTotale() > 228)) {

        labelRetard->setText("<font color = \"#FF7515\">" + ss + "</font>");

    }

    else {

        labelRetard->setText("<font color = \"#FF7515\">+00:00</font>");

    }

    labelHeure->setHtml("<font color = \"#FF7515\">" + QTime::currentTime().toString("HH:mm") + "</font>");

}

int GrandeFenetre::lecturePA() {

    int resultat = 0;

    if ((initPA && !portesOuvertes) || vitesse != 0.0) {

        int origineStation = 0;
        QString stringFichier = "";

        switch(Station) {

        case 0:

            origineStation = 0;
            stringFichier = "x"; //Pas de PA en arrière-gare!
            break;

        case 1:

            origineStation = 307;
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA1";
            break;

        case 2:

            origineStation = int(891 + 306 * PI/4 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA2";
            break;

        case 3:

            origineStation = int(1416 + 306 * PI/4 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA3";
            break;

        case 4:

            origineStation = int(2091 + 306 * PI/4 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA4";
            break;

        case 5:

            origineStation = int(2491 + 403 * PI/4 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA5";
            break;

        case 6:

            origineStation = int(2966 + 1081 * PI/4 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA6";
            break;

        case 7:

            origineStation = int(3441 + 1081 * PI/4 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA7";
            break;

        case 8:

            origineStation = int(3633 + 1225 * PI/4 + 200 * PI/3 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA8";
            break;

        case 9:

            origineStation = int(4218 + 1225 * PI/4 + 200 * PI/3 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA9";
            break;

        case 10:

            origineStation = int(4643 + 1225 * PI/4 + 300 * PI/3 + 50 * V2);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PABoucleBourse";
            break;


        case 11:
        case 101:

            origineStation = 100000;
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA101";
            break;

        case 102:

            origineStation = 100474; //100474.347877
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA102";
            break;

        case 103:

            origineStation = 101059; //101059.347877
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA103";
            break;

        case 104:

            origineStation = 101583; //101583.309501
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA104";
            break;

        case 105:

            origineStation = 102058; //102058.309501
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA105";
            break;

        case 106:

            origineStation = 103061; //103061.097067
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA106";
            break;

        case 107:

            origineStation = 103542; //103541.993078
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA107";
            break;

        case 108:

            origineStation = 104217; //104216.993078
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA108";
            break;

        case 109:

            origineStation = int(103801.35 + 931 * PI/4 + 200 * PI/3);
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA109";
            break;

        case 110:

            origineStation = 105613;
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PA110";
            break;

        default:

            stringFichier = "x";

        }

        if (demo) {

            origineStation = 0;
            stringFichier = "D:/Users/pioll/Mes programmes/SimMetro3D/PAD";

        }

        else {

            //on fera attention à traiter le cas démo à l'ancienne !

            int vObj = 0;
            double dTotale = f->distTotale();

            if (dTotale < 100000)
                vObj = vitessesPAAller [Station -   1][std::max(std::min(1099, int(dTotale - origineStation)), 0)];


            else
                vObj = vitessesPARetour[Station - 101][std::max(std::min(1099, int(dTotale - origineStation)), 0)];


            int couleurFeu = 0;

            if (dTotale < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2)
                couleurFeu = f->couleurFeu(f->numeroFeu(dTotale));

            else
                couleurFeu = f->couleurFeu(100 + f->numeroFeu(dTotale));


            if (couleurFeu == 2) {

                double distanceFeu = 1;

                if (f->distTotale() < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2)
                    distanceFeu = f->positionFeu(f->numeroFeu(dTotale)) - dTotale;

                else
                    distanceFeu = f->positionFeu(100 + f->numeroFeu(dTotale)) - dTotale;


                if (distanceFeu < 100) {

                    int vObj2 = vObj - 15;

                    if (vObj2 < 15 && vObj >= 15)
                        vObj2 = 15;

                    else
                        vObj2 = vObj;

                    vObj = vObj2 + int(distanceFeu) * (vObj - vObj2) / 100;

                }

            }

            else if (couleurFeu == 3) {

                double distanceFeu = 1;

                if (dTotale < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2)
                    distanceFeu = f->positionFeu(f->numeroFeu(dTotale)) - dTotale;

                else
                    distanceFeu = f->positionFeu(100 + f->numeroFeu(dTotale)) - dTotale;

                if (distanceFeu >= 52) {

                    if (vObj > 15 + int((distanceFeu - 52) * 0.1))
                        vObj = 15 + int((distanceFeu - 52) * 0.1);

                }

                else if (distanceFeu >= 32) {

                    if (vObj > 15)
                        vObj = 15;

                }

                else {

                    vObj = int(distanceFeu / 2 - 6);

                    if (vObj < 0)
                        return -400;

                }

            }

            if (vitesse > vObj && vObj >= 15) {

                vObj--; //Permet de ne pas bloquer à 0.5 km/h au-dessus de l'objectif

            }

            double diff = vitesse - vObj;

            if (diff < -15)
                resultat = 300;

            else if (diff < -0.5)
                resultat = int(-20 * diff);



            else if (diff < 0)
                resultat = 5;

            else if (diff < 0.5)
                resultat = -5;

            else if (diff < 10)
                resultat = int(-30 * diff);

            else
                resultat = -400;

            if (!estEnStation(dTotale) && f->z(dTotale) > f->z(dTotale - 75))
                resultat += 5 * (f->z(dTotale) - f->z(dTotale - 75));

        }

    }

    return resultat;

}

/*



        if (stringFichier != "x" && stringFichier != "") {

            QFile fichier(stringFichier);

            if (fichier.open(QIODevice::ReadOnly)) {

                QTextStream flux(&fichier);
                QString ligne;
                int a = 0;

                do {

                    ligne = flux.readLine();
                    a = QString(ligne.left(4)).toInt();

                    if (demo && f->distTotale() >= 100000) {

                        a = QString(ligne.left(6)).toInt();

                    }

                } while (a < (f->distTotale() - origineStation) - 0.5 && !flux.atEnd()); //La ligne choisie sera donc la ligne suivante

                ligne = flux.readLine();

                QString ligne2 = "";
                ligne2 += ligne[5];
                ligne2 += ligne[6];

                if (demo && f->distTotale() >= 100000) {

                    ligne2.clear();
                    ligne2 += ligne[7];
                    ligne2 += ligne[8];

                }

                int vObj = ligne2.toInt();

                int couleurFeu = 0;

                if (f->distTotale() < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2) {

                    couleurFeu = f->couleurFeu(f->numeroFeu(f->distTotale()));

                }

                else {

                    couleurFeu = f->couleurFeu(100 + f->numeroFeu(f->distTotale()));

                }

                if (couleurFeu == 2) {

                    double distanceFeu = 1;

                    if (f->distTotale() < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2) {

                        distanceFeu = f->positionFeu(f->numeroFeu(f->distTotale())) - f->distTotale();

                    }

                    else {

                        distanceFeu = f->positionFeu(100 + f->numeroFeu(f->distTotale())) - f->distTotale();

                    }

                    if (distanceFeu < 100) {

                        int vObj2 = vObj - 15;

                        if (vObj2 < 15 && vObj >= 15) {

                            vObj2 = 15;

                        }

                        else {

                            vObj2 = vObj;

                        }

                        vObj = vObj2 + int(distanceFeu) * (vObj - vObj2) / 100;

                    }

                }

                else if (couleurFeu == 3) {

                    double distanceFeu = 1;

                    if (f->distTotale() < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2) {

                        distanceFeu = f->positionFeu(f->numeroFeu(f->distTotale())) - f->distTotale();

                    }

                    else {

                        distanceFeu = f->positionFeu(100 + f->numeroFeu(f->distTotale())) - f->distTotale();

                    }

                    if (distanceFeu >= 52) {

                        if (vObj > 15 + int((distanceFeu - 52) * 0.1)) {

                            vObj = 15 + int((distanceFeu - 52) * 0.1);

                        }

                    }

                    else if (distanceFeu >= 32) {

                        if (vObj > 15) {

                            vObj = 15;

                        }

                    }

                    else {

                        vObj = int(distanceFeu / 2 - 6);

                        if (vObj < 0) {

                            return -400;

                        }

                    }

                }

                if (vitesse > vObj && vObj >= 15) {

                    vObj--; //Permet de ne pas bloquer à 0.5 km/h au-dessus de l'objectif

                }

                double diff = 0;
                diff = vitesse - vObj;

                if (diff < -15) {

                    resultat = 300;

                }

                else if (diff < -0.5) {

                    resultat = int(-20 * diff);

                }

                else if (diff < 0) {

                    resultat = 5;

                }

                else if (diff < 0.5) {

                    resultat = -5;

                }

                else if (diff < 10) {

                    resultat = int(-30 * diff);

                }

                else {

                    resultat = -400;

                }

                if (flux.atEnd()) {

                    resultat = -400;

                }

                if (!estEnStation(f->distTotale()) && f->z(f->distTotale()) > f->z(f->distTotale() - 75)) {

                    resultat += 5 * (f->z(f->distTotale()) - f->z(f->distTotale() - 75));

                }

            }

            else {

                QMessageBox::critical(this, "Erreur PA", "Impossible de lire le fichier PA joueur");
                sliderPACM->setValue(1);
                resultat = 0;

            }

        }

        else {

            QMessageBox::critical(this, "Erreur PA", "Impossible de lire le fichier PA joueur");
            sliderPACM->setValue(1);
            resultat = 0;

        }

    }

    else {

        resultat = -400; //arrêt en station non effectué -> le PA doit attendre avant de repartir

    }

    return resultat;*/

int GrandeFenetre::lecturePAAutres(int n) {

    int resultat = 0;
    int origineStation = 0;
    QString stringFichier = "";
    int station = 0;
    double di = f->distTotaleAutres(n);

    if (di >= 307 + 3)
        station = 1;
    if (di >= int(891 + 306 * PI/4 + 50 * V2) + 3)
        station = 2;
    if (di >= int(1416 + 306 * PI/4 + 50 * V2) + 3)
        station = 3;
    if (di >= int(2091 + 306 * PI/4 + 50 * V2) + 3)
        station = 4;
    if (di >= int(2491 + 403 * PI/4 + 50 * V2) + 3)
        station = 5;
    if (di >= int(2966 + 1081 * PI/4 + 50 * V2) + 3)
        station = 6;
    if (di >= int(3441 + 1081 * PI/4 + 50 * V2) + 3)
        station = 7;
    if (di >= int(3633 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) + 3)
        station = 8;
    if (di >= int(4218 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) + 3)
        station = 9;
    if (di >= int(4643 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) + 3)
        station = 10;
    if (di >= 100000 + 3)
        station = 101;
    if (di >= 100474 + 3)
        station = 102;
    if (di >= 101059 + 3)
        station = 103;
    if (di >= 101583 + 3)
        station = 104;
    if (di >= 102058 + 3)
        station = 105;
    if (di >= 103061 + 3)
        station = 106;
    if (di >= 103542 + 3)
        station = 107;
    if (di >= 104217 + 3)
        station = 108;
    if (di >= int(103801.35 + 931 * PI/4 + 200 * PI/3) + 3)
        station = 109;
    if (di >= 105613)
        station = 110;

    if (estEnStation(di) && arretEffectueAutres[n]) {

        station++;
        if (station == 11)
            station = 101;
        if (station == 111)
            station = 1;

    }

    //station = numéro de la station que je viens de desservir

    switch(station) {

    case 1:
        origineStation = 307;
        break;
    case 2:
        origineStation = int(891 + 306 * PI/4 + 50 * V2);
        break;
    case 3:
        origineStation = int(1416 + 306 * PI/4 + 50 * V2);
        break;
    case 4:
        origineStation = int(2091 + 306 * PI/4 + 50 * V2);
        break;
    case 5:
        origineStation = int(2491 + 403 * PI/4 + 50 * V2);
        break;
    case 6:
        origineStation = int(2966 + 1081 * PI/4 + 50 * V2);
        break;
    case 7:
        origineStation = int(3441 + 1081 * PI/4 + 50 * V2);
        break;
    case 8:
        origineStation = int(3633 + 1225 * PI/4 + 200 * PI/3 + 50 * V2);
        break;
    case 9:
        origineStation = int(4218 + 1225 * PI/4 + 200 * PI/3 + 50 * V2);
        break;
    case 10:
        origineStation = int(4643 + 1225 * PI/4 + 300 * PI/3 + 50 * V2);
        break;
    case 11:
    case 101:
        origineStation = 100000;
        break;
    case 102:
        origineStation = 100474; //100474.347877
        break;
    case 103:
        origineStation = 101059; //101059.347877
        break;
    case 104:
        origineStation = 101583; //101583.309501
        break;
    case 105:
        origineStation = 102058; //102058.309501
        break;
    case 106:
        origineStation = 103061; //103061.097067
        break;
    case 107:
        origineStation = 103542; //103541.993078
        break;
    case 108:
        origineStation = 104217; //104216.993078
        break;
    case 109:
        origineStation = int(103801.35 + 931 * PI/4 + 200 * PI/3);
        break;
    case 110:
        origineStation = 105613;
        break;
    }

    /*

    if (arretEffectueAutres[n]) { //le changement de fichier ne se fait que si arretEffectue, les 2m de marge servent à éviter un départ avant le début du PA (et donc un arrêt infini)

        if (di < int(891 + 306 * PI/4 + 50 * V2) - 3) {

            origineStation = 307;

        }


        else if (di < int(1416 + 306 * PI/4 + 50 * V2) - 3) {

            origineStation = int(891 + 306 * PI/4 + 50 * V2);

        }


        else if (di < int(2091 + 306 * PI/4 + 50 * V2) - 3) {

            origineStation = int(1416 + 306 * PI/4 + 50 * V2);

        }

        else if (di < int(2491 + 403 * PI/4 + 50 * V2) - 3) {

            origineStation = int(2091 + 306 * PI/4 + 50 * V2);

        }

        else if (di < int(2966 + 1081 * PI/4 + 50 * V2) - 3) {

            origineStation = int(2491 + 403 * PI/4 + 50 * V2);

        }

        else if (di < int(3441 + 1081 * PI/4 + 50 * V2) - 3) {

            origineStation = int(2966 + 1081 * PI/4 + 50 * V2);

        }

        else if (di < int(3633 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) - 3) {

            origineStation = int(3441 + 1081 * PI/4 + 50 * V2);

        }

        else if (di < int(4218 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) - 3) {

            origineStation = int(3633 + 1225 * PI/4 + 200 * PI/3 + 50 * V2);

        }

        else if (di < int(4643 + 1225 * PI/4 + 300 * PI/3 + 50 * V2) - 3) {

            origineStation = int(4218 + 1225 * PI/4 + 200 * PI/3 + 50 * V2);

        }

        else if (di < 9900-3) {

            origineStation = int(4643 + 1225 * PI/4 + 300 * PI/3 + 50 * V2);

        }

        else if (di < 100474-3) {//.347877

            origineStation = 100000;

        }

        else if (di < 101059-3) {

            origineStation = 100474; //100474.347877

        }

        else if (di < 101583-3) {

            origineStation = 101059; //101059.347877

        }

        else if (di < 102058-3) {

            origineStation = 101583; //101583.309501

        }

        else if (di < 103061-3) {

            origineStation = 102058; //102058.309501

        }

        else if (di < 103542-3) {

            origineStation = 103061; //103061.097067

        }

        else if (di < 104217-3) { //6.993078

            origineStation = 103542; //103541.993078

        }

        else if (di < 104742-3) {

            origineStation = 104217; //104216.993078

        }

        else if (di < 105613-3) {

            origineStation = 104742; //104741.993078

        }

        else {

            origineStation = 105613;

        }

    }

    */

    if (arretEffectueAutres[n] || vitesseAutres[n] > 0.0 || !estEnStation(di)) {
        //s'exécute toujours, sauf lorsque la rame ne s'est pas arrêtée, est immobile et est en station

        int vObj = 0;

        if (station < 100)
            vObj = vitessesPAAller[station - 1][std::max(0, std::min(1099, int(di - origineStation)))];

        else
            vObj = vitessesPARetour[station - 101][std::max(0, std::min(1099, int(di - origineStation)))];

        int couleurFeu = 0;

        if (di < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2)
            couleurFeu = f->couleurFeu(f->numeroFeu(di));

        else
            couleurFeu = f->couleurFeu(100 + f->numeroFeu(di));

        cFeu[n] = couleurFeu;

        if (couleurFeu == 2) {

            double distanceFeu = 1;

            if (di < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2)
                distanceFeu = f->positionFeu(f->numeroFeu(di)) - di;

            else
                distanceFeu = f->positionFeu(100 + f->numeroFeu(di)) - di;

            if (distanceFeu < 100) {

                int vObj2 = vObj - 15;

                if (vObj2 < 15 && vObj >= 15)
                    vObj2 = 15;

                else
                    vObj2 = vObj;

                vObj = vObj2 + int(distanceFeu) * (vObj - vObj2) / 100;

            }

        }

        else if (couleurFeu == 3) {

            double distanceFeu = 1;

            if (di < 4665.85 + 1225 * PI/4 + 300 * PI/3 + 240 * PI/2 + 50 * V2)
                distanceFeu = f->positionFeu(f->numeroFeu(di)) - di;

            else
                distanceFeu = f->positionFeu(100 + f->numeroFeu(di)) - di;


            if (distanceFeu >= 52) {

                if (vObj > 15 + int((distanceFeu - 52) * 0.1))
                    vObj = 15 + int((distanceFeu - 52) * 0.1);

            }

            else if (distanceFeu >= 32) {

                if (vObj > 15)
                    vObj = 15;

            }

            else {

                vObj = int(distanceFeu / 2 - 6);

                if (vObj < 0)
                    return -400;

            }

        }

        else if (vitesseAutres[n] < 1 && !estEnStation(di))
            vitesseAutres[n] = 3;

        if (vitesseAutres[n] > vObj && vObj >= 15)
            vObj--; //Permet de ne pas bloquer à 0.5 km/h au-dessus de l'objectif

        double diff = vitesseAutres[n] - vObj;

        if (diff < -15)
            resultat = 300;

        else if (diff < -0.5)
            resultat = int(-20*diff);

        else if (diff < 0)
            resultat = 5;

        else if (diff < 0.5)
            resultat = -5;

        else if (diff < 10)
            resultat = int(-30*diff);

        else
            resultat = -400;


        if (!estEnStation(di) && f->z(di) > f->z(di - 75))
            resultat += 5 * (f->z(di) - f->z(di - 75));

    }

    else
        resultat = 0;

    return resultat;

}

QString GrandeFenetre::codePosition(double p) {

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

bool GrandeFenetre::estEnStation(double d) {

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
            || (4727 + 1705 * PI/4 + 300 * PI/3 + 50 * V2 < d && d < 4734 + 1705 * PI/4 + 300 * PI/3 + 50 * V2) //6454.61838802; 6456.97381207
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

void GrandeFenetre::effetsSonores() {

    if (f->distTotale() >= 3491 + 1225 * PI/4 + 50 * V2 && f->distTotale() <= 3511 + 1225 * PI/4 + 50 * V2 && nbAiguillages == 1 && vitesse >= 27) {

        nbAiguillages++;
        sonAiguillage->play();

    }

    if (f->distTotale() >= 4271 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 && f->distTotale() <= 4291 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 && nbAiguillages == 2 && vitesse >= 27) {

        nbAiguillages++;
        sonAiguillage->play();

    }

    if (f->distTotale() >= 100020 && f->distTotale() <= 100040 && nbAiguillages == 3 && vitesse >= 27) {

        nbAiguillages++;
        sonAiguillage->play();

    }

    if (f->distTotale() >= 100344 && f->distTotale() <= 100364 && nbAiguillages == 4 && vitesse >= 27) {

        nbAiguillages++;
        sonAiguillage->play();

    }

    if (vitesse >= 52) {

        if (codePosition(f->distTotale()) == "HOP+" || codePosition(f->distTotale()) == "CSF-" || codePosition(f->distTotale()) == "HOP-" || codePosition(f->distTotale()) == "CSF+") {

            if (!sonVitesse55Joue) {

                sonVitesse55->play();
                sonVitesse55->setLoops(QSound::Infinite);
                sonVitesse55Joue = true;

            }

        }

        else {

            if (!sonDroite60Joue) {

                sonDroite60->play();
                sonDroite60->setLoops(QSound::Infinite);
                sonDroite60Joue = true;

            }

        }

    }

    else {

        sonVitesse55->stop();
        sonVitesse55Joue = false;
        sonDroite60->stop();
        sonDroite60Joue = false;

    }

    if ((vitesse < 16.5 && estEnCourbe(f->distTotale())) || (vitesse < 23.5 && !estEnCourbe(f->distTotale()))) {

        if (!sonDemarrageJoue && slider->value() > 210) {

            sonDemarrage->play();
            sonDemarrageJoue = true;

        }

        else if (slider->value() < 140) {

            sonDemarrage->stop();
            sonDemarrageJoue = false;

        }

    }

    else {

        sonDemarrage->stop();
        sonDemarrageJoue = false;

    }

    if (vitesse >= 16.5 && !estEnCourbe(f->distTotale()) && slider->value() > 180) {

        if (!sonAccelerationDroiteJoue && slider->value() > 210 && vitesse <= 21) {

            sonAccelerationDroite->play();
            sonAccelerationDroiteJoue = true;

        }

        else if (vitesse > 52) {

            sonAccelerationDroite->stop();
            sonAccelerationDroiteJoue = false;

        }

    }

    else {

        sonAccelerationDroite->stop();
        sonAccelerationDroiteJoue = false;

    }

    if (vitesse >= 16.5 && vitesse <= 27.5 && estEnCourbe(f->distTotale())) {

        if (!sonCourbe22Joue) {

            sonCourbe22->play();
            sonCourbe22->setLoops(QSound::Infinite);
            sonCourbe22Joue = true;

        }

    }

    else {

        sonCourbe22->stop();
        sonCourbe22Joue = false;

    }

    if (vitesse >= 27.5 && vitesse <= 38.5 && estEnCourbe(f->distTotale())) {

        if (!sonCourbe33Joue) {

            sonCourbe33->play();
            sonCourbe33->setLoops(QSound::Infinite);
            sonCourbe33Joue = true;

        }

    }

    else {

        sonCourbe33->stop();
        sonCourbe33Joue = false;

    }

    if (vitesse >= 38.5 && vitesse <= 47 && estEnCourbe(f->distTotale())) {

        if (!sonCourbe44Joue) {

            sonCourbe44->play();
            sonCourbe44->setLoops(QSound::Infinite);
            sonCourbe44Joue = true;

        }

    }

    else {

        sonCourbe44->stop();
        sonCourbe44Joue = false;

    }

    if (vitesse >= 47 && vitesse <= 55.5 && estEnCourbe(f->distTotale())) {

        if (!sonCourbe50Joue) {

            sonCourbe50->play();
            sonCourbe50->setLoops(QSound::Infinite);
            sonCourbe50Joue = true;

        }

    }

    else {

        sonCourbe50->stop();
        sonCourbe50Joue = false;

    }

    if (vitesse >= 23.5 && vitesse <= 38.5 && !estEnCourbe(f->distTotale())) { // && !sonAccelerationDroiteJoue) {

        if (!sonDroite31Joue) {

            sonDroite31->play();
            sonDroite31->setLoops(QSound::Infinite);
            sonDroite31Joue = true;

        }

    }

    else {

        sonDroite31->stop();
        sonDroite31Joue = false;

    }

    if (vitesse >= 38.5 && vitesse <= 52 && !estEnCourbe(f->distTotale())) { // && !sonAccelerationDroiteJoue) {

        if (!sonDroite46Joue) {

            sonDroite46->play();
            sonDroite46->setLoops(QSound::Infinite);
            sonDroite46Joue = true;

        }

    }

    else {

        sonDroite46->stop();
        sonDroite46Joue = false;

    }

}

bool GrandeFenetre::estEnCourbe(double d) {

    return     (d >= 356 && d <= 356 + 153 * PI/4) //sortie Place des Remparts dir. Bourse
            || (d >= 356 + 153 * PI/4 + 50 * V2 && d <= 356 + 306 * PI/4 + 50 * V2)
            || (d >= 2116 + 306 * PI/4 + 50 * V2 && d <= 2116 + 403 * PI/4 + 50 * V2) //Sortie Rue des Chenilles dir. Bourse
            || (d >= 3441 + 1081 * PI/4 + 50 * V2 && d <= 3441 + 1225 * PI/4 + 50 * V2) //Sortie Blainville dir. Bourse
            || (d >= 3541 + 1225 * PI/4 + 50 * V2 && d <= 3541 + 1225 * PI/4 + 102 * PI/3 + 50 * V2) //milieu Blainville-Mirla  -triche de 2*PI/3 pour avoir une coupure entre les 2 virages
            || (d >= 3541 + 1225 * PI/4 + 104 * PI/3 + 50 * V2 && d <= 3541 + 1225 * PI/4 + 200 * PI/3 + 50 * V2)
            || (d >= 4417.794919 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 && d <= 4417.794919 + 1225 * PI/4 + 247.5 * PI/3 + 50 * V2) //première courbe Histiaux-Bourse - on triche de 2*PI/3
            || (d >= 4417.794919 + 1225 * PI/4 + 249.5 * PI/3 + 50 * V2 && d <= 4417.794919 + 1225 * PI/4 + 300 * PI/3 + 50 * V2)
            || (d >= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 200 * PI/6 + 50 * V2 && d <= 4642.794919 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2) //boucle de Bourse jusqu'à la ligne droite
            || (d >= 4655.644576 + 1225 * PI/4 + 200 * PI/3 + 800 * PI/6 + 50 * V2 && d <= 4655.644576 + 1225 * PI/4 + 260 * PI/3 + 800 * PI/6 + 50 * V2) //dernière courbe de la boucle de Bourse
            || (d >= 101076.447042 && d <= 101076.447042 + 102 * PI/3) //Sortie Mirla d.République - triche comme au-dessus
            || (d >= 101076.447042 + 104 * PI/3 && d <= 101076.447042 + 200 * PI/3)
            || (d >= 101176.447042 + 200 * PI/3 && d <= 101176.447042 + 156 * PI/4 + 200 * PI/3) //arrivée à Blainville
            || (d >= 102501.431531 + 200 * PI/3 + 828 * PI/4 && d <= 102501.431531 + 200 * PI/3 + 931 * PI/4) //arrivée à Rue des Chenilles
            || (d >= 103951.431531 + 931 * PI/4 + 200 * PI/3 && d <= 103951.431531 + 931 * PI/4 + 200 * PI/3 + 200 * 0.173422) //décalage à la sortie de République
            || (d >= 104261.431531 + 931 * PI/4 + 200 * PI/3 && d <= 104261.431531 + 1078 * PI/4 + 200 * PI/3) //milieu République-Place des Remparts
            || (d >= 104261.431531 + 1078 * PI/4 + 200 * PI/3 + 50 * V2 && d <= 104261.431531 + 1225 * PI/4 + 200 * PI/3 + 50 * V2) //arrivée Place des Remparts
            || (d >= 104525.431531 + 1225 * PI/4 + 200 * PI/3 + 50 * V2 && d <= 104525.431531 + 1429 * PI/4 + 200 * PI/3 + 50 * V2) //triche de 2PI/4
            || (d >= 104525.431531 + 1433 * PI/4 + 200 * PI/3 + 50 * V2 && d <= 104525.431531 + 1719 * PI/4 + 200 * PI/3 + 50 * V2)
            || (d >= 104562.431531 + 1719 * PI/4 + 200 * PI/3 + 50 * V2 && d <= 104562.431531 + 1863 * PI/4 + 200 * PI/3 + 50 * V2)
            ;

}

void GrandeFenetre::setLabelMonde(int n) {

    QString s;
    s.setNum(n);

    if (n < 5) {

        labelMonde->setText("Monde: <5");

    }

    else if (n < 10) {

        labelMonde->setText("Monde: <10");

    }

    else {

        labelMonde->setText("Monde: " + s);

    }

}

void GrandeFenetre::setLabelCapacite(int n) {

    capacite = n;
    QString s;
    s.setNum(n);
    labelCapacite->setText("Places restantes: " + s);

}

void GrandeFenetre::recevoirIconePert(int i) {

    switch(i) {

    case 1:

        tab->setTabIcon(1, QIcon("D:/Users/pioll/Mes programmes/NouveauSimMetro/Pictogrammes/Vert.png"));
        break;

    case 2:

        tab->setTabIcon(1, QIcon("D:/Users/pioll/Mes programmes/NouveauSimMetro/Pictogrammes/Jaune.png"));
        break;

    case 3:

        tab->setTabIcon(1, QIcon("D:/Users/pioll/Mes programmes/NouveauSimMetro/Pictogrammes/Rouge.png"));
        break;

    default:

        tab->setTabIcon(1, QIcon("D:/Users/pioll/Mes programmes/NouveauSimMetro/Pictogrammes/Erreur.png"));
        break;

    }

}

void GrandeFenetre::afficherLimiteVitesse() {

    sceneCompteur->removeItem(ancienPath);
    pathVitesse->operator=(QPainterPath());
    pathVitesse->arcMoveTo(-203, -203, 406, 406, 216);
    pathVitesse->arcTo(-203, -203, 406, 406, 216, -6 - 3 * limiteVitesse);
    ancienPath = sceneCompteur->addPath(*pathVitesse, QPen(QBrush(QColor(15, 185, 15)), 5));

}

void GrandeFenetre::dessinerCarte() {

    sceneCarte->removeItem(tx0);
    sceneCarte->removeItem(tx1);
    sceneCarte->removeItem(tx2);
    sceneCarte->removeItem(tx3);
    sceneCarte->removeItem(tx4);
    sceneCarte->removeItem(tx5);
    sceneCarte->removeItem(tx6);
    sceneCarte->removeItem(tx7);
    sceneCarte->removeItem(tx8);
    sceneCarte->removeItem(tx9);

    sceneCarte->clear();

    double d = f->distTotale();

    if (d < 100000) {

        if (tx0->toPlainText() != "Place des Remparts") {

            tx0->setPlainText("Place des Remparts");
            tx1->setPlainText("République");
            tx2->setPlainText("Bouchers");
            tx3->setPlainText("Rue des Chenilles");
            tx4->setPlainText("Boulevard de l'Hôpital");
            tx5->setPlainText("Cerisiers - Framboisiers");
            tx6->setPlainText("Blainville");
            tx7->setPlainText("Mirla");
            tx8->setPlainText("Histiaux");
            tx9->setPlainText("Bourse");

        }

        const double posStations[12] = {0.                                       ,
                                        307.                                     ,
                                        891  +  306 * PI/4              + 50 * V2,
                                        1416 +  306 * PI/4              + 50 * V2,
                                        2091 +  306 * PI/4              + 50 * V2,
                                        2491 +  403 * PI/4              + 50 * V2,
                                        2966 + 1081 * PI/4              + 50 * V2,
                                        3441 + 1081 * PI/4              + 50 * V2,
                                        3633 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                        4218 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                        4643 + 1225 * PI/4 + 300 * PI/3 + 50 * V2,
                                        4730 + 1705 * PI/4 + 300 * PI/3 + 50 * V2
                                       };

        int pxStation[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        for (int i = 0; i < 12; i++) { //le contenu intéressant n'est en fait exécuté que pour un unique i

            if (d >= posStations[i] && d < posStations[i + 1]) {

                sceneCarte->addRect(20, 800, 30, 30);

                for (int j = i + 1; j < 12; j++) {

                    if (posStations[j] - d < 770 - 40 * (10 - j)) {

                        sceneCarte->addEllipse(20, 770 - posStations[j] + d, 30, 30);
                        pxStation[j] = int(800 - posStations[j] + d);

                    }

                    else {

                        sceneCarte->addEllipse(20, 40 * (10 - j), 30, 30);
                        pxStation[j] = 40 * (10 - j) + 30;

                    }

                }

            }

        }

        if (pxStation[1]  >= 0) {tx0->setPos(70, pxStation[1]  - 30); tx0->setVisible(true);} else tx0->setVisible(false);
        if (pxStation[2]  >= 0) {tx1->setPos(70, pxStation[2]  - 30); tx1->setVisible(true);} else tx1->setVisible(false);
        if (pxStation[3]  >= 0) {tx2->setPos(70, pxStation[3]  - 30); tx2->setVisible(true);} else tx2->setVisible(false);
        if (pxStation[4]  >= 0) {tx3->setPos(70, pxStation[4]  - 30); tx3->setVisible(true);} else tx3->setVisible(false);
        if (pxStation[5]  >= 0) {tx4->setPos(70, pxStation[5]  - 30); tx4->setVisible(true);} else tx4->setVisible(false);
        if (pxStation[6]  >= 0) {tx5->setPos(70, pxStation[6]  - 30); tx5->setVisible(true);} else tx5->setVisible(false);
        if (pxStation[7]  >= 0) {tx6->setPos(70, pxStation[7]  - 30); tx6->setVisible(true);} else tx6->setVisible(false);
        if (pxStation[8]  >= 0) {tx7->setPos(70, pxStation[8]  - 30); tx7->setVisible(true);} else tx7->setVisible(false);
        if (pxStation[9]  >= 0) {tx8->setPos(70, pxStation[9]  - 30); tx8->setVisible(true);} else tx8->setVisible(false);
        if (pxStation[10] >= 0) {tx9->setPos(70, pxStation[10] - 30); tx9->setVisible(true);} else tx9->setVisible(false);

        sceneCarte->addItem(tx0);
        sceneCarte->addItem(tx1);
        sceneCarte->addItem(tx2);
        sceneCarte->addItem(tx3);
        sceneCarte->addItem(tx4);
        sceneCarte->addItem(tx5);
        sceneCarte->addItem(tx6);
        sceneCarte->addItem(tx7);
        sceneCarte->addItem(tx8);
        sceneCarte->addItem(tx9);

    }

    else {

        if (tx0->toPlainText() != "Bourse") {

            tx0->setPlainText("Bourse");
            tx1->setPlainText("Histiaux");
            tx2->setPlainText("Mirla");
            tx3->setPlainText("Blainville");
            tx4->setPlainText("Cerisiers - Framboisiers");
            tx5->setPlainText("Boulevard de l'Hôpital");
            tx6->setPlainText("Rue des Chenilles");
            tx7->setPlainText("Bouchers");
            tx8->setPlainText("République");
            tx9->setPlainText("Place des Remparts");

        }

        const double posStations[12] = {0.                                            ,
                                        100000.                                       ,
                                        100474.35                                     ,
                                        101059.35                                     ,
                                        101251.35 +  156 * PI/4 + 200 * PI/3          ,
                                        101726.35 +  156 * PI/4 + 200 * PI/3          ,
                                        102201.35 +  828 * PI/4 + 200 * PI/3          ,
                                        102601.35 +  931 * PI/4 + 200 * PI/3          ,
                                        103276.35 +  931 * PI/4 + 200 * PI/3          ,
                                        103801.35 +  931 * PI/4 + 200 * PI/3          ,
                                        104370.35 + 1225 * PI/4 + 200 * PI/3 + 50 * V2,
                                        104952.35 + 1863 * PI/4 + 200 * PI/3 + 50 * V2
                                       };

        int pxStation[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        for (int i = 0; i < 12; i++) { //le contenu intéressant n'est en fait exécuté que pour un unique i

            if (d >= posStations[i] && d < posStations[i + 1]) {

                sceneCarte->addRect(20, 800, 30, 30);

                for (int j = i + 1; j < 12; j++) {

                    if (posStations[j] - d < 770 - 40 * (10 - j)) {

                        sceneCarte->addEllipse(20, 770 - posStations[j] + d, 30, 30);
                        pxStation[j] = int(800 - posStations[j] + d);

                    }

                    else {

                        sceneCarte->addEllipse(20, 40 * (10 - j), 30, 30);
                        pxStation[j] = 40 * (10 - j) + 30;

                    }

                }

            }

        }

        if (pxStation[1]  >= 0) {tx0->setPos(70, pxStation[1]  - 30); tx0->setVisible(true);} else tx0->setVisible(false);
        if (pxStation[2]  >= 0) {tx1->setPos(70, pxStation[2]  - 30); tx1->setVisible(true);} else tx1->setVisible(false);
        if (pxStation[3]  >= 0) {tx2->setPos(70, pxStation[3]  - 30); tx2->setVisible(true);} else tx2->setVisible(false);
        if (pxStation[4]  >= 0) {tx3->setPos(70, pxStation[4]  - 30); tx3->setVisible(true);} else tx3->setVisible(false);
        if (pxStation[5]  >= 0) {tx4->setPos(70, pxStation[5]  - 30); tx4->setVisible(true);} else tx4->setVisible(false);
        if (pxStation[6]  >= 0) {tx5->setPos(70, pxStation[6]  - 30); tx5->setVisible(true);} else tx5->setVisible(false);
        if (pxStation[7]  >= 0) {tx6->setPos(70, pxStation[7]  - 30); tx6->setVisible(true);} else tx6->setVisible(false);
        if (pxStation[8]  >= 0) {tx7->setPos(70, pxStation[8]  - 30); tx7->setVisible(true);} else tx7->setVisible(false);
        if (pxStation[9]  >= 0) {tx8->setPos(70, pxStation[9]  - 30); tx8->setVisible(true);} else tx8->setVisible(false);
        if (pxStation[10] >= 0) {tx9->setPos(70, pxStation[10] - 30); tx9->setVisible(true);} else tx9->setVisible(false);

        sceneCarte->addItem(tx0);
        sceneCarte->addItem(tx1);
        sceneCarte->addItem(tx2);
        sceneCarte->addItem(tx3);
        sceneCarte->addItem(tx4);
        sceneCarte->addItem(tx5);
        sceneCarte->addItem(tx6);
        sceneCarte->addItem(tx7);
        sceneCarte->addItem(tx8);
        sceneCarte->addItem(tx9);

    }

}

void GrandeFenetre::loadPA() {

    g = new FenetreChargement(900, "Chargement du PA...");

    int posAncienne = 0;
    int vAncienne = 0;

    QFile fichiersA[10], fichiersR[10];
    QTextStream fluxA[10], fluxR[10];

    for (int i = 0; i <= 9; i++) {

        QString s0;
        s0.setNum(i + 1);
        fichiersA[i].setFileName("D:/Users/pioll/Mes programmes/SimMetro3D/PA" + s0);

        if (fichiersA[i].open(QIODevice::ReadOnly)) {

            fluxA[i].setDevice(&fichiersA[i]);

            while (!fluxA[i].atEnd()) {

                QString s = fluxA[i].readLine();
                QString ss = s;
                ss.truncate(4);
                int pos = ss.toInt();
                ss.truncate(2);
                ss[0] = s[5];
                ss[1] = s[6];
                int v = ss.toInt();

                for (int indice = posAncienne; indice < pos; indice++) {

                    vitessesPAAller[i][indice] = vAncienne;

                }

                posAncienne = pos;
                vAncienne = v;

            }

            for (int indice = posAncienne; indice < 1100; indice++) {

                vitessesPAAller[i][indice] = vAncienne;

            }

            posAncienne = 0;
            vAncienne = 0;
            fichiersA[i].close();

        }

    }

    for (int i = 0; i <= 9; i++) {

        QString s0;
        s0.setNum(i + 101);
        fichiersR[i].setFileName("D:/Users/pioll/Mes programmes/SimMetro3D/PA" + s0);

        if (fichiersR[i].open(QIODevice::ReadOnly)) {

            fluxR[i].setDevice(&fichiersR[i]);

            while (!fluxR[i].atEnd()) {

                QString s = fluxR[i].readLine();
                QString ss = s;
                ss.truncate(4);
                int pos = ss.toInt();
                ss.truncate(2);
                ss[0] = s[5];
                ss[1] = s[6];
                int v = ss.toInt();

                for (int indice = posAncienne; indice < pos; indice++) {

                    vitessesPARetour[i][indice] = vAncienne;

                }

                posAncienne = pos;
                vAncienne = v;

            }

            for (int indice = posAncienne; indice < 1100; indice++) {

                vitessesPARetour[i][indice] = vAncienne;

            }

            posAncienne = 0;
            vAncienne = 0;
            fichiersR[i].close();

        }


    }




}
