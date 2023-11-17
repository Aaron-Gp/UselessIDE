import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import FluentUI 1.0
import "qrc:///example/qml/component"
import "../component"

FluScrollablePage{

    title:"Captcha"


    FluCaptcha{
        id:captcha
        Layout.topMargin: 20
        MouseArea{
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                captcha.refresh()
            }
        }
    }

    FluButton{
        text:"Refresh"
        Layout.topMargin: 20
        onClicked: {
            captcha.refresh()
        }
    }

    RowLayout{
        spacing: 10
        Layout.topMargin: 10
        FluTextBox{
            id:text_box
            placeholderText: "请输入验证码"
        }
        FluButton{
            text:"verify"
            onClicked: {
                var success =  captcha.verify(text_box.text)
                if(success){
                    showSuccess("验证码正确")
                }else{
                    showError("错误验证，请重新输入")
                }
            }
        }
    }


}
