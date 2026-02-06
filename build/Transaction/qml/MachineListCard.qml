import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property var model: null
    property bool dbSeeded: false

    width: parent ? parent.width : implicitWidth
    radius: 6
    color: "#f8fafc"
    border.color: "#cbd5e1"
    border.width: 1

    ColumnLayout {
        id: content
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8
        Layout.fillWidth: true

        RowLayout {
            spacing: 8
            Label {
                text: "Machines"
                font.pixelSize: 16
                font.bold: true
                Layout.alignment: Qt.AlignVCenter
            }
            Rectangle {
                radius: 12
                color: "#e2e8f0"
                Layout.alignment: Qt.AlignVCenter
                Label {
                    padding: 6
                    text: model ? model.count + " items" : "0"
                    font.pixelSize: 12
                    color: "#334155"
                }
            }
            Button {
                text: "Refresh"
                onClicked: model && model.refresh()
                Layout.alignment: Qt.AlignVCenter
            }
        }

        Label {
            visible: model && model.count === 0
            text: dbSeeded
                  ? "No machines found. Try refreshing or check DB."
                  : "Run the seeder to load starter machines."
            wrapMode: Text.Wrap
            color: "#475569"
        }

        ListView {
            Layout.fillWidth: true
            // Layout.fillHeight: true
            // Layout.preferredHeight: 360
            model: model
            clip: true
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            delegate: MachineListItem {
                name: name
                muscleGroup: muscleGroup
                weightMin: weightMin
                weightMax: weightMax
                note: note
            }
            // delegate: Rectangle {
            //     // width: ListView.view.width
            //     width: parent.width
            //     height: column.implicitHeight + 12
            //     color: index % 2 === 0 ? "#ffffff" : "#f1f5f9"
            //     border.color: "#e2e8f0"
            //     border.width: 1
            //     ColumnLayout {
            //         id: column
            //         anchors.fill: parent
            //         spacing: 2
            //         Label {
            //             text: name
            //             font.bold: true
            //             font.pixelSize: 14
            //             color: "#0f172a"
            //         }
            //         Label {
            //             text: muscleGroup + " • " + weightMin + " - " + weightMax + " lbs"
            //             color: "#334155"
            //             font.pixelSize: 12
            //         }
            //         Label {
            //             visible: note.length > 0
            //             text: note
            //             font.pixelSize: 12
            //             color: "#475569"
            //             wrapMode: Text.Wrap
            //         }
            //     }
            // }
        }
    }
}
