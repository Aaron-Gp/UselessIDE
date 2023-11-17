#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QQuickTextDocument>

class SyntaxHighlighter:public QSyntaxHighlighter
{
    Q_OBJECT
//    Q_PROPERTY(bool isDark READ isDark WRITE setIsDark CONSTANT)
public:
    explicit SyntaxHighlighter(QObject* parent=nullptr, bool isDark=true); //重写实现高亮
    //    Q_INVOKABLE void setDocument(QQuickTextDocument* document);
//    bool isDark = false;
    Q_INVOKABLE void setDocument(QTextDocument* document);

protected:
    void highlightBlock(const QString& text);

private:
    struct HighlightRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> highlightRules;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
};

#endif // SYNTAXHIGHLIGHTER_H
