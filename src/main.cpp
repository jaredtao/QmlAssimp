#include <QApplication>
#include <QSurfaceFormat>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QSplashScreen>
#include "item/item.h"
int main(int argc, char *argv[])
{
    qSetMessagePattern("log[%{file} %{function} %{line}] %{message}");
	
    QApplication a(argc, argv);
	qDebug() << a.applicationDirPath();
//![1] set OpenGL Format
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
#ifdef USE_GL_DEBUGGER
    fmt.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(fmt);
//![1]

//! [2] create a Splash
    QPixmap pixmap(":/splash.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading...", Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
    qApp->processEvents();
//![2]

//![3] create QuickView
    qmlRegisterType<Item>("Item", 1, 0, "Item");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QLatin1Literal("qrc:/main.qml")));
    QObject::connect(view.engine(), SIGNAL(quit()), &a, SLOT(quit()));
//    view.show();
//! [3]

//! [4] show splash and view
    auto widget = QWidget::createWindowContainer(&view);
    splash.finish(widget);
    widget->resize(1024, 768);
    widget->show();
//![4]

    return a.exec();
}
