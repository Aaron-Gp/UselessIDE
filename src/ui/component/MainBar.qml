import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI

SplitView {
    id: splitView
    orientation: Qt.Horizontal
    property int windowHeight: 640
    handle: Rectangle {
        implicitWidth: 3
        color: SplitHandle.hovered ? "#81e889" : "#FFFFFF"
    }

    SideBar{
        clip: true
        implicitWidth: 100
        anchors.left: parent.left
        SplitView.minimumWidth: 50
        SplitView.maximumWidth: 400
    }

    SplitView{
        id: vertSplit
        orientation: Qt.Vertical
        handle: Rectangle {
            implicitHeight: 3
            color: SplitHandle.hovered ? "#81e889" : "#FFFFFF"
        }

        EditorBar{
            anchors.right: parent.right
            SplitView.fillWidth: true
            SplitView.fillHeight: true
        }

        FootBar{
            implicitHeight: 100
            anchors.bottom: parent.bottom
            SplitView.minimumHeight: 150
            SplitView.maximumHeight: windowHeight-200
        }

    }
}


