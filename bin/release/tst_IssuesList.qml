import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE
import "./ui/component"
import QtTest


Item {
    id: issues

    SignalSpy {
        id: spy2
        target: FileManager
        signalName: "fileOpen"
    }

    SignalSpy {
        id: spy4
        target: FileManager
        signalName: "fileCompileIssues"
    }

    TestCase {
        name: "IssuesListSingleCompile"
        when: windowShown
        id: testcase

        function test_abnormal_cpp() {
            verify(spy2.valid);
            verify(spy4.valid);
            FileManager.openFile("file:///D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile2.cpp");
            FileManager.currentIdChange("D:/Qt-Dev/UselessIDE/bin/release/testdata/data_compile2.cpp")
            spy2.wait();
            compare(spy2.count, 1);
            spy2.clear();
            FileManager.compileFile();
            spy4.wait();
            compare(spy4.count, 1);
            var res4 = spy4.signalArguments[0][0];
            wait(1000);
            verify(res4.length===table_view.dataSource.length);
            spy4.clear();
        }
    }

    Component.onCompleted: {
        const dataSource = [{filename:"",line:"",col:"",description:""}]
        table_view.dataSource = dataSource
    }

    Connections{
        target: FileManager
        function onFileCompileIssues(resList){
            const dataSource = []
            console.log("reslistLen",resList.length)
            for (var i = 0; i < resList.length; i++) {
                dataSource.push({
                    filename:resList[i][0],
                    line:resList[i][1],
                    col:resList[i][2],
                    description:resList[i][3]
                })
            }
            table_view.dataSource = dataSource
        }
    }

    IssuesTableView{
        id:table_view
        anchors{
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        columnSource:[
            {
                title: lang.filename,
                dataIndex: 'filename',
                readOnly:true,
            },
            {
                title: lang.line,
                dataIndex: 'line',
                readOnly:true,
                miniumWidth:50,
            },
            {
                title: lang.col,
                dataIndex: 'col',
                readOnly:true,
                miniumWidth:50,
            },
            {
                title: lang.description,
                dataIndex: 'description',
                readOnly:true,
            }
        ]
    }
}
