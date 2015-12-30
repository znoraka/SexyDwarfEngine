#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T17:32:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameEngine
TEMPLATE = app


SOURCES += \
    engine/camera.cpp \
    engine/gamewindow.cpp \
    main.cpp \
    engine/openglwindow.cpp \
    engine/game.cpp \
    engine/scene.cpp \
    engine/entity.cpp \
    engine/components/component.cpp \
    game/trianglecomponent.cpp \
    engine/tools/pool.cpp \
    engine/components/positioncomponent.cpp \
    engine/systems/system.cpp \
    engine/systems/trianglesystem.cpp \
    engine/components/volumecomponent.cpp \
    game/mapcomponent.cpp \
    game/pathfollowercomponent.cpp \
    game/towercomponent.cpp \
    engine/tools/delaunay.c \
    game/testscene.cpp \
    game/towerghostcomponent.cpp

HEADERS  += \
    engine/camera.h \
    engine/gamewindow.h \
    engine/openglwindow.h \
    engine/game.h \
    engine/scene.h \
    engine/entity.h \
    engine/components/component.h \
    game/trianglecomponent.h \
    engine/tools/pool.h \
    engine/components/positioncomponent.h \
    engine/systems/system.h \
    engine/systems/trianglesystem.h \
    engine/components/volumecomponent.h \
    consts.h \
    game/mapcomponent.h \
    game/pathfollowercomponent.h \
    game/towercomponent.h \
    engine/tools/delaunay.h \
    game/testscene.h \
    game/towerghostcomponent.h

FORMS    +=
QMAKE_CXXFLAGS += -std=c++11 -lGL -lGLU -lglut -lassimp

LIBS += -L"/home/noe/Downloads/fmodstudioapi10703linux/api/studio/lib/  -libfmodex"
LIBS += -L"/home/noe/Downloads/fmodstudioapi10703linux/api/lowlevel/lib/  -libfmodex"
LIBS += -lassimp

INCLUDEPATH += "/home/noe/Downloads/fmodstudioapi10703linux/api/studio/inc"
INCLUDEPATH += "/home/noe/Downloads/fmodstudioapi10703linux/api/lowlevel/inc"

RESOURCES += \
    resource.qrc
