#include "compiler.h"

#include <windows.h>
#include <QProcess>
#include <QDebug>
#include <QElapsedTimer>
#include <QDir>
#include <QCoreApplication>


Compiler::Compiler()
{
    if(findGccPath()){//初始化gcc路径，如果找不到就设为空值
        gccPathCurrent = gccPathList[0];
    }else{
        gccPathCurrent = "";
    }
}

bool Compiler::compile(QFileInfo fileInfo, QList<CompileInfo> &resultList, QString &result){
    if(gccPathList.isEmpty()){
        result = "Cannot find the gcc.\n";
        return false;
    }
    QString originalPath = QCoreApplication::applicationFilePath();//记录原始路径
    if(!QDir::setCurrent(gccPathCurrent)){//尝试切换到gcc的路径
        result = "Cannot find the gcc.\n";
        return false;
    }
    QString baseName = fileInfo.baseName();//不带后缀的文件名
    QString suffix = fileInfo.suffix();//后缀
    QString absolutePath = fileInfo.absolutePath();//不带文件名的绝对路径(即包含文件的文件夹路径)
    int numError=0, numWarning=0;//error和warning的数量
    QProcess process;//创建进程
    QString command;//使用的指令
    QElapsedTimer timer;
    timer.start();//计时开始
    if (suffix == "c") {
        QString program = "gcc.exe";
        QStringList arguments;
        arguments << "-g3" << "-pipe" << "-o" << absolutePath + "/" + baseName + ".exe" << absolutePath + "/" + baseName + '.' + suffix;
        command = program + " \"" + absolutePath + "/" + baseName + '.' + suffix + "\" -o \"" + absolutePath + "/" + baseName + ".exe\"" + " -g3 -pipe";
        process.start(program, arguments);
        //gcc.exe    -o absolutePath/baseName.exe absolutePath/baseName.suffix
    } else if (suffix == "cpp") {
        QString program = "g++.exe";
        QStringList arguments;
        arguments << "-g3" << "-pipe" << "-o" << absolutePath + "/" + baseName + ".exe" << absolutePath + "/" + baseName + '.' + suffix;
        command = program + " \"" + absolutePath + "/" + baseName + '.' + suffix + "\" -o \"" + absolutePath + "/" + baseName + ".exe\"" + " -g3 -pipe";
        process.start(program, arguments);
    } else {
        result = "Compile unsuccessfully.\n"
                 "The file's suffix must be .c or .cpp.";
        QDir::setCurrent(originalPath);//切换回原来的路径
        return false;//文件不是c或者c++文件，不能编译
    }
    process.waitForFinished();
    double compileTime = (double)timer.nsecsElapsed()/(double)1000000;//计时结束
    QDir::setCurrent(originalPath);//切换回原来的路径
    QByteArray resultByte = process.readAllStandardError();//获得编译的错误反馈
    result = "";//result初始化
    result = QString::fromLocal8Bit(resultByte);
    Compiler::resultToResultList(result, resultList, numError, numWarning);
    Compiler::resultProcessing(fileInfo, result, numError, numWarning, compileTime, command);
    if(numError == 0){
        return true;//编译成功，返回true
    }
    return false;//编译出错，返回false
}

bool Compiler::run(QFileInfo fileInfo, QList<CompileInfo> &resultList, QString &result){
    QString baseName = fileInfo.baseName();//不带后缀的文件名
    QString absolutePath = fileInfo.absolutePath();//不带文件名的绝对路径(即包含文件的文件夹路径)

//    //判断是否编译过了
//    if(!isCompiled(absolutePath + "/" + baseName + ".exe")){
//        if(QMessageBox::Yes == QMessageBox::question(NULL,"Confirm","源代码未编译。\n是否立即编译?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
//            return compileAndRun(fileInfo, resultList, result);//如果要编译后运行就直接编译后运行
//        }else{
//            return false;//不编译就直接退出
//        }
//    }
    if(!isCompiled(absolutePath + "/" + baseName + ".exe")){//没有编译过就编译并运行
        return compileAndRun(fileInfo, resultList, result);
    }
    int fileTime = fileInfo.lastModified().toMSecsSinceEpoch();
    int exeTime = QFileInfo(absolutePath + "/" + baseName + ".exe").lastModified().toMSecsSinceEpoch();
    if(fileTime > exeTime){//上次保存还没编译
        return compileAndRun(fileInfo, resultList, result);
    }

    QDetachableProcess process;
    process.setCreateProcessArgumentsModifier(
        [](QProcess::CreateProcessArguments *args) {
            args->flags |= CREATE_NEW_CONSOLE;
            args->startupInfo->dwFlags &=~ STARTF_USESTDHANDLES;
        });//创建控制台窗口
    QString program = "cmd.exe";
    QStringList statementList = {"/c", absolutePath + "/" + baseName + ".exe", "&", "pause"};
    process.start(program,statementList);//使用cmd运行该程序
    process.detach();//分离式运行，不影响其他程序
    return true;
}

bool Compiler::compileAndRun(QFileInfo fileInfo,QList<CompileInfo> &resultList, QString &result){
    if(!compile(fileInfo, resultList, result)){//先编译，看编译是否成功
        return false;
    }
    if(!run(fileInfo, resultList, result)){//编译成功后再运行
        return false;
    }
    return true;
}

bool Compiler::isCompiled(QString filePath){
    QFileInfo fileInfo(filePath);
    if(fileInfo.isFile()){
        return true;
    }
    return false;
}

