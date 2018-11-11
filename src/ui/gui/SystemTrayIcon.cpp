#include "SystemTrayIcon.h"

#include <QAction>
#include <QApplication>
#include <QIcon>

using namespace hula;

/**
 * Construct a new instance of the SystemTrayIcon class.
 *
 * @param parent The parent object of SystemTrayIcon
 */
SystemTrayIcon::SystemTrayIcon(QObject *parent) : QObject(parent)
{
    trayIcon = new QSystemTrayIcon(QIcon(":/res/hulaloop-logo.svg"), this);
    trayIconMenu = new QMenu();
}

/**
 * Initializes the system tray icon with a custom menu.
 */
void SystemTrayIcon::init()
{

    QAction *record = new QAction(tr("Record"), this);
    QAction *stop = new QAction(tr("Stop"), this);

    QAction *minimizeToTray = new QAction(tr("Minimize to Tray"), this);

    QAction *checkUpdates = new QAction(tr("Check for Updates"), this);
    QAction *settings = new QAction(tr("Settings"), this);

    QAction *quit = new QAction(tr("Quit HulaLoop"), this);
    this->connect(quit, &QAction::triggered, this, &QApplication::quit);

    trayIconMenu->addAction(record);
    trayIconMenu->addAction(stop);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeToTray);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(checkUpdates);
    trayIconMenu->addAction(settings);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quit);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("HulaLoop");
    trayIcon->show();

}

/**
 * Displays a balloon message with the provided title and message to the user.
 *
 * @param title - Balloon message title
 * @param msg - Content of balloon message
 */
void SystemTrayIcon::showMessage(const QString &title, const QString &msg)
{
    trayIcon->showMessage(title, msg, QIcon(":/res/hulaloop-logo.png"), 10000);
}

/**
 * Changes the system tray icon tooltip text.
 *
 * @param text - The system tray icon's tooltip text
 */
void SystemTrayIcon::setToolTip(const QString &text)
{
    trayIcon->setToolTip(text);
}

/**
 * Changes the system tray icon to the default image.
 */
void SystemTrayIcon::setDefaultIcon()
{
    trayIcon->setIcon(QIcon(":/res/hulaloop-logo.svg"));
}

/**
 * Changes the system tray icon to the recording image.
 */
void SystemTrayIcon::setRecordIcon()
{
    trayIcon->setIcon(QIcon(":/res/hulaloop-logo-recording.svg"));
}