#include <QCursor>
#include <QGuiApplication>
#include <QMetaType>

#include "DataManager.hpp"
#include "Models/AppConfiguration.hpp"

namespace QtPiDeck {
void registerQmlTypes() {
    DataManager::registerTypes();
    Models::AppConfiguration::registerTypes();
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
