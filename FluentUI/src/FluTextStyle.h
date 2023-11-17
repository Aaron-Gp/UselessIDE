#ifndef FLUTEXTSTYLE_H
#define FLUTEXTSTYLE_H

#include <QObject>
#include <QtQml/qqml.h>
#include <QFont>
#include "stdafx.h"

class FluTextStyle : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY_AUTO(QFont,Caption);
    Q_PROPERTY_AUTO(QFont,Body);
    Q_PROPERTY_AUTO(QFont,BodyStrong);
    Q_PROPERTY_AUTO(QFont,Subtitle);
    Q_PROPERTY_AUTO(QFont,Title);
    Q_PROPERTY_AUTO(QFont,TitleLarge);
    Q_PROPERTY_AUTO(QFont,Display);
    QML_NAMED_ELEMENT(FluTextStyle)
    QML_SINGLETON
private:
    explicit FluTextStyle(QObject *parent = nullptr);
    static FluTextStyle* m_instance;
public:
    static FluTextStyle *getInstance();
    static FluTextStyle *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        return getInstance();
    }
};

#endif // FLUTEXTSTYLE_H
