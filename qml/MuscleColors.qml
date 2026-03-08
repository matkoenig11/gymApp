pragma Singleton
import QtQuick

QtObject {
    id: root
    // Default fallback when a code is missing or malformed
    readonly property color defaultColor: "#94a3b8"

    // Map of shade arrays keyed by letter
    readonly property var shadeMap: ({
        A: ["#fee2e2", "#fecdd3", "#fca5a5", "#f87171", "#ef4444", "#dc2626"], // warm reds/oranges
        B: ["#e0f2fe", "#bfdbfe", "#93c5fd", "#60a5fa", "#3b82f6", "#2563eb"], // blues
        C: ["#dcfce7", "#bbf7d0", "#86efac", "#4ade80", "#22c55e", "#16a34a"], // greens
        D: ["#ede9fe", "#ddd6fe", "#c4b5fd", "#a78bfa", "#8b5cf6", "#7c3aed"], // purples
        E: ["#fef3c7", "#fde68a", "#fcd34d", "#fbbf24", "#f59e0b", "#d97706"], // ambers
        F: ["#f8fafc", "#e2e8f0", "#cbd5e1", "#94a3b8", "#64748b", "#475569"]  // neutrals
    })

    // Explicit list of all codes as QtObjects (useful for palette UI)
    readonly property list<QtObject> palette: [
        // A group
        QtObject { property string code: "A1"; property string group: "A"; property int level: 1; property color value: "#fee2e2" },
        QtObject { property string code: "A2"; property string group: "A"; property int level: 2; property color value: "#fecdd3" },
        QtObject { property string code: "A3"; property string group: "A"; property int level: 3; property color value: "#fca5a5" },
        QtObject { property string code: "A4"; property string group: "A"; property int level: 4; property color value: "#f87171" },
        QtObject { property string code: "A5"; property string group: "A"; property int level: 5; property color value: "#ef4444" },
        QtObject { property string code: "A6"; property string group: "A"; property int level: 6; property color value: "#dc2626" },

        // B group
        QtObject { property string code: "B1"; property string group: "B"; property int level: 1; property color value: "#e0f2fe" },
        QtObject { property string code: "B2"; property string group: "B"; property int level: 2; property color value: "#bfdbfe" },
        QtObject { property string code: "B3"; property string group: "B"; property int level: 3; property color value: "#93c5fd" },
        QtObject { property string code: "B4"; property string group: "B"; property int level: 4; property color value: "#60a5fa" },
        QtObject { property string code: "B5"; property string group: "B"; property int level: 5; property color value: "#3b82f6" },
        QtObject { property string code: "B6"; property string group: "B"; property int level: 6; property color value: "#2563eb" },

        // C group
        QtObject { property string code: "C1"; property string group: "C"; property int level: 1; property color value: "#dcfce7" },
        QtObject { property string code: "C2"; property string group: "C"; property int level: 2; property color value: "#bbf7d0" },
        QtObject { property string code: "C3"; property string group: "C"; property int level: 3; property color value: "#86efac" },
        QtObject { property string code: "C4"; property string group: "C"; property int level: 4; property color value: "#4ade80" },
        QtObject { property string code: "C5"; property string group: "C"; property int level: 5; property color value: "#22c55e" },
        QtObject { property string code: "C6"; property string group: "C"; property int level: 6; property color value: "#16a34a" },

        // D group
        QtObject { property string code: "D1"; property string group: "D"; property int level: 1; property color value: "#ede9fe" },
        QtObject { property string code: "D2"; property string group: "D"; property int level: 2; property color value: "#ddd6fe" },
        QtObject { property string code: "D3"; property string group: "D"; property int level: 3; property color value: "#c4b5fd" },
        QtObject { property string code: "D4"; property string group: "D"; property int level: 4; property color value: "#a78bfa" },
        QtObject { property string code: "D5"; property string group: "D"; property int level: 5; property color value: "#8b5cf6" },
        QtObject { property string code: "D6"; property string group: "D"; property int level: 6; property color value: "#7c3aed" },

        // E group
        QtObject { property string code: "E1"; property string group: "E"; property int level: 1; property color value: "#fef3c7" },
        QtObject { property string code: "E2"; property string group: "E"; property int level: 2; property color value: "#fde68a" },
        QtObject { property string code: "E3"; property string group: "E"; property int level: 3; property color value: "#fcd34d" },
        QtObject { property string code: "E4"; property string group: "E"; property int level: 4; property color value: "#fbbf24" },
        QtObject { property string code: "E5"; property string group: "E"; property int level: 5; property color value: "#f59e0b" },
        QtObject { property string code: "E6"; property string group: "E"; property int level: 6; property color value: "#d97706" },

        // F group
        QtObject { property string code: "F1"; property string group: "F"; property int level: 1; property color value: "#f8fafc" },
        QtObject { property string code: "F2"; property string group: "F"; property int level: 2; property color value: "#e2e8f0" },
        QtObject { property string code: "F3"; property string group: "F"; property int level: 3; property color value: "#cbd5e1" },
        QtObject { property string code: "F4"; property string group: "F"; property int level: 4; property color value: "#94a3b8" },
        QtObject { property string code: "F5"; property string group: "F"; property int level: 5; property color value: "#64748b" },
        QtObject { property string code: "F6"; property string group: "F"; property int level: 6; property color value: "#475569" }
    ]

    function colorFor(code) {
        const norm = (code || "").toString().trim().toUpperCase()
        if (norm.length < 2)
            return defaultColor

        const letter = norm.charAt(0)
        const num = parseInt(norm.slice(1))
        const shadesFor = shadeMap[letter]

        if (!shadesFor || isNaN(num))
            return defaultColor

        const idx = Math.min(Math.max(num, 1), shadesFor.length) - 1
        return shadesFor[idx]
    }
}
