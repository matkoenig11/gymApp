#include "Backend.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrl>
#include <QVariant>
#include <QVariantMap>

namespace {

QString baselineSchemaSql() {
    return QStringLiteral(R"SQL(
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS schema_migrations (
    id INTEGER PRIMARY KEY,
    version TEXT NOT NULL UNIQUE,
    applied_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS machines (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    muscle_group TEXT,
    weight_min INTEGER NOT NULL DEFAULT 0,
    weight_max INTEGER NOT NULL DEFAULT 500,
    note TEXT,
    is_active INTEGER NOT NULL DEFAULT 1,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    updated_at TEXT,
    CHECK (weight_min <= weight_max)
);

CREATE TABLE IF NOT EXISTS sessions (
    id INTEGER PRIMARY KEY,
    started_at TEXT NOT NULL DEFAULT (datetime('now')),
    ended_at TEXT,
    note TEXT
);

CREATE TABLE IF NOT EXISTS exercises (
    id INTEGER PRIMARY KEY,
    session_id INTEGER NOT NULL,
    machine_id INTEGER,
    machine_name_snapshot TEXT,
    effort_rir INTEGER,
    order_index INTEGER NOT NULL DEFAULT 0,
    comment TEXT,
    FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE,
    FOREIGN KEY (machine_id) REFERENCES machines(id) ON DELETE SET NULL
);

CREATE INDEX IF NOT EXISTS idx_exercises_session ON exercises(session_id);

CREATE TABLE IF NOT EXISTS exercise_sets (
    id INTEGER PRIMARY KEY,
    exercise_id INTEGER NOT NULL,
    set_number INTEGER NOT NULL,
    reps INTEGER NOT NULL,
    weight_lbs REAL,
    FOREIGN KEY (exercise_id) REFERENCES exercises(id) ON DELETE CASCADE,
    CHECK (set_number > 0),
    CHECK (reps >= 0),
    UNIQUE (exercise_id, set_number)
);

CREATE INDEX IF NOT EXISTS idx_sets_exercise ON exercise_sets(exercise_id);

INSERT OR IGNORE INTO schema_migrations (version) VALUES ('001_init');
)SQL");
}

QStringList splitStatements(const QString &sql) {
    QStringList filteredLines;
    const auto lines = sql.split('\n');
    for (const QString &line : lines) {
        const QString trimmed = line.trimmed();
        if (trimmed.startsWith("--") || trimmed.startsWith("//")) {
            continue;
        }
        filteredLines.append(line);
    }
    QString cleaned = filteredLines.join('\n');
    QStringList statements;
    const auto rawStatements = cleaned.split(';', Qt::SkipEmptyParts);
    for (QString statement : rawStatements) {
        const QString trimmed = statement.trimmed();
        if (!trimmed.isEmpty()) {
            statements.append(trimmed);
        }
    }
    return statements;
}

}  // namespace

namespace gym {

Backend::Backend(QObject *parent) : QObject(parent) {}

QUrl Backend::machinesResourceUrl() const {
    return QUrl(QStringLiteral("qrc:/data/machines.json"));
}

QUrl Backend::sampleSessionResourceUrl() const {
    return QUrl(QStringLiteral("qrc:/data/gym_21_1_2026.json"));
}

DatabaseConfig Backend::defaultSqliteConfig() const {
    DatabaseConfig config;
    config.driverName = QStringLiteral("QSQLITE");
    config.databaseName = defaultSqlitePath();
    return config;
}

QString Backend::defaultSqlitePath() const {
    const QString envPath = qEnvironmentVariable("GYMAPP_DB_PATH");
    if (!envPath.isEmpty()) {
        return envPath;
    }

    const QString bundledDb =
        QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("gym.sqlite"));
    if (QFileInfo::exists(bundledDb)) {
        return bundledDb;
    }

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QCoreApplication::applicationDirPath();
    }

    QDir dir(dataDir);
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }

    return dir.filePath(QStringLiteral("gym.sqlite"));
}

