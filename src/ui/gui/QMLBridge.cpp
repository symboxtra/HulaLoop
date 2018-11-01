#include "QMLBridge.h"
//#include "FftRealPair.h"
#include "FftRealPair.cpp"

#include <QCoreApplication>
#include <QProcess>
#include <QUrl>

#include <string>
#include <thread>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include <thread>     
#include <chrono>
#include <math.h>
#include <stdio.h>


/**
 * Construct a new instance of the QMLBridge class.
 *
 * @param parent Object which QMLBridge should be child of.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    transport = new Transport;
    rb=transport->getController()->createBuffer(1);
    
}

/**
 * Return the current state of the Transport instance as a QString.
 *
 * @return state State of Transport instance
 */
QString QMLBridge::getTransportState() const
{
    return QString::fromStdString(transport->stateToStr(transport->getState()));
}

/**
 * Trigger record in the Transport and update the UI state via signal.
 */
void QMLBridge::record()
{
    transport->record();
    emit stateChanged();
    pauseNotPressed=true;
    getData();
}

/**
 * Trigger stop in the Transport and update the UI state via signal.
 */
void QMLBridge::stop()
{
    transport->stop();
    emit stateChanged();
}

/**
 * Trigger playback in the Transport and update the UI state via signal.
 */
void QMLBridge::play()
{
    transport->play();
    emit stateChanged();
    pauseNotPressed=true;
    getData();
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
void QMLBridge::pause()
{
    transport->pause();
    emit stateChanged();
    pauseNotPressed=false;
}



/**
 * Match a string that the user chose to the input device list
 * and notify the backend
 */
void QMLBridge::setActiveInputDevice(QString QDeviceName)
{
    cout << "setActiveDevice() called" << endl;
    string deviceName = QDeviceName.toStdString();
    vector<Device *> iDevices = transport->getController()->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
    for (auto const &device : iDevices)
    {
        if (device->getName() == deviceName)
        {
            transport->getController()->setActiveInputDevice(device);
            Device::deleteDevices(iDevices);
            return;
        }
    }
    //Should not get here should have found the device
    cerr << "Input device not found: " << deviceName << endl;
}

/**
 * Match a string that the user chose to the output device list
 * and notify the backend
 */
void QMLBridge::setActiveOutputDevice(QString QDeviceName)
{
    string deviceName = QDeviceName.toStdString();
    vector<Device *> oDevices = transport->getController()->getDevices(DeviceType::PLAYBACK);
    for (auto const &device : oDevices)
    {
        if (device->getName() == deviceName)
        {
            transport->getController()->setActiveOutputDevice(device);
            Device::deleteDevices(oDevices);
            return;
        }
    }
    //Should not get here should have found the device
    cerr << "Output device not found: " << deviceName << endl;
}

/**
 * Get the current input devices
 *
 * @return QString containing current input devices
 */
QString QMLBridge::getInputDevices()
{
    string devices;
    vector<Device *> vd = transport->getController()->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
    for (int i = 0; i < vd.size(); i++)
    {
        devices += vd[i]->getName();
        if (i < vd.size() - 1)
        {
            devices += ",";
        }
    }
    Device::deleteDevices(vd);
    return QString::fromStdString(devices);
}

/**
 * Get the current output devices
 *
 * @return QString containing current output devices
 */
QString QMLBridge::getOutputDevices()
{
    string devices;
    vector<Device *> vd = transport->getController()->getDevices(DeviceType::PLAYBACK);
    for (int i = 0; i < vd.size(); i++)
    {
        devices += vd[i]->getName();
        if (i < vd.size() - 1)
        {
            devices += ",";
        }
    }
    Device::deleteDevices(vd);
    return QString::fromStdString(devices);
}

/**
 * Get the directory the user wants to save to
 *
 * @param QString containing the directory
 *
 * @return String containing the directory
 */
void QMLBridge::saveFile(QString dir)
{
    // create a Qurl so it is easy to convert
    QUrl url(dir);
    string directory = url.path().toStdString();
    transport->exportFile(directory);
}
void QMLBridge::getData(){
    //rb=transport->getController()->createAndAddBuffer(.5);
    std::thread visThread(&updateVisualizer,this);
    visThread.detach();

}
void QMLBridge::updateVisualizer(QMLBridge *_this){
    _this->transport->getController()->addBuffer(_this->rb);
    int maxSize = 512;
    float * temp = new float[maxSize];
    while(1 && _this->getPauseState()){
        vector<double> actualoutreal;
        vector<double> actualoutimag;
        size_t bytesRead;
        while(actualoutreal.size()<maxSize){
            bytesRead=_this->rb->read(temp, maxSize);
            for(int i=0;i<bytesRead;i++){
                actualoutimag.push_back(temp[i]);
                actualoutreal.push_back(temp[i]);
            }
        }
	    Fft::transform(actualoutreal, actualoutimag);
        std::vector<double> heights;
        double sum=0;
        for(int i=0;i<bytesRead;i++){
                if(i%8==0){
                    sum=fabs(sum);
                    heights.push_back(sum);
                    sum=0;
                }
                else{
                    sum+=actualoutreal[i];
                }
        }
        _this->emit visData(heights);
    }
    _this->transport->getController()->removeBuffer(_this->rb);

}

bool QMLBridge::getPauseState(){
    return(this->pauseNotPressed);
}
/**
 * Launch the updater process.
 */
void QMLBridge::launchUpdateProcess()
{

    QProcess *proc = new QProcess(this);

    QString procName = QCoreApplication::applicationDirPath() + "/hulaloop-launcher";
    QStringList args;
    args << "silent";

    proc->setProgram(procName);
    proc->setArguments(args);

    proc->start(procName, args);
    proc->waitForFinished();

}
