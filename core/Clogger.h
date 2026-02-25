#pragma once

#include <QFile>
#include <QMutex>
#include <QString>

namespace gym {

enum class ELogColor {
    Default,
    Gray,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

class Clogger {
public:
    static void setLogFile(const QString &path);

    static void debug(const QString &message, ELogColor color = ELogColor::Default);
    static void debug(const QString &message,
                      ELogColor color,
                      const char *file,
                      int line,
                      const char *function);

private:
    static QString timestamp();
    static QString colorCode(ELogColor color);
    static QString colorName(ELogColor color);
    static void ensureFile();

    static inline QFile logFile_;
    static inline QString logPath_;
    static inline QMutex mutex_;
    static inline bool initialized_ = false;
};

}  // namespace gym
