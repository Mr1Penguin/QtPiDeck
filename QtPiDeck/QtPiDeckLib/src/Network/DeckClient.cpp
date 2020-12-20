#include "Network/DeckClient.hpp"

#include <QHostAddress>

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
    QDataStream out{&qba, QIODevice::WriteOnly};
    out.setByteOrder(QDataStream::BigEndian);
    out.setVersion(QDataStream::Qt_5_15);
    out << response;
    m_socket.write(qba);
    m_socket.flush();
}

void DeckClient::readData() {
    QDataStream qds(&m_socket);
    qds.setByteOrder(QDataStream::BigEndian);
    qds.setVersion(QDataStream::Qt_5_15);

    QtPiDeck::Network::MessageHeader header{};

    qds.startTransaction();
    qds >> header;

    if (!qds.commitTransaction()) {
        return;
    }

    qDebug() << "We got pong? -" << (header.messageId == QtPiDeck::Network::MessageId::Pong);
}
}
