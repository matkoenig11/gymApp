import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

CollapsibleCard {
    id: card
    property int exerciseId: -1
    property string machineName: ""
    property string muscleGroup: ""
    property string comment: ""
    property var effortRir: null
    property var sets: []
    property int listIndex: -1
    
    title: machineName.length > 0 ? machineName : "Exercise"
    subtitle: muscleGroup
    collapsedPreview: comment.length > 0 ? comment : "Sets: " + sets.length
    showRemove: true
    theme: Theme

    onRemove: SessionEditor.removeExercise(exerciseId)

    ColumnLayout {
        spacing: 8
        Layout.fillWidth: true

        RowLayout {
            spacing: 8
            Layout.fillWidth: true
            Label { text: "RIR"; color: (theme && theme.current) ? theme.current.textSecondary : "#475569"; font.bold: true }
            SpinBox {
                id: rirField
                from: 0
                to: 5
                stepSize: 1
                value: effortRir === null ? 2 : effortRir
                editable: true
                Layout.preferredWidth: 120
            }
            Label { text: "0 = all out, 5 = very easy"; color: (theme && theme.current) ? theme.current.textTertiary : "#94a3b8"; font.pixelSize: 11; Layout.fillWidth: true }
        }

        // TextArea {
        //     id: commentArea
        //     text: comment
        //     placeholderText: "Exercise notes (form cues, tempo, etc.)"
        //     wrapMode: TextArea.Wrap
        //     Layout.fillWidth: true
        //     Layout.preferredHeight: 70
        // }

        // Button {
        //     text: "Save Notes"
        //     Layout.alignment: Qt.AlignLeft
        //     onClicked: SessionEditor.updateExercise(exerciseId, commentArea.text, 0, "", rirField.value)
        // }

        // Rectangle {
        //     width: parent.width
        //     height: 1
        //     color: (theme && theme.current) ? theme.current.border : "#e2e8f0"
        //     Layout.fillWidth: true
        // }

        ColumnLayout {
            id: addSetColumn
            spacing: 6
            Layout.fillWidth: true

            Label { 
                text: "Sets"
                font.pixelSize: 13
                font.bold: true
                color: (theme && theme.current) ? theme.current.textPrimary : "#0f172a"
            }

            Repeater {
                model: sets
                delegate: SetRowEditor {
                    setData: modelData
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight
                }
            }

            SetInputRow {
                id: inputRow
                reps: 8
                weight: 100
            }

            Button {
                text: "Add Set"
                Layout.alignment: Qt.AlignLeft
                onClicked: {
                    SessionEditor.addSet(exerciseId, inputRow.reps, inputRow.weight)
                }
            }
        }
    }
}
