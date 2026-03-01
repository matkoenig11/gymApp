pragma Singleton
import QtQuick

QtObject {
    id: root

    // Theme mode
    property bool isDarkMode: false

    // Light theme colors
    readonly property QtObject light: QtObject {
        // Primary & Background
        readonly property string primary: "#00a169"
        readonly property string primaryDark: "#008652"
        readonly property string background: "#ffffff"
        readonly property string surface: "#f8fafc"
        readonly property string surfaceVariant: "#f1f5f9"

        // Text
        readonly property string textPrimary: "#0f172a"
        readonly property string textSecondary: "#334155"
        readonly property string textTertiary: "#475569"
        readonly property string textDisabled: "#94a3b8"

        // Borders & Dividers
        readonly property string border: "#e2e8f0"
        readonly property string borderLight: "#f1f5f9"
        readonly property string divider: "#e2e8f0"

        // Buttons
        readonly property string buttonBackground: "#ffffff"
        readonly property string buttonText: "#0f172a"
        readonly property string buttonHover: "#f1f5f9"
        readonly property string buttonPressed: "#e2e8f0"

        // Accents
        readonly property string accentSuccess: "#10b981"
        readonly property string accentWarning: "#f59e0b"
        readonly property string accentDanger: "#ef4444"
        readonly property string accentDangerLight: "#fee2e2"
        readonly property string accentDangerDark: "#7f1d1d"

        // Special
        readonly property string tooltip: "#1e293b"
        readonly property string shadow: "#00000015"
    }

    // Dark theme colors
    readonly property QtObject dark: QtObject {
        // Primary & Background
        readonly property string primary: "#10b981"
        readonly property string primaryDark: "#059669"
        readonly property string background: "#0f172a"
        readonly property string surface: "#1e293b"
        readonly property string surfaceVariant: "#334155"

        // Text
        readonly property string textPrimary: "#f1f5f9"
        readonly property string textSecondary: "#cbd5e1"
        readonly property string textTertiary: "#94a3b8"
        readonly property string textDisabled: "#64748b"

        // Borders & Dividers
        readonly property string border: "#334155"
        readonly property string borderLight: "#1e293b"
        readonly property string divider: "#334155"

        // Buttons
        readonly property string buttonBackground: "#1e293b"
        readonly property string buttonText: "#f1f5f9"
        readonly property string buttonHover: "#334155"
        readonly property string buttonPressed: "#475569"

        // Accents
        readonly property string accentSuccess: "#10b981"
        readonly property string accentWarning: "#fbbf24"
        readonly property string accentDanger: "#f87171"
        readonly property string accentDangerLight: "#7f1d1d"
        readonly property string accentDangerDark: "#fee2e2"

        // Special
        readonly property string tooltip: "#f1f5f9"
        readonly property string shadow: "#00000040"
    }

    // Active theme
    readonly property QtObject current: isDarkMode ? dark : light

    // Convenience accessors
    function primary() { return current.primary }
    function background() { return current.background }
    function surface() { return current.surface }
    function textPrimary() { return current.textPrimary }
    function textSecondary() { return current.textSecondary }
    function border() { return current.border }
    function buttonBackground() { return current.buttonBackground }
    function buttonText() { return current.buttonText }
}
