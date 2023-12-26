import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE

Item {
    id: editor
    property string content: ""
    property string fileId: ""
    property int fontPointSize: 15
    property string fontFamily: "FiraCode Nerd Font Mono"


    Flickable {
        id: flick
        boundsMovement: Flickable.StopAtBounds

        anchors {
            fill: parent
        }

        contentWidth: edit.width + bar.width
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

        function ensureVisible(r) // 加减padding
        {
            if (contentX >= r.x)
                contentX = r.x-10;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width+10;
            if (contentY >= r.y)
                contentY = r.y-10;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height+10;
        }

        TextMetrics {
            id: fm
            font {
                family: fontFamily
                pointSize: fontPointSize
            }
            text: "#include"
            Component.onCompleted: {
                console.log(width/8)
            }
        }

        Column{
            id: bar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 9.5
            height: edit.contentHeight
            clip: true
            spacing: 0
            Repeater{
                model: edit.lineCount
                Rectangle{
                    width: lineNumberWidth(edit.lineCount)
                    height: padding.contentHeight
                    color: {
                        if(FluTheme.dark){
                            return "#333"
                        }else{
                            return "#eee"
                        }
                    }
                    Text {
                        id: showLineNumber
                        anchors{
                            centerIn: parent
                        }
                        text: index+1
                        color: "gray"
                        font.family: fontFamily
                        font.pointSize: fontPointSize
                    }
                }
            }
        }

        TextEdit{
            id:padding
            font.pointSize:fontPointSize
            font.family: fontFamily
            visible: false
            text: " "
        }

        MouseArea{
            id: mouseArea
            anchors.fill: parent
            propagateComposedEvents: true
            onWheel: (wheel)=>{
                if (wheel.pixelDelta === Qt.point(0, 0)
                        && wheel.angleDelta === Qt.point(0, 0)) {
                    return
                }

                if (wheel.modifiers & Qt.ControlModifier) {
                    // 缩放
                    if (wheel.angleDelta.y > 0) {
                        fontPointSize = Math.min(fontPointSize + 1, 72)
                    }
                    else {
                        fontPointSize = Math.max(fontPointSize - 1, 10)
                    }
                }else{
                    wheel.accepted=false
                }
            }
        }

        TextEdit {
            id: edit
            text: content
            anchors.left: bar.right
            width: contentWidth + 20 < editor.width - bar.width ? editor.width - bar.width : contentWidth + 20
            height: contentHeight + 20 < editor.height ? editor.height : contentHeight + 20
            font.family: fontFamily
            font.pointSize: fontPointSize
            tabStopDistance: fm.width/8 * 4
            color: {
                if(FluTheme.dark){
                    return "white"
                }else{
                    return "black"
                }
            }
            leftPadding: 10
            rightPadding: 10
            topPadding: 10
            bottomPadding: 10
            focus: true
            onCursorRectangleChanged: {
                flick.ensureVisible(cursorRectangle)
            }
            Component.onCompleted: {
                FileManager.setTextDocument(editor.fileId, edit.textDocument)
                console.log(tabStopDistance, font.pixelSize)
            }
            onCursorPositionChanged: {
                if(visible){
                    FileManager.changeCursorPosition(selectionStart, selectionEnd)
                    console.log(selectionStart, selectionEnd)
                }
            }

            onVisibleChanged: {
                if(visible){
                    edit.forceActiveFocus()
                }
            }

            onCanPasteChanged: {
                if(visible){
                    FileManager.changeCanPaste(canPaste)
                }
            }

            onCanRedoChanged: {
                if(visible){
                    FileManager.changeCanRedo(canRedo)
                }
            }

            onCanUndoChanged: {
                if(visible){
                    FileManager.changeCanUndo(canUndo)
                }
            }

            selectionColor: {
                if(FluTheme.dark){
                    return Qt.rgba(1,1,0,0.6)
                }else{
                    return Qt.rgba(1,0,1,0.7)
                }
            }

            selectedTextColor: {
                if(FluTheme.dark){
                    return "white"
                }else{
                    return "black"
                }
            }

            Keys.onTabPressed: {
                if(visible){
                    FileManager.indent();
                }
            }





            Connections{
                target: FileManager
                function onCursorPositionGet(){
                    if(visible){
                        FileManager.changeCursorPosition(edit.cursorPosition)
                    }
                }
                function onEditCut(){
                    if(visible){
                        edit.cut()
                    }
                }
                function onEditCopy(){
                    if(visible){
                        edit.copy()
                    }
                }
                function onEditPaste(){
                    if(visible){
                        edit.paste()
                    }
                }
                function onEditStateGet(){
                    if(visible){
                        FileManager.changeEditState(edit.canUndo, edit.canRedo, edit.canPaste)
                    }
                }

                function onCursorSelectionSet(start, end){
                    if(visible){
                        edit.select(start, end);
                    }
                }
            }
        }
    }

    function lineNumberWidth(lineCount){
        var width=1;
        while(lineCount>=10){
            lineCount/=10;
            ++width;
        }
        return width*20
    }
}

