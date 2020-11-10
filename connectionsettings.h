#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QObject>
#include <qqml.h>

class ConnectionSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostAddress READ hostAddress WRITE setHostAddress NOTIFY hostAddressChanged);
    Q_PROPERTY(QString hostPort READ hostPort WRITE setHostPort NOTIFY hostPortChanged);
    QML_ELEMENT

public:
    explicit ConnectionSettings(QObject *parent = nullptr);

    QString hostAddress();
    void setHostAddress(const QString & hostAddress);
    QString hostPort();
    void setHostPort(const QString & hostPort);

signals:
    void hostAddressChanged();
    void hostPortChanged();

private:
    QString m_hostAddress;
    QString m_hostPort;
};

#endif // CONNECTIONSETTINGS_H
