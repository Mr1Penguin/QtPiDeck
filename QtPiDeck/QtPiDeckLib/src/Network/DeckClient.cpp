#include "Network/DeckClient.hpp"

namespace QtPiDeck::Network {
DeckClient::DeckClient(QObject* parent) : BaseDeckClient(parent) {
}

void DeckClient::connectToServer() {
  const auto& settingStorage = service<Services::IClientSettingsStorage>();
  BaseDeckClient::connectToServer(settingStorage->deckServerAddress(), settingStorage->deckServerPort().toInt());
}
}
