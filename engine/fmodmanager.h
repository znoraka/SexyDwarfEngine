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

typedef int FMODBank;
typedef int FMODEventInstance;
typedef int FMODParameterInstance;

class FMODManager
{
public:
    static FMODManager *getInstance();

    FMODBank loadBank(QString path);
    FMOD::Studio::Bank *getBank(int bankId);

    FMODEventInstance addEventInstance(QString description);
    void startEventInstance(FMODEventInstance eventInstance);

    void setParameterValue(FMODEventInstance eventInstance, QString parameter, float value);

    void setListenerPosition(QVector3D position);
    void setEventInstancePosition(FMODEventInstance ei, QVector3D position);

    void set3DSettings(float dopplerScale, float distanceFactor, float rolloffScale);

    void update();

private:
    FMODManager();
    void ERRCHECK(FMOD_RESULT result);

    FMOD::Studio::System* system;
    FMOD::System* lowLevelSystem;

    QVector<FMOD::Studio::Bank*> banks;
    QVector<FMOD::Studio::EventInstance*> eventInstances;

};

#endif // FMODMANAGER_H
