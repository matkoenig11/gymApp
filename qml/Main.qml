import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts

import GymApp

ApplicationWindow {
    id: window
    property int defaultFontSize: 18
    property int baseMargin: Math.max(width * 0.04, 12)
    property var theme: Theme

    width: 480
    height: 860
    visible: true
    title: "Gym Tracker Prototype"
    Material.theme: Material.Light

    header: HeaderBar {
        titleText: qsTr("Gym Tracker")
        showBack: navStack.depth > 1
        onBack: navStack.pop()
        onImportSqlite: sqliteDialog.open()
        onImportJson: console.log("Import JSON action not wired yet")
        onExportJson: console.log("Export JSON action not wired yet")
        onOpenSettings: navStack.push(settingsComponent)
    }

    StackView {
        id: navStack
        anchors.fill: parent
        anchors.margins: baseMargin
        initialItem: Component {
            ColumnLayout {
                anchors.fill: parent
                spacing: 12

                TabBar {
                    id: tabs
                    Layout.fillWidth: true
                    currentIndex: swipeView.currentIndex
                    onCurrentIndexChanged: swipeView.currentIndex = currentIndex
                    TabButton { text: "Machines"; font.pixelSize: window.defaultFontSize }
                    TabButton { text: "Sessions"; font.pixelSize: window.defaultFontSize }
                }

                SwipeView {
                    id: swipeView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: tabs.currentIndex

                    Item {
                        MachineListCard {
                            id: machineListCard
                            anchors.centerIn: parent
                            listModel: MachineList
                            loaderModel: LoaderModel {}
                        }
                    }

                    // Sessions page
                    Item {
                        SessionView {
                            anchors.fill: parent
                            anchors.margins: 12
                        }
                    }
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

    Component {
        id: settingsComponent
        SettingsView { }
    }
}