QSqlDatabase Backend::openDatabase(const QString &connectionName,
                                   const DatabaseConfig &config) const {
    const QString driver = config.driverName.isEmpty() ? QStringLiteral("QSQLITE") : config.driverName;
    QSqlDatabase db = QSqlDatabase::contains(connectionName)
                          ? QSqlDatabase::database(connectionName)
                          : QSqlDatabase::addDatabase(driver, connectionName);

    if (!config.databaseName.isEmpty()) {
        db.setDatabaseName(config.databaseName);
    }
    if (!config.hostName.isEmpty()) {
        db.setHostName(config.hostName);
    }
    if (config.port > 0) {
        db.setPort(config.port);
    }
    if (!config.userName.isEmpty()) {
        db.setUserName(config.userName);
    }
    if (!config.password.isEmpty()) {
        db.setPassword(config.password);
    }
    if (!config.connectOptions.isEmpty()) {
        db.setConnectOptions(config.connectOptions);
    }

    if (!db.isOpen() && !db.open()) {
        qWarning() << "Failed to open DB connection"
                   << connectionName << "(" << driver << ")"
                   << db.lastError().text();
    } else {
        QSqlQuery pragma(db);
        pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));
        qDebug() << "DB connection ready" << connectionName << driver << db.databaseName();
    }

    return db;
}

bool Backend::ensureSchema(QSqlDatabase &db) const {
    if (!db.isOpen()) {
        qWarning() << "ensureSchema: database not open";
        return false;
    }

    auto tableExists = [&](const QString &name) {
        QSqlQuery check(db);
        check.prepare(QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' AND name=?"));
        check.addBindValue(name);
        return check.exec() && check.next();
    };

    // Create baseline schema if missing core tables.
    if (!tableExists(QStringLiteral("machines"))) {
        const QStringList statements = splitStatements(baselineSchemaSql());
        for (const QString &statement : statements) {
            QSqlQuery query(db);
            if (!query.exec(statement)) {
                qWarning() << "Schema statement failed" << statement.left(80)
                           << query.lastError().text();
                return false;
            }
        }
        qDebug() << "Baseline schema created in database" << db.connectionName();
    }

    // Lightweight migrations for existing DBs ---------------------------------
    auto columnMissing = [&](const QString &table, const QString &column) {
        QSqlQuery q(db);
        if (!q.exec(QStringLiteral("PRAGMA table_info(%1)").arg(table))) {
            return false;
        }
        while (q.next()) {
            if (q.value(1).toString().compare(column, Qt::CaseInsensitive) == 0) {
                return false;
            }
        }
        return true;
    };

    // Add effort_rir to exercises if absent (0–5, nullable)
    if (columnMissing(QStringLiteral("exercises"), QStringLiteral("effort_rir"))) {
        QSqlQuery alter(db);
        if (!alter.exec(QStringLiteral("ALTER TABLE exercises ADD COLUMN effort_rir INTEGER"))) {
            qWarning() << "Failed to add effort_rir column:" << alter.lastError().text();
            return false;
        }
        qDebug() << "Added effort_rir column to exercises";
    }

    return true;
}

bool Backend::hasMinimumSeedData(QSqlDatabase &db) const {
    if (!db.isOpen()) {
        return false;
    }

    bool hasIsActive = false;
    {
        QSqlQuery info(db);
        if (info.exec(QStringLiteral("PRAGMA table_info(machines)"))) {
            while (info.next()) {
                if (info.value(1).toString() == QStringLiteral("is_active")) {
                    hasIsActive = true;
                    break;
                }
            }
        }
    }

    const QString sql = hasIsActive
                            ? QStringLiteral("SELECT COUNT(*) FROM machines WHERE is_active = 1")
                            : QStringLiteral("SELECT COUNT(*) FROM machines");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qWarning() << "Failed to count machines:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

QVariantMap Backend::inspectSqliteFile(const QString &filePath) const {
    QVariantMap result;

    QUrl url = QUrl::fromUserInput(filePath);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : filePath;
    qDebug() << "[inspectSqliteFile] requested path:" << filePath << "resolved to" << localPath;
    QFileInfo info(localPath);
    if (!info.exists() || !info.isFile()) {
        qWarning() << "[inspectSqliteFile] file missing:" << localPath;
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("message"), QStringLiteral("File not found"));
        return result;
    }

    DatabaseConfig cfg = defaultSqliteConfig();
    cfg.databaseName = localPath;
    cfg.connectOptions = QStringLiteral("QSQLITE_OPEN_READONLY=1");

    const QString connName =
        QStringLiteral("inspect_%1").arg(QDateTime::currentMSecsSinceEpoch());
    qDebug() << "[inspectSqliteFile] opening database read-only with connection" << connName;
    QSqlDatabase db = openDatabase(connName, cfg);
    if (!db.isOpen()) {
        qWarning() << "[inspectSqliteFile] open failed for" << localPath;
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("message"), QStringLiteral("Unable to open database"));
        return result;
    }

    auto fetchCount = [&](const QString &table) -> int {
        QSqlQuery query(db);
        const QString sql = QStringLiteral("SELECT COUNT(*) FROM %1").arg(table);
        if (!query.exec(sql)) {
            qWarning() << "[inspectSqliteFile] count query failed" << table << query.lastError().text();
            return -1;
        }
        if (query.next()) {
            return query.value(0).toInt();
        }
        return -1;
    };

    const int machines = fetchCount(QStringLiteral("machines"));
    const int sessions = fetchCount(QStringLiteral("sessions"));
    const int exercises = fetchCount(QStringLiteral("exercises"));
    const int sets = fetchCount(QStringLiteral("exercise_sets"));

    qDebug() << "[inspectSqliteFile] counts" << info.absoluteFilePath()
             << "machines" << machines << "sessions" << sessions
             << "exercises" << exercises << "sets" << sets;

    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);

    result.insert(QStringLiteral("ok"), true);
    result.insert(QStringLiteral("file"), info.absoluteFilePath());
    result.insert(QStringLiteral("machines"), machines);
    result.insert(QStringLiteral("sessions"), sessions);
    result.insert(QStringLiteral("exercises"), exercises);
    result.insert(QStringLiteral("exercise_sets"), sets);
    result.insert(QStringLiteral("message"),
                  QStringLiteral("machines=%1, sessions=%2, exercises=%3, sets=%4")
                      .arg(machines)
                      .arg(sessions)
                      .arg(exercises)
                      .arg(sets));
    return result;
}

