#ifndef COMPILER_H
#define COMPILER_H

#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QDebug>

struct CompileInfo
{
    QString row;//行
    QString column;//列
    QString fileName;//错误的文件
    QString information;//错误的信息
};

class Compiler : public QObject
{
    Q_OBJECT

public:
    Compiler();
    QStringList gccPathList;
    QString gccPathCurrent;

    bool compile(QFileInfo fileInfo, QList<CompileInfo> &resultList, QString &result);//编译功能
    bool run(QFileInfo fileInfo, QList<CompileInfo> &resultList, QString &result);//运行功能
    bool compileAndRun(QFileInfo fileInfo, QList<CompileInfo> &resultList, QString &result);//编译后运行功能
    bool isCompiled(QString filePath);//根据包含完整文件名的文件路径判断是否已经编译(.exe文件是否存在)
    bool resultToResultList(QString result, QList<CompileInfo> &resultList, int &numError, int &numWarning);//将编译结果修改为列表形式,并记录warning和error的数量
    void resultProcessing(QFileInfo fileInfo, QString &result, int numError, int numWarning, double compilationTime, QString command);//加工结果

    bool findGccPath();//通过环境变量获取gcc的路径
    bool addGccPath(QString gccPath);//添加自己选择的gcc的路径(结尾应该是\bin)
    bool changeGccPath(QString gccPath);//切换到gcc路径下
    bool deleteGccPath(QString gccPath);//删除已有的gcc路径
};

class QDetachableProcess : public QProcess
{
public:
    QDetachableProcess(QObject *parent = 0)
        : QProcess(parent) {
    }
    void detach() {
        waitForStarted();
        setProcessState(QProcess::NotRunning);
    }
};




#endif // COMPILER_H
