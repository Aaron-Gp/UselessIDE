#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "../component/filemanager.h"
#include "../AppInfo.h"

class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() {}

public slots:
    void applicationAvailable()
    {
        // Initialization that only requires the QGuiApplication object to be available
    }

    void qmlEngineAvailable(QQmlEngine *engine)
    {
        // Initialization requiring the QQmlEngine to be constructed
        QQmlContext* context = engine->rootContext();
        Lang* lang = AppInfo().lang();
        context->setContextProperty("lang",lang);
        qmlRegisterSingletonType<FileManager>("UselessIDE",1,0,"FileManager",FileManager::singleton_provider);
    }

    void cleanupTestCase()
    {
        // Implement custom resource cleanup
    }
};

QUICK_TEST_MAIN_WITH_SETUP(uitest, Setup)
#include "testui.moc"
