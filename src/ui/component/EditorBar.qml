import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE
import "qrc:/UselessIDE/ui/component"


Item {
    id: centerItem

    EditorTabView{
        id:tab_view
        closeButtonVisibility: FluTabViewType.OnHover
        tabWidthBehavior: FluTabViewType.SizeToContent
        anchors.fill: parent
        Component.onCompleted: {
        }
        onCurrentIndexChanged: function(id){
            FileManager.currentIdChange(id)
            FileManager.changeCursorPosition(-1)
        }

        Component{
            id:editor
            LineNumberEditor{
                anchors.fill: parent
                content: argument.content
                fileId: argument.id
            }
        }
        onNewPressed:{
            FileManager.newFile()
        }
        function newTab(id, title, content){
            tab_view.appendTab("",title,editor,id,{content:content, id: id})
            tab_view.setTabIndex(tab_view.count()-1)
        }
    }

    Connections{
        target: FileManager
        function onFileOpen(id, title, content=""){
            tab_view.newTab(id, title, content)
        }
    }

    Connections{
        target: FileManager
        function onFileNew(id){
            console.log("new id: ", id);
            tab_view.newTab(id, id, "")
        }
    }
}

