#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QDir>
#include <QQmlEngine>
#include <QQuickTextDocument>
#include "highlighter.h"
#include "compiler.h"
#include "cursorcommand.h"
#include <QFileSystemModel>

struct File{
    QString path;
    QString title;

    QByteArray fileContent;
    QFileInfo* info;
    QTextDocument* document;
    QSyntaxHighlighter* highlighter;
    QTextCursor* cursor;
    CursorCommand* command;

    bool hasSaved = false;
};

class FileManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FileManager)
public:
    static FileManager* instance();
    static QObject* singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

    static void initFileSystem(QQmlEngine *engine);

    Q_INVOKABLE void newFile();
    Q_INVOKABLE void openFile(const QString &path);
    Q_INVOKABLE void saveFile();
    Q_INVOKABLE void saveFileAs(const QString &path);
    Q_INVOKABLE void closeFile();
    Q_INVOKABLE void setTextDocument(QString id, QQuickTextDocument* quickDocument);
    Q_INVOKABLE bool needSaveAs();
    Q_INVOKABLE bool needSaveWhenClose();
    Q_INVOKABLE void currentIdChange(QString id);
    Q_INVOKABLE void exitProgram();
    Q_INVOKABLE void setDark(bool isDark);

    Q_INVOKABLE void openDir(const QString &path);

    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();
    Q_INVOKABLE void cut();
    Q_INVOKABLE void copy();
    Q_INVOKABLE void paste();
    Q_INVOKABLE void indent();
    Q_INVOKABLE void unIndent();
    Q_INVOKABLE void toggleComment();
    Q_INVOKABLE void changeCanUndo(bool can);
    Q_INVOKABLE void changeCanRedo(bool can);
    Q_INVOKABLE void changeCanPaste(bool can);
    Q_INVOKABLE void changeEditState(bool undo, bool redo, bool paste);

    Q_INVOKABLE void searchForward(const QString &str, bool case_sensitively=false);
    Q_INVOKABLE void searchBackward(const QString &str, bool case_sensitively=false);
    Q_INVOKABLE void replace(const QString &oldStr, const QString &newStr, bool case_sensitively=false);
    Q_INVOKABLE void replaceAll(const QString &oldStr, const QString &newStr, bool case_sensitively=false);

    Q_INVOKABLE void compileFile();
    Q_INVOKABLE void runFile();
    Q_INVOKABLE void formatFile();
    Q_INVOKABLE void changeCursorPosition(int start, int end=-1);
    Q_INVOKABLE void openAbout();

signals:
    Q_SIGNAL void fileNew(QString id);
    Q_SIGNAL void fileOpen(QString id, QString title, QString content);
    Q_SIGNAL void fileSave(QString id);
    Q_SIGNAL void fileClose();
    Q_SIGNAL void canFileClose();
    Q_SIGNAL void fileSavingAs(QString id, QString title);
    Q_SIGNAL void fileConfirm(QString id);
    Q_SIGNAL void showError(QString msg);
    Q_SIGNAL void showInfo(QString msg);
    Q_SIGNAL void programExit();
    Q_SIGNAL void editCut();
    Q_SIGNAL void editCopy();
    Q_SIGNAL void editPaste();
    Q_SIGNAL void canUndoChange(bool);
    Q_SIGNAL void canRedoChange(bool);
    Q_SIGNAL void canPasteChange(bool);
    Q_SIGNAL void editStateGet();
    Q_SIGNAL void cursorPositionChange(int row, int col, int lines);
    Q_SIGNAL void cursorPositionGet();
    Q_SIGNAL void cursorPositionSet(int pos);
    Q_SIGNAL void cursorSelectionSet(int start, int end);
    Q_SIGNAL void aboutOpen();
    Q_SIGNAL void fileCompileOutput(QString output);
    Q_SIGNAL void fileCompileIssues(QList<QVector<QString>> resultList);

    Q_SIGNAL void noFileOpened();
    Q_SIGNAL void hasFileOpened();

    Q_SIGNAL void dirOpen(QFileSystemModel* model, QModelIndex index);


private:
    FileManager(QObject *parent = nullptr);
    static FileManager* m_instance;
    static QFileSystemModel* m_fileSystem;

    Compiler* m_compiler;
    QMap<QString, File*> m_fileMap;
    int m_fileCnt = 1;
    QString m_currentId = "";
    bool m_isDark = true;
    static QQmlEngine *m_engine;


};

#endif // FILEMANAGER_H
