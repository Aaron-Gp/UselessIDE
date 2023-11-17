import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import UselessIDE

RowLayout{
    id:menu_bar
    property string lineNumber: ""
    property string colNumber: ""
    property string linesNumber: ""
    spacing: 20


    FluText{
        id: text_line
        text: lang.line+": " + menu_bar.lineNumber
        Layout.alignment: Qt.AlignRight
        Layout.maximumWidth: 120
        Layout.preferredWidth: 120
    }


    FluText{
        id: text_col
        text: lang.col+": " + menu_bar.colNumber
        Layout.alignment: Qt.AlignRight
        Layout.maximumWidth: 120
        Layout.preferredWidth: 120
    }

    FluText{
        id: text_lines
        text: lang.lines+": " + menu_bar.linesNumber
        Layout.alignment: Qt.AlignRight
        Layout.maximumWidth: 120
        Layout.preferredWidth: 120
    }

    Connections{
        target: FileManager
        function onCursorPositionChange(row, col, lines){
            menu_bar.lineNumber = row;
            menu_bar.colNumber = col;
            menu_bar.linesNumber = lines;
        }
    }

}

