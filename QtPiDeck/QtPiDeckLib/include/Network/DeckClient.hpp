#pragma once

#include <QObject>
#include <QTcpSocket>

#include "Services/IClientSettingsStorage.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Network {
class DeckClient : public QObject,
                   public Services::UseServices<Services::IClientSettingsStorage, Services::IMessageBus> {
  Q_OBJECT // NOLINT
public:
  explicit DeckClient(QObject* parent = nullptr);

  void connectToServer();

private:
  void sendPing();
  void readData();

  QTcpSocket m_socket{};
};
}
