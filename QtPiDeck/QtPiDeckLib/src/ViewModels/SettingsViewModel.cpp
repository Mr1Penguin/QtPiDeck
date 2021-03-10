#include "ViewModels/SettingsViewModel.hpp"

#include <chrono>

#include <QTimer>

#include "Application.hpp"
#include "Network/TestDeckClient.hpp"

using namespace std::chrono_literals;

namespace QtPiDeck::ViewModels {
SettingsViewModel::SettingsViewModel(QObject* parent) noexcept : SettingsViewModel(parent, nullptr, nullptr) {}

SettingsViewModel::SettingsViewModel(QObject* parent,
                                     const std::shared_ptr<Services::IClientSettingsStorage>& storageService,
                                     const std::shared_ptr<Network::DeckClient>& deckClient) noexcept
    : QObject(parent), m_testEnabled{true}, m_testingStatus{TestingStatus::None} {
  setService(storageService);
  setService(deckClient);

  const auto& settingsStorage = service<Services::IClientSettingsStorage>();
  if (settingsStorage) {
    m_deckServerAddress = settingsStorage->deckServerAddress();
    m_deckServerPort = settingsStorage->deckServerPort();
  } else {
    qDebug() << "No IClientSettingsStorage provided";
  }

  connect(this, &SettingsViewModel::deckServerAddressChanged, this, &SettingsViewModel::updateTestEnabled);
  connect(this, &SettingsViewModel::deckServerPortChanged, this, &SettingsViewModel::updateTestEnabled);

  if (deckClient) {
    connect(deckClient.get(), &Network::DeckClient::connected, this, &SettingsViewModel::updateTestEnabled);
    connect(deckClient.get(), &Network::DeckClient::disconnected, this, &SettingsViewModel::updateTestEnabled);
  }

  updateTestEnabled();
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

void SettingsViewModel::saveSettings() noexcept {
  auto& settingsStorage = service<Services::IClientSettingsStorage>();
  if (!settingsStorage) {
    qDebug() << "No IClientSettingsStorage provided";
    return;
  }

  settingsStorage->setDeckServerAddress(m_deckServerAddress);
  settingsStorage->setDeckServerPort(m_deckServerPort);
}

void SettingsViewModel::testConnection() noexcept {
  if (const auto& [settings, client] =
          std::tuple(service<Services::IClientSettingsStorage>(), service<Network::DeckClient>());
      m_deckServerAddress == settings->deckServerAddress() && m_deckServerPort == settings->deckServerPort() &&
      client->isConnected()) {
    return;
  }

  auto testClient = std::make_shared<Network::TestDeckClient>(nullptr);

  connect(testClient.get(), &Network::TestDeckClient::connected, testClient.get(), &Network::TestDeckClient::sendPing);
  connect(testClient.get(), &Network::TestDeckClient::pongReceived, this,
          [this]() { setTestingStatus(TestingStatus::Ok); });
  testClient->connectToServer(m_deckServerAddress, m_deckServerPort.toInt());

  setTestingStatus(TestingStatus::InProgress);

  QTimer::singleShot(2s, this, [testClient, this]() {
    if (testClient && testClient->isPongReceived()) {
      return;
    }

    setTestingStatus(TestingStatus::Timeout);
  });
}

void SettingsViewModel::setTestEnabled(bool testEnabled) noexcept {
  if (m_testEnabled == testEnabled) {
    return;
  }

  m_testEnabled = testEnabled;
  emit testEnabledChanged();
}

void SettingsViewModel::updateTestEnabled() noexcept {
  const auto& settings = service<Services::IClientSettingsStorage>();
  const auto& client = service<Network::DeckClient>();

  const auto value = !settings || !client || m_deckServerAddress != settings->deckServerAddress() ||
                     m_deckServerPort != settings->deckServerPort() || !client->isConnected();

  setTestEnabled(value);
}

void SettingsViewModel::setTestingStatus(TestingStatus testingStatus) noexcept {
  if (m_testingStatus == testingStatus) {
    return;
  }

  m_testingStatus = testingStatus;

  emit testingStatusChanged();
}

void SettingsViewModel::registerType() { qmlRegisterType<SettingsViewModel>("QtPiDeck", 1, 0, "SettingsViewModel"); }
}

// not so private anymore, huh
// overload object creation in qml for a specific type
namespace QQmlPrivate {
using namespace QtPiDeck;
template<>
class QQmlElement<ViewModels::SettingsViewModel> final : public ViewModels::SettingsViewModel // NOLINT
{
public:
  using ViewModels::SettingsViewModel::SettingsViewModel;
  ~QQmlElement() override { QQmlPrivate::qdeclarativeelement_destructor(this); }
  static void operator delete(void* ptr) { // NOLINT
    ::operator delete(ptr);
  }
  static void operator delete(void*, void*) { // NOLINT
  }
};

template<>
#if QT_VERSION_MAJOR == 6
void createInto<ViewModels::SettingsViewModel>(void* memory, void*) { // NOLINT
#else
void createInto<ViewModels::SettingsViewModel>(void* memory) {
#endif
  const auto& ioc = Application::current()->ioc();
  using viewModelType = QQmlElement<ViewModels::SettingsViewModel>;
  [[maybe_unused]] auto* _ = ioc.make<viewModelType, Services::CreationType::RawInMemory>(memory);
}
}
