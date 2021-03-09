#include "Network/BaseDeckClient.hpp"

#include "Network/DeckDataStream.hpp"

namespace QtPiDeck::Network {
BaseDeckClient::BaseDeckClient(QObject* parent) : QObject(parent) {
  connect(&m_socket, &QTcpSocket::connected, this, &BaseDeckClient::connected);
  connect(&m_socket, &QTcpSocket::readyRead, this, &BaseDeckClient::readHeader);
}

void BaseDeckClient::connectToServer(const QString& address, int port) noexcept {
  m_socket.close();
  m_socket.connectToHost(address, port);
}

auto BaseDeckClient::send(MessageType messageType, const QString& requestId) noexcept -> std::optional<SendError> {
  if (!isConnected()) {
    return SendError::NotConnected;
  }

  const MessageHeader response = getEmptyMessageHeader(messageType, requestId);
  QByteArray qba;
  DeckDataStream out{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  out << response;
  m_socket.write(qba);
  //m_socket.flush();
  return std::nullopt;
}

void BaseDeckClient::readHeader() {
  DeckDataStream qds(&m_socket);
  MessageHeader header{};

  qds.startTransaction();
  qds >> header;

  if (!qds.commitTransaction()) {
    return;
  }

  emit headerReceived(header);

  qDebug() << "We got pong? -" << (header.messageType == MessageType::Pong) << header.requestId;
}
}
