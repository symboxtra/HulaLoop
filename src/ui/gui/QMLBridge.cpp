#include "QMLBridge.h"
//#include "FftRealPair.h"
#include "FftRealPair.cpp"

#include <QCoreApplication>
#include <QProcess>
#include <QUrl>
#include <QDir>

#include <iostream>
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


using namespace hula;

/**
 * Construct a new instance of the QMLBridge class.
 *
 * @param parent Object which QMLBridge should be child of.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    transport = new Transport();
    rb = transport->getController()->createBuffer(0.5);
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
bool QMLBridge::record()
{
    bool success = transport->record();
    emit stateChanged();

    startVisThread();

    return success;
}

/**
 * Trigger stop in the Transport and update the UI state via signal.
 */
bool QMLBridge::stop()
{
    bool success = transport->stop();
    emit stateChanged();

    stopVisThread();

    return success;
}

/**
 * Trigger playback in the Transport and update the UI state via signal.
 */
bool QMLBridge::play()
{
    bool success = transport->play();
    emit stateChanged();

    startVisThread();

    return success;
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
bool QMLBridge::pause()
{
    bool success = transport->pause();
    emit stateChanged();

    stopVisThread();

    return success;
}

/**
 * Deletes all the temp files that the program has created
 */
void QMLBridge::discard()
{
    transport->discard();
}

/**
 * Match a string that the user chose to the input device list
 * and notify the backend
 */
void QMLBridge::setActiveInputDevice(QString QDeviceName)
{
    hlDebug() << "setActiveDevice() called" << std::endl;

    std::string deviceName = QDeviceName.toStdString();
    std::vector<Device *> iDevices = transport->getController()->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
    for (auto const &device : iDevices)
    {
        if (device->getName() == deviceName)
        {
            transport->getController()->setActiveInputDevice(device);
            Device::deleteDevices(iDevices);
            return;
        }
    }

    // Should not get here should have found the device
    hlDebug() << "Input device not found: " << deviceName << std::endl;
}

/**
 * Match a string that the user chose to the output device list
 * and notify the backend
 */
void QMLBridge::setActiveOutputDevice(QString QDeviceName)
{
    std::string deviceName = QDeviceName.toStdString();
    std::vector<Device *> oDevices = transport->getController()->getDevices(DeviceType::PLAYBACK);
    for (auto const &device : oDevices)
    {
        if (device->getName() == deviceName)
        {
            transport->getController()->setActiveOutputDevice(device);
            Device::deleteDevices(oDevices);
            return;
        }
    }

    // Should not get here should have found the device
    hlDebug() << "Output device not found: " << deviceName << std::endl;
}

/**
 * Get the current input devices
 *
 * @return QString containing current input devices
 */
QString QMLBridge::getInputDevices()
{
    std::string devices;
    std::vector<Device *> vd = transport->getController()->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
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
    std::string devices;
    std::vector<Device *> vd = transport->getController()->getDevices(DeviceType::PLAYBACK);
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
    std::string directory = dir.toStdString();
    int substrLen = 7;
    if (dir[9] == ':')
    {
        // WINDOWS
        substrLen = 8;
    }
    directory = directory.substr(substrLen);
    transport->exportFile(directory);
}

/**
 * Start the thread that reads the ring buffer and updates
 * the visualizer.
 */
void QMLBridge::startVisThread()
{
    // Stop the previous thread if present
    stopVisThread();

    // Reset the state
    endVis.store(false);

    // Start the new thread
    visThreads.emplace_back(std::thread(&updateVisualizer, this));
}

/**
 * Stop the thread that updates the visualizer.
 */
void QMLBridge::stopVisThread()
{
    endVis.store(true);

    for (auto &t : visThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    visThreads.clear();
}

/**
 * Perform FFT and update the visualizer.
 */
void QMLBridge::updateVisualizer(QMLBridge *_this)
{
    _this->transport->getController()->addBuffer(_this->rb);

    int maxSize = 512;
    int accuracy = 5;
    // float *temp = new float[maxSize];

    while (!_this->endVis.load())
    {
        vector<double> actualoutreal;
        vector<double> actualoutimag;

        float *data1;
        float *data2;
        ring_buffer_size_t size1;
        ring_buffer_size_t size2;
        ring_buffer_size_t bytesRead;

        // Only process every @ref accuracy cycles
        int cycle = 1;

        while (!_this->endVis.load() && actualoutreal.size() < maxSize)
        {
                // Do directRead until Windows read is fixed
                // bytesRead =_this->rb->read(temp, maxSize
                bytesRead = _this->rb->directRead(maxSize, (void **)&data1, &size1, (void **)&data2, &size2);

                // Keep draining the buffer, but only actually
                // process the drained data every nth cycle
                if (cycle % accuracy == 0)
                {
                    for (int i = 0; i < size1 && actualoutreal.size() < maxSize; i++)
                    {
                        actualoutimag.push_back(data1[i]);
                        actualoutreal.push_back(data1[i]);
                    }

                    for (int i = 0; i < size2 && actualoutreal.size() < maxSize; i++)
                    {
                        actualoutimag.push_back(data2[i]);
                        actualoutreal.push_back(data2[i]);
                    }
                }
                else
                {
                    // Only count cycles that actually have data
                    if (bytesRead > 0)
                    {
                        cycle++;
                    }
                }
        }

        Fft::transform(actualoutreal, actualoutimag);

        std::vector<double> heights;
        double sum = 0;
        for (int i = 0; i < actualoutreal.size(); i++)
        {
            if (i % 8 == 0)
            {
                sum = fabs(sum);

                #if _WIN32
                // Adjust the values since WASAPI data comes in screaming loud
                heights.push_back(sum * 0.2);
                #else
                heights.push_back(sum);
                #endif

                sum = 0;
            }
            else
            {
                sum += actualoutreal[i];
            }
        }

        _this->emit visData(heights);
    }

    _this->transport->getController()->removeBuffer(_this->rb);
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

/**
 * Destructor for QMLBridge
 */
QMLBridge::~QMLBridge()
{
    hlDebugf("QMLBridge destructor called\n");

    stopVisThread();
    delete transport;
    delete rb;
}