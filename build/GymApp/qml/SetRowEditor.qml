import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp

RowLayout {
    id: root
    property var setData
    property int exerciseId: setData ? setData.exerciseId : -1

    spacing: 6
    Layout.fillWidth: true

    Label {
        text: "#" + (setData ? setData.setNumber : 0)
        font.bold: true
        color: "#0f172a"
        Layout.alignment: Qt.AlignVCenter
    }

    SpinBox {
        id: repsField
        from: 0
        to: 200
        value: setData ? setData.reps : 0
        editable: true
        Layout.preferredWidth: 70
    }
    Label { text: "reps"; color: "#475569"; Layout.alignment: Qt.AlignVCenter }

    SpinBox {
        id: weightField
        from: 0
        to: 1000
        value: setData ? setData.weight : 0
        editable: true
        stepSize: 5
        Layout.preferredWidth: 90
    }
    Label { text: "lbs"; color: "#475569"; Layout.alignment: Qt.AlignVCenter }

    SpinBox {
        id: rpeField
        from: 0
        to: 10
        stepSize: 1
        value: setData ? Math.round(setData.rpe) : 0
        editable: true
        Layout.preferredWidth: 70
    }
    Label { text: "RPE"; color: "#475569"; Layout.alignment: Qt.AlignVCenter }

    CheckBox {
        id: warmupBox
        text: "Warmup"
        checked: setData ? setData.warmup : false
    }

    Button {
        text: "Update"
        onClicked: {
            if (!setData) return
            SessionEditor.updateSet(setData.id, repsField.value, weightField.value, rpeField.value, warmupBox.checked)
        }
    }
    Button {
        text: "Remove Set"
        highlighted: true
        palette.button: "#fca5a5"
        palette.buttonText: "#7f1d1d"
        onClicked: {
            if (!setData) return
            SessionEditor.removeSet(setData.id)
        }
    }
}
