#ifndef STORE_H
#define STORE_H

#include <QObject>
//#include "component/highlighter.h"

class Store : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(SyntaxHighlighter* highlighter READ highlighter CONSTANT)
public:
    static Store* instance();

//    SyntaxHighlighter* highlighter(QTextDocument* document = 0) const;
//    void setHighlighter(MySyntaxHighlighter* highlighter);

signals:

private:
    explicit Store(QObject *parent = nullptr);

//    SyntaxHighlighter *m_highlighter;
};

#endif // STORE_H
