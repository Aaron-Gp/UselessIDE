import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import UselessIDE
import QtTest

Item{
    id: editor

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

    TestCase {
        name: "OutputTextAreaSingleCompile"
        when: windowShown
        id: testcase

        function test_abnormal_cpp() {
            verify(spy2.valid);
            verify(spy3.valid);
            FileManager.openFile("file:///D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile2.cpp");
            FileManager.currentIdChange("D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile2.cpp")
            spy2.wait();
            compare(spy2.count, 1);
            spy2.clear();
            FileManager.compileFile();
            spy3.wait();
            compare(spy3.count, 1);
            var res3 = spy3.signalArguments[0][0].toString();
            res3 = res3.replace("\r","");
            wait(1000);
//            console.log(edit.text.slice(208,210));
//            console.log(res3[209]);
            compare(edit.text, res3);
            spy3.clear();
        }
    }

    Flickable {
        id: flick
        boundsMovement: Flickable.StopAtBounds
        anchors.fill: parent
        contentWidth: edit.width
        contentHeight: edit.height
        clip: true
        flickableDirection: Flickable.AutoFlickIfNeeded
        ScrollBar.vertical: FluScrollBar {
            width: 20
            rightPadding: 5
        }
        ScrollBar.horizontal: FluScrollBar{
            width: 20
        }

        function ensureVisible(r)
        {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
        }

        TextEdit {
            id: edit
            text: ""
            readOnly: true
            selectByKeyboard: false
            cursorVisible: false
            width: contentWidth + 20 < editor.width ? editor.width : contentWidth + 20
            height: contentHeight + 20 < editor.height ? editor.height : contentHeight + 20
            font.pointSize: 15
            color: {
                if(FluTheme.dark){
                    return "white"
                }else{
                    return "black"
                }
            }
            font.family: "FiraCode Nerd Font Mono"
            leftPadding: 10
            rightPadding: 10
            topPadding: 10
            bottomPadding: 10
            focus: true
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
            Component.onCompleted: {

            }

            Connections{
                target: FileManager
                function onFileCompileOutput(output){
                    edit.append(output)
                }
            }
        }
    }

}
