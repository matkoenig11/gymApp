import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts

import GymApp

ApplicationWindow {
    id: window
    width: 520
    height: 620
    visible: true
    title: "Gym Tracker Prototype"
    Material.theme: Material.Light

    property string sqliteInspectText: ""
    property string sqliteInspectPath: ""
    // property string activeDbPath: dbPath
    // property bool activeDbSeeded: dbSeeded

    Rectangle {
        anchors.fill: parent
        color: "#335270"
    }
    // header: HeaderBar {
    //     titleText: qsTr("Gym Tracker")
    //     onImportSqlite: sqliteDialog.open()
    //     onImportJson: console.log("Import JSON action not wired yet")
    //     onExportJson: console.log("Export JSON action not wired yet")
    // }

    // ScrollView {
    //     id: scroll
    //     anchors.fill: parent
    //     anchors.margins: 12
    //     clip: true

    //     ColumnLayout {
    //         id: content
    //         width: parent.width
    //         spacing: 16

    //         ColumnLayout {
    //             id: headerLayout
    //             spacing: 4
    //             Layout.fillWidth: true
    //             Label {
    //                 text: "Gym Tracker"
    //                 font.pixelSize: 24
    //                 font.bold: true
    //                 Layout.fillWidth: true
    //             }
    //             Label {
    //                 text: "Backend wiring + machine list smoke test"
    //                 color: "#666"
    //                 wrapMode: Text.Wrap
    //                 Layout.fillWidth: true
    //             }
    //         }

    //         DatabaseCard {
    //             id: databaseCard
    //             Layout.fillWidth: true
    //             dbPath: activeDbPath
    //             dbSeeded: activeDbSeeded
    //             machinesResourceUrl: machinesResourceUrl
    //             sampleSessionResourceUrl: sampleSessionResourceUrl
    //             sqliteInspectText: sqliteInspectText
    //         }

    //         MachineListCard {
    //             id: machineListCard
    //             Layout.fillWidth: true
    //             Layout.minimumHeight: 200
    //             Layout.preferredHeight: 420
    //             model: machineListModel
    //             dbSeeded: activeDbSeeded
    //         }
    //     }
    // }

    // FileDialog {
    //     id: sqliteDialog
    //     title: "Choose SQLite database"
    //     nameFilters: ["SQLite files (*.sqlite *.db)", "All files (*)"]
    //     onAccepted: {
    //         if (!backend) {
    //             sqliteInspectText = "Backend unavailable";
    //             return;
    //         }
    //         const res = backend.switchSqliteDatabase(selectedFile, dbConnectionName)
    //         if (res && res.ok) {
    //             sqliteInspectPath = res.file || selectedFile
    //             sqliteInspectText = res.message
    //             activeDbPath = sqliteInspectPath
    //             activeDbSeeded = !!res.seeded
    //             if (machineListModel) {
    //                 machineListModel.refresh()
    //             }
    //         } else {
    //             sqliteInspectText = "Failed to read DB: " + (res && res.message ? res.message : "Unknown error")
    //         }
    //     }
    //     onRejected: {
    //         // no-op
    //     }
    // }
}
