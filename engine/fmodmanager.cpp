#include "fmodmanager.h"

FMODManager *FMODManager::getInstance()
{
    static FMODManager *instance = new FMODManager();
    return instance;
}

#ifdef __linux__

void FMODManager::loadBank(QString path)
{
    FMOD::Studio::Bank* bank = NULL;
    ERRCHECK( system->loadBankFile(FileUtility::loadTempFile(path).toStdString().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank) );
    qDebug() << "[FMODManager]:" << "loaded bank" << path;
    int count;
    system->getBankCount(&count);

    qDebug() << "[FMODManager]:" << "bank count = " << count;
    bank->getEventCount(&count);
    if(count > 0) {
        QVector<FMOD::Studio::EventDescription *> events;
        events.resize(count);
        qDebug() << "[FMODManager]:" << "found" << count << "events";
        bank->getEventList(&events[0], count, NULL);
        for(FMOD::Studio::EventDescription *i : events) {
            char s[100];
            i->getPath(s, 100, NULL);
            qDebug() << "[FMODManager]:" << "event found : " << s;
        }
    } else {
        qDebug() << "[FMODManager]:" << "no event found!";
    }
}

void FMODManager::setCurrentEvent(QString description)
{
    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK( system->getEvent(description.toStdString().c_str(), &eventDescription) );

    ERRCHECK( eventDescription->createInstance(&eventInstance) );

//    qDebug() << "[FMODManager]:" << "added event instance of" << description;
}

void FMODManager::startEventInstance()
{
    eventInstance->start();
//    qDebug() << "[FMODManager]:" << "started event instance:" << eventInstance;
    eventInstance->release();
}

void FMODManager::setParameterValue(QString parameter, float value)
{
    FMOD::Studio::ParameterInstance* param = NULL;
    ERRCHECK(eventInstance->getParameter(parameter.toStdString().c_str(), &param) );
    ERRCHECK(param->setValue(value));
}

void FMODManager::setListenerPosition(QVector3D position)
{
    FMOD_3D_ATTRIBUTES attributes = { { 0 } };
    attributes.forward.z = -1.0f;
    attributes.up.y = 1.0f;
    attributes.position.x = position.x();
    attributes.position.y = position.y();
    attributes.position.z = position.z();
    ERRCHECK( system->setListenerAttributes(0, &attributes) );

    attributes = { { 0 } };
    attributes.up.y = 1.0f;
    attributes.position.x = position.x();
    attributes.position.y = position.y();
    attributes.position.z = position.z();
    ERRCHECK( currentMusic->set3DAttributes(&attributes) );
}

void FMODManager::setEventInstancePosition(QVector3D position)
{
    FMOD_3D_ATTRIBUTES attributes = { { 0 } };
    attributes.up.y = 1.0f;
    attributes.position.x = position.x();
    attributes.position.y = position.y();
    attributes.position.z = position.z();
    ERRCHECK( eventInstance->set3DAttributes(&attributes) );
}

void FMODManager::setEventInstanceVolume(float volume)
{
    eventInstance->setVolume(volume);
}

void FMODManager::setCurrentMusic(QString music)
{
    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK( system->getEvent(music.toStdString().c_str(), &eventDescription) );

    ERRCHECK( eventDescription->createInstance(&currentMusic) );
    qDebug() << "[FMODManager]:" << "added current music" << music;
}

void FMODManager::setNextMusic(QString music)
{
    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK( system->getEvent(music.toStdString().c_str(), &eventDescription) );

    ERRCHECK( eventDescription->createInstance(&nextMusic) );
    qDebug() << "[FMODManager]:" << "added current music" << music;
}

void FMODManager::setCurrentMusicVolume(float volume)
{
    currentMusic->setVolume(volume);
}

void FMODManager::setCurrentMusicParameterValue(QString parameter, float value)
{
    FMOD::Studio::ParameterInstance* param = NULL;
    ERRCHECK(currentMusic->getParameter(parameter.toStdString().c_str(), &param) );
    ERRCHECK(param->setValue(value));
    ERRCHECK(currentMusic->setPaused(false));
}

void FMODManager::setNextMusicCurrent()
{
    currentMusic->release();
    currentMusic = nextMusic;
}

void FMODManager::startCurrentMusic()
{
    currentMusic->start();
}

void FMODManager::pauseCurrentMusic(bool paused)
{
    currentMusic->setPaused(paused);
}

void FMODManager::set3DSettings(float dopplerScale, float distanceFactor, float rolloffScale)
{
    ERRCHECK( lowLevelSystem->set3DSettings(dopplerScale, distanceFactor, rolloffScale));
}

void FMODManager::update()
{
    ERRCHECK( system->update() );
}

FMODManager::FMODManager()
{
    ERRCHECK( FMOD::Studio::System::create(&system) );
    ERRCHECK( system->getLowLevelSystem(&lowLevelSystem) );
    ERRCHECK( lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );
    ERRCHECK( system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );
}

void FMODManager::ERRCHECK(FMOD_RESULT result) {
    if(result != FMOD_OK) {
        qDebug() << "error : " << FMOD_ErrorString(result);
    }
}

#else


void FMODManager::loadBank(QString path)
{

}

void FMODManager::setCurrentEvent(QString description)
{

}

void FMODManager::startEventInstance()
{

}

void FMODManager::setParameterValue(QString parameter, float value)
{

}

void FMODManager::setListenerPosition(QVector3D position)
{

}

void FMODManager::setEventInstancePosition(QVector3D position)
{

}

void FMODManager::setEventInstanceVolume(float volume)
{
}

void FMODManager::setCurrentMusic(QString music)
{

}

void FMODManager::setNextMusic(QString music)
{

}

void FMODManager::setCurrentMusicVolume(float volume)
{
}

void FMODManager::setCurrentMusicParameterValue(QString parameter, float value)
{

}

void FMODManager::setNextMusicCurrent()
{

}

void FMODManager::startCurrentMusic()
{
}

void FMODManager::pauseCurrentMusic(bool paused)
{
}

void FMODManager::set3DSettings(float dopplerScale, float distanceFactor, float rolloffScale)
{
}

void FMODManager::update()
{
}

FMODManager::FMODManager()
{

}

void FMODManager::ERRCHECK(FMOD_RESULT result) {

}

#endif
