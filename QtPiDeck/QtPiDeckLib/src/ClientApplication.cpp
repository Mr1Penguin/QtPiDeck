#include "ClientApplication.hpp"

#include <QGuiApplication>
#include <QCursor>
#include <QIcon>

static void initStaticResouces() {
    Q_INIT_RESOURCE(qml); // NOLINT
    Q_INIT_RESOURCE(icons); // NOLINT
}

namespace QtPiDeck::Client {
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

void ClientApplication::appStartupPreparations() {
    Application::appStartupPreparations();

    // read doc about modules
    // usa same logic as for cursor visibility
    // need to add optional load of InputPanel
    // everything may change if custom keys are required
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QIcon::setThemeName("maintheme");

    setCursorVisibility();
    registerQmlTypes();

    initStaticResouces();
}

void ClientApplication::setupEngine(QQmlApplicationEngine & engine) {
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &m_deckClient, &Network::DeckClient::connectToServer);
}
}
