#include "ClientApplication.hpp"

#include <QGuiApplication>
#include <QCursor>
#include <QIcon>

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
}

void ClientApplication::setupEngine(QQmlApplicationEngine & /*engine*/) {
}
}
