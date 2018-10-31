#include "QMLBridge.h"
//#include "FftRealPair.h"
#include "FftRealPair.cpp"

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
    getData();
    pauseNotPressed=true;
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
void QMLBridge::pause()
{
    transport->pause();
    emit stateChanged();
    pauseNotPressed=false;
    //cout<<"pause clicked\n";
    
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
    //cout<<"in get data\n";
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
    int maxSize = 512;
    float * temp = new float[maxSize];
    //cout<<"before the for loop\n";
    //for(int i=0;i<10;i++){
    while(1){
        size_t bytesRead=_this->rb->read(temp, maxSize);
        //printf("Read %zu bytes.\n", bytesRead);
        vector<double> actualoutreal;
	    vector<double> actualoutimag;
        for(int i=0;i<bytesRead;i++){
            actualoutimag.push_back(temp[i]);
            actualoutreal.push_back(temp[i]);
        }
        //printf("before transform real vector:\n");
        for(int i=0;i<bytesRead;i++){
            //printf("%d:%f\n",i,actualoutreal[i]);
        }/*
         printf("before transform imag vector:\n");
        for(int i=0;i<bytesRead;i++){
            printf("%f\n",actualoutimag[i]);
        }*/
	    Fft::transform(actualoutreal, actualoutimag);
        //printf("after transform real vector:\n");
        std::vector<double> heights;
        double sum=0;
        for(int i=0;i<bytesRead;i++){
            //printf("%f\n",actualoutreal[i]);
            //get every 32 values, sum them, absolute value them, then make them the heights
                if(i%8==0){
                    sum=fabs(sum);
                    heights.push_back(sum);
                    sum=0;
                }
                else{
                    sum+=actualoutreal[i];
                }
        }
        for(int i=0;i<heights.size();i++){
        //printf("height%d: %f\n",i,heights[i]);
        }
        
        /* printf("after transform imag vector:\n");
        for(int i=0;i<bytesRead;i++){
            printf("%f\n",actualoutimag[i]);
        }*/
        _this->emit visData(heights);
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
   /* vector<qreal> dataFake;
    for(int i=0;i<10;i++){
        dataFake.push_back((qreal) .5);
    }
    _this->emit visData(dataFake);*/
}

