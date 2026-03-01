import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Reusable list header with title, count badge, and action buttons.
 * Replaces the repeated pattern in SessionListPage, SessionView, MachineListCard.
 * 
 * Usage:
 * ListHeader {
 *     title: "Sessions"
 *     itemCount: SessionList.count
 *     onRefresh: SessionList.refresh()
 * }
 */
RowLayout {
    property string title: "Items"
    property int itemCount: 0
    property bool showRefresh: true
    property var additionalButtons: []  // For custom buttons

    signal refresh()
    signal buttonClicked(string buttonId)

    spacing: 8
    Layout.fillWidth: true

    Label {
        text: title
        font.pixelSize: 16
        font.bold: true
        Layout.alignment: Qt.AlignVCenter
    }

    Rectangle {
        radius: 12
        color: "#e2e8f0"
        Layout.alignment: Qt.AlignVCenter
        implicitHeight: 28
        implicitWidth: countLabel.implicitWidth + 12

        Label {
            id: countLabel
            anchors.centerIn: parent
            text: itemCount + " items"
            font.pixelSize: 12
            color: "#334155"
        }
    }

    Button {
        visible: showRefresh
        text: "Refresh"
        onClicked: refresh()
        Layout.alignment: Qt.AlignVCenter
    }

    // Placeholder for additional buttons
    Item { Layout.fillWidth: true }

    Repeater {
        model: additionalButtons
        delegate: Button {
            text: modelData.text
            onClicked: buttonClicked(modelData.id)
            Layout.alignment: Qt.AlignVCenter
        }
    }
}
