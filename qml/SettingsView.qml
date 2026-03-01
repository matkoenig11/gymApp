import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

ColumnLayout {
    id: root
    anchors.fill: parent
    spacing: 16
    anchors.margins: 10
    property var theme: Theme

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 8

        Label {
            text: "Settings"
            font.pixelSize: 20
            font.bold: true
            color: theme.current.textPrimary
        }

        Rectangle {
            width: parent.width
            height: 1
            color: theme.current.border
            Layout.fillWidth: true
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 12

        Label {
            text: "Theme"
            font.pixelSize: 14
            font.bold: true
            color: theme.current.textSecondary
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label {
                text: "Dark Mode:"
                color: theme.current.textTertiary
                Layout.alignment: Qt.AlignVCenter
            }

            Switch {
                checked: theme.isDarkMode
                onToggled: theme.isDarkMode = checked
                Layout.alignment: Qt.AlignVCenter
            }
        }
    }

    Rectangle {
        width: parent.width
        height: 1
        color: theme.current.border
        Layout.fillWidth: true
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 12

        Label {
            text: "Database"
            font.pixelSize: 14
            font.bold: true
            color: theme.current.textSecondary
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: theme.current.surface
            border.color: theme.current.border
            border.width: 1
            radius: 6

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 4

                Label {
                    text: "SQLite Database Path:"
                    font.pixelSize: 11
                    font.bold: true
                    color: theme.current.textSecondary
                }

                TextEdit {
                    id: dbPathText
                    text: Backend.currentDatabasePath()
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    font.pixelSize: 10
                    color: theme.current.textPrimary
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    selectByMouse: true
                }
            }
        }
    }

    Item {
        Layout.fillHeight: true
    }
}
