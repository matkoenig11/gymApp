import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Reusable row for entering set data (reps + weight).
 * Used in SetRowEditor, ExerciseCardEditor add-set section, etc.
 * 
 * Usage:
 * SetInputRow {
 *     id: inputs
 *     reps: 8
 *     weight: 100
 * }
 * Then access inputs.repsField.value, inputs.weightField.value
 */
RowLayout {
    property alias reps: repsField.value
    property alias weight: weightField.value
    property alias repsField: repsField
    property alias weightField: weightField
    property var theme: Theme

    spacing: 6
    Layout.fillWidth: true

    SpinBox {
        id: repsField
        from: 0
        to: 200
        value: 8
        editable: true
        Layout.preferredWidth: 130
        up.indicator.implicitWidth: 15
        up.indicator.implicitHeight: 15
        down.indicator.implicitWidth: 15
        down.indicator.implicitHeight: 15
    }

    Label {
        text: "reps"
        color: (theme && theme.current) ? theme.current.textTertiary : "#475569"
        Layout.alignment: Qt.AlignVCenter
    }

    SpinBox {
        id: weightField
        from: 0
        to: 1000
        value: 100
        stepSize: 5
        editable: true
        Layout.preferredWidth: 140
        up.indicator.implicitWidth: 20
        up.indicator.implicitHeight: 15
        down.indicator.implicitWidth: 20
        down.indicator.implicitHeight: 15
    }

    Label {
        text: "lbs"
        color: (theme && theme.current) ? theme.current.textTertiary : "#475569"
        Layout.alignment: Qt.AlignVCenter
    }
}
