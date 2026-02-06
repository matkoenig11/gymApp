import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

Rectangle {
    id: root
    property var listModel: MachineList
    property var loaderModel: null
    width: parent ? parent.width : implicitWidth
    radius: 6
    color: "#f8fafc"
    border.color: "#cbd5e1"
    border.width: 1

    implicitHeight: content.implicitHeight + 24
    implicitWidth: content.implicitWidth + 24

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
                    text: listModel ? listModel.count + " items" : "0"
                    font.pixelSize: 12
                    color: "#334155"
                }
            }
            Button {
                text: "Refresh"
                onClicked: listModel && listModel.refresh()
                Layout.alignment: Qt.AlignVCenter
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
            implicitHeight: 300

            model: listModel
            clip: true
            delegate: MachineListItem {
                width: ListView.view ? ListView.view.width : 0
                name: model.name
                muscleGroup: model.muscleGroup
                weightMin: model.weightMin
                weightMax: model.weightMax
                note: note
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
}