bool Backend::hasSeeds(const QString &connectionName) const {
    if (!QSqlDatabase::contains(connectionName)) {
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    return hasMinimumSeedData(db);
}

QString Backend::currentDatabasePath(const QString &connectionName) const {
    if (!QSqlDatabase::contains(connectionName)) {
        return {};
    }
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    return db.databaseName();
}

QVariantMap Backend::switchSqliteDatabase(const QString &filePath, const QString &connectionName) {
    QVariantMap result;

    const QString localPath = QUrl::fromUserInput(filePath).isLocalFile()
                                  ? QUrl::fromUserInput(filePath).toLocalFile()
                                  : filePath;
    qDebug() << "[switchSqliteDatabase] request" << localPath << "connection" << connectionName;

    QFileInfo info(localPath);
    if (!info.exists() || !info.isFile()) {
        qWarning() << "[switchSqliteDatabase] file missing" << localPath;
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("message"), QStringLiteral("File not found"));
        return result;
    }

    QSqlDatabase db = QSqlDatabase::contains(connectionName)
                          ? QSqlDatabase::database(connectionName)
                          : QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);

    if (db.isOpen()) {
        db.close();
    }
    db.setDatabaseName(info.absoluteFilePath());
    db.setConnectOptions(QString());

    if (!db.open()) {
        qWarning() << "[switchSqliteDatabase] open failed" << db.lastError().text();
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("message"), QStringLiteral("Unable to open database"));
        return result;
    }

    QSqlQuery pragma(db);
    pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));

    const bool schemaOk = ensureSchema(db);
    const bool seeded = hasMinimumSeedData(db);

    qDebug() << "[switchSqliteDatabase] switched to" << db.databaseName()
             << "schemaOk" << schemaOk << "seeded" << seeded;

    result.insert(QStringLiteral("ok"), schemaOk);
    result.insert(QStringLiteral("file"), db.databaseName());
    result.insert(QStringLiteral("seeded"), seeded);
    result.insert(QStringLiteral("message"),
                  schemaOk
                      ? QStringLiteral("Switched to %1 (seeded=%2)")
                            .arg(db.databaseName())
                            .arg(seeded ? QStringLiteral("true") : QStringLiteral("false"))
                      : QStringLiteral("Schema setup failed"));

    return result;
}

}  // namespace gym
