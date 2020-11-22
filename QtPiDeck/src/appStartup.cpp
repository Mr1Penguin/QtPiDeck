#include <QCursor>
#include <QGuiApplication>

#include "Models/appConfiguration.hpp"

namespace QtPiDeck {
void registerQmlTypes() {
    qmlRegisterType<Models::AppConfiguration>("data.appconfiguration", 1, 0, "AppConfiguration");
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
