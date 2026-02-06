import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

Item {
    id: root

    property int selectedSessionId: -1
    property int rightTabIndex: 0  // 0 = Details, 1 = Edit

    onRightTabIndexChanged: rightTabs.currentIndex = rightTabIndex

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            spacing: 8
            Layout.fillWidth: true
            Label {
                text: "Sessions"
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
                    text: SessionList ? SessionList.count + " items" : "0"
                    font.pixelSize: 12
                    color: "#334155"
                }
            }
            Button {
                text: "Refresh"
                onClicked: {
                    SessionList && SessionList.refresh()
                    if (selectedSessionId >= 0) {
                        SessionDetail.loadSession(selectedSessionId)
                    }
                }
                Layout.alignment: Qt.AlignVCenter
            }
            Button {
                text: "Start New Session"
                onClicked: {
                    SessionEditor.loadSession(-1)
                    SessionList.refresh()
                    rightTabIndex = 1
                }
            }
            Button {
                text: "Edit Selected"
                enabled: selectedSessionId >= 0
                onClicked: {
                    if (selectedSessionId >= 0) {
                        SessionEditor.loadSession(selectedSessionId)
                        rightTabIndex = 1
                    }
                }
            }
            Button {
                text: "View Selected"
                enabled: selectedSessionId >= 0
                onClicked: {
                    rightTabIndex = 0
                    if (selectedSessionId >= 0) {
                        SessionDetail.loadSession(selectedSessionId)
                    }
                }
            }
        }

        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: sessionList
                implicitWidth: 200
                Layout.fillHeight: true
                model: SessionList
                clip: true
                delegate: Rectangle {
                    width: ListView.view ? ListView.view.width : 0
                    height: 56
                    color: ListView.isCurrentItem ? "#cbd5e1" : (index % 2 === 0 ? "#ffffff" : "#f8fafc")
                    border.color: "#e2e8f0"
                    border.width: 1
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 2
                        Label {
                            text: startedAt
                            font.bold: true
                            font.pixelSize: 13
                            color: "#0f172a"
                            elide: Text.ElideRight
                        }
                        Label {
                            text: note
                            font.pixelSize: 11
                            color: "#475569"
                            elide: Text.ElideRight
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            sessionList.currentIndex = index
                            root.selectedSessionId = id
                            SessionDetail.loadSession(id)
                            rightTabIndex = 0
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                TabBar {
                    id: rightTabs
                    Layout.fillWidth: true
                    currentIndex: rightTabIndex
                    onCurrentIndexChanged: rightTabIndex = currentIndex
                    TabButton { text: "Details" }
                    TabButton { text: "Edit" }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: rightTabIndex

                    // Details view
                    ScrollView {
                        clip: true
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 8

                            Label {
                                text: SessionDetail.sessionStarted || "Select a session"
                                font.pixelSize: 16
                                font.bold: true
                                color: "#0f172a"
                                Layout.fillWidth: true
                            }
                            Label {
                                visible: SessionDetail.sessionNote.length > 0
                                text: SessionDetail.sessionNote
                                wrapMode: Text.Wrap
                                color: "#475569"
                                Layout.fillWidth: true
                            }

                            ListView {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 360
                                model: SessionDetail
                                clip: true
                                delegate: Rectangle {
                                    width: ListView.view ? ListView.view.width : 0
                                    height: column.implicitHeight + 10
                                    color: index % 2 === 0 ? "#ffffff" : "#f8fafc"
                                    border.color: "#e2e8f0"
                                    border.width: 1

                                    ColumnLayout {
                                        id: column
                                        anchors.fill: parent
                                        anchors.margins: 8
                                        spacing: 2
                                        Label {
                                            text: machineName
                                            font.bold: true
                                            font.pixelSize: 14
                                            color: "#0f172a"
                                        }
                                        Label {
                                            visible: muscleGroup.length > 0
                                            text: muscleGroup
                                            font.pixelSize: 12
                                            color: "#334155"
                                        }
                                        Label {
                                            visible: note.length > 0
                                            text: note
                                            font.pixelSize: 12
                                            color: "#475569"
                                            wrapMode: Text.Wrap
                                        }
                                        Repeater {
                                            model: sets
                                            delegate: Label {
                                                text: modelData
                                                font.pixelSize: 12
                                                color: "#0f172a"
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Editor view
                    ScrollView {
                        clip: true
                        SessionEditorPane {
                            anchors.fill: parent
                            anchors.margins: 8
                            selectedSessionId: root.selectedSessionId
                        }
                    }
                }
            }
        }
    }
}
