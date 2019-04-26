#include "QMLBridge.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QProcess>
#include <QQuickStyle>
#include <QUrl>
#include <QtWidgets>
#include <random>
#include <string>

#include "FFTRealPair.h"

using namespace hula;

/**
 * Construct a new instance of the QMLBridge class.
 *
 * @param parent Object which QMLBridge should be child of.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    try
    {
        transport = new Transport();
    }
    catch (const ControlException &ce)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
            exit(1);
        }
    }

    try
    {
        rb = transport->getController()->createBuffer(0.5);
    }
    catch(const AudioException &ae)
    {
        ControlException ce(ae.getErrorCode());

        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
            exit(1);
        }
    }
    loadSettings();
}

/**
 * Loads all settings stored in the JSON settings file.
 */
void QMLBridge::loadSettings()
{

    QFile file(QDir::homePath() + "/.hulaloop.json");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QString json = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isNull() || !doc.isObject())
        return;

    QJsonObject rootObj = doc.object();
    if (rootObj.isEmpty())
        return;

    QVariantMap map = rootObj.toVariantMap();

    bool val = map["show_record_devices"].toBool();
    QString defVisualizer = map["visualizer"].toString();
    QString defLanguage = map["language"].toString();

    setShowRecordDevices(val);
    visType = defVisualizer;
    loadLanguage(defLanguage);

    file.close();

}

/**
 * Saves all settings into a JSON settings file.
 */
void QMLBridge::saveSettings()
{

    QFile file(QDir::homePath() + "/.hulaloop.json");
    if(!file.open(QIODevice::WriteOnly))
        return;

    QVariantMap map;
    map.insert("show_record_devices", HulaAudioSettings::getInstance()->getShowRecordDevices());
    map.insert("visualizer", (visType.isEmpty()) ? "Line" : visType);
    map.insert("language", (language.isEmpty()) ? "en" : language);

    QJsonObject obj = QJsonObject::fromVariantMap(map);

    QJsonDocument doc;
    doc.setObject(obj);

    hlDebug() << "doc = " << QString(doc.toJson()).toStdString() << "\n";

    file.write(doc.toJson());
    file.flush();
    file.close();

}

/**
 * Return the current visualizer type.
 *
 * @return QString - The name of the visualizer
 */
QString QMLBridge::getVisualizerType()
{
    return visType;
}

/**
 * Sets the current visualizer type.
 *
 * @param QString - The name of the visualizer
 */
void QMLBridge::setVisualizerType(const QString &type)
{
    visType = type;
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
    bool success = false;
    try
    {
        success = transport->record();
    }
    catch (const ControlException &ce)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
        }

    }
    emit stateChanged();

    if (success)
    {
        startVisThread();
    }

    return success;

}

/**
 * Trigger stop in the Transport and update the UI state via signal.
 */
bool QMLBridge::stop()
{
    bool success = false;
    try
    {
        success = transport->stop();
    }
    catch (const ControlException &ce)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
        }

    }
    emit stateChanged();

    stopVisThread();

    return success;
}

/**
 * Trigger playback in the Transport and update the UI state via signal.
 */
bool QMLBridge::play()
{
    bool success = false;
    try
    {
        success = transport->play();
    }
    catch (const ControlException &ce)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
        }

    }
    emit stateChanged();

    if (success)
    {
        startVisThread();
    }

    return success;

}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
bool QMLBridge::pause()
{
    bool success = false;
    try
    {
        success = transport->pause();
    }
    catch (const ControlException &ce)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
        }

    }
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

    emit stateChanged();
    emit discarded();
}

/**
 * Return an empty QString to force QML to update when a new language is loaded.
 *
 * @return QString - An empty QString
 */
QString QMLBridge::getEmptyStr()
{
    return "";
}

/**
 * Match a string that the user chose to the input device list
 * and notify the backend
 */
