QT += widgets
QT += opengl
QT += gui
QT += core
QT += multimedia

LIBS += -lOpengl32
LIBS += -lglut
LIBS += -lglut32
LIBS += -lglu32
LIBS += -lglaux

SOURCES += \
    main.cpp \
    fenetreprincipale.cpp \
    grandefenetre.cpp \
    mafenetre.cpp \
    fenetreaccueil.cpp \
    fenetrechargement.cpp

HEADERS += \
    fenetreprincipale.h \
    grandefenetre.h \
    mafenetre.h \
    fenetreaccueil.h \
    fenetrechargement.h

DISTFILES += \
    PAD \
    PA101 \
    PA102 \
    PA103 \
    PA104 \
    PA105 \
    PA106 \
    PA107 \
    PA108 \
    PA109 \
    PA0 \
    PA9 \
    PA8 \
    PA7 \
    PA6 \
    PA5 \
    PA4 \
    PA3 \
    PA2 \
    PA1 \
    PA110 \
    PA10
