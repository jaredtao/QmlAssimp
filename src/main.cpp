#include <QApplication>
#include <QSurfaceFormat>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QSplashScreen>
#include <QScreen>
#include "item/FBOItem.h"
#include "model/model.h"
#include "src/item/Camera.h"
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

    qmlRegisterType<FBOItem>("J3D", 1, 0, "FBOItem");
    qmlRegisterType<JCamera>("J3D", 1, 0, "JCamera");
    qmlRegisterType<JKeyCamera>("J3D", 1, 0, "JKeyCamera");
    qmlRegisterType<Model>("J3D", 1, 0, "Model");
    QQuickView view;
    view.setSource(QUrl(QLatin1Literal("qrc:/main.qml")));
    view.setResizeMode(QQuickView::SizeViewToRootObject);
    QObject::connect(view.engine(), &QQmlEngine::quit, &a, &QApplication::quit);
    view.show();
    return a.exec();
}
