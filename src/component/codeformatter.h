#ifndef CODEFORMATTER_H
#define CODEFORMATTER_H

#include <cstdlib>

#include <QMap>
#include <QObject>
#define PATH QString("./bin/")

class CodeFormatter
{
public:
    static QString format(QString code,QMap<QString,QString> config=QMap<QString,QString>());
};

//config
//      TabWidth 制表符宽度
//      BaseOnStyle
//              LLVM 符合 LLVM 编码标准的样式
//              Google 符合 Google 的 C ++ 的风格指南样式
//              Chromium 符合 Chromium 的风格指南样式
//              Mozilla 符合 Mozilla 的风格指南样式
//              WebKit 符合 WebKit 的风格指南样式
//更多的可以看https://www.jianshu.com/p/5dea6bdbbabb和https://clang.llvm.org/docs/ClangFormatStyleOptions.html

#endif // CODEFORMATTER_H
