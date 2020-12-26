#include "Network/DeckClient.hpp"

#include <QHostAddress>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Client::Network {
DeckClient::DeckClient(QObject* parent) : QObject(parent) {
    connect(&m_socket, &QTcpSocket::connected, this, &DeckClient::sendPing);
    connect(&m_socket, &QTcpSocket::readyRead, this, &DeckClient::readData);
}

void DeckClient::connectToServer() {
    constexpr qint16 defaultPort = 13000;
    m_socket.close();
    m_socket.connectToHost(QHostAddress::LocalHost, defaultPort);
}

void DeckClient::sendPing() {
    if (m_socket.state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Wrong state";
        return;
    }

    qDebug() << "Sending ping";

    QtPiDeck::Network::MessageHeader response{0, QtPiDeck::Network::MessageId::Ping};
    QByteArray qba;
    QtPiDeck::Network::DeckDataStream out{&qba, QtPiDeck::Network::DeckDataStream::OpenModeFlag::WriteOnly};
    out << response;
    m_socket.write(qba);
    m_socket.flush();
}

void DeckClient::readData() {
    QtPiDeck::Network::DeckDataStream qds(&m_socket);

    QtPiDeck::Network::MessageHeader header{};

    qds.startTransaction();
    qds >> header;

    if (!qds.commitTransaction()) {
        return;
    }

    qDebug() << "We got pong? -" << (header.messageId == QtPiDeck::Network::MessageId::Pong);
}
}
