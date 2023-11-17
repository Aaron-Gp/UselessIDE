import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import FluentUI
import UselessIDE
import "qrc:/UselessIDE/ui/component"

CustomWindow {
    id:window
    title: "UselessIDE"
    width: 1000
    height: 640
    closeDestory:false
    minimumWidth: 520
    minimumHeight: 200
    appBarVisible: false
    launchMode: FluWindowType.SingleTask

    closeFunc:function(event){
        dialog_close.open()
        event.accepted = false
    }

    Component.onCompleted: {
        FluTools.setQuitOnLastWindowClosed(false)
    }

    SystemTrayIcon {
        id:system_tray
        visible: true
        icon.source: "qrc:/UselessIDE/res/image/favicon.ico"
        tooltip: "UselessIDE"
        menu: Menu {
            MenuItem {
                text: "退出"
                onTriggered: {
                    window.deleteWindow()
                    FluApp.closeApp()
                }
            }
        }
        onActivated:
            (reason)=>{
                if(reason === SystemTrayIcon.Trigger){
                    window.show()
                    window.raise()
                    window.requestActivate()
                }
            }
    }

    FluContentDialog{
        id:dialog_close
        title:"退出"
        message:"确定要退出程序吗？"
        negativeText:"最小化"
        buttonFlags: FluContentDialogType.NegativeButton | FluContentDialogType.NeutralButton | FluContentDialogType.PositiveButton
        onNegativeClicked:{
            window.hide()
            system_tray.showMessage("友情提示","UselessIDE已隐藏至托盘,点击托盘可再次激活窗口");
        }
        positiveText:"退出"
        neutralText:"取消"
        onPositiveClicked:{
            window.deleteWindow()
            FluApp.closeApp()
        }
    }

    FontLoader{
        id: regularFont
        source: "qrc:/UselessIDE/res/font/Fira Code Regular Nerd Font Complete Mono.ttf"
    }
    FontLoader{
        id: boldFont
        source: "qrc:/UselessIDE/res/font/Fira Code Bold Nerd Font Complete Mono.ttf"
    }

    Connections{
        target: FileManager
        function onProgramExit(){
            dialog_close.open()
        }
    }

    FluAppBar {
        property int clickCount: 0
        id:app_bar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        darkText: lang.dark_mode
        showDark: true
        darkClickListener:(button)=>handleDarkChanged(button)
        icon: "qrc:/UselessIDE/res/image/favicon.ico"
        title:"UselessIDE"
        MouseArea{
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.iconSize
            height: parent.height
            acceptedButtons: Qt.LeftButton
            onClicked: {
                parent.clickCount+=1
                showSuccess("功德+%1".arg(parent.clickCount))
            }
        }
    }

    MenuBar{
        id: menu_bar
        anchors.top: app_bar.bottom
    }

    FluArea{
        id: tool_bar_area
        height: toolBar.implicitHeight+10
        anchors.top: menu_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ToolBar{
            id: toolBar
            iconSize: 16
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    FluArea{
        id:main_bar_area
        anchors.top: tool_bar_area.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: status_bar_area.top

        MainBar{
            id: main_bar
            anchors.fill: parent
            windowHeight: window.height
        }
    }


    FluArea{
        id:status_bar_area
        height: 40
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        StatusBar{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 50
        }

    }

    Connections{
        target: FileManager
        function onShowError(msg){
            showError(msg);
        }
        function onShowInfo(msg){
            showInfo(msg);
        }
    }

    Component{
        id:com_reveal
        CircularReveal{
            id:reveal
            target:window.contentItem
            anchors.fill: parent
            onAnimationFinished:{
                //动画结束后释放资源
                loader_reveal.sourceComponent = undefined
            }
            onImageChanged: {
                changeDark()
            }
        }
    }

    Loader{
        id:loader_reveal
        anchors.fill: parent
    }

    function distance(x1,y1,x2,y2){
        return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))
    }

    function handleDarkChanged(button){
        if(FluTools.isMacos() || !FluTheme.enableAnimation){
            changeDark()
        }else{
            loader_reveal.sourceComponent = com_reveal
            var target = window.contentItem
            var pos = button.mapToItem(target,0,0)
            var mouseX = pos.x
            var mouseY = pos.y
            var radius = Math.max(distance(mouseX,mouseY,0,0),distance(mouseX,mouseY,target.width,0),distance(mouseX,mouseY,0,target.height),distance(mouseX,mouseY,target.width,target.height))
            var reveal = loader_reveal.item
            reveal.start(reveal.width*Screen.devicePixelRatio,reveal.height*Screen.devicePixelRatio,Qt.point(mouseX,mouseY),radius)
        }
        FileManager.setDark(FluTheme.dark)
    }

    function changeDark(){
        if(FluTheme.dark){
            FluTheme.darkMode = FluThemeType.Light
        }else{
            FluTheme.darkMode = FluThemeType.Dark
        }
    }

}
