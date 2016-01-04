#ifndef FMODMANAGER_H
#define FMODMANAGER_H

#include <QDebug>
#include <QVector>
#include <QVector3D>

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_common.h"
#include "fmod_dsp_effects.h"
#include "fmod_codec.h"
#include "fmod_output.h"
#include "fmod_dsp.h"
#include "fmod_errors.h"

#include "engine/tools/fileutility.h"

class FMODManager
{
public:
    static FMODManager *getInstance();

    void loadBank(QString path);

    void setCurrentEvent(QString description);
    void startEventInstance();

    void setParameterValue(QString parameter, float value);

    void setListenerPosition(QVector3D position);
    void setEventInstancePosition(QVector3D position);
    void setEventInstanceVolume(float volume);

    void setCurrentMusic(QString music);
    void setNextMusic(QString music);

    void setCurrentMusicVolume(float volume);
    void setCurrentMusicParameterValue(QString parameter, float value);
    void setNextMusicCurrent();
    void startCurrentMusic();
    void pauseCurrentMusic(bool paused);

    void set3DSettings(float dopplerScale, float distanceFactor, float rolloffScale);

    void update();

private:
    FMODManager();
    void ERRCHECK(FMOD_RESULT result);

    FMOD::Studio::System* system;
    FMOD::System* lowLevelSystem;

    FMOD::Studio::EventInstance *eventInstance;
    FMOD::Studio::EventInstance *currentMusic, *nextMusic;

    FMOD::ChannelGroup *eventsGroup, *musicGroup;

};

#endif // FMODMANAGER_H
