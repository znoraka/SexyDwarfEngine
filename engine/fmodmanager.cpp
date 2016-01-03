#include "fmodmanager.h"

FMODManager *FMODManager::getInstance()
{
    static FMODManager *instance = new FMODManager();
    return instance;
}

int FMODManager::loadBank(QString path)
{
    FMOD::Studio::Bank* bank = NULL;
    ERRCHECK( system->loadBankFile(FileUtility::loadTempFile(path).toStdString().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank) );
    banks.push_back(bank);
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

    return banks.size() - 1;
}

FMOD::Studio::Bank *FMODManager::getBank(int bankId)
{
    if(bankId >= banks.size()) return nullptr;

    return banks[bankId];
}

FMODEventInstance FMODManager::addEventInstance(QString description)
{

    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK( system->getEvent(description.toStdString().c_str(), &eventDescription) );

    FMOD::Studio::EventInstance* eventInstance = NULL;
    ERRCHECK( eventDescription->createInstance(&eventInstance) );
//    int count;
//    eventInstance->getParameterCount(&count);
//    qDebug() << "[FMODManager]:" << "found" << count << "parameters";

//    for (int i = 0; i < count; ++i) {
//        FMOD::Studio::ParameterInstance *p;
//        eventInstance->getParameterByIndex(i, &p);
//        FMOD_STUDIO_PARAMETER_DESCRIPTION *description;
//        p->getDescription(description);
//        eventInstance->setParameterValue(description->name, 1);
//        qDebug() << description->name;
//    }

    eventInstances.push_back(eventInstance);
    qDebug() << "[FMODManager]:" << "added event instance of" << description;

    return eventInstances.size() - 1;
}

void FMODManager::startEventInstance(FMODEventInstance eventInstance)
{
    qDebug() << "[FMODManager]:" << "started event instance:" << eventInstance;

    eventInstances[eventInstance]->start();
}

void FMODManager::setParameterValue(FMODEventInstance eventInstance, QString parameter, float value)
{
    FMOD::Studio::ParameterInstance* param = NULL;
    ERRCHECK( eventInstances[eventInstance]->getParameter(parameter.toStdString().c_str(), &param) );
    ERRCHECK(param->setValue(value));
}

void FMODManager::setListenerPosition(QVector3D position)
{
    FMOD_3D_ATTRIBUTES attributes = { { 0 } };
    attributes.forward.z = 1.0f;
    attributes.up.y = 1.0f;
    attributes.position.x = position.x();
    attributes.position.y = position.y();
    attributes.position.z = position.z();
    ERRCHECK( system->setListenerAttributes(0, &attributes) );
}

void FMODManager::setEventInstancePosition(FMODEventInstance ei, QVector3D position)
{
    FMOD::Studio::EventInstance* eventInstance = eventInstances[ei];
    FMOD_3D_ATTRIBUTES attributes = { { 0 } };
    attributes.up.y = 1.0f;
    attributes.position.x = position.x();
    attributes.position.y = position.y();
    attributes.position.z = position.z();
    ERRCHECK( eventInstance->set3DAttributes(&attributes) );
}

void FMODManager::set3DSettings(float dopplerScale, float distanceFactor, float rolloffScale)
{
    lowLevelSystem->set3DSettings(dopplerScale, distanceFactor, rolloffScale);
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
