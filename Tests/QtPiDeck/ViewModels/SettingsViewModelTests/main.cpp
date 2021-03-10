#include <QtTest>
#include <QTcpServer>

#include "Application.hpp"
#include "Services/Ioc.hpp"
#include "ViewModels/SettingsViewModel.hpp"

namespace QtPiDeck::ViewModels::Tests {
class SettingsViewModelTests : public QObject {
  Q_OBJECT // NOLINT

private slots:
  void addressIsDefaultWithoutService();
  void portIsDefaultWithoutService();
  void saveSettingsDoNothingWithoutService();
  void addressIsInitedWithService();
  void portIsInitedWithService();
  void saveSettingsSetsStorageValuesWithService();
  void testEnabledWithNoServices();
  void testEnabledWithoutConnection();
  void testEnabledIfConnectedToDifferentServer();
  void testDisabledIfConnectedToSameServer();

private: // NOLINT(readability-redundant-access-specifiers)
  std::unique_ptr<SettingsViewModel> m_viewModel;
};

void SettingsViewModelTests::addressIsDefaultWithoutService() {
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
  QVERIFY(m_viewModel->deckServerAddress().isEmpty());
}

void SettingsViewModelTests::portIsDefaultWithoutService() {
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
  QVERIFY(m_viewModel->deckServerPort().isEmpty());
}

void SettingsViewModelTests::saveSettingsDoNothingWithoutService() {
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
  m_viewModel->saveSettings();
}

class EmptySettingsStorage : public Services::IClientSettingsStorage {
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

void SettingsViewModelTests::addressIsInitedWithService() {
  auto settingsStorage = std::make_shared<DummySettingsStorage>();
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr, settingsStorage, nullptr);
  QCOMPARE(m_viewModel->deckServerAddress(), DummySettingsStorage::address);
}

void SettingsViewModelTests::portIsInitedWithService() {
  auto settingsStorage = std::make_shared<DummySettingsStorage>();
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr, settingsStorage, nullptr);
  QCOMPARE(m_viewModel->deckServerPort(), DummySettingsStorage::port);
}

void SettingsViewModelTests::saveSettingsSetsStorageValuesWithService() {
  auto settingsStorage = std::make_shared<DummySettingsStorage>();
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr, settingsStorage, nullptr);
  const QString newAddress{"52.52.52.52"};
  const QString newPort{"5252"};
  m_viewModel->setDeckServerAddress(newAddress);
  m_viewModel->setDeckServerPort(newPort);
  QVERIFY(settingsStorage->deckServerAddress() != newAddress);
  QVERIFY(settingsStorage->deckServerPort() != newPort);
  m_viewModel->saveSettings();
  QCOMPARE(settingsStorage->newAddress(), newAddress);
  QCOMPARE(settingsStorage->newPort(), newPort);
}

void SettingsViewModelTests::testEnabledWithNoServices() {
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr);
  QVERIFY(m_viewModel->testEnabled());
}

void SettingsViewModelTests::testEnabledWithoutConnection() {
  m_viewModel = std::make_unique<SettingsViewModel>(nullptr, std::make_shared<DummySettingsStorage>(),
                                                    std::make_shared<Network::DeckClient>());
  QVERIFY(m_viewModel->testEnabled());
}

class DefaultSettingsStorage final : public EmptySettingsStorage {
  [[nodiscard]] auto deckServerAddress() const noexcept -> QString final { return QLatin1String{"localhost"}; }
  [[nodiscard]] auto deckServerPort() const noexcept -> QString final { return QLatin1String{"13000"}; }
};

void SettingsViewModelTests::testEnabledIfConnectedToDifferentServer() {
  QEventLoop loop;
  Services::Ioc ioc;
  ioc.registerService<Services::IClientSettingsStorage, DefaultSettingsStorage>();
  ioc.registerSingleton<Network::DeckClient>(
      ioc.make<Network::DeckClient, Services::CreationType::SharedPointer>());
  auto client = ioc.resolveService<Network::DeckClient>();
  m_viewModel = ioc.make<SettingsViewModel>();

  const QLatin1String newAddress{"localhost"};
  const QLatin1String newPort{"13001"};
  m_viewModel->setDeckServerAddress(newAddress);
  m_viewModel->setDeckServerPort(newPort);

  constexpr int port = 13000;

  QTcpServer server;
  server.listen(QHostAddress{QLatin1String{"localhost"}}, port);

  QVERIFY(server.isListening());

  QSignalSpy sspy{&server, &QTcpServer::newConnection};

  QTcpSocket sock;
  QSignalSpy spy{client.get(), &Network::DeckClient::connected};
  connect(client.get(), &Network::DeckClient::connected, &loop, [&loop]() { loop.exit(); });
  QTimer::singleShot(0, client.get(), &Network::DeckClient::connectToServer);
  loop.exec();

  QCOMPARE(sspy.count(), 1);
  QCOMPARE(spy.count(), 1);

  QVERIFY(m_viewModel->testEnabled());
}

void SettingsViewModelTests::testDisabledIfConnectedToSameServer() {
  QEventLoop loop;
  Services::Ioc ioc;
  ioc.registerService<Services::IClientSettingsStorage, DefaultSettingsStorage>();
  ioc.registerSingleton<Network::DeckClient>(
      ioc.make<Network::DeckClient, Services::CreationType::SharedPointer>());
  auto client = ioc.resolveService<Network::DeckClient>();
  m_viewModel = ioc.make<SettingsViewModel>();

  const QLatin1String newAddress{"localhost"};
  const QLatin1String newPort{"13000"};
  m_viewModel->setDeckServerAddress(newAddress);
  m_viewModel->setDeckServerPort(newPort);

  constexpr int port = 13000;

  QTcpServer server;
  server.listen(QHostAddress{QLatin1String{"localhost"}}, port);

  QVERIFY(server.isListening());

  QSignalSpy sspy{&server, &QTcpServer::newConnection};

  QTcpSocket sock;
  QSignalSpy spy{client.get(), &Network::DeckClient::connected};
  connect(client.get(), &Network::DeckClient::connected, &loop, [&loop]() { loop.exit(); });
  QTimer::singleShot(0, client.get(), &Network::DeckClient::connectToServer);
  loop.exec();

  QCOMPARE(sspy.count(), 1);
  QCOMPARE(spy.count(), 1);

  QVERIFY(!m_viewModel->testEnabled());
}
}

QTEST_GUILESS_MAIN(QtPiDeck::ViewModels::Tests::SettingsViewModelTests) // NOLINT

#include "main.moc"
