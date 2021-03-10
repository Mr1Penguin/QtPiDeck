#pragma once

#include "Application.hpp"
#include "Network/DeckClient.hpp"

namespace QtPiDeck {
class ClientApplication : public Application {
public:
  ClientApplication();

protected:
  auto mainPage() -> QUrl final;
  void initialPreparations() final;
  void appCreated() final;
  void engineCreated(QQmlApplicationEngine& engine) final;

private:
  std::shared_ptr<Network::DeckClient> m_deckClient;
};
}
