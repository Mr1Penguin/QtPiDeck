#pragma once

#include "Application.hpp"
#include "Network/DeckClient.hpp"

namespace QtPiDeck::Client {
class ClientApplication : public Application {
public:
    ClientApplication();

protected:
    auto mainPage() -> QUrl final;
    void appStartupPreparations() final;
    void setupEngine(QQmlApplicationEngine & engine) final;

private:
    Network::DeckClient m_deckClient;
};
}
