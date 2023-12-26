import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import FluentUI
import UselessIDE
import QtTest

Row{
    id: control
    property int iconSize : 20
    anchors.fill: parent
    leftPadding: 10

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
        name: "ToolSingleCompile"
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
            compile_btn.clicked();
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
            compile_btn.clicked();
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

    FileDialog {
        id: filesSaveDialog
        title: "文件另存为"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            console.log("You chose: " + filesSaveDialog.file)
            FileManager.saveFileAs(filesSaveDialog.file)
        }
    }

    Connections{
        target: FileManager
        function onNoFileOpened(){
            save_btn.enabled = false
            save_as_btn.enabled = false
            reformat_cdoe_btn.enabled = false
            compile_btn.enabled = false
            run_btn.enabled = false
        }

        function onHasFileOpened(){
            save_btn.enabled = true
            save_as_btn.enabled = true
            reformat_cdoe_btn.enabled = true
            compile_btn.enabled = true
            run_btn.enabled = true
        }
    }

    FluIconButton{
        text: lang.new_+"(Ctrl+N)"
        id: new_files_btn
        iconSize: control.iconSize
        iconSource: FluentIcons.SubscriptionAdd
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            FileManager.newFile()
        }
    }

    FluIconButton{
        text: lang.open+"(Ctrl+O)"
        id: open_file_btn
        iconSize: control.iconSize
        iconSource: FluentIcons.OpenFile
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            fileOpenDialog.open()
        }
    }

    FluIconButton{
        text: lang.save+"(Ctrl+S)"
        id: save_btn
        enabled: false
        iconSize: control.iconSize
        iconSource: FluentIcons.Save
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            if(FileManager.needSaveAs()){
                filesSaveDialog.open()
            }else{
                FileManager.saveFile()
            }
        }
    }

    FluIconButton{
        text: lang.save_as+"(Ctrl+Shift+S)"
        id: save_as_btn
        enabled: false
        iconSize: control.iconSize
        iconSource: FluentIcons.SaveAs
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            filesSaveDialog.open()
        }
    }

    ToolSeparator{anchors.verticalCenter: parent.verticalCenter}

    FluIconButton {
        id: reformat_cdoe_btn
        enabled: false
        text: lang.reformat+"(Ctrl+Shift+A)"
        iconSize: control.iconSize
        iconSource: FluentIcons.Code
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            FileManager.formatFile();
        }
    }

    ToolSeparator{anchors.verticalCenter: parent.verticalCenter}

    FluIconButton{
        text: lang.compile+"(F9)"
        id: compile_btn
        enabled: false
        iconSize: control.iconSize
        iconSource: FluentIcons.ViewAll
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            FileManager.compileFile()
        }
    }

    FluIconButton{
        text: lang.run+"(F11)"
        id: run_btn
        enabled: false
        iconSize: control.iconSize
        iconSource: FluentIcons.PlayBadge12
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            FileManager.runFile()
        }
    }

    ToolSeparator{anchors.verticalCenter: parent.verticalCenter}

    FluIconButton{
        text: lang.about_
        id: setting_btn
        iconSize: control.iconSize
        iconSource: FluentIcons.Settings
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            FluApp.navigate("/setting")
        }
    }
}
