#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T17:32:41
#
#-------------------------------------------------

QT       += core gui opengl

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
    game/towerghostcomponent.cpp \
    game/bulletcomponent.cpp \
    engine/tools/fileutility.cpp \
    game/enemycomponent.cpp \
    engine/fmodmanager.cpp

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
    game/towerghostcomponent.h \
    game/bulletcomponent.h \
    engine/tools/fileutility.h \
    game/enemycomponent.h \
    engine/fmodmanager.h

FORMS    +=
QMAKE_CXXFLAGS += -std=c++11 -lGL -lGLU -lglut -lassimp -libfmodstudio -libfmod -lfmod -lfmodstudio

unix {
INCLUDEPATH += "./fmod/unix/api/lowlevel/inc"
INCLUDEPATH += "./fmod/unix/api/studio/inc"
LIBS += -L"$$_PRO_FILE_PWD_/fmod/unix/api/studio/lib/x86_64/" -lfmodstudio
LIBS += -L"$$_PRO_FILE_PWD_/fmod/unix/api/lowlevel/lib/x86_64/" -lfmod
LIBS += -lassimp
}

win64 {
LIBS += -L"/home/noe/Downloads/FMOD Studio API Windows/api" -lfmodstudio
LIBS += -L"/home/noe/Downloads/FMOD Studio API Windows/api" -lfmod
}

RESOURCES += \
    resource.qrc
