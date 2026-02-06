#include <QtTest>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "../core/SessionRepository.h"
#include "../model/SessionDetailModel.h"

using namespace gym;

class SessionDetailModelTest : public QObject {
    Q_OBJECT

private slots:
    void loadSession_readsExercisesAndSets();
};

void SessionDetailModelTest::loadSession_readsExercisesAndSets() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    auto execSql = [&](const QString &sql) {
        QSqlQuery q(db);
        bool ok = q.exec(sql);
        if (!ok) qWarning() << "SQL failed" << sql.left(80) << "..." << q.lastError().text();
        return ok;
    };

    // Load the real schema from migrations to stay in sync with the app.
    auto findMigration = []() -> QString {
        const QString relative = "sql/migrations/001_init.sql";
        QDir dir(QCoreApplication::applicationDirPath());
        for (int up = 0; up < 5; ++up) {
            QString candidate = dir.filePath(QString("../").repeated(up) + relative);
            if (QFile::exists(candidate)) {
                return candidate;
            }
        }
        return {};
    };
    const QString migrationPath = findMigration();
    QVERIFY2(!migrationPath.isEmpty(), "Could not locate sql/migrations/001_init.sql");

    QFile f(migrationPath);
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QString cleaned;
    while (!f.atEnd()) {
        const QString line = QString::fromUtf8(f.readLine());
        const QString trimmed = line.trimmed();
        if (trimmed.startsWith("--") || trimmed.isEmpty()) {
            continue;
        }
        cleaned += line;
    }
    for (const QString &stmt : cleaned.split(';', Qt::SkipEmptyParts)) {
        const QString trimmed = stmt.trimmed();
        if (trimmed.isEmpty())
            continue;
        QVERIFY(execSql(trimmed));
    }

    // Seed minimal rows matching the real schema (defaults cover other columns).
    QVERIFY(execSql("INSERT INTO sessions (id, started_at, note) "
                    "VALUES (1, '2026-01-21T18:51:00', 'Test session')"));
    QVERIFY(execSql("INSERT INTO machines (id, name, muscle_group) "
                    "VALUES (1, 'Test Machine', 'B4')"));
    QVERIFY(execSql("INSERT INTO exercises (id, session_id, machine_id, machine_name_snapshot, order_index, comment) "
                    "VALUES (1, 1, 1, 'Test Machine', 0, 'Pulls')"));
    QVERIFY(execSql("INSERT INTO exercise_sets (id, exercise_id, set_number, reps, weight_lbs, rpe, is_warmup) "
                    "VALUES (1, 1, 1, 10, 100, 8.5, 0), "
                           "(2, 1, 2, 8, 110, 9.0, 0)"));

    SessionRepository repo(db);
    SessionDetailModel model;
    model.setSessionRepository(repo);

    QVERIFY(model.loadSession(1));
    QCOMPARE(model.rowCount(), 1);

    const QModelIndex idx = model.index(0, 0);
    QCOMPARE(model.data(idx, SessionDetailModel::MachineNameRole).toString(), QStringLiteral("Test Machine"));
    QVariantList sets = model.data(idx, SessionDetailModel::SetsRole).toList();
    QCOMPARE(sets.size(), 2);
    QVERIFY(sets.at(0).toString().contains(QStringLiteral("10")));
    QVERIFY(sets.at(1).toString().contains(QStringLiteral("8")));

    QSqlDatabase::removeDatabase("test");
}

QTEST_MAIN(SessionDetailModelTest)
#include "SessionDetailModelTest.moc"
