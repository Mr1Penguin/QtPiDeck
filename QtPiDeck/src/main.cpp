#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

#include "appStartup.hpp"

void setCursorVisibility();

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QIcon::setThemeName("maintheme");

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);

    QtPiDeck::setCursorVisibility();
    QtPiDeck::registerQmlTypes();

    const QUrl url(QStringLiteral("qrc:/qml/pages/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
