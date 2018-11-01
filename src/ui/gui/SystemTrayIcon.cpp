#include "SystemTrayIcon.h"

#include <QAction>
#include <QApplication>
#include <QIcon>

SystemTrayIcon::SystemTrayIcon(QObject *parent) : QObject(parent)
{
    trayIcon = new QSystemTrayIcon(QIcon(":/res/hulaloop-logo.svg"), this);
    trayIconMenu = new QMenu();
}

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

void SystemTrayIcon::showMessage(const QString &title, const QString &msg)
{
    trayIcon->showMessage(title, msg, QIcon(":/res/hulaloop-logo.png"), 10000);
}

void SystemTrayIcon::setToolTip(const QString &text)
{
    trayIcon->setToolTip(text);
}

void SystemTrayIcon::setDefaultIcon()
{
    trayIcon->setIcon(QIcon(":/res/hulaloop-logo.svg"));
}

void SystemTrayIcon::setRecordIcon()
{
    trayIcon->setIcon(QIcon(":/res/hulaloop-logo-recording.svg"));
}