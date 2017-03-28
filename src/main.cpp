#include <QApplication>
#include <QSurfaceFormat>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QSplashScreen>
#include <QScreen>
#include "item/item.h"
int main(int argc, char *argv[])
{
    qSetMessagePattern("log[%{file} %{function} %{line}] %{message}");

    QApplication a(argc, argv);
    //![1] set OpenGL Format
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
#ifdef USE_GL_DEBUGGER
    fmt.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(fmt);
    //![1]

    qmlRegisterType<Item>("Item", 1, 0, "Item");
    QQmlApplicationEngine engine(QUrl(QLatin1Literal("qrc:/main.qml")));

    return a.exec();
}
