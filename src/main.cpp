#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include "item/item.h"
int main(int argc, char *argv[])
{
    qSetMessagePattern("log[%{file} %{function} %{line}] %{message}");
    QGuiApplication a(argc, argv);

    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
#ifdef USE_GL_DEBUGGER
    fmt.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(fmt);

    qmlRegisterType<Item>("Item", 1, 0, "Item");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QLatin1Literal("qrc:/main.qml")));
    QObject::connect(view.engine(), SIGNAL(quit()), &a, SLOT(quit()));
    view.show();

    return a.exec();
}
