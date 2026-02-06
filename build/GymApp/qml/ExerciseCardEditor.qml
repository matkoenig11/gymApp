import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

Rectangle {
    id: card
    property int exerciseId: -1
    property string machineName: ""
    property string muscleGroup: ""
    property string comment: ""
    property var sets: []
    property int listIndex: -1

    radius: 8
    color: "#ffffff"
    border.color: "#e2e8f0"
    border.width: 1
    Layout.fillWidth: true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 6

        RowLayout {
            Layout.fillWidth: true
            spacing: 6
            Label {
                text: machineName.length > 0 ? machineName : "Exercise"
                font.pixelSize: 15
                font.bold: true
                color: "#0f172a"
                Layout.fillWidth: true
            }
            Label {
                visible: muscleGroup.length > 0
                text: muscleGroup
                color: "#334155"
                font.pixelSize: 12
            }
            Button {
                text: "Move Up"
                enabled: listIndex > 0
                onClicked: SessionEditor.moveExercise(exerciseId, listIndex - 1)
            }
            Button {
                text: "Move Down"
                enabled: listIndex >= 0
                onClicked: SessionEditor.moveExercise(exerciseId, listIndex + 1)
            }
            Button {
                text: "Remove Exercise"
                highlighted: true
                palette.button: "#fee2e2"
                palette.buttonText: "#7f1d1d"
                onClicked: SessionEditor.removeExercise(exerciseId)
            }
        }

        TextArea {
            id: commentArea
            text: comment
            placeholderText: "Exercise notes (form cues, tempo, etc.)"
            wrapMode: TextArea.Wrap
            Layout.fillWidth: true
            Layout.preferredHeight: 70
        }

        Button {
            text: "Save Notes"
            Layout.alignment: Qt.AlignLeft
            onClicked: SessionEditor.updateExercise(exerciseId, commentArea.text, 0, "")
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#e2e8f0"
            Layout.fillWidth: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Label {
                text: "Sets"
                font.pixelSize: 13
                font.bold: true
                color: "#0f172a"
            }

            Repeater {
                model: sets
                delegate: SetRowEditor {
                    setData: modelData
                }
            }

            RowLayout {
                id: addSetRow
                spacing: 6
                Layout.fillWidth: true

                Label { text: "Add set"; font.bold: true; color: "#0f172a" }
                SpinBox { id: repsInput; from: 1; to: 200; value: 8; editable: true; Layout.preferredWidth: 70 }
                Label { text: "reps"; color: "#475569" }
                SpinBox { id: weightInput; from: 0; to: 1000; value: 100; stepSize: 5; editable: true; Layout.preferredWidth: 90 }
                Label { text: "lbs"; color: "#475569" }
                SpinBox { id: rpeInput; from: 0; to: 10; stepSize: 1; value: 8; editable: true; Layout.preferredWidth: 70 }
                Label { text: "RPE"; color: "#475569" }
                CheckBox { id: warmupInput; text: "Warmup" }
                Button {
                    text: "Add Set"
                    onClicked: {
                        SessionEditor.addSet(exerciseId, repsInput.value, weightInput.value, rpeInput.value, warmupInput.checked)
                    }
                }
            }
        }
    }
}
