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
    title:"Find"
    width: 600
    height: 400
    fixSize: false
    launchMode: FluWindowType.SingleTask
    property bool hasInit: false

    onArgumentChanged:
        ()=>{
            if(hasInit){
                if(argument.tab==="search"){
                    tab_view.setTabIndex(0)
                }else{
                    tab_view.setTabIndex(1)
                }
            }
        }

    onInitArgument:
        (argument)=>{
            if(argument.tab==="search"){
                tab_view.setTabIndex(0)
            }else{
                tab_view.setTabIndex(1)
            }
            hasInit=true
        }

    SearchTabView{
        id: tab_view
        closeButtonVisibility: FluTabViewType.Nerver
        tabWidthBehavior: FluTabViewType.SizeToContent
        addButtonVisibility: false
        anchors.fill: parent
        Component.onCompleted: {
            newTab("Search", search)
            newTab("Replace", replace)
        }

        Component{
            id:search
            FluArea{
                anchors.fill: parent
                RowLayout{
                    anchors.fill: parent
                    ColumnLayout{
                        Layout.topMargin: 10
                        Layout.rightMargin: 10
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        RowLayout{
                            FluText{
                                text: "Text to Find:"
                            }
                            FluTextBox{
                                id: strToFind
                                Layout.fillWidth: true
                            }
                        }
                        ColumnLayout{
                            Layout.topMargin: 10
                            FluText{
                                text: "Options:"
                            }
                            FluArea{
                                topPadding: 10
                                FluCheckBox{
                                    id: case_sensitive
                                    text: "Case Sensitive"
                                }
                            }
                        }
                    }
                    ColumnLayout{
                        Layout.topMargin: 10
                        Layout.rightMargin: 10
                        spacing: 10
                        width: 200
                        Layout.alignment: Qt.AlignTrailing | Qt.AlignTop
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Find Previous"
                            onClicked: {
                                FileManager.searchBackward(strToFind.text, case_sensitive.checked);
                            }
                        }
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Find Next"
                            onClicked: {
                                FileManager.searchForward(strToFind.text, case_sensitive.checked);
                            }
                        }
                        FluDivider{
                            Layout.alignment: Qt.AlignHCenter
                            height: 20
                        }
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Close"
                            onClicked: {
                                window.close()
                            }
                        }
                    }
                }
            }
        }

        Component{
            id:replace
            FluArea{
                anchors.fill: parent
                RowLayout{
                    anchors.fill: parent

                    ColumnLayout{
                        Layout.topMargin: 10
                        Layout.rightMargin: 10
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        RowLayout{
                            FluText{
                                text: "Text to Find:"
                            }

                            FluTextBox{
                                id: oldstr
                                Layout.fillWidth: true
                            }
                        }

                        RowLayout{
                            FluText{
                                text: "Replace with:"
                            }

                            FluTextBox{
                                id: newstr
                                Layout.fillWidth: true
                            }
                        }

                        ColumnLayout{
                            Layout.topMargin: 10
                            FluText{
                                text: "Options:"
                            }
                            FluArea{
                                topPadding: 10
                                FluCheckBox{
                                    id: case_sensitive2
                                    text: "Case Sensitive"
                                }
                            }
                        }
                    }
                    ColumnLayout{
                        Layout.topMargin: 10
                        Layout.rightMargin: 10
                        spacing: 10
                        width: 200
                        Layout.alignment: Qt.AlignTrailing | Qt.AlignTop
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Find Previous"
                            onClicked: {
                                FileManager.searchBackward(oldstr.text, case_sensitive2.checked);
                            }
                        }
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Find Next"
                            onClicked: {
                                FileManager.searchForward(oldstr.text, case_sensitive2.checked);
                            }
                        }
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Replace"
                            onClicked: {
                                FileManager.replace(oldstr.text, newstr.text, case_sensitive2.checked);
                            }
                        }
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Replace All"
                            onClicked: {
                                FileManager.replaceAll(oldstr.text, newstr.text, case_sensitive2.checked);
                            }
                        }
                        FluDivider{
                            Layout.alignment: Qt.AlignHCenter
                            height: 20
                        }
                        FluButton{
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            text: "Close"
                            onClicked: {
                                window.close()
                            }
                        }
                    }
                }
            }
        }

        function newTab(title, page){
            tab_view.appendTab("",title, page)
        }

    }

}
