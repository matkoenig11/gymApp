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
    property var effortRir: null
    property var sets: []
    property int listIndex: -1
    property bool expanded: true

    radius: 8
    color: "#ffffff"
    border.color: "#e2e8f0"
    border.width: 1
    
    implicitHeight: expanded ? content.implicitHeight + 20
                              : headerRow.implicitHeight + collapsedRow.implicitHeight + 20

    ColumnLayout {
        id: content
        anchors.fill: parent
        anchors.margins: 10
        spacing: 6

            RowLayout {
                id: headerRow
                Layout.fillWidth: true
                spacing: 6

                RowLayout {
                    id: headerClickable
                    Layout.fillWidth: true
                    spacing: 6
                    Label {
                        text: expanded ? "[-]" : "[+]"
                        font.pixelSize: 14
                        color: hovercollapser.hovered ? "#0f172a" : "#2a3758"
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: card.expanded = !card.expanded
                            hoverEnabled: true
                        }
                        HoverHandler {
                            id: hovercollapser
                        }
                    }
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
                    Label {
                        visible: !expanded
                        text: comment.length > 0 ? comment : "Sets: " + sets.length
                        color: "#475569"
                        elide: Text.ElideRight
                        Layout.fillWidth: false
                    }
                }
            // Button {
            //     text: "Move Up"
            //     enabled: listIndex > 0
            //     onClicked: SessionEditor.moveExercise(exerciseId, listIndex - 1)
            //     visible: expanded
            // }
            // Button {
            //     text: "Move Down"
            //     enabled: listIndex >= 0
            //     onClicked: SessionEditor.moveExercise(exerciseId, listIndex + 1)
            //     visible: expanded
            // }
            Button {
                text: "Remove Exercise"
                highlighted: true
                palette.button: "#fee2e2"
                palette.buttonText: "#7f1d1d"
                onClicked: SessionEditor.removeExercise(exerciseId)
                visible: expanded
            }
        }

        ColumnLayout {
            id: collapsedRow
            visible: false
            spacing: 0
        }

        ColumnLayout {
            id: expandedContent
            Layout.fillWidth: true
            spacing: 6
            visible: expanded

            RowLayout {
                spacing: 8
                Layout.fillWidth: true
                Label { text: "RIR"; color: "#334155"; font.bold: true }
                SpinBox {
                    id: rirField
                    from: 0
                    to: 5
                    stepSize: 1
                    value: effortRir === null ? 2 : effortRir
                    editable: true
                    Layout.preferredWidth: 120
                }
                Label { text: "0 = all out, 5 = very easy"; color: "#64748b"; font.pixelSize: 11; Layout.fillWidth: true }
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
                onClicked: SessionEditor.updateExercise(exerciseId, commentArea.text, 0, "", rirField.value)
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#e2e8f0"
                Layout.fillWidth: true
            }

            ColumnLayout {
                id: addSetColumn
                spacing: 6
                Layout.fillWidth: true

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
                        Layout.fillWidth: true
                        Layout.preferredHeight: implicitHeight
                    }
                }

                RowLayout {
                    id: inputRow
                    spacing: 6
                    Layout.fillWidth: true

                    SpinBox { id: repsInput; from: 1; to: 200; value: 8; editable: true; Layout.preferredWidth: 130 }
                    Label { text: "reps"; color: "#475569" }
                    SpinBox { id: weightInput; from: 0; to: 1000; value: 100; stepSize: 5; editable: true; Layout.preferredWidth: 140 }
                    Label { text: "lbs"; color: "#475569" }
                }

                RowLayout {
                    spacing: 6
                    Layout.fillWidth: true

                    Button {
                        text: "Add Set"
                        onClicked: {
                            SessionEditor.addSet(exerciseId, repsInput.value, weightInput.value)
                        }
                    }
                }
            }
        }
    }
}
