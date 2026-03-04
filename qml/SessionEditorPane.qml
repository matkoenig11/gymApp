import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

ColumnLayout {
    id: root
    property int selectedSessionId: -1
    property int lastAddedExerciseId: -1
    property real savedScrollY: 0

    function captureScroll() {
        savedScrollY = exerciseList.contentY
    }

    onSelectedSessionIdChanged: {
        console.log("[SessionEditorPane] selectedSessionId changed from", SessionEditor.sessionId, "to", selectedSessionId)
        if (selectedSessionId >= 0 && SessionEditor.sessionId !== selectedSessionId) {
            SessionEditor.loadSession(selectedSessionId)
        }
    }

    spacing: 10
    Layout.fillWidth: true
    Layout.fillHeight: true
 
    Flow {
        id: headerFlow
        width: parent ? parent.width : implicitWidth
        spacing: 8
        flow: Flow.LeftToRight

        ColumnLayout {
            id: titleColumn
            width: Math.min(headerFlow.width, 320)
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
                    Layout.preferredWidth: 180
                    Layout.alignment: Qt.AlignVCenter
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

        ActionButtonGroup {
            primaryButtonText: "End Session"
            dangerButtonText: "Delete"
            showPrimary: SessionEditor.sessionId > 0
            showDanger: SessionEditor.sessionId > 0
            onPrimaryClicked: {
                SessionEditor.endSessionNow()
                SessionList.refresh()
            }
            onDangerClicked: {
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
            id: addExerciseFlow
            Layout.fillWidth: true

            ComboBox {
                id: machineCombo
                model: MachineList
                textRole: "name"
                valueRole: "id"
                Layout.fillWidth: true
            }

            Button {
                text: "Add"
                onClicked: {
                    root.captureScroll()
                    const machineId = machineCombo.currentValue || 0
                    const newId = SessionEditor.addExercise(machineId, "", "", 0)
                    if (newId > 0) {
                        lastAddedExerciseId = newId
                    }
                    SessionList.refresh()
                    SessionDetail.loadSession(SessionEditor.sessionId)
                    exerciseComment.text = ""
                    customName.text = ""
                    addRirControl.value = 2
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
            effortRir: model.effortRir
            sets: model.sets
            listIndex: index
            startExpanded: exerciseId === root.lastAddedExerciseId
            captureScroll: root.captureScroll
        }
        Component.onCompleted: {
            console.log("[SessionEditorPane] ListView completed; count =", SessionEditor.count)
        }

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
            Qt.callLater(function() {
                exerciseList.contentY = root.savedScrollY
            })
        }
    }
}
