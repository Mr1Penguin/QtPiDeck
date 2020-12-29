#include "ViewModels/SettingsViewModel.hpp"

#include "Application.hpp"

namespace QtPiDeck::ViewModels {
SettingsViewModel::SettingsViewModel(QObject* parent) noexcept : QObject(parent) {
    Services::setServices(*this, Application::current()->ioc());

    if (const auto& settingsStorage = service<Services::IClientSettingsStorage>();
            settingsStorage != nullptr) {
        m_deckServerAddress = settingsStorage->deckServerAddress();
        m_deckServerPort = settingsStorage->deckServerPort();
    }
}

void SettingsViewModel::setDeckServerAddress(const QString& deckServerAddress) noexcept {
    if (deckServerAddress == m_deckServerAddress) {
        return;
    }

    m_deckServerAddress = deckServerAddress;
    emit deckServerAddressChanged();
}

void SettingsViewModel::setDeckServerPort(const QString& deckServerPort) noexcept {
    if (deckServerPort == m_deckServerPort) {
        return;
    }

    m_deckServerPort = deckServerPort;
    emit deckServerPortChanged();
}

void SettingsViewModel::registerType() {
    qmlRegisterType<SettingsViewModel>("QtPiDeck", 1, 0, "SettingsViewModel");
}
}
