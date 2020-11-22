#pragma once

#include <QObject>
#include <QtQml>

#include "Models/AppConfiguration.hpp"
#include "Utils.hpp"

namespace QtPiDeck {
class DataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Models::AppConfiguration* appConfiguration READ appConfiguration CONSTANT)
    QML_ELEMENT

public:
    Models::AppConfiguration* appConfiguration();

    Q_INVOKABLE void loadConfiguration();
    Q_INVOKABLE void saveConfiguration();

    static void registerTypes();
protected:
    explicit DataManager(QObject *parent = nullptr);
private:
    Models::AppConfiguration m_appConfiguration;
    inline static const QString ConfigurationFilename = "config.json";
};
}