bool QMLBridge::setActiveInputDevice(QString QDeviceName)
{
    std::string deviceName = QDeviceName.toStdString();
    DeviceType type = DeviceType::LOOPBACK;
    if (HulaSettings::getInstance()->getShowRecordDevices())
        type = (DeviceType)(type | DeviceType::RECORD);

    Device *device = transport->getController()->findDeviceByName(deviceName, type);

    if (device != nullptr)
    {
        bool success = false;
        try
        {
            success = transport->getController()->setActiveInputDevice(device);
        }
        catch(const AudioException &ae)
        {
            ControlException ce(ae.getErrorCode());

            QMessageBox msgBox;
            msgBox.setWindowTitle("HulaLoop Error");
            msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
            msgBox.setStandardButtons(QMessageBox::Ok);

            if (msgBox.exec() == QMessageBox::Ok)
            {
                // Don't do anything since this isn't fatal
            }
        }

        return success;
    }

    // Should not get here should have found the device
    hlDebug() << "Input device not found: " << deviceName << std::endl;

    return false;
}

/**
 * Match a string that the user chose to the output device list
 * and notify the backend
 */
bool QMLBridge::setActiveOutputDevice(QString QDeviceName)
{
    std::string deviceName = QDeviceName.toStdString();
    Device *device = transport->getController()->findDeviceByName(deviceName, DeviceType::PLAYBACK);

    if (device != nullptr)
    {
        bool success = false;
        try
        {
            success = transport->getController()->setActiveOutputDevice(device);
        }
        catch(const AudioException &ae)
        {
            ControlException ce(ae.getErrorCode());

            QMessageBox msgBox;
            msgBox.setWindowTitle("HulaLoop Error");
            msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
            msgBox.setStandardButtons(QMessageBox::Ok);

            if (msgBox.exec() == QMessageBox::Ok)
            {
                // Don't do anything since this isn't fatal
            }
        }

        return success;
    }

    // Should not get here should have found the device
    hlDebug() << "Output device not found: " << deviceName << std::endl;

    return false;
}

/**
 * Get the current input devices
 *
 * @return QString containing current input devices
 */
