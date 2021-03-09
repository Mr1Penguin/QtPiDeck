#pragma once

#include <QDebug>
#include <QObject>

#include "Network/DeckClient.hpp"
#include "Services/IClientSettingsStorage.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::ViewModels {
class SettingsViewModel : public QObject,
                          public Services::UseServices<Services::IClientSettingsStorage, Network::DeckClient> {
  Q_OBJECT // NOLINT
  Q_PROPERTY(
      QString deckServerAddress READ deckServerAddress WRITE setDeckServerAddress NOTIFY deckServerAddressChanged)
  Q_PROPERTY(QString deckServerPort READ deckServerPort WRITE setDeckServerPort NOTIFY deckServerPortChanged)
  Q_PROPERTY(bool testEnabled READ testEnabled NOTIFY testEnabledChanged)
  Q_PROPERTY(TestingStatus testingStatus READ testingStatus NOTIFY testingStatusChanged)
public:
  enum class TestingStatus {
    None,
    InProgress,
    Timeout,
    Ok
  };

  Q_ENUM(TestingStatus);

  explicit SettingsViewModel(QObject* parent = nullptr) noexcept;
  SettingsViewModel(QObject* parent, const std::shared_ptr<Services::IClientSettingsStorage>& storageService,
                    const std::shared_ptr<Network::DeckClient>& deckClient) noexcept;

  [[nodiscard]] auto deckServerAddress() const noexcept -> QString { return m_deckServerAddress; }
  void setDeckServerAddress(const QString& deckServerAddress) noexcept;
  [[nodiscard]] auto deckServerPort() const noexcept -> QString { return m_deckServerPort; }
  void setDeckServerPort(const QString& deckServerPort) noexcept;
  [[nodiscard]] auto testEnabled() const noexcept -> bool { return m_testEnabled; }
  [[nodiscard]] auto testingStatus() const noexcept -> TestingStatus { return m_testingStatus; }

  Q_INVOKABLE void saveSettings() noexcept;
  Q_INVOKABLE void testConnection() noexcept;

  static void registerType();

signals:
  void deckServerAddressChanged();
  void deckServerPortChanged();
  void testEnabledChanged();
  void testingStatusChanged();

private:
  QString m_deckServerAddress;
  QString m_deckServerPort;
  bool m_testEnabled;
  TestingStatus m_testingStatus;

  void setTestEnabled(bool testEnabled) noexcept;
  void updateTestEnabled() noexcept;
  void setTestingStatus(TestingStatus testingStatus) noexcept;
};
}
