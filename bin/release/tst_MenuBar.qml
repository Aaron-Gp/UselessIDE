import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import Qt.labs.platform
import FluentUI
import UselessIDE
import QtTest

FluMenuBar{
    id:menu_bar
    property var searchPageRegister: registerForWindowResult("/search")
    property int menuWidth: 200

    SignalSpy {
        id: spy1
        target: FileManager
        signalName: "fileClose"
    }

    SignalSpy {
        id: spy2
        target: FileManager
        signalName: "fileOpen"
    }

    SignalSpy {
        id: spy3
        target: FileManager
        signalName: "fileCompileOutput"
    }

    SignalSpy {
        id: spy4
        target: FileManager
        signalName: "fileCompileIssues"
    }

    TestCase {
        name: "MenuSingleCompile"
        when: windowShown
        id: testcase

        function test_abnormal_cpp() {
            verify(spy2.valid);
            verify(spy3.valid);
            verify(spy4.valid);
            FileManager.openFile("file:///D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile2.cpp");
            FileManager.currentIdChange("D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile2.cpp")
            spy2.wait();
            compare(spy2.count, 1);
            spy2.clear();
            compile.trigger();
            spy3.wait();
            compare(spy3.count, 1);
            var res3 = spy3.signalArguments[0][0].toString();
            verify(res3.length>0);
            spy3.clear();
            spy4.wait();
            compare(spy4.count, 1);
            var res4 = spy4.signalArguments[0][0];
            verify(res4.length===7);
            spy4.clear();
        }

        function test_other_txt() {
            verify(spy2.valid);
            verify(spy3.valid);
            verify(spy4.valid);
            FileManager.openFile("file:///D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile5.txt");
            FileManager.currentIdChange("D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile5.txt")
            spy2.wait();
            compare(spy2.count, 1);
            spy2.clear();
            compile.trigger();
            spy3.wait();
            compare(spy3.count, 1);
            var res3 = spy3.signalArguments[0][0].toString();
            compare(res3, "Compile unsuccessfully.\nThe file's suffix must be .c or .cpp.");
            spy3.clear();
            spy4.wait();
            compare(spy4.count, 1);
            var res4 = spy4.signalArguments[0][0];
            verify(res4.length===0);
            spy4.clear();
        }
    }

    FileDialog {
        id: fileOpenDialog
        title: "选择一个文件"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            console.log("You chose: " + fileOpenDialog.file)
            FileManager.openFile(fileOpenDialog.file)
        }
    }

    FolderDialog {
        id: dirOpenDialog
        title: "选择工作目录"
        folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
            console.log("You chose: " + dirOpenDialog.folder)
            FileManager.openDir(dirOpenDialog.folder)
        }
    }

    FileDialog {
        id: filesSaveDialog
        title: lang.save_as
        fileMode: FileDialog.SaveFile
        onAccepted: {
            console.log("You chose: " + filesSaveDialog.file)
            FileManager.saveFileAs(filesSaveDialog.file)
        }
    }

    FluMenu {
        title: lang.file
        width: menuWidth
        Action {
            id: new_file
            text: lang.new_+"(Ctrl+N)"
            shortcut: "Ctrl+N"
            onTriggered: FileManager.newFile()
        }
        Action {
            id: open_file
            text: lang.open+"(Ctrl+O)"
            shortcut: "Ctrl+O"
            onTriggered: {
                fileOpenDialog.open()
            }
        }
        Action {
            id: open_dir
            text: lang.dir
            onTriggered: {
                dirOpenDialog.open()
            }
        }
        FluMenuSeparator { }
        Action {
            id: svae_file
            text: lang.save+"(Ctrl+S)"
            shortcut: "Ctrl+S"
            onTriggered: {
                if(FileManager.needSaveAs()){
                    filesSaveDialog.open()
                }else{
                    FileManager.saveFile()
                }
            }
        }
        Action {
            id: save_as
            text: lang.save_as+"(Ctrl+Shift+S)"
            shortcut: "Ctrl+Shift+S"
            onTriggered: {
                filesSaveDialog.open()
            }
        }
        FluMenuSeparator { }
        Action {
            id: close_file
            text: lang.close+"(Ctrl+W)"
            shortcut: "Ctrl+W"
            onTriggered: {
                FileManager.closeFile()
            }
        }
        FluMenuSeparator { }
        Action {
            text: lang.exit+"(Ctrl+Q)"
            shortcut: "Ctrl+Q"
            onTriggered: {
                FileManager.exitProgram()
            }
        }
    }
    Connections{
        target: FileManager
        function onCanUndoChange(can){
            console.log("undo", can)
            undo.enabled = can
        }

        function onCanRedoChange(can){
            redo.enabled = can
        }

        function onCanPasteChange(can){
            paste.enabled = can
        }
        function onNoFileOpened(){
            svae_file.enabled = false
            save_as.enabled = false
            close_file.enabled = false
            undo.enabled = false
            redo.enabled = false
            cut.enabled = false
            copy.enabled = false
            paste.enabled = false
            indent.enabled = false
            unindent.enabled = false
            toggle_comment.enabled = false
            find.enabled = false
            replace.enabled = false
            reformat.enabled = false
            compile.enabled = false
            run.enabled = false
        }

        function onHasFileOpened(){
            svae_file.enabled = true
            save_as.enabled = true
            close_file.enabled = true
            cut.enabled = true
            copy.enabled = true
            indent.enabled = true
            unindent.enabled = true
            toggle_comment.enabled = true
            find.enabled = true
            replace.enabled = true
            reformat.enabled = true
            compile.enabled = true
            run.enabled = true
        }
    }

    FluMenu {
        width: menuWidth
        title: lang.edit
        Action {
            id: undo
            enabled: false
            text: lang.undo+"(Ctrl+Z)"
            shortcut: "Ctrl+Z"
            onTriggered: {
                FileManager.undo()
            }
        }
        Action {
            id: redo
            enabled: false
            text: lang.redo+"(Ctrl+Shift+Z)"
            shortcut: "Ctrl+Shift+Z"
            onTriggered: {
                FileManager.redo()
            }
        }
        FluMenuSeparator { }
        Action {
            id:cut
            enabled: false
            text: lang.cut+"(Ctrl+X)"
            shortcut: "Ctrl+X"
            onTriggered: {
                FileManager.cut()
            }
        }
        Action {
            id:copy
            enabled: false
            text: lang.copy+"(Ctrl+C)"
            shortcut: "Ctrl+C"
            onTriggered: {
                FileManager.copy()
            }
        }
        Action {
            id: paste
            text: lang.paste+"(Ctrl+V)"
            shortcut: "Ctrl+V"
            enabled: false
            onTriggered: {
                FileManager.paste()
            }
        }
        FluMenuSeparator { }
        Action {
            id: indent
            enabled: false
            text: lang.indent+"(Tab)"
            shortcut: "Tab"
            onTriggered: {
                FileManager.indent();
            }
        }
        Action {
            id: unindent
            enabled: false
            text: lang.unindent+"(Shift+Tab)"
            shortcut: "Shift+Tab"
            onTriggered: {
                FileManager.unIndent();
            }
        }
        FluMenuSeparator { }
        Action {
            id: toggle_comment
            enabled: false
            text: lang.toggle_comment+"(Ctrl+/)"
            shortcut: "Ctrl+/"
            onTriggered: {
                FileManager.toggleComment();
            }
        }
    }
    FluMenu {
        width: menuWidth
        title: lang.search
        Action {
            id: find
            enabled: false
            text: lang.find
            shortcut: "Ctrl+F"
            onTriggered: {
                searchPageRegister.launch({tab:"search"})
            }
        }
        Action {
            id: replace
            enabled: false
            text: lang.replace
            shortcut: "Ctrl+R"
            onTriggered: {
                searchPageRegister.launch({tab:"replace"})
            }
        }
    }
    FluMenu {
        width: menuWidth
        title: lang.code
        Action {
            id: reformat
            enabled: false
            text: lang.reformat+"(Ctrl+Shift+A)"
            shortcut: "Ctrl+Shift+A"
            onTriggered: {
                FileManager.formatFile();
            }
        }
    }
    FluMenu {
        width: menuWidth
        title: lang.execute
        Action {
            id: compile
            enabled: false
            text: lang.compile+"(F9)"
            shortcut: "F9"
            onTriggered: {
                FileManager.compileFile();
            }
        }
        Action {
            id: run
            enabled: false
            text: lang.run+"(F11)"
            shortcut: "F11"
            onTriggered: {
                FileManager.runFile();
            }
        }
    }
    FluMenu {
        width: menuWidth
        title: lang.tools
        Action {
            text: lang.options
            onTriggered: {
                FluApp.navigate("/setting")
            }
        }
    }
    FluMenu {
        width: menuWidth
        title: lang.help
        Action {
            text: lang.about_
            onTriggered: {
                FluApp.navigate("/about")
            }
        }
    }
}

