#include "filemanager.h"
#include <QDebug>
#include <QDir>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <component/codeformatter.h>
#include <QModelIndex>

FileManager* FileManager::m_instance = 0;
QFileSystemModel* FileManager::m_fileSystem = 0;
QQmlEngine* FileManager::m_engine = 0;

FileManager::FileManager(QObject *parent) : QObject(parent)
{
    m_fileMap = QMap<QString, File*>();
    m_compiler = new Compiler;
}

FileManager *FileManager::instance()
{
    static QMutex mutex;
    if (!m_instance) {
        QMutexLocker locker(&mutex);
        if (!m_instance)
            m_instance = new FileManager;
    }
    return m_instance;
}

QObject *FileManager::singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    FileManager *fileManager = instance();
    return fileManager;
}

void FileManager::initFileSystem(QQmlEngine *engine)
{
    m_engine = engine;
    m_fileSystem = new QFileSystemModel(engine);
    m_fileSystem->setRootPath("");
    m_fileSystem->setResolveSymlinks(true);
    m_fileSystem->setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
}

void FileManager::newFile(){
    QString id = "untitled"+QString::number(m_fileCnt);
    auto *f = new File;
    f->path="";
    f->title="untitled"+QString::number(m_fileCnt);
    f->hasSaved = false;
    m_fileMap.insert(id, f);
    emit fileNew(id);
    m_fileCnt++;
}

void FileManager::openFile(const QString &path){
    QString filePath = path.sliced(8);
    qDebug()<<"open file: "<< filePath;
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        QByteArray fileContent;
        if (file.open(QFile::ReadOnly | QFile::Text)){
            fileContent = file.readAll();
            qDebug()<<fileContent.mid(0,100);
            auto f = new File;
            f->path = filePath;
            f->hasSaved = true;
            f->info = new QFileInfo(file);
            f->title = filePath.split("/").last();
            m_fileMap.insert(filePath, f);
            emit fileOpen(filePath, filePath.split("/").last() ,QString(fileContent));
            file.close();
        }else{
            emit showError("cannot open file " +filePath);
            return;
        }
    }
}

void FileManager::saveFile(){
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    auto doc = f->document;
    qDebug()<<"save "<<m_currentId;
    QString filePath = f->path;
    if(!filePath.isEmpty()){
        QFile file(filePath);
        if(file.open(QFile::WriteOnly|QFile::Text)){
            QTextStream stream(&file);
            stream<<doc->toPlainText();
            f->info->refresh();
            doc->setModified(false);
            file.close();
        }else{
            emit showError("cannot save file "+filePath);
            return;
        }
    }
}

void FileManager::saveFileAs(const QString &path){
    if(m_currentId=="") return;
    qDebug()<<"save as "<<path;
    QString filePath = path.sliced(8);
    if(!filePath.isEmpty()){
        QFile file(filePath);
        if(file.open(QFile::WriteOnly|QFile::Text)){
            QTextStream stream(&file);
            auto f = m_fileMap[m_currentId];
            auto doc = f->document;
            stream<<doc->toPlainText();
            f->info = new QFileInfo(file);
            f->title = f->info->fileName();
            f->path = f->info->absoluteFilePath();
            f->hasSaved = true;
            f->document->setModified(false);
            m_fileMap.remove(m_currentId);
            m_fileMap.insert(filePath, f);
            m_currentId = f->path;
            file.close();
            emit fileSavingAs(m_currentId, filePath.split("/").last());
        }else{
            emit showError("cannot save file "+filePath);
            return;
        }
    }
}

void FileManager::closeFile()
{
    if(m_currentId=="") return;
    m_fileMap.remove(m_currentId);
    emit fileClose();
}

void FileManager::setTextDocument(QString id, QQuickTextDocument *quickDocument)
{
    auto f = m_fileMap[id];
    f->document = quickDocument->textDocument();
    f->document->setModified(false);
    auto highlighter = new SyntaxHighlighter(nullptr,m_isDark);
    highlighter->setDocument(f->document);
    f->cursor = new QTextCursor(f->document);
    f->command = new CursorCommand(f->document);
}

bool FileManager::needSaveAs()
{
    if(m_currentId=="") return false;
    auto f = m_fileMap[m_currentId];
    qDebug()<<m_currentId<<" "<<f->hasSaved<<f->document->isModified();
    return !f->hasSaved;
}

bool FileManager::needSaveWhenClose()
{
    if(m_currentId=="") return false;
    auto f = m_fileMap[m_currentId];
    qDebug()<<m_currentId<<" "<<f->document->isModified();
    return f->document->isModified();
}

