import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Reusable labeled spinbox - commonly used in forms for reps, weight, RIR, etc.
 * Usage: LabeledSpinBox { label: "Reps"; from: 0; to: 200; value: 8; onValueChanged: ... }
 */
RowLayout {
    property alias label: labelText.text
    property alias from: spinBox.from
    property alias to: spinBox.to
    property alias value: spinBox.value
    property alias stepSize: spinBox.stepSize
    property alias editable: spinBox.editable
    property string labelColor: "#475569"
    property string unit: ""

    spacing: 6

    SpinBox {
        id: spinBox
        editable: true
        Layout.preferredWidth: 130
    }

    Label {
        id: labelText
        color: labelColor
        Layout.alignment: Qt.AlignVCenter
        font.pixelSize: 12
    }

    Label {
        visible: unit.length > 0
        text: unit
        color: labelColor
        Layout.alignment: Qt.AlignVCenter
        font.pixelSize: 12
    }
}
