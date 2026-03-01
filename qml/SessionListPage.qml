import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

ColumnLayout {
    spacing: 8
    Layout.fillWidth: true

    ListHeader {
        title: "Sessions"
        itemCount: SessionList ? SessionList.count : 0
        onRefresh: SessionList && SessionList.refresh()
    }

    ListView {
        Layout.fillWidth: true
        Layout.preferredHeight: 320
        model: SessionList
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
                anchors.margins: 6
                spacing: 2
                Label {
                    text: startedAt
                    font.bold: true
                    font.pixelSize: 14
                    color: "#0f172a"
                }
                Label {
                    visible: note.length > 0
                    text: note
                    font.pixelSize: 12
                    color: "#475569"
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}