void FileManager::currentIdChange(QString id)
{
    m_currentId = id;
    emit editStateGet();
    if(id==""){
        emit noFileOpened();
    }else{
        emit hasFileOpened();
    }
}

void FileManager::exitProgram()
{
    emit programExit();
}

void FileManager::setDark(bool isDark)
{
    m_isDark = isDark;
    QMapIterator<QString, File*> itor(m_fileMap);
    while (itor.hasNext())
    {
        itor.next();
        auto f = itor.value();
        auto highlighter = new SyntaxHighlighter(nullptr,m_isDark);
        highlighter->setDocument(f->document);
    }
}

void FileManager::openDir(const QString &path)
{
    QString filePath = path.sliced(8);
    if(!filePath.isEmpty()){
        m_fileSystem = new QFileSystemModel(m_engine);
        auto rootIndex=m_fileSystem->setRootPath(filePath);
        m_fileSystem->setResolveSymlinks(true);
        m_fileSystem->setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
//        auto rootIndex = m_fileSystem->setRootPath(filePath);
        emit dirOpen(m_fileSystem, rootIndex);
    }
}

void FileManager::undo()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    f->document->undo();
}

void FileManager::redo()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    f->document->redo();
}

void FileManager::cut()
{
    emit editCut();
}

void FileManager::copy()
{
    emit editCopy();
}

void FileManager::paste()
{
    emit editPaste();
}

void FileManager::indent()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    f->command->Indent(f->cursor->position(),4);
}

void FileManager::unIndent()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    f->command->unIndent(f->cursor->position(),4);
}

void FileManager::toggleComment()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    if(f->cursor->hasSelection()){
        f->command->ToggleCommentBlock(f->cursor->selectionStart(), f->cursor->selectionEnd());
    }else{
        f->command->CommentLine(f->cursor->position());
    }
}

void FileManager::changeCanUndo(bool can)
{
    qDebug()<<"undo cpp"<<can;
    emit canUndoChange(can);
}

void FileManager::changeCanRedo(bool can)
{
    emit canRedoChange(can);
}

void FileManager::changeCanPaste(bool can)
{
    emit canPasteChange(can);
}

void FileManager::changeEditState(bool undo, bool redo, bool paste)
{
    emit canUndoChange(undo);
    emit canRedoChange(redo);
    emit canPasteChange(paste);
}

void FileManager::searchForward(const QString &str, bool case_sensitively)
{
    if(m_currentId=="") return;
    if(str.isEmpty()) return;
    auto f = m_fileMap[m_currentId];
    auto cursorFrom = f->cursor;
    qDebug()<<"fw selected:"<<cursorFrom->hasSelection();
    if(cursorFrom->hasSelection()){
        cursorFrom->setPosition(cursorFrom->selectionEnd());
    }
    QTextCursor cursor(f->document);
    if(case_sensitively)
        cursor=f->document->find(str,*cursorFrom, QTextDocument::FindCaseSensitively);
    else
        cursor=f->document->find(str,*cursorFrom);
    if(cursor.isNull()){
        cursorFrom->movePosition(QTextCursor::Start);
        if(case_sensitively)
            cursor=f->document->find(str,*cursorFrom, QTextDocument::FindCaseSensitively);
        else
            cursor=f->document->find(str,*cursorFrom);
        if(cursor.isNull()){
            emit showInfo("未找到");
        }
        else{
            cursorFrom->setPosition(cursor.selectionStart());
            cursorFrom->setPosition(cursor.selectionEnd(),QTextCursor::KeepAnchor);
            emit cursorSelectionSet(cursor.selectionStart(), cursor.selectionEnd());
        }
    }
    else{
        cursorFrom->setPosition(cursor.selectionStart());
        cursorFrom->setPosition(cursor.selectionEnd(),QTextCursor::KeepAnchor);
        emit cursorSelectionSet(cursor.selectionStart(), cursor.selectionEnd());
    }
}

