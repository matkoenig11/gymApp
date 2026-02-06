#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QUrl>
#include <QVariantMap>

#include <QtQml/qqmlregistration.h>

namespace gym {

struct DatabaseConfig {
    QString driverName;     // e.g., QSQLITE, QPSQL
    QString databaseName;   // file path for SQLite; DB name for Postgres
    QString hostName;
    int port = 0;
    QString userName;
    QString password;
    QString connectOptions;
};

// Thin placeholder for wiring backend services; exposes resource URLs for seed data and DB bootstrap.
class Backend : public QObject {
    Q_OBJECT

    QML_SINGLETON
    QML_NAMED_ELEMENT(Backend)

public:
    explicit Backend(QObject *parent = nullptr);

    Q_INVOKABLE QUrl machinesResourceUrl() const;
    Q_INVOKABLE QUrl sampleSessionResourceUrl() const;

    Q_INVOKABLE DatabaseConfig defaultSqliteConfig() const;
    Q_INVOKABLE QString defaultSqlitePath() const;
    Q_INVOKABLE QString defaultConnectionName() const { return QStringLiteral("gymapp"); }
    QSqlDatabase openDatabase(const QString &connectionName,
                              const DatabaseConfig &config) const;

    // Ensures the baseline schema exists; creates tables if missing.
    bool ensureSchema(QSqlDatabase &db) const;
    // Light check to help the UI warn when seeds are missing.
    bool hasMinimumSeedData(QSqlDatabase &db) const;
    Q_INVOKABLE bool hasSeeds(const QString &connectionName = QStringLiteral("gymapp")) const;
    Q_INVOKABLE QString currentDatabasePath(const QString &connectionName = QStringLiteral("gymapp")) const;

    // Inspect an arbitrary SQLite file and return table counts; intended for QML use.
    Q_INVOKABLE QVariantMap inspectSqliteFile(const QString &filePath) const;
    // Switch the named connection to a new SQLite file, ensuring schema; returns status and seed info.
    Q_INVOKABLE QVariantMap switchSqliteDatabase(const QString &filePath,
                                                 const QString &connectionName = QStringLiteral("gymapp"));
};

}  // namespace gym
