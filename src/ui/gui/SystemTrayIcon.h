#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QMenu>
#include <QSystemTrayIcon>

/**
 * Class for communicating between QML and C++.
 * This is designed to be added as a QML type and used in QML.
 */
class SystemTrayIcon : public QObject {
        Q_OBJECT

    private:
        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;

    public:
        explicit SystemTrayIcon(QObject *parent = nullptr);

        Q_INVOKABLE void init();
        Q_INVOKABLE void showMessage(const QString &, const QString &);
        Q_INVOKABLE void setToolTip(const QString &);
        Q_INVOKABLE void setDefaultIcon();
        Q_INVOKABLE void setRecordIcon();
};

#endif // SYSTEMTRAYICON_H