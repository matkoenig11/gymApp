#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "../core/SessionRepository.h"
#include "../core/ExerciseRepository.h"
#include "../core/SetRepository.h"

using namespace gym;

class RepositoryCrudTest : public QObject {
    Q_OBJECT

private slots:
    void crud_flow();
};

void RepositoryCrudTest::crud_flow() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "crud");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    auto execSql = [&](const QString &sql) {
        QSqlQuery q(db);
        bool ok = q.exec(sql);
        if (!ok) {
            qWarning() << "SQL failed" << sql << q.lastError().text();
        }
        return ok;
    };

    QVERIFY(execSql("PRAGMA foreign_keys = ON"));
    QVERIFY(execSql("CREATE TABLE sessions (id INTEGER PRIMARY KEY, started_at TEXT, ended_at TEXT, note TEXT)"));
    QVERIFY(execSql("CREATE TABLE machines (id INTEGER PRIMARY KEY, name TEXT, muscle_group TEXT)"));
    QVERIFY(execSql("CREATE TABLE exercises (id INTEGER PRIMARY KEY, session_id INTEGER NOT NULL, machine_id INTEGER, machine_name_snapshot TEXT, order_index INTEGER NOT NULL DEFAULT 0, comment TEXT, FOREIGN KEY(session_id) REFERENCES sessions(id) ON DELETE CASCADE)"));
    QVERIFY(execSql("CREATE TABLE exercise_sets (id INTEGER PRIMARY KEY, exercise_id INTEGER NOT NULL, set_number INTEGER NOT NULL, reps INTEGER NOT NULL, weight_lbs REAL, UNIQUE(exercise_id, set_number), FOREIGN KEY(exercise_id) REFERENCES exercises(id) ON DELETE CASCADE)"));

    SessionRepository sessionRepo(db);
    ExerciseRepository exerciseRepo(db);
    SetRepository setRepo(db);

    const int sessionId = sessionRepo.createSession("2026-01-28T10:00:00", "Morning");
    QVERIFY(sessionId > 0);

    QVERIFY(execSql("INSERT INTO machines (id, name, muscle_group) VALUES (1, 'Press', 'Chest')"));
    const int exId = exerciseRepo.addExercise(sessionId, 1, QString(), "Warmup", -1);
    QVERIFY(exId > 0);

    int set1 = setRepo.addSet(exId, 1, 10, 95.0);
    int set2 = setRepo.addSet(exId, 2, 8, 135.0);
    QVERIFY(set1 > 0 && set2 > 0);

    QVector<ExerciseRow> exRows = exerciseRepo.fetchExercises(sessionId);
    QCOMPARE(exRows.size(), 1);
    QCOMPARE(exRows.first().machineNameResolved, QStringLiteral("Press"));

    QVector<SetRow> setRows = setRepo.fetchSets(exId);
    QCOMPARE(setRows.size(), 2);
    QCOMPARE(setRows.at(1).setNumber, 2);

    QVERIFY(setRepo.updateSet(set2, 2, 9, 135.0));
    setRows = setRepo.fetchSets(exId);
    QCOMPARE(setRows.at(1).reps, 9);

    QVERIFY(setRepo.removeSet(set1));
    QVERIFY(setRepo.resequenceSets(exId));
    setRows = setRepo.fetchSets(exId);
    QCOMPARE(setRows.first().setNumber, 1);

    QList<int> newOrder;
    newOrder << exId;
    QVERIFY(exerciseRepo.reorderExercises(sessionId, newOrder));

    QVERIFY(sessionRepo.endSession(sessionId, "2026-01-28T11:00:00"));
    Session s = sessionRepo.fetchOne(sessionId);
    QCOMPARE(s.endedAt, QStringLiteral("2026-01-28T11:00:00"));

    QVERIFY(sessionRepo.deleteSession(sessionId));
    exRows = exerciseRepo.fetchExercises(sessionId);
    QCOMPARE(exRows.size(), 0);

    QSqlDatabase::removeDatabase("crud");
}

QTEST_MAIN(RepositoryCrudTest)
#include "RepositoryCrudTest.moc"
