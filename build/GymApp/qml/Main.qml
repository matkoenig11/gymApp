import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts

import GymApp

ApplicationWindow {
    id: window
    width: 1400
    height: 620
    visible: true
    title: "Gym Tracker Prototype"
    Material.theme: Material.Light

    header: HeaderBar {
        titleText: qsTr("Gym Tracker")
        onImportSqlite: sqliteDialog.open()
        onImportJson: console.log("Import JSON action not wired yet")
        onExportJson: console.log("Export JSON action not wired yet")
    }

    ScrollView {
        id: scroll
        anchors.fill: parent
        // anchors.margins: 12
            // clip: true
        Rectangle {
            anchors.fill: parent
            color: "#30527e"
        }
        ColumnLayout {
            id: content
            anchors.fill: parent
            // spacing: 12

            TabBar {
                id: tabs
                Layout.fillWidth: true
                currentIndex: swipeView.currentIndex
                onCurrentIndexChanged: swipeView.currentIndex = currentIndex
                TabButton { text: "Machines" }
                TabButton { text: "Sessions" }
            }

            SwipeView {
                id: swipeView
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: tabs.currentIndex
                
                ColumnLayout {
                    // anchors.fill: parent
                    spacing: 8
                    width: content.width

                    DatabaseCard {
                        implicitWidth: content.width
                        dbPath: Backend.currentDatabasePath("gymapp")
                        dbSeeded: Backend.hasSeeds("gymapp")
                        machinesResourceUrl: Backend.machinesResourceUrl()
                        sampleSessionResourceUrl: Backend.sampleSessionResourceUrl()
                        sqliteInspectText: ""
                    }

                    MachineListCard {
                        id: machineListCard
                        implicitWidth: content.width
                        // Layout.fillWidth: true
                        // Layout.minimumHeight: 200
                        Layout.preferredHeight: 420
                        listModel: MachineList
                        loaderModel: LoaderModel {}
                    }
                }

                SessionView { 
                    // Layout.fillWidth: true
                    // Layout.fillHeight: true
                }
            }
        }
    }

    FileDialog {
        id: sqliteDialog
        title: "Choose SQLite database"
        nameFilters: ["SQLite files (*.sqlite *.db)", "All files (*)"]
        onAccepted: {
            const res = Backend.switchSqliteDatabase(selectedFile, Backend.defaultConnectionName())
            if (res && res.ok) {
                MachineList.refresh()
                SessionList.refresh()
                SessionDetail.loadSession(-1)
            }
        }
    }
}
