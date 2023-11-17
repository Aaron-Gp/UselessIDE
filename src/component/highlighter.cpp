#include "highlighter.h"
#include <QFont>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMutex>
#include <QMutexLocker>

SyntaxHighlighter::SyntaxHighlighter(QObject* parent, bool isDark):QSyntaxHighlighter(parent)
{
    HighlightRule rule;
    //普通文本
    QTextCharFormat normalTextFormat;
    normalTextFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("[a-zA-Z]+");
    rule.format = normalTextFormat;
    highlightRules.append(rule);
    //数字
    QTextCharFormat numberFormat;
    numberFormat.setFontWeight(QFont::Bold);
    if(isDark) {
        numberFormat.setForeground(QColor(255, 0, 0));
    } else {
        numberFormat.setForeground(QColor(191,162,121));//棕色
    }
    rule.format = numberFormat;
    rule.pattern = QRegularExpression("\\b((0x[A-Fa-f0-9]*)|(\\d+(\\.\\d+)?)|(\\d+))\\b");
    highlightRules.append(rule);

    //    //类名
    //    QTextCharFormat classNameFormat;
    //    classNameFormat.setFontWeight(QFont::Bold);
    //    classNameFormat.setForeground(QColor(33,111,133));
    //    rule.format = classNameFormat;
    //    rule.pattern = QRegularExpression("\\b[A-Z]+\\w*");
    //    highlightRules.append(rule);
    //字符串
    QTextCharFormat stringFormat;
    stringFormat.setFontWeight(QFont::Bold);
    if(isDark) {
        stringFormat.setForeground(QColor(100,192,152));
    } else {
        stringFormat.setForeground(QColor(170,192,152));
    }
    rule.format = stringFormat;
    rule.pattern = QRegularExpression("(['\"`])(.*?)\\1");
    highlightRules.append(rule);
    //函数
    QTextCharFormat functionFormat;
    functionFormat.setFontWeight(QFont::Bold);
    if(isDark) {
        functionFormat.setForeground(QColor(189,154,180));
    } else {
        functionFormat.setForeground(QColor(119,154,180));
    }
    rule.format = functionFormat;
    rule.pattern = QRegularExpression("\\w+\\(");
    highlightRules.append(rule);
    rule.pattern = QRegularExpression("\\)");
    highlightRules.append(rule);
    //关键字
    QTextCharFormat keyWordFormat;
    keyWordFormat.setFontWeight(QFont::Bold);
    if(isDark) {
        keyWordFormat.setForeground(QColor(248,172,140));
    } else {
        keyWordFormat.setForeground(QColor(167,130,181));
    }
    QStringList keywordPatterns;
    keywordPatterns << "\\bif\\b" << "\\bint\\b" << "\\bfor\\b"  << "\\bdo\\b"
                    << "\\bnew\\b" << "\\btry\\b" << "\\basm\\b" << "\\belse\\b"
                    << "\\bchar\\b" << "\\bfloat\\b" << "\\blong\\b" << "\\bvoid\\b"
                    << "\\bshort\\b" << "\\bwhile\\b" << "\\bdouble\\b" << "\\bbreak\\b"
                    << "\\btypedef\\b" << "\\bregister\\b" << "\\bcontinue\\b" << "\\bcatch\\b"
                    << "\\bsigned\\b" << "\\bunsigned\\b" << "\\bauto\\b" << "\\bstatic\\b"
                    << "\\bextern\\b" << "\\bsizeof\\b" << "\\bdelete\\b" << "\\bthrow\\b"
                    << "\\bconst\\b" << "\\bclass\\b" << "\\bfriend\\b" << "\\breturn\\b"
                    << "\\bswitch\\b" << "\\bpublic\\b" << "\\bunion\\b" << "\\bgoto\\b"
                    << "\\boperator\\b" << "\\btemplate\\b" << "\\benum\\b" << "\\bprivate\\b"
                    << "\\bvolatile\\b" << "\\bthis\\b" << "\\bvirtual\\b" << "\\bcase\\b"
                    << "\\bdefault\\b" << "\\binline\\b" << "\\bprotected\\b" << "\\bstruct\\b"
                    << "\\bexplicit\\b" << "\\bnamespace\\b" << "\\busing\\b" << "\\btypename\\b"
                    << "\\bbool\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keyWordFormat;
        highlightRules.append(rule);
    }

    //预处理指令
    QTextCharFormat preprocessingCommandFormat;
    preprocessingCommandFormat.setFontWeight(QFont::Bold);
    preprocessingCommandFormat.setForeground(Qt::darkMagenta);
    QStringList preprocessingPatterns;
    preprocessingPatterns << "\\B\\#define\\b" << "\\B\\#include\\b" << "\\B\\#if\\b" <<
        "\\B\\#elif\\b" << "\\B\\#ifndef\\b" << "\\B\\#ifdef\\b" <<
        "\\B\\#endif\\b" << "\\B\\#undef\\b" << "\\B\\#error\\b" <<
        "\\B\\#line\\b" << "\\B\\#pragma\\b" ;

    foreach (const QString &pattern, preprocessingPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = preprocessingCommandFormat;
        highlightRules.append(rule);
    }

    //单行注释
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setFontWeight(QFont::Normal);
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    singleLineCommentFormat.setFontItalic(true);
    rule.format = singleLineCommentFormat;
    rule.pattern = QRegularExpression("//[^\n]*"); //单行注释
    highlightRules.append(rule);
}

//void SyntaxHighlighter::setDocument(QQuickTextDocument* document)
//{
//    QSyntaxHighlighter::setDocument(document->textDocument());
//}

void SyntaxHighlighter::setDocument(QTextDocument *document)
{
    QSyntaxHighlighter::setDocument(document);
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{

    foreach (const HighlightRule &rule, highlightRules) {
        QRegularExpression express(rule.pattern);
        QRegularExpressionMatchIterator matchIterator = express.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            int matchStart = match.capturedStart();
            int matchLength = match.capturedLength();
            setFormat(matchStart, matchLength, rule.format);
        }
    }

    QTextCharFormat multieLineCommentFormat;
    multieLineCommentFormat.setFontWeight(QFont::Normal);
    multieLineCommentFormat.setForeground(Qt::darkGreen);
    multieLineCommentFormat.setFontItalic(true);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
    setCurrentBlockState(0); // 重置块状态

    int startIndex = 0;
    if (previousBlockState() == 1) {
        startIndex = 0; // 如果之前的块已经是多行注释，从0开始匹配
    } else {
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1); // 设置块状态为多行注释开始
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multieLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

