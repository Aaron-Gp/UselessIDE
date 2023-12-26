#include "../component/compiler.h"
#include <QProcess>
#include <QProcessEnvironment>
#include <QTest>

class TestCompiler : public QObject {
  Q_OBJECT
private:
  QString _gccPath;
  QString _env;
  QString _dir;

private slots:

    void initTestCase(){
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString path = env.value("PATH");
        QStringList pathList = path.split(QDir::listSeparator());
        foreach (const QString &path, pathList) {
            if (path.contains("mingw64")) {
                _gccPath=path;
                break;
            }
        }
        _env = QString::fromLatin1(qgetenv("PATH"));
//        qDebug()<<_env;
        QFile file("data_init.txt");
        QFileInfo info(file);
        _dir = info.absolutePath();
        qDebug()<<_dir;
    }

    void cleanupTestCase(){}

    void init(){}

    void cleanup(){}

    void testInit(){
        Compiler compiler;
        QCOMPARE(compiler.gccPathCurrent, _gccPath);
    }

    void testFindGccPath_data(){
        QString path = QString::fromLatin1(qgetenv("PATH"));
        QStringList pathList = path.split(";");
        QStringList gccList = pathList.filter("mingw64");
        foreach (const QString gcc, gccList) {
            pathList.removeOne(gcc);
        }
        QString newpath = pathList.join(";");
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("result");

        QTest::newRow("has gcc") << path << true;
        QTest::newRow("no gcc") << newpath << false;
        QTest::newRow("no PATH") << "" << false;
    }

    void testFindGccPath(){
        QFETCH(QString, path);
        QFETCH(bool, result);
        if (path == ""){
            qunsetenv("PATH");
        }else{
            qputenv("PATH", path.toLatin1());
        }
        Compiler compiler;
        bool res = compiler.findGccPath();
        qputenv("PATH", _env.toLatin1());

        QCOMPARE(res, result);
    }

    void testAddGccPath_data(){
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("result");

        QTest::newRow("has gcc") << "D:\\Program Files\\RedPanda-CPP\\MinGW64\\bin" << true;
        QTest::newRow("has gcc but repeat") << "D:\\mingw64\\bin" << false;
        QTest::newRow("no gcc") << "C:\\Program Files" << false;
    }

    void testAddGccPath(){
        QFETCH(QString, path);
        QFETCH(bool, result);
        Compiler compiler;
        bool res = compiler.addGccPath(path);

        QCOMPARE(res, result);
    }

    void testChangeGccPath_data(){
        QTest::addColumn<int>("id");
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("ret");
        QTest::addColumn<QString>("curpath");

        QTest::newRow("has-gcc") << 1 << "D:\\Program Files\\RedPanda-CPP\\MinGW64\\bin" << true << "D:\\Program Files\\RedPanda-CPP\\MinGW64\\bin";
        QTest::newRow("no-gcc") << 2 << "C:\\Program Files" << false << _gccPath;
    }

    void testChangeGccPath(){
        QFETCH(int, id);
        QFETCH(QString, path);
        QFETCH(bool, ret);
        QFETCH(QString, curpath);
        Compiler compiler;
        bool res = compiler.addGccPath(path);

        QCOMPARE(res, ret);
        QCOMPARE(compiler.gccPathCurrent, curpath);
    }

    void testDeleteGccPath_data(){
        QTest::addColumn<int>("id");
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("ret");
        QTest::addColumn<QString>("curpath");

        QTest::newRow("found and empty") << 1 << "D:\\mingw64\\bin" << true << "";
        QTest::newRow("not found") << 2 << "C:\\Program Files" << false << _gccPath;
        QTest::newRow("found and unempty") << 3 << "D:\\mingw64\\bin" << true << "D:\\Program Files\\RedPanda-CPP\\MinGW64\\bin";
    }

    void testDeleteGccPath(){
        QFETCH(int, id);
        QFETCH(QString, path);
        QFETCH(bool, ret);
        QFETCH(QString, curpath);
        Compiler compiler;
        if(id==3){
            compiler.addGccPath(curpath);
        }
        bool res = compiler.deleteGccPath(path);

        QCOMPARE(res, ret);
        QCOMPARE(compiler.gccPathList.indexOf(path), -1);
        QCOMPARE(compiler.gccPathCurrent, curpath);
    }

