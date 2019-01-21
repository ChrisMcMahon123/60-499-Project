#include <QApplication>
#include <VPApplication>

#include <QQmlApplicationEngine>

#include "backEnd.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //registered name must be capitalized ie backEnd doesn't work
    //QML: import WordMap.BackEnd 1.0
    qmlRegisterType<backEnd>("WordMap.BackEnd", 1, 2, "BackEnd");

    VPApplication vplay;

    // Use platform-specific fonts instead of V-Play's default font
    vplay.setPreservePlatformFonts(true);

    QQmlApplicationEngine engine;
    vplay.initialize(&engine);

    // use this during development
    // for PUBLISHING, use the entry point below
    vplay.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    // use this instead of the above call to avoid deployment of the qml files and compile them into the binary with qt's resource system qrc
    // this is the preferred deployment option for publishing games to the app stores, because then your qml files and js files are protected
    // to avoid deployment of your qml files and images, also comment the DEPLOYMENTFOLDERS command in the .pro file
    // also see the .pro file for more details
    // vplay.setMainQmlFileName(QStringLiteral("qrc:/qml/Main.qml"));

    engine.load(QUrl(vplay.mainQmlFileName()));

    return app.exec();
}
