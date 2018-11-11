#include "QMLBridge.h"

#include <QCoreApplication>
#include <QProcess>
#include <QUrl>
#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlDebuggingEnabler>
#include <QQuickStyle>
#include <QtDebug>
#include <QMessageBox>
#include <QtWidgets>
#include <QDir>
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
}

/**
 * Match a string that the user chose to the input device list
 * and notify the backend
 */
void QMLBridge::setActiveInputDevice(QString QDeviceName)
{
    std::cout << "setActiveDevice() called" << std::endl;
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
    std::cerr << "Input device not found: " << deviceName << std::endl;
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
    std::cerr << "Output device not found: " << deviceName << std::endl;
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
 * Get if the user has unsaved files
 *
 * @return true if the user has unsaved files
 */
bool QMLBridge::wannaClose()
{
    // Check if user has unsaved audio
    if (transport->hasExportPaths())
    {
        // user has unsaved audio prompt them
        QMessageBox msgBox;
        msgBox.setWindowTitle("Exit???");
        QPixmap pix(":/res/hulaloop-logo-small.png");
        msgBox.setIconPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio));
        msgBox.setText("You have unsaved audio.");
        msgBox.setInformativeText("Would you like to save your audio?");
        QAbstractButton *goBackAndSave = msgBox.addButton(tr("Go back and save"), QMessageBox::RejectRole);
        QIcon ico = QApplication::style()->standardIcon(QStyle::SP_DialogYesButton);
        goBackAndSave->setIcon(ico);
        msgBox.addButton(QMessageBox::Discard);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
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
 * Deletes all the temp files that the program has created
 */
void QMLBridge::cleanTempFiles()
{
    transport->deleteTempFiles();
}
