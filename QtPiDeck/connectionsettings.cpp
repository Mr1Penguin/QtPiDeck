#include "connectionsettings.h"

ConnectionSettings::ConnectionSettings(QObject *parent) : QObject(parent)
{
}

QString ConnectionSettings::hostAddress() {
    return m_hostAddress;
}

void ConnectionSettings::setHostAddress(const QString & hostAddress) {
    if (m_hostAddress == hostAddress) {
        return;
    }

    m_hostAddress = hostAddress;
    emit hostAddressChanged();
}

QString ConnectionSettings::hostPort() {
    return m_hostPort;
}

void ConnectionSettings::setHostPort(const QString & hostPort) {
    if (m_hostPort == hostPort){
        return;
    }

    m_hostPort = hostPort;
    emit hostPortChanged();
}
