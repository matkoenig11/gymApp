import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Reusable action button group - for primary and danger actions
 * Replaces repeated button patterns in SetRowEditor, ExerciseCardEditor, etc.
 *
 * Usage:
 * ActionButtonGroup {
 *     primaryButtonText: "Update"
 *     dangerButtonText: "Remove"
 *     onPrimaryClicked: SessionEditor.updateSet(...)
 *     onDangerClicked: SessionEditor.removeSet(...)
 * }
 */
RowLayout {
    property string primaryButtonText: "Save"
    property string dangerButtonText: ""
    property bool showDanger: true
    property bool showPrimary: true
    property var theme: Theme

    signal primaryClicked()
    signal dangerClicked()

    spacing: 6

    Button {
        visible: showPrimary
        text: primaryButtonText
        onClicked: primaryClicked()
    }

    Button {
        visible: showDanger && dangerButtonText.length > 0
        text: dangerButtonText
        highlighted: true
        palette.button: theme ? theme.current.accentDangerLight : "#fca5a5"
        palette.buttonText: theme ? theme.current.accentDangerDark : "#7f1d1d"
        onClicked: dangerClicked()
    }
}