    void testResultToResultList_data(){
        QTest::addColumn<int>("id");
        QTest::addColumn<QString>("path");
        QTest::addColumn<QString>("result");
        QTest::addColumn<int>("listlen");
        QTest::addColumn<int>("error");
        QTest::addColumn<int>("warn");

        QTest::newRow("normal .cpp") << 1 << ".\\testdata\\data_compile1.cpp" << "" << 0 << 0 << 0;
        QTest::newRow("abnormal .cpp") << 2 << ".\\testdata\\data_compile2.cpp" << "./testdata/data_compile2.cpp: In function 'int main()':\n./testdata/data_compile2.cpp:4:9: error: 'cout' was not declared in this scope; did you mean 'std::cout'?\n    4 |         cout<<\"Hello World!\"<<endl;\n      |         ^~~~\n      |         std::cout\nIn file included from ./testdata/data_compile2.cpp:1:\nD:/mingw64/include/c++/13.2.0/iostream:63:18: note: 'std::cout' declared here\n   63 |   extern ostream cout;          ///< Linked to standard output\n      |                  ^~~~\n./testdata/data_compile2.cpp:4:31: error: 'endl' was not declared in this scope; did you mean 'std::endl'?\n    4 |         cout<<\"Hello World!\"<<endl;\n      |                               ^~~~\n      |                               std::endl\nIn file included from D:/mingw64/include/c++/13.2.0/iostream:41:\nD:/mingw64/include/c++/13.2.0/ostream:735:5: note: 'std::endl' declared here\n  735 |     endl(basic_ostream<_CharT, _Traits>& __os)\n      |     ^~~~\n" << 7 << 2 << 2;
        QTest::newRow("abnormal .c") << 3 << ".\\testdata\\data_compile4.c" << "./testdata/data_compile4.c:1:10: fatal error: iostream: No such file or directory\n    1 | #include <iostream>\n      |          ^~~~~~~~~~\ncompilation terminated.\r\n" << 2 << 1 << 0;

    }

    void testResultToResultList(){
        QFETCH(int, id);
        QFETCH(QString, result);
        QFETCH(int, listlen);
        QFETCH(int, error);
        QFETCH(int, warn);

        QList<CompileInfo> resultList;
        int numError;
        int numWarning;

        Compiler compiler;
        compiler.resultToResultList(result, resultList, numError, numWarning);

        QCOMPARE(resultList.length(), listlen);
        QCOMPARE(numError, error);
        QCOMPARE(numWarning, warn);
    }

    void testResultProcessing_data(){
        QTest::addColumn<int>("id");
        QTest::addColumn<QString>("path");
        QTest::addColumn<QString>("resultin");
        QTest::addColumn<double>("ctime");
        QTest::addColumn<int>("error");
        QTest::addColumn<int>("warn");
        QTest::addColumn<QString>("command");
        QTest::addColumn<QString>("resultout");

        QTest::newRow(".cpp") << 1 << ".\\testdata\\data_compile1.cpp" << "" << 0.01 << 0 << 0 << "$command" << "Compiling single file...\n------------------\n- Filename: D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile1.cpp\n- Compiler Version: g++.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders) 13.2.0\r\n\nProcessing C++ source file:\n------------------\nC++ Compiler: D:\\mingw64\\bin\\g++.exe\nCommand: $command\n\n\nCompile Result:\n------------------\n- Errors: 0\n- Warnings: 0\n- Output Filename: D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile1.exe\n";
        QTest::newRow(".c") << 2 << ".\\testdata\\data_compile4.c" << "./testdata/data_compile4.c:1:10: fatal error: iostream: No such file or directory\n    1 | #include <iostream>\n      |          ^~~~~~~~~~\ncompilation terminated.\r\n" << 0.071 << 1 << 0 << "$command" << "Compiling single file...\n------------------\n- Filename: D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile4.c\n- Compiler Version: gcc.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders) 13.2.0\r\n\nProcessing C source file:\n------------------\nC Compiler: D:\\mingw64\\bin\\gcc.exe\nCommand: $command\n./testdata/data_compile4.c:1:10: fatal error: iostream: No such file or directory\n    1 | #include <iostream>\n      |          ^~~~~~~~~~\ncompilation terminated.\r\n\n\nCompile Result:\n------------------\n- Errors: 1\n- Warnings: 0\n- Output Filename: D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile4.exe\n- Output Size: 0 KiB\n- Compilation Time: 0.071 ms\n";
    }

