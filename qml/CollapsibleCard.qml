import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Reusable collapsible card - used in ExerciseCardEditor for expand/collapse behavior.
 * Encapsulates the expand/collapse toggle pattern with proper styling.
 *
 * Usage:
 * CollapsibleCard {
 *     title: "Exercise Name"
 *     subtitle: "Muscle Group"
 *     collapsedPreview: "3 sets"
 *     onRemove: SessionEditor.removeExercise(...)
 *     showRemove: true
 *
 *     expandedContent: ColumnLayout {
 *         // content here
 *     }
 * }
 */
Rectangle {
    id: card
    property string title: "Card"
    property string subtitle: ""
    property string collapsedPreview: ""
    property bool showRemove: false
    property var theme: Theme
    property alias expandedContent: expandedLayout.children
    property bool expanded: true

    signal remove()

    radius: 8
    color: (theme && theme.current) ? theme.current.background : "#ffffff"
    border.color: (theme && theme.current) ? theme.current.border : "#e2e8f0"
    border.width: 1

    implicitHeight: expanded ? content.implicitHeight + 20
                              : headerRow.implicitHeight + 20

    ColumnLayout {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        spacing: 6
        Layout.fillWidth: true

        RowLayout {
            id: headerRow
            Layout.fillWidth: true
            spacing: 6
            Layout.preferredHeight: headerRow.implicitHeight

            Label {
                text: expanded ? "[-]" : "[+]"
                font.pixelSize: 14
                color: hoverHandler.hovered 
                       ? (theme && theme.current ? theme.current.textPrimary : "#0f172a")
                       : (theme && theme.current ? theme.current.textSecondary : "#475569")

                MouseArea {
                    anchors.fill: parent
                    onClicked: card.expanded = !card.expanded
                    hoverEnabled: true
                }

                HoverHandler {
                    id: hoverHandler
                }
            }

            Label {
                text: title
                font.pixelSize: 15
                font.bold: true
                color: (theme && theme.current) ? theme.current.textPrimary : "#0f172a"
                Layout.fillWidth: true
            }

            Label {
                visible: subtitle.length > 0
                text: subtitle
                color: (theme && theme.current) ? theme.current.textSecondary : "#475569"
                font.pixelSize: 12
            }

            Label {
                visible: !expanded && collapsedPreview.length > 0
                text: collapsedPreview
                color: (theme && theme.current) ? theme.current.textTertiary : "#94a3b8"
                elide: Text.ElideRight
                Layout.fillWidth: false
            }

            Button {
                visible: showRemove && expanded
                text: "Remove"
                highlighted: true
                palette.button: (theme && theme.current) ? theme.current.accentDangerLight : "#fee2e2"
                palette.buttonText: (theme && theme.current) ? theme.current.accentDangerDark : "#7f1d1d"
                onClicked: card.remove()
            }
        }

        ColumnLayout {
            id: expandedLayout
            Layout.fillWidth: true
            spacing: 6
            visible: expanded
            Component.onCompleted: { var _ = expandedLayout.implicitHeight }
            Layout.preferredHeight: expandedLayout.implicitHeight
        }
    }
}
