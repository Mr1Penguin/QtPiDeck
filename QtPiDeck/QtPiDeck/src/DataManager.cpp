#include "DataManager.hpp"

namespace QtPiDeck {
DataManager::DataManager(QObject *parent) : QObject(parent) {
}

Models::AppConfiguration* DataManager::appConfiguration() {
    return &m_appConfiguration;
}

void DataManager::loadConfiguration() {
    auto exePath = QCoreApplication::applicationDirPath();

    auto fallback = [this](QString reason) {
        m_appConfiguration.setHostAddress(QString{});
        m_appConfiguration.setHostPort(QString{});
        //global notification?
        qDebug().noquote() << reason;
        return true;
    };

    QDir outputDir(exePath + "/data");

    if (!outputDir.exists() && fallback("Invalid data directory")) {
        return;
    }

    QFile jsonFile;
    jsonFile.setFileName(outputDir.filePath(ConfigurationFilename));

    if ((!jsonFile.exists() || !jsonFile.isReadable()) && fallback("Invalid configuration file")) {
        return;
    }

    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << errno;
        return;
    }

    auto jsonString = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError parseResult;
    auto document = QJsonDocument::fromJson(jsonString, &parseResult);

    if (parseResult.error != QJsonParseError::NoError
            && fallback("Json parsing error: " + parseResult.errorString())) {
        return;
    }

    auto jsonObject = document.object();

    if (jsonObject.contains("hostAddress") && jsonObject["hostAddress"].isString()) {
        m_appConfiguration.setHostAddress(jsonObject["hostAddress"].toString());
    }

    if (jsonObject.contains("hostPort") && jsonObject["hostPort"].isString()) {
        m_appConfiguration.setHostPort(jsonObject["hostPort"].toString());
    }
}

void DataManager::saveConfiguration() {
    auto exePath = QCoreApplication::applicationDirPath();
    QDir outputDir(exePath + "/data");

    if (!outputDir.exists()) {
        if(!QDir{QCoreApplication::applicationDirPath()}.mkdir("data")) {
            qDebug() << "Failed to create directory:" << errno;
            return;
        }
    }

    QJsonObject jsonObject;
    jsonObject["hostAddress"] = m_appConfiguration.hostAddress();
    jsonObject["hostPort"] = m_appConfiguration.hostPort();

    QJsonDocument document;
    document.setObject(jsonObject);
    QFile jsonFile;
    jsonFile.setFileName(outputDir.filePath(ConfigurationFilename));
    if (!jsonFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        qDebug() << "Failed to open file:" << errno;
        return;
    }

    jsonFile.write(document.toJson());
    jsonFile.close();
}

void DataManager::registerTypes() {
    qRegisterMetaType<Models::AppConfiguration*>("Models::AppConfiguration*");
    qmlRegisterSingletonType<DataManager>("QtPiDeck", 1, 0, "DataManager", qmlSingletonProvider<DataManager>);
}
}
