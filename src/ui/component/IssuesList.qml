import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE
import "qrc:/UselessIDE/ui/component"


Item {
    id: issues

    Component.onCompleted: {
        const dataSource = [{filename:"",line:"",col:"",description:""}]
        table_view.dataSource = dataSource
    }

    Connections{
        target: FileManager
        function onFileCompileIssues(resList){
            const dataSource = []
            console.log("outer",resList.length)
            for (var i = 0; i < resList.length-1; i++) {
                console.log("inner",resList[i].length)
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

