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

    eventInstances.push_back(eventInstance);
    qDebug() << "[FMODManager]:" << "added event instance of" << description;

    return eventInstances.size() - 1;
}

void FMODManager::startEventInstance(FMODEventInstance eventInstance)
{
    eventInstances[eventInstance]->start();
}

void FMODManager::setParameterValue(FMODEventInstance eventInstance, QString parameter, float value)
{
    FMOD::Studio::ParameterInstance* param = NULL;
    ERRCHECK( eventInstances[eventInstance]->getParameter(parameter.toStdString().c_str(), &param) );
    ERRCHECK(param->setValue(value));
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

    //    FMOD::Studio::EventDescription* eventDescription = NULL;
    //    ERRCHECK( system->getEvent("event:/Vehicles/Basic Engine", &eventDescription) );

    //    FMOD::Studio::EventInstance* eventInstance = NULL;
    //    ERRCHECK( eventDescription->createInstance(&eventInstance) );

    //    FMOD::Studio::ParameterInstance* rpm = NULL;
    //    ERRCHECK( eventInstance->getParameter("RPM", &rpm) );

    //    ERRCHECK( rpm->setValue(2150) );

    //    ERRCHECK( eventInstance->start() );

    //    // Position the listener at the origin
    //        FMOD_3D_ATTRIBUTES attributes = { { 0 } };
    //        attributes.forward.z = 1.0f;
    //        attributes.up.y = 1.0f;
    //        ERRCHECK( system->setListenerAttributes(0, &attributes) );

    //        // Position the event 2 units in front of the listener
    //        attributes.position.z = 2.0f;
    //        ERRCHECK( eventInstance->set3DAttributes(&attributes) );
}

void FMODManager::ERRCHECK(FMOD_RESULT result) {
    if(result != FMOD_OK) {
        qDebug() << "error : " << FMOD_ErrorString(result);
    }
}
