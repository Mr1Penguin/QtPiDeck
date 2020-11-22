#include "Models/appConfiguration.hpp"

namespace QtPiDeck::Models {
AppConfiguration::AppConfiguration(QObject *parent) : QObject(parent)
{
}

QString AppConfiguration::hostAddress() {
    return m_hostAddress;
}

void AppConfiguration::setHostAddress(const QString & hostAddress) {
    if (m_hostAddress == hostAddress) {
        return;
    }

    m_hostAddress = hostAddress;
    emit hostAddressChanged();
}

QString AppConfiguration::hostPort() {
    return m_hostPort;
}

void AppConfiguration::setHostPort(const QString & hostPort) {
    if (m_hostPort == hostPort){
        return;
    }

    m_hostPort = hostPort;
    emit hostPortChanged();
}
}
