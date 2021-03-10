#include "Network/DeckClient.hpp"

namespace QtPiDeck::Network {
DeckClient::DeckClient(QObject* parent) noexcept : BaseDeckClient(parent) {
}

void DeckClient::connectToServer() noexcept {
  const auto& settingStorage = service<Services::IClientSettingsStorage>();
  auto a =  settingStorage->deckServerPort();
  BaseDeckClient::connectToServer(settingStorage->deckServerAddress(), settingStorage->deckServerPort().toInt());
}
}
