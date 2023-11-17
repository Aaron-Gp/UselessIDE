import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE
import Qt.labs.folderlistmodel

Item{
    clip: true
    property int selectionMode: FluTreeViewType.None
    property var currentElement
    property var currentParentElement
    signal itemClicked(var item)
    id:root

    Component{
        id: delegate_root
        Column{
            width: calculateWidth()
            property var itemModel: model
            Repeater{
                id: repeater_first_level
                model: model
                delegate: delegate_items
            }
            function calculateWidth(){
                var w = 0;
                for(var i = 0; i < repeater_first_level.count; i++) {
                    var child = repeater_first_level.itemAt(i)
                    if(w < child.width_hint){
                        w = child.width_hint;
                    }
                }
                return w;
            }
        }
    }
    Component{
        id:delegate_items
        Column{
            id:item_layout
            property real level: (mapToItem(list_root,0,0).x+list_root.contentX)/0.001
            property var text: model.fileName??"Item"
            property bool hasChild : (model.items !== undefined) && (model.items.count !== 0)
            property var items: model.items??[]
            property var expanded: model.expanded??true
            property int width_hint: calculateWidth()
            property bool singleSelected: currentElement === model
            property var itemModel: model
            function calculateWidth(){
                var w = Math.max(list_root.width, item_layout_row.implicitWidth + 10);
                if(expanded){
                    for(var i = 0; i < repeater_items.count; i++) {
                        var child = repeater_items.itemAt(i)
                        if(w < child.width_hint){
                            w = child.width_hint;
                        }
                    }
                }
                return w;
            }
            Item{
                id:item_layout_rect
                width: list_root.contentWidth
                height: item_layout_row.implicitHeight
                Rectangle{
                    anchors.fill: parent
                    anchors.margins: 2
                    color:{
                        if(FluTheme.dark){
                            if(item_layout.singleSelected && selectionMode === FluTreeViewType.Single){
                                return Qt.rgba(62/255,62/255,62/255,1)
                            }
                            return (item_layout_mouse.containsMouse || item_layout_expanded.hovered || item_layout_checkbox.hovered)?Qt.rgba(62/255,62/255,62/255,1):Qt.rgba(0,0,0,0)
                        }else{
                            if(item_layout.singleSelected && selectionMode === FluTreeViewType.Single){
                                return Qt.rgba(0,0,0,0.06)
                            }
                            return (item_layout_mouse.containsMouse || item_layout_expanded.hovered || item_layout_checkbox.hovered)?Qt.rgba(0,0,0,0.03):Qt.rgba(0,0,0,0)
                        }
                    }
                    Rectangle{
                        width: 3
                        color:FluTheme.primaryColor.dark
                        visible: item_layout.singleSelected && (selectionMode === FluTreeViewType.Single)
                        radius: 3
                        height: 20
                        anchors{
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    MouseArea{
                        id:item_layout_mouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            item_layout_rect.onClickItem()
                        }
                    }
                }
                function onClickItem(){
                    if(selectionMode === FluTreeViewType.None){
                        itemClicked(model)
                    }
                    if(selectionMode === FluTreeViewType.Single){
                        currentElement = model
                        if(item_layout.parent.parent.parent.itemModel){
                            currentParentElement = item_layout.parent.parent.parent.itemModel
                        }else{
                            if(item_layout.parent.itemModel){
                                currentParentElement = item_layout.parent.itemModel
                            }
                        }
                        itemClicked(model)
                    }
                    if(selectionMode === FluTreeViewType.Multiple){

                    }
                }
                RowLayout{
                    id:item_layout_row
                    anchors.verticalCenter: item_layout_rect.verticalCenter
                    Item{
                        width: 15*level
                        Layout.alignment: Qt.AlignVCenter
                    }
                    FluIconButton{
                        id:item_layout_expanded
                        color:"#00000000"
                        opacity: item_layout.hasChild
                        onClicked: {
                            if(!item_layout.hasChild){
                                item_layout_rect.onClickItem()
                                return
                            }
                            model.expanded = !model.expanded
                        }
                        contentItem: FluIcon{
                            rotation: item_layout.expanded?0:-90
                            iconSource:FluentIcons.ChevronDown
                            iconSize: 15
                            Behavior on rotation {
                                enabled: FluTheme.enableAnimation
                                NumberAnimation{
                                    duration: 167
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }
                    FluText {
                        text:  item_layout.text
                        Layout.alignment: Qt.AlignVCenter
                        topPadding: 7
                        bottomPadding: 7
                    }
                }
            }
            Item{
                id:item_sub
                visible: {
                    if(!hasChild){
                        return false
                    }
                    return item_layout.expanded??false
                }

                width: item_sub_layout.implicitWidth
                height: item_sub_layout.implicitHeight
                x:0.001
                Column{
                    id: item_sub_layout
                    Repeater{
                        id:repeater_items
                        model: item_layout.items
                        delegate: delegate_items
                    }
                }
            }
        }
    }
    ListView {
        id: list_root
        anchors.fill: parent
        delegate: delegate_root
        contentWidth: contentItem.childrenRect.width
        model: tree_model
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        clip: true
        boundsBehavior: ListView.StopAtBounds
        ScrollBar.vertical: FluScrollBar {}
        ScrollBar.horizontal: FluScrollBar { }
    }

    TreeView{
        id: tree
        anchors.fill: parent
        delegate: Item {
            id: treeDelegate

            implicitWidth: padding + label.x + label.implicitWidth + padding
            implicitHeight: label.implicitHeight * 1.5

            readonly property real indent: 20
            readonly property real padding: 5

            // Assigned to by TreeView:
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth

            TapHandler {
                onTapped: treeView.toggleExpanded(row)
            }

            Text {
                id: indicator
                visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                x: padding + (treeDelegate.depth * treeDelegate.indent)
                anchors.verticalCenter: label.verticalCenter
                text: "▸"
                rotation: treeDelegate.expanded ? 90 : 0
                color: {
                    if(FluTheme.dark){
                        return "white"
                    }else{
                        return "black"
                    }
                }
            }

            Text {
                id: label
                x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
                width: treeDelegate.width - treeDelegate.padding - x
                clip: true
                text: model.display
                color: {
                    if(FluTheme.dark){
                        return "white"
                    }else{
                        return "black"
                    }
                }
                MouseArea{
                    anchors.fill: parent
                    onDoubleClicked: {
                        showInfo(model.filePath);
                        FileManager.openFile("file:///"+model.filePath)
                    }
                }
            }
        }
    }

    Connections{
        target: FileManager
        function onDirOpen(model, index){
            tree.model=model
            tree.model.rootIndex = index
        }
    }
}

