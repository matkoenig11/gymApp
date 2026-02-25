#include "Clogger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QMutexLocker>
#include <iostream>

namespace gym {

namespace {
constexpr const char *kDefaultFileName = "clogger.log";
}

QString Clogger::timestamp() {
    return QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
}

QString Clogger::colorCode(ELogColor color) {
    switch (color) {
    case ELogColor::Gray:
        return "\033[90m";
    case ELogColor::Red:
        return "\033[31m";
    case ELogColor::Green:
        return "\033[32m";
    case ELogColor::Yellow:
        return "\033[33m";
    case ELogColor::Blue:
        return "\033[34m";
    case ELogColor::Magenta:
        return "\033[35m";
    case ELogColor::Cyan:
        return "\033[36m";
    case ELogColor::White:
        return "\033[97m";
    default:
        return "";
    }
}

QString Clogger::colorName(ELogColor color) {
    switch (color) {
    case ELogColor::Gray:
        return "gray";
    case ELogColor::Red:
        return "red";
    case ELogColor::Green:
        return "green";
    case ELogColor::Yellow:
        return "yellow";
    case ELogColor::Blue:
        return "blue";
    case ELogColor::Magenta:
        return "magenta";
    case ELogColor::Cyan:
        return "cyan";
    case ELogColor::White:
        return "white";
    default:
        return "default";
    }
}

void Clogger::ensureFile() {
    if (initialized_) {
        return;
    }
    QMutexLocker locker(&mutex_);
    if (initialized_) {
        return;
    }

    QString path = logPath_;
    if (path.isEmpty()) {
        const QString dir = QCoreApplication::applicationDirPath();
        path = dir + QDir::separator() + kDefaultFileName;
    }
    logFile_.setFileName(path);
    logFile_.open(QIODevice::Append | QIODevice::Text);
    initialized_ = true;
}

void Clogger::setLogFile(const QString &path) {
    QMutexLocker locker(&mutex_);
    logPath_ = path;
    initialized_ = false;
    if (logFile_.isOpen()) {
        logFile_.close();
    }
}

void Clogger::debug(const QString &message, ELogColor color) {
    Clogger::debug(message, color, __FILE__, __LINE__, __func__);
}

void Clogger::debug(const QString &message,
                    ELogColor color,
                    const char *file,
                    int line,
                    const char *function) {
    ensureFile();

    const QString ts = timestamp();
    const QString location = QStringLiteral("%1:%2 (%3)").arg(file).arg(line).arg(function);
    const QString colorLabel = colorName(color);
    const QString plainLine = QStringLiteral("[%1][DEBUG][%2] %3 | %4\n")
                                  .arg(ts, colorLabel, location, message);

    {
        QMutexLocker locker(&mutex_);
        if (logFile_.isOpen()) {
            QTextStream out(&logFile_);
            out << plainLine;
            logFile_.flush();
        }
    }

    const QString ansiColor = colorCode(color);
    const QString reset = ansiColor.isEmpty() ? QString() : QStringLiteral("\033[0m");
    std::cout << ansiColor.toStdString() << plainLine.toStdString() << reset.toStdString();
}

}  // namespace gym
