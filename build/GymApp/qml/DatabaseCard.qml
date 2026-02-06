import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property string dbPath: ""
    property bool dbSeeded: false
    property string machinesResourceUrl: ""
    property string sampleSessionResourceUrl: ""
    property string sqliteInspectText: ""

    implicitHeight: content.implicitHeight + 24
    width: parent ? parent.width : implicitWidth
    radius: 6
    color: "#0f172a"
    border.color: "#1e293b"
    border.width: 1

    ColumnLayout {
        id: content
        spacing: 6
        anchors.fill: parent
        anchors.margins: 12

        Label {
            text: "Database"
            font.pixelSize: 16
            font.bold: true
            color: "#e2e8f0"
            Layout.fillWidth: true
        }
        Label {
            text: "Path: " + (dbPath || "unknown")
            wrapMode: Text.Wrap
            color: "#cbd5e1"
            Layout.fillWidth: true
        }
        Label {
            text: dbSeeded ? "Seed data present" : "Seed data missing — run sql/seed_sqlite.py"
            color: dbSeeded ? "#22c55e" : "#f97316"
            wrapMode: Text.Wrap
            Layout.fillWidth: true
        }
        Label {
            text: "Machines data: " + (machinesResourceUrl || "unavailable")
            wrapMode: Text.Wrap
            color: "#cbd5e1"
            Layout.fillWidth: true
        }
        Label {
            text: "Sample session: " + (sampleSessionResourceUrl || "unavailable")
            wrapMode: Text.Wrap
            color: "#cbd5e1"
            Layout.fillWidth: true
        }
        Label {
            text: sqliteInspectText.length > 0 ? sqliteInspectText : "Select a SQLite file to inspect via menu."
            wrapMode: Text.Wrap
            color: "#cbd5e1"
            Layout.fillWidth: true
        }
    }
}