QString QMLBridge::getInputDevices()
{
    std::string devices;
    std::vector<Device *> vd;
    try
    {
        vd = transport->getController()->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
    }
    catch(const AudioException &ae)
    {
        ControlException ce(ae.getErrorCode());

        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
        }
    }

    for (int i = 0; i < vd.size(); i++)
    {
        devices += vd[i]->getName();
        if (i < vd.size() - 1)
        {
            // Concat with the separator that ButtonPanel.qml expects
            devices += "%%%%%%";
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
    std::vector<Device *> vd;
    try
    {
        vd = transport->getController()->getDevices(DeviceType::PLAYBACK);
    }
    catch(const AudioException &ae)
    {
        ControlException ce(ae.getErrorCode());

        QMessageBox msgBox;
        msgBox.setWindowTitle("HulaLoop Error");
        msgBox.setText(QString::fromStdString(ce.getErrorMessage()));
        msgBox.setStandardButtons(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok)
        {
        }
    }

    for (int i = 0; i < vd.size(); i++)
    {
        devices += vd[i]->getName();
        if (i < vd.size() - 1)
        {
            // Concat with the separator that ButtonPanel.qml expects
            devices += "%%%%%%";
        }
    }

    Device::deleteDevices(vd);
    return QString::fromStdString(devices);
}

/**
 * Modifies settings to display record devices.
 *
 * @param bool - True to show record devices, false otherwise
 */
void QMLBridge::setShowRecordDevices(bool val)
{
    HulaSettings::getInstance()->setShowRecordDevices(val);
    showRecDevices = val;
    saveSettings();
}

/**
 * Returns the setting value for showing record devices.
 *
 * @return bool - True to show record devices, false otherwise
 */
bool QMLBridge::getShowRecordDevices()
{
    return showRecDevices;
}

/**
 * Modifies settings to load the specified language.
 *
 * @return bool - True if loading the language was successful, false otherwise
 */
bool QMLBridge::loadLanguage(const QString &id)
{
    if (HulaSettings::getInstance()->loadLanguage(QCoreApplication::instance(), id.toStdString()))
    {
        language = id;
        saveSettings();

        emit languageChanged();
        emit stateChanged();
        return true;
    }
    return false;
}

/**
 * Returns the setting value for showing record devices.
 *
 * @return bool - True to show record devices, false otherwise
 */
QString QMLBridge::getSelectedLanguage()
{
    return language;
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
    HulaSettings *s = HulaSettings::getInstance();
    _this->transport->getController()->addBuffer(_this->rb);

    int maxSize = 512;
    int accuracy = 8;
    ring_buffer_size_t samplesProcessed = 0;
    // float *temp = new float[maxSize];

    while (!_this->endVis.load())
    {
        std::vector<double> actualoutreal;
        std::vector<double> actualoutimag;
        std::vector<double> realData;

        float *data1;
        float *data2;
        ring_buffer_size_t size1;
        ring_buffer_size_t size2;
        ring_buffer_size_t samplesRead = 0;

        while (!_this->endVis.load() && actualoutreal.size() < maxSize)
        {
            // Keep draining the buffer, but only actually
            // process the drained data every nth cycle
            samplesRead = _this->rb->directRead(maxSize, (void **)&data1, &size1, (void **)&data2, &size2);
            // hlDebug() << "Process " << samplesRead << std::endl;

            for (int i = 0; i < size1 && actualoutreal.size() < maxSize; i++)
            {
                actualoutimag.push_back(data1[i]);
                actualoutreal.push_back(data1[i]);
                realData.push_back(data1[i]);
            }

            for (int i = 0; i < size2 && actualoutreal.size() < maxSize; i++)
            {
                actualoutimag.push_back(data2[i]);
                actualoutreal.push_back(data2[i]);
                realData.push_back(data2[i]);
            }

            samplesProcessed += samplesRead;
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
                heights.push_back(sum * 0.3);
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

        _this->emit visData(realData, heights, samplesProcessed);
        samplesProcessed = 0;

        // Accumulate some audio
        // We have to make sure this delay is shorter than the length of the ring buffer
        // We approximate it to accuracy * the length (seconds) of our buffer period
        std::this_thread::sleep_for(std::chrono::milliseconds((maxSize / NUM_CHANNELS * 1000 / s->getSampleRate()) * accuracy));

        // Completely drain the rest of the buffer
        samplesRead = 1;
        while (samplesRead != 0)
        {
            samplesRead = _this->rb->directRead(maxSize * 2, (void **)&data1, &size1, (void **)&data2, &size2);
            // hlDebug() << "Read " << bytesRead << std::endl;
            samplesProcessed += samplesRead;
        }

        // Accumulate more audio
        // We have to make sure this delay is shorter than the length of the ring buffer
        // We approximate it to accuracy * the length (seconds) of our buffer period
        std::this_thread::sleep_for(std::chrono::milliseconds((maxSize / NUM_CHANNELS * 1000 / s->getSampleRate())));
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
        msgBox.setWindowTitle(tr("Unsaved Changes"));
        QPixmap pix(":/res/hulaloop-logo-small.png");
        msgBox.setIconPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio));
        msgBox.setText(tr("You have unsaved audio."));
        msgBox.setInformativeText(tr("Would you like to save your audio?"));
        QAbstractButton *goBackAndSave = msgBox.addButton(tr("Go back and save"), QMessageBox::RejectRole);
        QIcon ico = QApplication::style()->standardIcon(QStyle::SP_DialogYesButton);
        goBackAndSave->setIcon(ico);
        msgBox.addButton(QMessageBox::Discard);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
            // the user did not want to exit just go back
            return false;
        }
        else
        {
            // the user wanted to exit, exit and delete files
            transport->discard();
            return true;
        }
    }
    // user has no unsaved audio, just exit
    else
    {
        return true;
    }
}

/**
 * Deletes all the temp files that the program has created
 */
void QMLBridge::cleanTempFiles()
{
    transport->discard();
}

/**
 * Destructor for QMLBridge
 */
QMLBridge::~QMLBridge()
{
    hlDebugf("QMLBridge destructor called\n");

    stopVisThread();
    saveSettings();
    delete transport;
    delete rb;
}
