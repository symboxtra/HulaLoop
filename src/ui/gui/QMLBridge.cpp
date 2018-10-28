#include "QMLBridge.h"

#include <QUrl>
#include <string>
#include <thread>
#include <stdlib.h>
#include <thread>     
#include <chrono>


/**
 * Construct a new instance of the QMLBridge class.
 *
 * @param parent Object which QMLBridge should be child of.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    transport = new Transport;
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
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
void QMLBridge::pause()
{
    transport->pause();
    emit stateChanged();
    cout<<"pause clicked\n";
    getData();
    //emit visData(dataFake);
}



/**
 * Match a string that the user chose to the input device list
 * and notify the backend
 */
void QMLBridge::setActiveInputDevice(QString QDeviceName)
{
    cout << "SETACTIVEiDEVICECALLED" << endl;
    string deviceName = QDeviceName.toStdString();
    vector<Device *> iDevices = transport->getController()->getInputDevices();
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
    vector<Device *> oDevices = transport->getController()->getOutputDevices();
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
    vector<Device *> vd = transport->getController()->getInputDevices();
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
    vector<Device *> vd = transport->getController()->getOutputDevices();
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
    rb=transport->getController()->createAndAddBuffer(.5);
    //std::this_thread::sleep_for (std::chrono::seconds(1));
    cout<<"in get data\n";
    std::thread visThread(&updateVisualizer,this);
    visThread.detach();

}
void QMLBridge::updateVisualizer(QMLBridge *_this){
    /**CREATE BUFFER
     * WHILE(1)
     * READ FROM BUFFER
     * GET NUM OF BINS
     * PROCESS THE DATA
     * UPDATE VISUALIZER
     * SLEEP
     * REPEAT
     */
    //FOR NOW JUST GIVE RANDOM DATA IN ARRAY
    int maxSize = 256;
    float * temp = new float[maxSize];
    cout<<"before the for loop\n";
    //for(int i=0;i<10;i++){
    while(1){
        size_t bytesRead=_this->rb->read(temp, maxSize);
        //printf("Read %zu bytes.\n", bytesRead);
        std::this_thread::sleep_for (std::chrono::milliseconds(5));
    }
   /* vector<qreal> dataFake;
    for(int i=0;i<10;i++){
        dataFake.push_back((qreal) .5);
    }
    _this->emit visData(dataFake);*/


}

