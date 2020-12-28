#include "ClientApplication.hpp"

#include <QGuiApplication>
#include <QCursor>
#include <QIcon>

#include "Services/SettingsStorage.hpp"

static void initStaticResouces() {
    Q_INIT_RESOURCE(qml); // NOLINT
    Q_INIT_RESOURCE(icons); // NOLINT
}

namespace QtPiDeck {
ClientApplication::ClientApplication() = default;

auto ClientApplication::mainPage() -> QUrl {
    return QUrl{QStringLiteral("qrc:/qml/pages/main.qml")};
}

namespace {
void registerQmlTypes() {
    /*DataManager::registerTypes();
    Models::AppConfiguration::registerTypes();*/
}

void setCursorVisibility() {
    auto hideCursorVar = qgetenv("HIDE_CURSOR");
    auto hideCursor = [](){
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

    if (hideCursorVar.toInt() != 0) {
        hideCursor();
    }
}
}


void ClientApplication::initialPreparations() {
    Application::initialPreparations();

    /*
     * @todo Disable request for virtual keyboard for Qt6.0.0
     * @body Qt6.0.0 does not have virtual keyboard. Need to check for Qt6.1/6.2. Module can be moved to Qt Marketplace.
     * */
    // read doc about modules
    // usa same logic as for cursor visibility
    // need to add optional load of InputPanel
    // everything may change if custom keys are required
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION_MAJOR < 6
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
}

void ClientApplication::appCreated() {

    QIcon::setThemeName("maintheme");

    setCursorVisibility();
    registerQmlTypes();

    initStaticResouces();

    ioc().registerService<Services::ISettingsStorage, Services::SettingsStorage>();
}

void ClientApplication::engineCreated(QQmlApplicationEngine & engine) {
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &m_deckClient, &Network::DeckClient::connectToServer); // clazy:exclude=connect-non-signal
}
}
