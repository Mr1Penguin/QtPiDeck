#pragma once

#include "Application.hpp"

namespace QtPiDeck::Client {
class ClientApplication : public Application {
public:
    ClientApplication();

protected:
    auto mainPage() -> QUrl final;
    void appStartupPreparations() final;
    void setupEngine(QQmlApplicationEngine & engine) final;
};
}
