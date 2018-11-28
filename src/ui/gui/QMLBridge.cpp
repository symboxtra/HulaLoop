#include "QMLBridge.h"

#include <QCoreApplication>
#include <QProcess>
#include <QUrl>

#include <iostream>
#include <string>

using namespace hula;

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
bool QMLBridge::record()
{
    bool success = transport->record();
    emit stateChanged();
    return success;
}

/**
 * Trigger stop in the Transport and update the UI state via signal.
 */
bool QMLBridge::stop()
{
    bool success = transport->stop();
    emit stateChanged();
    return success;
}

/**
 * Trigger playback in the Transport and update the UI state via signal.
 */
bool QMLBridge::play()
{
    bool success = transport->play();
    emit stateChanged();
    return success;
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
bool QMLBridge::pause()
{
    bool success = transport->pause();
    emit stateChanged();
    return success;
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
    //Should not get here should have found the device
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
    //Should not get here should have found the device
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
    // create a Qurl so it is easy to convert
    QUrl url(dir);
    std::string directory = url.path().toStdString();
    transport->exportFile(directory);
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
