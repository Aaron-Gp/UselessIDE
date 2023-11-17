#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include "AppInfo.h"
#include <FramelessHelper/Quick/framelessquickmodule.h>
#include <FramelessHelper/Core/private/framelessconfig_p.h>
#include "component/CircularReveal.h"
#include "component/filemanager.h"
#include <QFontDatabase>

FRAMELESSHELPER_USE_NAMESPACE

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE","Basic");
    FramelessHelper::Quick::initialize();
    QGuiApplication::setApplicationName("UselessIDE");
    QGuiApplication app(argc, argv);
    FramelessConfig::instance()->set(Global::Option::DisableLazyInitializationForMicaMaterial);
    FramelessConfig::instance()->set(Global::Option::CenterWindowBeforeShow);
    FramelessConfig::instance()->set(Global::Option::ForceNonNativeBackgroundBlur);
    FramelessConfig::instance()->set(Global::Option::EnableBlurBehindWindow);
#ifdef Q_OS_WIN // 此设置仅在Windows下生效
    FramelessConfig::instance()->set(Global::Option::ForceHideWindowFrameBorder);
    FramelessConfig::instance()->set(Global::Option::EnableBlurBehindWindow,false);
#endif
    AppInfo* appInfo = new AppInfo();

    QQmlApplicationEngine engine;
    FramelessHelper::Quick::registerTypes(&engine);
    qmlRegisterType<CircularReveal>("UselessIDE", 1, 0, "CircularReveal");
    qmlRegisterSingletonType<FileManager>("UselessIDE",1,0,"FileManager",FileManager::singleton_provider);
    appInfo->init(&engine);
    FileManager::initFileSystem(&engine);

    const QUrl url(u"qrc:/UselessIDE/ui/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
