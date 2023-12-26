import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import UselessIDE

Item{
    id: editor

    Flickable {
        id: flick
        boundsMovement: Flickable.StopAtBounds
        anchors.fill: parent
        contentWidth: edit.width
        contentHeight: edit.height
        clip: true
        flickableDirection: Flickable.AutoFlickIfNeeded
        ScrollBar.vertical: FluScrollBar {
            id: scroll_vert
            width: 20
            rightPadding: 5
        }
        ScrollBar.horizontal: FluScrollBar{
            id: scroll_hori
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
            cursorVisible: true
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
                    edit.clear()
                    edit.append(output)
                    edit.cursorPosition = 0
                    flick.contentX = 0
                    flick.contentY = 0
                    scroll_vert.position = 0
                    scroll_hori.position = 0
                }
            }
        }
    }

}
