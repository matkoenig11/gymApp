import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

Rectangle {
    id: root
    property var setData
    property int exerciseId: setData ? setData.exerciseId : -1
    implicitHeight: rootLayout.implicitHeight + 12
    radius: 6
    color: "#ffffff"
    border.color: "#e2e8f0"
    border.width: 1

    RowLayout {
        id: rootLayout
        anchors.fill: parent
        spacing: 6

        Label {
            text: "#" + (setData ? setData.setNumber : 0)
            font.bold: true
            color: "#0f172a"
            Layout.alignment: Qt.AlignVCenter
        }

        ColumnLayout {
            spacing: 2

            SetInputRow {
                id: inputRow
                reps: setData ? setData.reps : 0
                weight: setData ? setData.weight : 0
            }

            ActionButtonGroup {
                primaryButtonText: "Update"
                dangerButtonText: "Remove Set"
                onPrimaryClicked: {
                    if (!setData) return
                    SessionEditor.updateSet(setData.id, inputRow.reps, inputRow.weight)
                }
                onDangerClicked: {
                    if (!setData) return
                    SessionEditor.removeSet(setData.id)
                }
            }
        }
    }
}
