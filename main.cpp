#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tabletmouseconverter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TabletMouseConverter tmc;
    engine.rootContext()->setContextProperty("tabletMouseConverter", &tmc);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *topLevel = engine.rootObjects().value(0);
    tmc.setWindow(topLevel);
    app.installEventFilter(&tmc); // Comment this out to observe no MouseArea functionality

    return app.exec();
}
