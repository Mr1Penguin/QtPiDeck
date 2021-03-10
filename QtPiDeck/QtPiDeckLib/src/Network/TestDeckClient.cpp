#include "Network/TestDeckClient.hpp"

namespace QtPiDeck::Network {
TestDeckClient::TestDeckClient(QObject* parent) : BaseDeckClient(parent), m_pongReceived{false} {
  connect(this, &TestDeckClient::headerReceived, this, &TestDeckClient::handlerHeader);
}

void TestDeckClient::sendPing() {
  [[maybe_unused]] auto _ = send(MessageType::Ping, QStringLiteral("ConnectionTest"));
}

void TestDeckClient::handlerHeader(const MessageHeader& header) {
  if (header.messageType != MessageType::Pong) {
    qWarning() << "Unexpected message from server:" << static_cast<uint32_t>(header.messageType);
    return;
  }

  if (header.requestId != QStringLiteral("ConnectionTest")) {
    return;
  }

  m_pongReceived = true;
  emit pongReceived();
}
}
