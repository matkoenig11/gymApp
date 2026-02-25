
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

Item {
    id: root

    property int selectedSessionId: -1
    property int rightTabIndex: 0  // 0 = Details, 1 = Edit
    property bool listOpen: false
    property int contentMargin: 12

    function ensureEditorSessionLoaded() {
        if (selectedSessionId >= 0 && SessionEditor.sessionId !== selectedSessionId) {
            SessionEditor.loadSession(selectedSessionId)
        }
    }

    onRightTabIndexChanged: {
        rightTabs.currentIndex = rightTabIndex
        if (rightTabIndex === 1) {
            ensureEditorSessionLoaded()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: contentMargin
        spacing: 12

        Flow {
            Layout.fillWidth: true
            spacing: 8
            Label {
                text: "Sessions"
                font.pixelSize: 14
                font.bold: true
            }
            Label {
                padding: 6
                text: SessionList ? SessionList.count + " items" : "0"
                font.pixelSize: 10
                color: "#334155"
            }
            Button {
                text: "Refresh"
                onClicked: {
                    SessionList && SessionList.refresh()
                    if (selectedSessionId >= 0) {
                        SessionDetail.loadSession(selectedSessionId)
                    }
                }
            }
            Button {
                text: listOpen ? "Hide List" : "Show List"
                onClicked: listOpen = !listOpen
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

        // Details / Editor stacked below

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: rightTabIndex

            // Details view
            ScrollView {
                clip: true
                contentWidth: parent ? parent.width : undefined
                ColumnLayout {
                    width: parent ? parent.width : undefined
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
                        boundsBehavior: Flickable.StopAtBounds
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
                contentWidth: parent ? parent.width : undefined
                SessionEditorPane {
                    width: parent ? parent.width : undefined
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 8
                    selectedSessionId: root.selectedSessionId
                }
            }
        }        
    }

    // Floating action button
    RoundButton {
        id: fab
        text: "+"
        font.pixelSize: 26
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 16
        width: 56
        height: 56
        highlighted: true
        onClicked: {
            SessionEditor.loadSession(-1)
            root.selectedSessionId = -1
            SessionList.refresh()
            rightTabIndex = 1
        }
    }

    // Slide-in session list
    Rectangle {
        id: sessionListContainer
        color: "#f8fafc"
        border.color: "#e2e8f0"
        width: listOpen ? Math.min(parent.width * 0.7, 300) : 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        z: 10
        clip: true
        Behavior on width { NumberAnimation { duration: 200; easing.type: Easing.InOutQuad } }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 6

            RowLayout {
                Layout.fillWidth: true
                Label {
                    text: "Sessions"
                    font.bold: true
                    color: "#0f172a"
                }
                Item { Layout.fillWidth: true }
                Button {
                    text: "Close"
                    onClicked: listOpen = false
                }
            }

            ListView {
                id: sessionList
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: SessionList
                boundsBehavior: Flickable.StopAtBounds
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
                            listOpen = false
                        }
                    }
                }
            }
        }
    }

    // Dim overlay to close drawer when tapping outside
    MouseArea {
        anchors.fill: parent
        visible: listOpen
        enabled: listOpen
        z: 9
        onClicked: listOpen = false
        hoverEnabled: false
        propagateComposedEvents: false
    }
}