void FileManager::searchBackward(const QString &str, bool case_sensitively)
{
    if(m_currentId=="") return;
    if(str.isEmpty()) return;
    auto f = m_fileMap[m_currentId];
    auto cursorFrom = f->cursor;
    qDebug()<<"bw selected:"<<cursorFrom->hasSelection();
    if(cursorFrom->hasSelection()){
        cursorFrom->setPosition(cursorFrom->selectionStart());
    }
    QTextCursor cursor(f->document);
    if(case_sensitively)
        cursor=f->document->find(str,*cursorFrom, QTextDocument::FindBackward|QTextDocument::FindCaseSensitively);
    else
        cursor=f->document->find(str,*cursorFrom, QTextDocument::FindBackward);
    if(cursor.isNull()){
        cursorFrom->movePosition(QTextCursor::End);
        if(case_sensitively)
            cursor=f->document->find(str,*cursorFrom, QTextDocument::FindBackward|QTextDocument::FindCaseSensitively);
        else
            cursor=f->document->find(str,*cursorFrom, QTextDocument::FindBackward);
        if(cursor.isNull()){
            emit showInfo("未找到");
        }
        else{
            cursorFrom->setPosition(cursor.selectionStart());
            cursorFrom->setPosition(cursor.selectionEnd(),QTextCursor::KeepAnchor);
            emit cursorSelectionSet(cursor.selectionStart(), cursor.selectionEnd());
        }
    }
    else{
        cursorFrom->setPosition(cursor.selectionStart());
        cursorFrom->setPosition(cursor.selectionEnd(),QTextCursor::KeepAnchor);
        emit cursorSelectionSet(cursor.selectionStart(), cursor.selectionEnd());
    }
}

void FileManager::replace(const QString &oldStr, const QString &newStr, bool case_sensitively)
{
    if(m_currentId=="") return;
    if(oldStr.isEmpty()) return;
    auto f = m_fileMap[m_currentId];
    auto cursorFrom = f->cursor;
    qDebug()<<"rp selected:"<<cursorFrom->hasSelection();
    if(cursorFrom->hasSelection()){
        if(cursorFrom->selectedText()==oldStr){
            cursorFrom->removeSelectedText();
            cursorFrom->insertText(newStr);
            searchForward(oldStr, case_sensitively);
            return;
        }
    }
    searchForward(oldStr, case_sensitively);
    if(cursorFrom->hasSelection()){
        if(cursorFrom->selectedText()==oldStr){
            cursorFrom->removeSelectedText();
            cursorFrom->insertText(newStr);
            searchForward(oldStr, case_sensitively);
            return;
        }
    }
}

void FileManager::replaceAll(const QString &oldStr, const QString &newStr, bool case_sensitively)
{
    if(m_currentId=="") return;
    if(oldStr.isEmpty()) return;
    auto f = m_fileMap[m_currentId];
    QString text = f->document->toPlainText();
    text.replace(oldStr, newStr, case_sensitively ? Qt::CaseSensitive : Qt::CaseInsensitive);
    f->document->clear();
    f->document->setPlainText(text);
}

void FileManager::compileFile()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    QList<CompileInfo> resultList;
    QString result;
    m_compiler->compile(*f->info, resultList, result);
    emit fileCompileOutput(result);
    auto resL = QList<QVector<QString>>();
    foreach (auto info, resultList) {
        auto vinfo = QVector<QString>();
        vinfo.append(info.fileName);
        vinfo.append(info.row);
        vinfo.append(info.column);
        vinfo.append(info.information);
        resL.append(vinfo);
    };
    emit fileCompileIssues(resL);
}

void FileManager::runFile()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    QList<CompileInfo> resultList;
    QString result;
    m_compiler->run(*f->info, resultList, result);
    emit fileCompileOutput(result);
    auto resL = QList<QVector<QString>>();
    foreach (auto info, resultList) {
        auto vinfo = QVector<QString>();
        vinfo.append(info.fileName);
        vinfo.append(info.row);
        vinfo.append(info.column);
        vinfo.append(info.information);
        resL.append(vinfo);
    };
    emit fileCompileIssues(resL);
}

void FileManager::formatFile()
{
    if(m_currentId=="") return;
    auto f = m_fileMap[m_currentId];
    QString code = f->document->toPlainText();
    QString formattedCode = CodeFormatter::format(code);
    qDebug()<<formattedCode;
    f->document->clear();
    f->document->setPlainText(formattedCode);
}

void FileManager::changeCursorPosition(int start, int end)
{
    if(m_currentId!=""){
        if(start==-1){
            emit cursorPositionGet();
            return;
        }
        auto f = m_fileMap[m_currentId];
        int row = 1;
        int col = 1;
        auto text = f->document->toPlainText();
        for (int i = 0; i < start; i++) {
            if (text[i] == '\n') {
                row++;
                col = 1;
            } else {
                col++;
            }
        }
        int lines = f->document->lineCount();
        if(end!=-1){
            f->cursor->setPosition(start);
            f->cursor->setPosition(end, QTextCursor::KeepAnchor);
        }else{
            f->cursor->setPosition(start);
        }
        emit cursorPositionChange(row, col, lines);
    }
}

void FileManager::openAbout()
{
    emit aboutOpen();
}


