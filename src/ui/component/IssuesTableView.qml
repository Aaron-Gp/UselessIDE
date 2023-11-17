﻿import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Basic
import Qt.labs.qmlmodels
import FluentUI

Rectangle {
    property var columnSource
    property var dataSource
    property color selectionColor: Qt.alpha(FluTheme.primaryColor.lightest,0.6)
    property color hoverButtonColor: Qt.alpha(selectionColor,0.2)
    property color pressedButtonColor: Qt.alpha(selectionColor,0.4)
    id:control
    color: FluTheme.dark ? Qt.rgba(39/255,39/255,39/255,1) : Qt.rgba(251/255,251/255,253/255,1)

    Component{
        id: com_column
        FluTableModelColumn{

        }
    }

    onColumnSourceChanged: {
        if(columnSource.length!==0){
            var columns= []
            var header_rows = {}
            columnSource.forEach(function(item){
                var column = com_column.createObject(table_model,{display:item.dataIndex});
                columns.push(column)
                header_rows[item.dataIndex] = item.title
            })
            table_model.columns = columns
            header_model.columns = columns
            d.header_rows = [header_rows]
        }
    }
    QtObject{
        id:d
        property int defaultItemWidth: 100
        property int defaultItemHeight: 42
        property var header_rows:[]
        property bool selectionFlag: true
        function obtEditDelegate(column,row){
            var display = table_model.data(table_model.index(row,column),"display")
            var cellItem = table_view.itemAtCell(column, row)
            var cellPosition = cellItem.mapToItem(scroll_table, 0, 0)
            item_loader.column = column
            item_loader.row = row
            item_loader.x = table_view.contentX + cellPosition.x
            item_loader.y = table_view.contentY + cellPosition.y
            item_loader.width = table_view.columnWidthProvider(column)
            item_loader.height = table_view.rowHeightProvider(row)
            item_loader.display = display
            return com_edit
        }
    }
    onDataSourceChanged: {
        table_model.clear()
        dataSource.forEach(function(item){
            table_model.appendRow(item)
        })
    }
    TableModel {
        id:table_model
    }
    Component{
        id:com_edit
        FluTextBox{
            text: display
            readOnly: true === columnSource[column].readOnly
            Component.onCompleted: {
                forceActiveFocus()
                selectAll()
            }
            onCommit: {
                if(!readOnly){
                    display = text
                }
                tableView.closeEditor()
            }
        }
    }

    Component{
        id:com_text
        FluText {
            id:item_text
            text: itemData
            anchors.fill: parent
            anchors.margins: 10
            elide: Text.ElideRight
            wrapMode: Text.NoWrap
            verticalAlignment: Text.AlignVCenter
            HoverHandler{
                id: hover_handler
            }
            FluTooltip{
                text: item_text.text
                delay: 500
                visible: item.text &&  hover_handler.hovered
            }
        }
    }
    ScrollView{
        id:scroll_table
        anchors.left: header_vertical.right
        anchors.top: header_horizontal.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        TableView {
            id:table_view

            ListModel{
                id:model_columns
            }
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.horizontal: FluScrollBar{
                width: 20
            }
            ScrollBar.vertical: FluScrollBar{
                width: 20
            }
            selectionModel: ItemSelectionModel {
                id:selection_model
                model: table_model
                onSelectionChanged: {
                    if(selection_rect.dragging){
                        d.selectionFlag = !d.selectionFlag
                    }
                }
            }
            columnWidthProvider: function(column) {
                var w = columnSource[column].width
                if(!w){
                    w = columnSource[column].minimumWidth
                }
                if(!w){
                    w = d.defaultItemWidth
                }
                if(column === item_loader.column){
                    item_loader.width = w
                }
                if(column === item_loader.column-1){
                    let cellItem = table_view.itemAtCell(item_loader.column, item_loader.row)
                    if(cellItem){
                        let cellPosition = cellItem.mapToItem(scroll_table, 0, 0)
                        item_loader.x = table_view.contentX + cellPosition.x
                    }
                }
                return w
            }
            rowHeightProvider: function(row) {
                if(row>=table_model.rowCount){
                    return 0
                }
                var h = table_model.getRow(row).height
                if(!h){
                    h = table_model.getRow(row).minimumHeight
                }
                if(!h){
                    return d.defaultItemHeight
                }
                if(row === item_loader.row){
                    item_loader.height = h
                }
                if(row === item_loader.row-1){
                    let cellItem = table_view.itemAtCell(item_loader.column, item_loader.row)
                    if(cellItem){
                        let cellPosition = cellItem.mapToItem(scroll_table, 0, 0)
                        item_loader.y = table_view.contentY + cellPosition.y
                    }
                }
                return h
            }
            model: table_model
            clip: true
            delegate: Rectangle {
                id:item_table
                property var position: Qt.point(column,row)
                required property bool selected
                color: (row%2!==0) ? control.color : (FluTheme.dark ? Qt.rgba(1,1,1,0.06) : Qt.rgba(0,0,0,0.06))
                implicitHeight: 40
                implicitWidth: {
                    var w = columnSource[column].width
                    if(!w){
                        w = columnSource[column].minimumWidth
                    }
                    if(!w){
                        w = d.defaultItemWidth
                    }
                    return w
                }
                Rectangle{
                    anchors.fill: parent
                    visible: !item_loader.sourceComponent
                    color: selected ? control.selectionColor : "#00000000"
                }
                MouseArea{
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onPressed:{
                        closeEditor()
                        table_view.interactive = false
                    }
                    onCanceled: {
                        table_view.interactive = true
                    }
                    onReleased: {
                        table_view.interactive = true
                    }
                    onDoubleClicked:{
                        if(display instanceof Component){
                            return
                        }
                        item_loader.sourceComponent = d.obtEditDelegate(column,row)
                    }
                    onClicked:
                        (event)=>{
                            item_loader.sourceComponent = undefined
                            if(!(event.modifiers & Qt.ControlModifier)){
                                selection_model.clear()
                            }
                            selection_model.select(table_model.index(row,column),ItemSelectionModel.Select)
                            d.selectionFlag = !d.selectionFlag
                            event.accepted = true
                        }
                }
                Loader{
                    property var itemData: display
                    property var tableView: table_view
                    property var tableModel: table_model
                    property var position: item_table.position
                    property int row: position.y
                    property int column: position.x
                    anchors.fill: parent
                    sourceComponent: {
                        if(itemData instanceof Component){
                            return itemData
                        }
                        return com_text
                    }
                }
            }
        }
        Loader{
            id:item_loader
            z:2
            property var display
            property int column
            property int row
            property var tableView: control
            sourceComponent: undefined
            onDisplayChanged: {
                var obj = table_model.getRow(row)
                obj[columnSource[column].dataIndex] = display
                table_model.setRow(row,obj)
            }
        }
    }
    Component{
        id:com_handle
        Item {}
    }
    SelectionRectangle {
        id:selection_rect
        target: table_view
        bottomRightHandle:com_handle
        topLeftHandle: com_handle
        onDraggingChanged: {
            if(!dragging){
                table_view.interactive = true
            }
        }
    }
    TableView {
        id: header_horizontal
        model: TableModel{
            id:header_model
            rows: d.header_rows
        }
        syncDirection: Qt.Horizontal
        anchors.left: scroll_table.left
        anchors.top: parent.top
        implicitWidth: syncView ? syncView.width : 0
        implicitHeight: Math.max(1, contentHeight)
        syncView: table_view
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        delegate: Rectangle {
            id:column_item_control
            readonly property real cellPadding: 8
            property bool canceled: false
            readonly property var obj : columnSource[column]
            implicitWidth: column_text.implicitWidth + (cellPadding * 2)
            implicitHeight: Math.max(36, column_text.implicitHeight + (cellPadding * 2))
            color:{
                d.selectionFlag
                if(column_item_control_mouse.pressed){
                    return control.pressedButtonColor
                }
                if(selection_model.isColumnSelected(column)){
                    return control.hoverButtonColor
                }
                return column_item_control_mouse.containsMouse&&!canceled ? control.hoverButtonColor :  FluTheme.dark ? Qt.rgba(50/255,50/255,50/255,1) : Qt.rgba(247/255,247/255,247/255,1)
            }
            border.color: FluTheme.dark ? "#252525" : "#e4e4e4"
            FluText {
                id: column_text
                text: model.display
                width: parent.width
                height: parent.height
                font.bold:{
                    d.selectionFlag
                    return selection_model.columnIntersectsSelection(column)
                }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                id:column_item_control_mouse
                anchors.fill: parent
                anchors.rightMargin: 6
                hoverEnabled: true
                onCanceled: {
                    column_item_control.canceled = true
                }
                onContainsMouseChanged: {
                    if(!containsMouse){
                        column_item_control.canceled = false
                    }
                }
                onClicked:
                    (event)=>{
                        closeEditor()
                        if(!(event.modifiers & Qt.ControlModifier)){
                            selection_model.clear()
                        }
                        for(var i=0;i<=table_view.rows;i++){
                            selection_model.select(table_model.index(i,column),ItemSelectionModel.Select)
                        }
                        d.selectionFlag = !d.selectionFlag
                    }
            }
            MouseArea{
                property point clickPos: "0,0"
                height: parent.height
                width: 6
                anchors.right: parent.right
                acceptedButtons: Qt.LeftButton
                hoverEnabled: true
                visible: !(obj.width === obj.minimumWidth && obj.width === obj.maximumWidth && obj.width)
                cursorShape: Qt.SplitHCursor
                onPressed :
                    (mouse)=>{
                        header_horizontal.interactive = false
                        FluTools.setOverrideCursor(Qt.SplitHCursor)
                        clickPos = Qt.point(mouse.x, mouse.y)
                    }
                onReleased:{
                    header_horizontal.interactive = true
                    FluTools.restoreOverrideCursor()
                }
                onCanceled: {
                    header_horizontal.interactive = true
                    FluTools.restoreOverrideCursor()
                }
                onPositionChanged:
                    (mouse)=>{
                        if(!pressed){
                            return
                        }
                        var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                        var minimumWidth = obj.minimumWidth
                        var maximumWidth = obj.maximumWidth
                        var w = obj.width
                        if(!w){
                            w = d.defaultItemWidth
                        }
                        if(!minimumWidth){
                            minimumWidth = d.defaultItemWidth
                        }
                        if(!maximumWidth){
                            maximumWidth = 65535
                        }
                        obj.width = Math.min(Math.max(minimumWidth, w + delta.x),maximumWidth)
                        table_view.forceLayout()
                    }
            }
        }
    }
    TableView {
        id: header_vertical
        boundsBehavior: Flickable.StopAtBounds
        anchors.top: scroll_table.top
        anchors.left: parent.left
        implicitWidth: Math.max(1, contentWidth)
        implicitHeight: syncView ? syncView.height : 0
        syncDirection: Qt.Vertical
        syncView: table_view
        clip: true
        model: TableModel{
            TableModelColumn {}
            rows: {
                if(dataSource)
                    return dataSource
                return []
            }
        }
        delegate: Rectangle{
            id:item_control
            readonly property real cellPadding: 8
            property bool canceled: false
            implicitWidth: Math.max(30, row_text.implicitWidth + (cellPadding * 2))
            implicitHeight: row_text.implicitHeight + (cellPadding * 2)
            color: {
                d.selectionFlag
                if(item_control_mouse.pressed){
                    return control.pressedButtonColor
                }
                if(selection_model.isRowSelected(row)){
                    return control.hoverButtonColor
                }
                return item_control_mouse.containsMouse&&!canceled ? control.hoverButtonColor :  FluTheme.dark ? Qt.rgba(50/255,50/255,50/255,1) : Qt.rgba(247/255,247/255,247/255,1)
            }
            border.color: FluTheme.dark ? "#252525" : "#e4e4e4"
            FluText{
                id:row_text
                anchors.centerIn: parent
                text: row + 1
                font.bold:{
                    d.selectionFlag
                    return selection_model.rowIntersectsSelection(row)
                }
            }
            MouseArea{
                id:item_control_mouse
                anchors.fill: parent
                anchors.bottomMargin: 6
                hoverEnabled: true
                onCanceled: {
                    item_control.canceled = true
                }
                onContainsMouseChanged: {
                    if(!containsMouse){
                        item_control.canceled = false
                    }
                }
                onClicked:
                    (event)=>{
                        closeEditor()
                        if(!(event.modifiers & Qt.ControlModifier)){
                            selection_model.clear()
                        }
                        for(var i=0;i<=columnSource.length;i++){
                            selection_model.select(table_model.index(row,i),ItemSelectionModel.Select)
                        }
                        d.selectionFlag = !d.selectionFlag
                    }
            }
            MouseArea{
                property point clickPos: "0,0"
                height: 6
                width: parent.width
                anchors.bottom: parent.bottom
                acceptedButtons: Qt.LeftButton
                cursorShape: Qt.SplitVCursor
                visible: {
                    var obj = table_model.getRow(row)
                    return !(obj.height === obj.minimumHeight && obj.height === obj.maximumHeight && obj.height)
                }
                onPressed :
                    (mouse)=>{
                        header_vertical.interactive = false
                        FluTools.setOverrideCursor(Qt.SplitVCursor)
                        clickPos = Qt.point(mouse.x, mouse.y)
                    }
                onReleased:{
                    header_vertical.interactive = true
                    FluTools.restoreOverrideCursor()
                }
                onCanceled: {
                    header_vertical.interactive = true
                    FluTools.restoreOverrideCursor()
                }
                onPositionChanged:
                    (mouse)=>{
                        if(!pressed){
                            return
                        }
                        var obj = table_model.getRow(row)
                        var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                        var minimumHeight = obj.minimumHeight
                        var maximumHeight = obj.maximumHeight
                        var h = obj.height
                        if(!h){
                            h = d.defaultItemHeight
                        }
                        if(!minimumHeight){
                            minimumHeight = d.defaultItemHeight
                        }
                        if(!maximumHeight){
                            maximumHeight = 65535
                        }
                        obj.height = Math.min(Math.max(minimumHeight, h + delta.y),maximumHeight)
                        table_model.setRow(row,obj)
                        table_view.forceLayout()
                    }
            }
        }
    }
    function closeEditor(){
        item_loader.sourceComponent = null
    }
    function resetPosition(){
        table_view.positionViewAtCell(Qt.point(0, 0),Qt.AlignTop|Qt.AlignLeft)
    }
}


