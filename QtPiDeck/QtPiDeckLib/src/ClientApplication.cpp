#include "ClientApplication.hpp"

#include <QCursor>
#include <QGuiApplication>
#include <QIcon>
#include <QQuickWindow>

#include "Services/MessageBus.hpp"
#include "Services/SettingsStorage.hpp"
#include "ViewModels/SettingsViewModel.hpp"

#include <QRegularExpressionValidator>

static void initStaticResouces() {
  Q_INIT_RESOURCE(qml); // NOLINT
}

namespace QtPiDeck {
ClientApplication::ClientApplication() = default;

auto ClientApplication::mainPage() -> QUrl { return QUrl{QStringLiteral("qrc:/qml/pages/main.qml")}; }

namespace {
void registerQmlTypes() {
  /*DataManager::registerTypes();
  Models::AppConfiguration::registerTypes();*/
}

constexpr int OptionEnabled = 1;

void setCursorVisibility() {
  auto hideCursorVar = qgetenv("HIDE_CURSOR");
  auto hideCursor = []() {
    QCursor cursor(Qt::BlankCursor);
    QGuiApplication::setOverrideCursor(cursor);
    QGuiApplication::changeOverrideCursor(cursor);
  };
  if (hideCursorVar.isEmpty()) {
#ifdef Q_PROCESSOR_ARM
    hideCursor();
#endif
    return;
  }

  if (hideCursorVar.toInt() == OptionEnabled) {
    hideCursor();
  }
}

void requestKeyboard() {
  const auto disableKeyboard = qgetenv("DISABLE_KEYBOARD");
  if (disableKeyboard.isEmpty() || disableKeyboard.toInt() != OptionEnabled) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
  }
}
}

void ClientApplication::initialPreparations() {
  Application::initialPreparations();
  requestKeyboard();

#if QT_VERSION_MAJOR < 6
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
}

void ClientApplication::appCreated() {

  QIcon::setThemeName("maintheme");

  setCursorVisibility();
  registerQmlTypes();

  initStaticResouces();

  ioc().registerSingleton<Services::IMessageBus>(std::make_shared<Services::MessageBus>(nullptr));
  ioc().registerService<Services::IClientSettingsStorage, Services::SettingsStorage>();
}

void ClientApplication::engineCreated(QQmlApplicationEngine& engine) {
  engine.addImportPath(QStringLiteral("qrc:/qml/components"));
  m_deckClient = ioc().make<Network::DeckClient>();
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, m_deckClient.get(), // clazy:exclude=connect-non-signal
                   &Network::DeckClient::connectToServer);

  ViewModels::SettingsViewModel::registerType();
}
}
