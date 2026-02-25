import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GymApp


Rectangle {
    
    id: root
    property var setData
    property int exerciseId: setData ? setData.exerciseId : -1
    implicitHeight: rootLayout.implicitHeight + 12
    radius: 6
    color: "#ffffff"
    border.color: "#e2e8f0"
    border.width: 1

    RowLayout {
        id: rootLayout
        anchors.fill: parent
        spacing: 6
        Label {
                text: "#" + (setData ? setData.setNumber : 0)
                font.bold: true
                color: "#0f172a"
                Layout.alignment: Qt.AlignVCenter
            }

        ColumnLayout {
            spacing: 2

            RowLayout {
                
                spacing: 6
                SpinBox {
                    id: repsField
                    from: 0
                    to: 200
                    value: setData ? setData.reps : 0
                    editable: true
                    Layout.preferredWidth: 130
                    // contentItem.font.pixelSize: 13
                    up.indicator.implicitWidth:15
                    up.indicator.implicitHeight: 15
                    down.indicator.implicitWidth: 15
                    down.indicator.implicitHeight: 15
                }
                Label { text: "reps"; color: "#475569"; Layout.alignment: Qt.AlignVCenter }

                SpinBox {
                    id: weightField
                    from: 0
                    to: 1000
                    value: setData ? setData.weight : 0
                    editable: true
                    stepSize: 5
                    Layout.preferredWidth: 140
                    // contentItem.font.pixelSize: 13
                    up.indicator.implicitWidth: 20
                    up.indicator.implicitHeight: 15
                    down.indicator.implicitWidth: 20
                    down.indicator.implicitHeight: 15
                }
                Label { text: "lbs"; color: "#475569"; Layout.alignment: Qt.AlignVCenter }
            }

            RowLayout {
                spacing: 6

                Button {
                    text: "Update"
                    onClicked: {
                        if (!setData) return
                        SessionEditor.updateSet(setData.id, repsField.value, weightField.value)
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
        }
    }
}
