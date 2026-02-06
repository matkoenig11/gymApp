import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ToolBar {
    id: root
    property alias titleText: titleLabel.text

    signal importSqlite()
    signal importJson()
    signal exportJson()

    Material.primary: "#000000ff"
    Material.background: "transparent"
    Material.foreground: "#ffffff"
    background: Rectangle { color: "#00a169ff"; opacity: 1 }

    RowLayout {
        anchors.fill: parent
        spacing: 8
        ToolButton {
            text: "\u2630"
            onClicked: mainMenu.open()
        }
        Label {
            id: titleLabel
            text: qsTr("Gym Tracker")
            font.pixelSize: 18
            Layout.fillWidth: true
        }
    }

    Menu {
        id: mainMenu
        Action {
            text: qsTr("Import SQLite")
            onTriggered: root.importSqlite()
        }
        Action {
            text: qsTr("Import JSON")
            onTriggered: root.importJson()
        }
        Action {
            text: qsTr("Export JSON")
            onTriggered: root.exportJson()
        }
    }
}
