#include "BoostUnitTest.hpp"

#include <QCoreApplication>
#include <QEventLoop>
#include <QTcpServer>
#include <QTimer>

#include <array>

#include "Application.hpp"
#include "Services/Ioc.hpp"
#include "SignalCatcher.hpp"
#include "ViewModels/SettingsViewModel.hpp"

using namespace QtPiDeck::Tests;

class EmptySettingsStorage : public QtPiDeck::Services::IClientSettingsStorage {
  [[nodiscard]] auto read(const QString& /*key*/, const QVariant& /*defaultValue*/ = QVariant{}) const noexcept
      -> QVariant override {
    return {};
  }
  void store(const QString& /*key*/, const QVariant& /*value*/) noexcept override {}
  [[nodiscard]] auto deckServerAddress() const noexcept -> QString override { return {}; }
  void setDeckServerAddress(const QString& /*deckServerAddress*/) noexcept override {}
  [[nodiscard]] auto deckServerPort() const noexcept -> QString override { return {}; }
  void setDeckServerPort(const QString& /*deckServerPort*/) noexcept override {}
};

CT_BOOST_AUTO_TEST_SUITE(SettingsViewModelTests)

using namespace QtPiDeck::ViewModels;

struct FixtureEmpty {
  std::unique_ptr<SettingsViewModel> viewModel{std::make_unique<SettingsViewModel>(nullptr)};
};

CT_BOOST_FIXTURE_TEST_CASE(addressIsDefaultWithoutService, FixtureEmpty) {
  CT_BOOST_TEST(viewModel->deckServerAddress().isEmpty());
}

CT_BOOST_FIXTURE_TEST_CASE(portIsDefaultWithoutService, FixtureEmpty) {
  CT_BOOST_TEST(viewModel->deckServerPort().isEmpty());
}

CT_BOOST_FIXTURE_TEST_CASE(saveSettingsDoNothingWithoutService, FixtureEmpty) {
  viewModel->saveSettings();
  CT_BOOST_TEST(true);
}

CT_BOOST_FIXTURE_TEST_CASE(testEnabledWithNoServices, FixtureEmpty) { CT_BOOST_TEST(viewModel->testEnabled()); }

struct FixtureWithSettings {
  class DummySettingsStorage final : public EmptySettingsStorage {
  public:
    [[nodiscard]] auto deckServerAddress() const noexcept -> QString final { return address; }
    void setDeckServerAddress(const QString& deckServerAddress) noexcept final { m_newAddress = deckServerAddress; }
    [[nodiscard]] auto deckServerPort() const noexcept -> QString final { return port; }
    void setDeckServerPort(const QString& deckServerPort) noexcept final { m_newPort = deckServerPort; }

    [[nodiscard]] auto newAddress() const noexcept -> QString { return m_newAddress; }
    [[nodiscard]] auto newPort() const noexcept -> QString { return m_newPort; }

    inline static const QString address = "123.123.123.123";
    inline static const QString port = "13999";

  private:
    QString m_newAddress{};
    QString m_newPort{};
  };

  FixtureWithSettings()
      : m_settingsStorage(std::make_shared<DummySettingsStorage>()),
        m_viewModel(std::make_unique<SettingsViewModel>(nullptr, m_settingsStorage, nullptr)){};

  auto settingsStorage() noexcept -> std::shared_ptr<DummySettingsStorage>& { return m_settingsStorage; }
  auto viewModel() noexcept -> std::unique_ptr<QtPiDeck::ViewModels::SettingsViewModel>& { return m_viewModel; }

private:
  std::shared_ptr<DummySettingsStorage> m_settingsStorage;
  std::unique_ptr<QtPiDeck::ViewModels::SettingsViewModel> m_viewModel;
};

CT_BOOST_FIXTURE_TEST_CASE(addressIsInitedWithService, FixtureWithSettings) {
  CT_BOOST_TEST(viewModel()->deckServerAddress() == DummySettingsStorage::address);
}

CT_BOOST_FIXTURE_TEST_CASE(portIsInitedWithService, FixtureWithSettings) {
  CT_BOOST_TEST(viewModel()->deckServerPort() == DummySettingsStorage::port);
}

CT_BOOST_FIXTURE_TEST_CASE(saveSettingsSetsStorageValuesWithService, FixtureWithSettings) {
  const QString newAddress{"52.52.52.52"};
  const QString newPort{"5252"};
  viewModel()->setDeckServerAddress(newAddress);
  viewModel()->setDeckServerPort(newPort);
  CT_BOOST_TEST(settingsStorage()->deckServerAddress() != newAddress);
  CT_BOOST_TEST(settingsStorage()->deckServerPort() != newPort);
  viewModel()->saveSettings();
  CT_BOOST_TEST(settingsStorage()->newAddress() == newAddress);
  CT_BOOST_TEST(settingsStorage()->newPort() == newPort);
}

struct FixtureWithSettingsAndClient {
  using DummySettingsStorage = FixtureWithSettings::DummySettingsStorage;

  FixtureWithSettingsAndClient()
      : m_settingsStorage(std::make_shared<DummySettingsStorage>()),
        m_viewModel(std::make_unique<SettingsViewModel>(nullptr, m_settingsStorage,
                                                        std::make_shared<QtPiDeck::Network::DeckClient>())){};

