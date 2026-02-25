import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

Rectangle {
    id: root
    property var listModel: MachineList
    property var loaderModel: null
    // width: parent ? parent.width : implicitWidth
    radius: 6
    color: "#f8fafc"
    border.color: "#cbd5e1"
    border.width: 1

    implicitHeight: content.implicitHeight + 24
    implicitWidth: content.implicitWidth + 24

    ColumnLayout {
        id: content
        anchors.centerIn: parent
        anchors.margins: 12
        spacing: 8

       
        RowLayout {
            spacing: 8
            Label {
                text: "Machines"
                font.pixelSize: 16
                font.bold: true
                Layout.alignment: Qt.AlignVCenter
            }
            Label {
                padding: 6
                text: listModel ? listModel.count + " items" : "0"
                font.pixelSize: 12
                color: "#334155"
            }
        }

        RowLayout {
            Button {
                text: "Refresh"
                onClicked: listModel && listModel.refresh()
                Layout.alignment: Qt.AlignVCenter
            }
            Button {
                text: "Add"
                onClicked: {
                    editorTitle.text = "Add Machine"
                    machineForm.idValue = -1
                    nameField.text = ""
                    muscleField.text = ""
                    minField.value = 0
                    maxField.value = 500
                    noteField.text = ""
                    activeBox.checked = true
                    editorDialog.open()
                }
            }
        }
        // Label {
        //     visible: model && model.count === 0
        //     text: dbSeeded
        //           ? "No machines found. Try refreshing or check DB."
        //           : "Run the seeder to load starter machines."
        //     wrapMode: Text.Wrap
        //     color: "#475569"
        // }

        ListView {
            id: listView
            Layout.fillWidth: true
            implicitHeight:400 
            implicitWidth: 300

            model: listModel
            clip: true
            delegate: MachineListItem {
                width: ListView.view ? ListView.view.width : 0
                machineId: model.id
                name: model.name
                muscleGroup: model.muscleGroup
                weightMin: model.weightMin
                weightMax: model.weightMax
                note: model.note
                onClicked: {
                    editorTitle.text = "Edit Machine"
                    machineForm.idValue = model.id
                    nameField.text = model.name
                    muscleField.text = model.muscleGroup
                    minField.value = model.weightMin
                    maxField.value = model.weightMax
                    noteField.text = model.note
                    activeBox.checked = true
                    editorDialog.open()
                }
            }

            footer: Item {
                width: ListView.view ? ListView.view.width : 0
                height: listModel && listModel.count === 0 ? 64 : 0
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8
                    visible: listModel && listModel.count === 0
                    Label {
                        text: "No machines loaded."
                        color: "#475569"
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Button {
                        text: "Load seeds"
                        enabled: loaderModel !== null
                        onClicked: {
                            if (loaderModel) {
                                const res = loaderModel.loadMachinesFromJson("gymapp", Backend.machinesResourceUrl())
                                console.log("Seed load:", JSON.stringify(res))
                                if (listModel) listModel.refresh()
                            }
                        }
                        Layout.alignment: Qt.AlignVCenter
                    }
                }
            }
        }
    }

    Dialog {
        id: editorDialog
        modal: true
        standardButtons: Dialog.Save | Dialog.Cancel
        title: ""
        anchors.centerIn: Overlay.overlay
        onAccepted: {
            if (machineForm.idValue < 0) {
                listModel.addMachine(
                            nameField.text,
                            muscleField.text,
                            minField.value,
                            maxField.value,
                            noteField.text)
            } else {
                listModel.updateMachine(
                            machineForm.idValue,
                            nameField.text,
                            muscleField.text,
                            minField.value,
                            maxField.value,
                            noteField.text,
                            activeBox.checked)
            }
        }

        footer: RowLayout {
            spacing: 8
            visible: machineForm.idValue >= 0
            Button {
                text: "Delete"
                palette.button: "#fee2e2"
                palette.buttonText: "#7f1d1d"
                onClicked: {
                    listModel.deleteMachine(machineForm.idValue)
                    editorDialog.close()
                }
            }
        }

        ColumnLayout {
            id: machineForm
            property int idValue: -1
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            Label { id: editorTitle; text: "Edit Machine"; font.bold: true; font.pixelSize: 16 }
            TextField { id: nameField; placeholderText: "Name"; Layout.fillWidth: true }
            TextField { id: muscleField; placeholderText: "Muscle group"; Layout.fillWidth: true }
            RowLayout {
                spacing: 6
                SpinBox { id: minField; from: 0; to: 2000; value: 0; editable: true; Layout.preferredWidth: 150 }
                Label { text: "to"; }
                SpinBox { id: maxField; from: 0; to: 2000; value: 500; editable: true; Layout.preferredWidth: 150 }
                Label { text: "lbs" }
            }
            TextArea { id: noteField; placeholderText: "Note"; Layout.fillWidth: true; Layout.preferredHeight: 80 }
            CheckBox { id: activeBox; text: "Active"; checked: true }
            Button {
                text: "Save"
                onClicked: editorDialog.accept()
                Layout.alignment: Qt.AlignRight
            }
        }
    }
}
