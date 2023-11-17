#include "codeformatter.h"

#include <QFile>
#include <QProcess>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

QString CodeFormatter::format(QString code, QMap<QString, QString> config) {
    QString basePath =  QDir::currentPath();
    qDebug()<<"base:"<<basePath;
    QFile inputFile(PATH + "input.txt");
    QFileInfo info(inputFile);
    qDebug()<<info.absoluteFilePath();
    if (!inputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return code;
    QTextStream out(&inputFile);
    out << code;
    inputFile.close();

    QFile configFile(PATH + ".clang-format");
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return code;
    QTextStream configStream(&configFile);
    for(auto it=config.begin();it!=config.end();it++)
        configStream<<it.key()<<": "<<it.value()<<"\n";
    configFile.close();

    QProcess process;
    process.setWorkingDirectory(PATH);

    // 设置cmd的标准输入、输出和错误流重定向
    process.setProcessChannelMode(QProcess::MergedChannels);

    QStringList arguments;
    arguments << "/c" << "clang-format.exe -style=Google input.txt > output.txt";
    qDebug()<<arguments;

    // 启动cmd并等待它完成
    process.start("cmd", arguments);
    process.waitForFinished();

    QFile outputFile(PATH + "output.txt");
    if (!outputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return code;
    QTextStream in(&outputFile);
    QString result = in.readAll();
    outputFile.close();

//    qDebug()<< result;

    return result;
}

