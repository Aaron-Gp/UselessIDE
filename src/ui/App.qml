import QtQuick
import QtQuick.Window
import FluentUI

Window {
    id: app
    flags: Qt.SplashScreen

    Component.onCompleted: {
        FluApp.init(app)
        FluTheme.darkMode = FluThemeType.System
        FluTheme.enableAnimation = true
        FluTheme.nativeText = false
        FluApp.routes = {
            "/": "qrc:/UselessIDE/ui/window/MainWindow.qml",
            "/about":"qrc:/UselessIDE/ui/window/AboutWindow.qml",
            "/search":"qrc:/UselessIDE/ui/window/SearcherWindow.qml",
            "/setting":"qrc:/UselessIDE/ui/window/SettingWindow.qml"
        }
        FluApp.initialRoute = "/"
        FluApp.run()
    }
}
