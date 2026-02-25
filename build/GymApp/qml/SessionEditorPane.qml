import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

ColumnLayout {
    id: root
    property int selectedSessionId: -1

    onSelectedSessionIdChanged: {
        console.log("[SessionEditorPane] selectedSessionId changed from", SessionEditor.sessionId, "to", selectedSessionId)
        if (selectedSessionId >= 0 && SessionEditor.sessionId !== selectedSessionId) {
            SessionEditor.loadSession(selectedSessionId)
        }
    }

    spacing: 10
    Layout.fillWidth: true
    Layout.fillHeight: true
 
    RowLayout {
        spacing: 8
        Layout.fillWidth: true
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            Label {
                text: SessionEditor.sessionId > 0
                      ? "Session #" + SessionEditor.sessionId
                      : "New Session"
                font.pixelSize: 16
                font.bold: true
                color: "#0f172a"
                Layout.fillWidth: true
            }
            RowLayout {
                spacing: 6
                Label { text: "Started:"; color: "#334155" }
                TextField {
                    id: startedAtField
                    text: SessionEditor.sessionStarted
                    placeholderText: "YYYY-MM-DD or ISO date"
                    Layout.preferredWidth: 220
                }
                Button {
                    text: "Save Date"
                    onClicked: {
                        SessionEditor.updateSessionStarted(startedAtField.text)
                        SessionList.refresh()
                    }
                }
            }
        }
        Button {
            text: "End Session"
            enabled: SessionEditor.sessionId > 0
            onClicked: {
                SessionEditor.endSessionNow()
                SessionList.refresh()
            }
        }
        Button {
            text: "Delete"
            highlighted: true
            palette.button: "#fee2e2"
            palette.buttonText: "#7f1d1d"
            // enabled: SessionEditor.sessionId > 0
            onClicked: {
                SessionEditor.deleteSession()
                SessionList.refresh()
            }
        }
    }

    

    GroupBox {
        title: "Add Exercise"
        Layout.fillWidth: true
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 6
            spacing: 6

            RowLayout {
                spacing: 6
                ComboBox {
                    id: machineCombo
                    model: MachineList
                    textRole: "name"
                    valueRole: "id"
                    Layout.preferredWidth: 220
                }
                TextField {
                    id: customName
                    placeholderText: "Or custom name"
                    Layout.preferredWidth: 200
                }
                TextField {
                    id: exerciseComment
                    placeholderText: "Comment (optional)"
                    Layout.fillWidth: true
                }
                Button {
                    text: "Add"
                    onClicked: {
                        const machineId = machineCombo.currentValue || 0
                        SessionEditor.addExercise(machineId, customName.text, exerciseComment.text)
                        SessionList.refresh()
                        SessionDetail.loadSession(SessionEditor.sessionId)
                        exerciseComment.text = ""
                        customName.text = ""
                    }
                }
            }
        }
    }

    ListView {
        id: exerciseList
        Layout.fillWidth: true
        Layout.preferredHeight: 300
        implicitHeight: Math.max(contentHeight, 200)
        clip: true
        spacing: 8
        model: SessionEditor
        delegate: ExerciseCardEditor {
            width: ListView.view ? ListView.view.width : 0
            exerciseId: model.exerciseId
            machineName: model.machineName
            muscleGroup: model.muscleGroup
            comment: model.comment
            sets: model.sets
            listIndex: index
        }
        Component.onCompleted: {
            console.log("[SessionEditorPane] ListView completed; count =", SessionEditor.count)
        }
        // when fcount  changed

        Connections {
            target: SessionEditor
            function onCountChanged() {
                console.log("[SessionEditorPane] ListView count changed;", SessionEditor.count)
            }
        }
        footer: Label {
            visible: SessionEditor.count === 0
            text: "No exercises yet. Add your first exercise above."
            color: "#475569"
            padding: 8
        }

        onModelChanged: {
            console.log("[SessionEditorPane] model changed; count =", SessionEditor.count)
        }
        onCountChanged: {
            console.log("[SessionEditorPane] count changed;", SessionEditor.count)
        }
    }
    GroupBox {
        title: "Session Note"
        Layout.fillWidth: true
        Layout.preferredHeight: 120

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 6
            TextArea {
                id: noteArea
                text: SessionEditor.sessionNote
                placeholderText: "How you felt, focus, duration..."
                wrapMode: TextArea.Wrap
                Layout.fillWidth: true
    Layout.fillHeight: true
            }
            Connections {
                target: SessionEditor
                function onSessionChanged() {
                    noteArea.text = SessionEditor.sessionNote
                    startedAtField.text = SessionEditor.sessionStarted
                    console.log("[SessionEditorPane] SessionChanged signal; note length", noteArea.text.length,
                                "sessionId", SessionEditor.sessionId)
                }
            }
            Button {
                text: "Save Note"
                onClicked: {
                    SessionEditor.updateSessionNote(noteArea.text)
                    SessionList.refresh()
                    if (SessionEditor.sessionId > 0)
                        SessionDetail.loadSession(SessionEditor.sessionId)
                }
                Layout.alignment: Qt.AlignLeft
            }
        }
    }

    Connections {
        target: SessionEditor
        function onExercisesChanged() {
            console.log("[SessionEditorPane] exercisesChanged; count =", SessionEditor.count,
                        "sessionId =", SessionEditor.sessionId)
            try {
                console.log("[SessionEditorPane] exercises snapshot:", JSON.stringify(SessionEditor.debugExercises()))
            } catch (e) {
                console.log("[SessionEditorPane] debugExercises threw", e)
            }
            if (SessionEditor.sessionId > 0) {
                SessionList.refresh()
                SessionDetail.loadSession(SessionEditor.sessionId)
            }
        }
    }
}
