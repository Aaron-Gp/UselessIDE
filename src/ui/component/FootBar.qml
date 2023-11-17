import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import "qrc:/UselessIDE/ui/component"

Item {
    property var colors : [FluColors.Yellow,FluColors.Orange,FluColors.Red,FluColors.Magenta,FluColors.Purple,FluColors.Blue,FluColors.Teal,FluColors.Green]

    id: centerItem

    CustomTabView{
        id:tab_view
        addButtonVisibility: false
        closeButtonVisibility: FluTabViewType.Nerver
        tabWidthBehavior: FluTabViewType.SizeToContent
        anchors.fill: parent
        Component.onCompleted: {
            newTab("Issues", issues)
            newTab("Output", output)
        }
        Component{
            id:issues
            IssuesList{
                anchors.fill: parent
            }
        }
        Component{
            id:output
            OutputTextArea{
                id: output_textArea
                anchors.fill: parent
            }
        }
        function newTab(title, page){
            tab_view.appendTab("",title, page,colors[Math.floor(Math.random() * 8)].dark)
        }
    }
}

