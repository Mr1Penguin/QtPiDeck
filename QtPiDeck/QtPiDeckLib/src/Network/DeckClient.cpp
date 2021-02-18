#include "Network/DeckClient.hpp"

#include <QHostAddress>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {
DeckClient::DeckClient(QObject* parent) : QObject(parent) {
    connect(&m_socket, &QTcpSocket::connected, this, &DeckClient::sendPing);
    connect(&m_socket, &QTcpSocket::readyRead, this, &DeckClient::readData);
}

void DeckClient::connectToServer() {
    m_socket.close();
    const auto &settingStorage = service<Services::IClientSettingsStorage>();
    m_socket.connectToHost(settingStorage->deckServerAddress(), settingStorage->deckServerPort().toInt());
}

void DeckClient::sendPing() {
    if (m_socket.state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Wrong state";
        return;
    }

    qDebug() << "Sending ping";

    const MessageHeader response{0, MessageId::Ping};
    QByteArray qba;
    DeckDataStream out{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
    out << response;
    m_socket.write(qba);
    m_socket.flush();
}

void DeckClient::readData() {
    DeckDataStream qds(&m_socket);
    MessageHeader header{};

    qds.startTransaction();
    qds >> header;

    if (!qds.commitTransaction()) {
        return;
    }

    qDebug() << "We got pong? -" << (header.messageId == MessageId::Pong);
}
}