bool Compiler::resultToResultList(QString result, QList<CompileInfo> &resultList, int &numError, int &numWarning){
    QStringList resultTemp = result.split('\n');
    for(QString &resultRow : resultTemp){//每一行进行分析
        if(resultRow.isEmpty()){
            continue;
        }
        if(resultRow.front() == ' '){//如果是用来指的行直接去掉，如 4 | a
            resultRow = "";
            continue;
        }
        CompileInfo compileInfo;
        int index0 = resultRow.indexOf(':');
        int index1 = resultRow.indexOf(':',index0+1);
        compileInfo.fileName = resultRow.mid(0, index1);//获得第二个冒号之前的东西
        int index2 = resultRow.indexOf(':', index1+1);
        if(index2 == -1){
            compileInfo.information = resultRow.mid(index1+2, resultRow.length()-index1-1);//:后面没有:了，说明后面都是信息,+2是因为还要去掉空格
            resultList.append(compileInfo);
        }else{
            bool ok;//判断第二个值是不是行数
            QString row = resultRow.mid(index1+1,index2-index1-1);
            row.toInt(&ok, 10);
            if(!ok){
                compileInfo.information = resultRow.mid(index1+2, resultRow.length()-index1-1);//:后面有:，但是两个:中间不是数字，说明后面都是信息,+2是因为还要去掉空格
                resultList.append(compileInfo);
            }else{
                compileInfo.row = row;//行号
                int index3 = resultRow.indexOf(':', index2+1);
                compileInfo.column = resultRow.mid(index2+1, index3-index2-1);//列号
                compileInfo.information = resultRow.mid(index3+2, resultRow.length()-index3-1);//信息,+2是因为还要去掉空格
                resultList.append(compileInfo);
            }
        }
        if(compileInfo.information.length()<5){//比5少，说明不可能是error
            continue;
        }
        if(compileInfo.information.mid(0,5) == "error"){
            numError++;
            continue;
        }
        if(compileInfo.information.mid(0,5) == "fatal"){
            numError++;
            continue;
        }
        if(compileInfo.information.length()<7){//比7小，说明不可能是warning
            continue;
        }
        if(compileInfo.information.mid(0,7) == "warning"){
            numWarning++;
        }
    }
    return true;
}

void Compiler::resultProcessing(QFileInfo fileInfo, QString &result, int numError, int numWarning, double compilationTime, QString command){
    QString originalPath = QCoreApplication::applicationFilePath();//记录原始路径
    QString beforeResult, afterResult, type, version;
    QDir::setCurrent(gccPathCurrent);
    if(fileInfo.suffix() == "c"){
        type = "";
        QProcess process;
        process.start("gcc.exe",{"--version"});
        process.waitForFinished();
        QDir::setCurrent(originalPath);//切换回原来的路径
        QByteArray output = process.readAllStandardOutput();
        version = QString::fromLocal8Bit(output).split("\n")[0];//获得版本
    }else{
        type = "++";
        QProcess process;
        process.start("g++.exe",{"--version"});
        process.waitForFinished();
        QDir::setCurrent(originalPath);//切换回原来的路径
        QByteArray output = process.readAllStandardOutput();
        version = QString::fromLocal8Bit(output).split("\n")[0];//获得版本
    }
    beforeResult = "Compiling single file...\n"
                   "------------------\n"
                   "- Filename: " + fileInfo.absoluteFilePath() + "\n"
                   "- Compiler Version: " + version + "\n\n"
                   "Processing C" + type +" source file:\n"
                   "------------------\n"
                   "C" + type +" Compiler: " + gccPathCurrent + "\\g" + QString(fileInfo.suffix() == "c"?"cc":"++") +".exe\n"
                   "Command: " + command + "\n";
    afterResult = "\n\n"
                "Compile Result:\n"
                "------------------\n"
                "- Errors: " + QString::number(numError) + "\n"
                "- Warnings: " + QString::number(numWarning) + "\n"
                "- Output Filename: " + fileInfo.absolutePath() + "/" + fileInfo.baseName() + ".exe" + "\n"
                "- Output Size: " + (numError>0?"0":QString::number(QFileInfo(fileInfo.absolutePath() + "/" + fileInfo.baseName() + ".exe").size()/1024.0)) + " KiB\n"
                "- Compilation Time: " + QString::number(compilationTime) + " ms\n";
    result = beforeResult + result + afterResult;
}

bool Compiler::findGccPath(){
    QStringList environment = QProcess::systemEnvironment();//获取环境变量
    QString path ="";//获取Path
    for(QString &str : environment){
        if(str.sliced(0,5).toLower() == "path="){
            path = str;
            break;
        }
    }
    QStringList pathList = path.remove(0,5).split(";");//将Path里的路径分开
    QStringList pathWithBin = pathList.filter("\\bin");//找到带了\bin的路径
    if(pathList.isEmpty()){
        return false;
    }
    for(QString &str : pathWithBin){//在每个带\bin的路径判断是否有gcc.exe
        if(QFileInfo(str+"\\gcc.exe").isFile()){
            gccPathList.append(str);
        }
    }
    if(gccPathList.isEmpty()){
        return false;//没有gcc的路径
    }
    return true;
}

bool Compiler::addGccPath(QString gccPath){
    if(QFileInfo(gccPath+"\\gcc.exe").isFile()){
        gccPathList.append(gccPath);
        gccPathCurrent = gccPath;
        return true;
    }
    return false;
}

bool Compiler::changeGccPath(QString gccPath){
    if(QFileInfo(gccPath+"\\gcc.exe").isFile()){
        gccPathCurrent = gccPath;
        return true;
    }
    return false;
}

bool Compiler::deleteGccPath(QString gccPath){
    int index = gccPathList.indexOf(gccPath);//找到要删除的索引
    if(index == -1){
        return false;//没找到这个路径
    }
    gccPathList.removeAt(index);
    if(!gccPathList.isEmpty()){
        gccPathCurrent = gccPathList[0];
    }
    return true;
}
