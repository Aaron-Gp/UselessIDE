import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE
import "qrc:/UselessIDE/ui/component"

CustomWindow {
    id:window
    title:"Settings"
    width: 600
    height: 400
    fixSize: false
    launchMode: FluWindowType.SingleTask
    FluScrollablePage{
        anchors.fill: parent
        FluArea{
            Layout.fillWidth: true
            Layout.topMargin: 20
            height: 128
            paddings: 10

            ColumnLayout{
                spacing: 5
                anchors{
                    top: parent.top
                    left: parent.left
                }
                FluText{
                    text:lang.dark_mode
                    font: FluTextStyle.BodyStrong
                    Layout.bottomMargin: 4
                }
                Repeater{
                    model: [{title:lang.system,mode:FluThemeType.System},{title:lang.light,mode:FluThemeType.Light},{title:lang.dark,mode:FluThemeType.Dark}]
                    delegate:  FluRadioButton{
                        checked : FluTheme.darkMode === modelData.mode
                        text:modelData.title
                        clickListener:function(){
                            FluTheme.darkMode = modelData.mode
                        }
                    }
                }
            }
        }

        FluArea{
            Layout.fillWidth: true
            Layout.topMargin: 20
            height: 80
            paddings: 10

            ColumnLayout{
                spacing: 10
                anchors{
                    top: parent.top
                    left: parent.left
                }

                FluText{
                    text:lang.locale
                    font: FluTextStyle.BodyStrong
                    Layout.bottomMargin: 4
                }

                Flow{
                    spacing: 5
                    Repeater{
                        model: ["Zh","En"]
                        delegate: FluRadioButton{
                            checked: appInfo.lang.objectName === modelData
                            text:modelData
                            clickListener:function(){
                                appInfo.changeLang(modelData)
                            }
                        }
                    }
                }
            }
        }
    }
}
