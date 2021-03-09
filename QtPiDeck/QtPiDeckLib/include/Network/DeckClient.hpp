#pragma once

#include <QObject>
#include <QTcpSocket>

#include "Network/BaseDeckClient.hpp"
#include "Services/IClientSettingsStorage.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Network {
class DeckClient : public BaseDeckClient,
                   public Services::UseServices<Services::IClientSettingsStorage, Services::IMessageBus>,
                   public Services::ServiceInterface {
  Q_OBJECT // NOLINT
public:
  explicit DeckClient(QObject* parent = nullptr);

  void connectToServer();
};
}
