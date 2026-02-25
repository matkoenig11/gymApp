import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


Item {
    id: root
    implicitWidth: 200
    implicitHeight: column.implicitHeight + 12

    signal clicked(int id)
    property int machineId: -1
    property string name: "undefined"
    property string muscleGroup
    property int weightMin
    property int weightMax
    property string note
    Rectangle {
        anchors.fill: parent
        color: "#f1f5f9"
        border.color: "#e2e8f0"
        border.width: 1
        MouseArea {
            anchors.fill: parent
            onClicked: root.clicked(root.machineId)
            hoverEnabled: true
        }

        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.margins: 6
            spacing: 2

            Label {
                text: root.name
                font.bold: true
                font.pixelSize: 14
                color: "#0f172a"
            }
            Label {
                text: root.muscleGroup + " • " + root.weightMin + " - " + root.weightMax + " lbs"
                color: "#334155"
                font.pixelSize: 12
            }
            Label {
                text: root.note
                font.pixelSize: 12
                color: "#475569"
                wrapMode: Text.Wrap
            }
        }
    }
}
