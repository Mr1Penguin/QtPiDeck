#pragma once

#include <QObject>
#include <QTcpSocket>

#include "Network/BaseDeckClient.hpp"
#include "Services/IClientSettingsStorage.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Network {
class TestDeckClient : public BaseDeckClient {
  Q_OBJECT // NOLINT
public:
  explicit TestDeckClient(QObject* parent = nullptr);

  void sendPing();
  [[nodiscard]] auto isPongReceived() const noexcept -> bool { return m_pongReceived; }

signals:
  void pongReceived();

private:
  void handlerHeader(const MessageHeader&);

  bool m_pongReceived;
};
}