    void testResultProcessing(){
        QFETCH(int, id);
        QFETCH(QString, path);
        QFETCH(QString, resultin);
        QFETCH(double, ctime);
        QFETCH(int, error);
        QFETCH(int, warn);
        QFETCH(QString, command);
        QFETCH(QString, resultout);

        QFile file(path);
        QFileInfo fileInfo(file);

        Compiler compiler;
        compiler.resultProcessing(fileInfo, resultin, error, warn, ctime, command);

        if(id==1){
            QCOMPARE(resultin.sliced(0, 456), resultout);
        }else if(id==2){
            QCOMPARE(resultin, resultout);
        }
    }

    void testCompile_data(){
        QTest::addColumn<int>("id");
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("ret");
        QTest::addColumn<QString>("result");
        QTest::addColumn<QString>("resultlist");

        QTest::newRow("normal .cpp") << 1 << ".\\testdata\\data_compile1.cpp" << true << "null" << "0";
        QTest::newRow("abnormal .cpp") << 2 << ".\\testdata\\data_compile2.cpp" << false << "null" << "7";
        QTest::newRow("normal .c") << 3 << ".\\testdata\\data_compile3.c" << true << "null" << "0";
        QTest::newRow("abnormal .c") << 4 << ".\\testdata\\data_compile4.c" << false << "null" << "1";
        QTest::newRow("other .txt") << 5 << ".\\testdata\\data_compile5.txt" << false << "Compile unsuccessfully.\nThe file's suffix must be .c or .cpp." << "0";
        QTest::newRow("empty file") << 6 << ".\\testdata\\data_compile0.cpp" << false << "File does not exists\n" << "0";
        QTest::newRow("no gcc") << 7 << ".\\testdata\\data_compile1.cpp" << false << "Cannot find the gcc.\n" << "0";
    }

    void testCompile(){
        QFETCH(int, id);
        QFETCH(QString, path);
        QFETCH(bool, ret);
        QFETCH(QString, result);
        QFETCH(QString, resultlist);

        if(id==7){
            qputenv("PATH", "");
        }

        Compiler compiler;

        QFile file(path);
        QFileInfo info(file);

        QList<CompileInfo> resList;
        QString res;
        bool ans = compiler.compile(info, resList, res);
        if(id==7){
            qputenv("PATH", _env.toLatin1());
        }

        QCOMPARE(ans, ret);
        if(result!="null"){
            QCOMPARE(res, result);
        }
        if(resultlist!="null"){
            QCOMPARE(resList.length(), resultlist.toInt());
        }
    }

    void testIsCompiled_data(){
        QTest::addColumn<int>("id");
        QTest::addColumn<QString>("path");
        QTest::addColumn<bool>("ret");

        QTest::newRow("normal .cpp") << 1 << ".\\testdata\\data_compile1.cpp" << true;
        QTest::newRow("abnormal .cpp") << 2 << ".\\testdata\\data_compile2.cpp" << false;
    }

    void testIsCompiled(){
        QFETCH(int, id);
        QFETCH(QString, path);
        QFETCH(bool, ret);
        Compiler compiler;

        QFile file(path);
        QFileInfo info(file);

        QList<CompileInfo> resList;
        QString res;
        compiler.compile(info, resList, res);
        QString baseName = info.baseName();
        QString absolutePath = info.absolutePath();
        bool ans = compiler.isCompiled(absolutePath + "/" + baseName + ".exe");

        QCOMPARE(ans, ret);
    }

};

QTEST_MAIN(TestCompiler)
#include "testcompiler.moc"