  auto settingsStorage() noexcept -> std::shared_ptr<DummySettingsStorage>& { return m_settingsStorage; }
  auto viewModel() noexcept -> std::unique_ptr<QtPiDeck::ViewModels::SettingsViewModel>& { return m_viewModel; }

private:
  std::shared_ptr<DummySettingsStorage> m_settingsStorage;
  std::unique_ptr<QtPiDeck::ViewModels::SettingsViewModel> m_viewModel;
};

CT_BOOST_FIXTURE_TEST_CASE(testEnabledWithoutConnection, FixtureWithSettingsAndClient) {
  CT_BOOST_TEST(viewModel()->testEnabled());
}

struct FixtureWithDefaultSettingsAndClient {
  class DefaultSettingsStorage final : public EmptySettingsStorage {
    [[nodiscard]] auto deckServerAddress() const noexcept -> QString final { return QLatin1String{"localhost"}; }
    [[nodiscard]] auto deckServerPort() const noexcept -> QString final { return QLatin1String{"13000"}; }
  };

  FixtureWithDefaultSettingsAndClient() : m_execName{"QtPiDeckCodeTests.exe"} {
    m_argv = {m_execName.data()};
    m_app = std::make_unique<QCoreApplication>(m_argc, m_argv.data());
    m_loop = std::make_unique<QEventLoop>();
    m_ioc.registerService<QtPiDeck::Services::IClientSettingsStorage, DefaultSettingsStorage>();
    m_ioc.registerSingleton<QtPiDeck::Network::DeckClient>(
        m_ioc.make<QtPiDeck::Network::DeckClient, QtPiDeck::Services::CreationType::SharedPointer>());
    m_client = m_ioc.resolveService<QtPiDeck::Network::DeckClient>();
    m_viewModel = m_ioc.make<SettingsViewModel>();
  }

  auto viewModel() noexcept -> std::unique_ptr<QtPiDeck::ViewModels::SettingsViewModel>& { return m_viewModel; }
  auto client() noexcept -> std::shared_ptr<QtPiDeck::Network::DeckClient>& { return m_client; }
  auto loop() noexcept -> QEventLoop& { return *m_loop; }

private:
  std::shared_ptr<QtPiDeck::Network::DeckClient> m_client;
  std::unique_ptr<QtPiDeck::ViewModels::SettingsViewModel> m_viewModel;
  std::unique_ptr<QCoreApplication> m_app;
  std::unique_ptr<QEventLoop> m_loop;

  std::string m_execName;
  int m_argc{1};
  std::array<char*, 1> m_argv;

  QtPiDeck::Services::Ioc m_ioc;
};

CT_BOOST_FIXTURE_TEST_CASE(testEnabledIfConnectedToDifferentServer, FixtureWithDefaultSettingsAndClient) {
  const QLatin1String newAddress{"localhost"};
  const QLatin1String newPort{"13001"};
  viewModel()->setDeckServerAddress(newAddress);
  viewModel()->setDeckServerPort(newPort);

  constexpr int port = 13000;

  QTcpServer server;
  server.listen(QHostAddress{QLatin1String{"localhost"}}, port);

  CT_BOOST_TEST(server.isListening());

  SignalCatcher serverCatcher{&server, &QTcpServer::newConnection};

  QTcpSocket sock;
  SignalCatcher clientCatcher{static_cast<QtPiDeck::Network::BaseDeckClient*>(client().get()),
                              &QtPiDeck::Network::DeckClient::connected};
  QObject::connect(client().get(), &QtPiDeck::Network::DeckClient::connected, &loop(),
                   [&loop = loop()]() { loop.exit(); });
  QTimer::singleShot(0, client().get(), &QtPiDeck::Network::DeckClient::connectToServer);
  loop().exec();

  CT_BOOST_TEST(serverCatcher.count() == 1);
  CT_BOOST_TEST(clientCatcher.count() == 1);

  CT_BOOST_TEST(viewModel()->testEnabled());
}

CT_BOOST_FIXTURE_TEST_CASE(testDisabledIfConnectedToSameServer, FixtureWithDefaultSettingsAndClient) {
  const QLatin1String newAddress{"localhost"};
  const QLatin1String newPort{"13000"};
  viewModel()->setDeckServerAddress(newAddress);
  viewModel()->setDeckServerPort(newPort);

  constexpr int port = 13000;

  QTcpServer server;
  server.listen(QHostAddress{QLatin1String{"localhost"}}, port);

  CT_BOOST_TEST(server.isListening());

  SignalCatcher serverCatcher{&server, &QTcpServer::newConnection};

  QTcpSocket sock;
  SignalCatcher clientCatcher{static_cast<QtPiDeck::Network::BaseDeckClient*>(client().get()),
                              &QtPiDeck::Network::DeckClient::connected};
  QObject::connect(client().get(), &QtPiDeck::Network::DeckClient::connected, &loop(),
                   [&loop = loop()]() { loop.exit(); });
  QTimer::singleShot(0, client().get(), &QtPiDeck::Network::DeckClient::connectToServer);
  loop().exec();

  CT_BOOST_TEST(serverCatcher.count() == 1);
  CT_BOOST_TEST(clientCatcher.count() == 1);

  CT_BOOST_TEST(!viewModel()->testEnabled());
}

CT_BOOST_AUTO_TEST_SUITE_END()