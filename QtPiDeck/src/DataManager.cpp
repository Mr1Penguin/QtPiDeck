#include "DataManager.hpp"

namespace QtPiDeck {
DataManager::DataManager(QObject *parent) : QObject(parent) {
}

Models::AppConfiguration* DataManager::appConfiguration() {
    return &m_appConfiguration;
}

void DataManager::registerTypes() {
    qRegisterMetaType<Models::AppConfiguration*>("Models::AppConfiguration*");
    qmlRegisterSingletonType<DataManager>("QtPiDeck", 1, 0, "DataManager", qmlSingletonProvider<DataManager>);
}
}
