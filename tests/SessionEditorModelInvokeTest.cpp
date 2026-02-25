#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include "../model/SessionEditorModel.h"

using namespace gym;

class SessionEditorModelInvokeTest : public QObject {
    Q_OBJECT

private slots:
    void invoke_methods_behave_like_qml();
};

void SessionEditorModelInvokeTest::invoke_methods_behave_like_qml() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "invoke");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    auto execSql = [&](const QString &sql) {
        QSqlQuery q(db);
        bool ok = q.exec(sql);
        if (!ok) qWarning() << "SQL failed" << sql << q.lastError().text();
        return ok;
    };

    QVERIFY(execSql("PRAGMA foreign_keys = ON"));
    QVERIFY(execSql("CREATE TABLE sessions (id INTEGER PRIMARY KEY, started_at TEXT, ended_at TEXT, note TEXT)"));
    QVERIFY(execSql("CREATE TABLE machines (id INTEGER PRIMARY KEY, name TEXT, muscle_group TEXT)"));
    QVERIFY(execSql("CREATE TABLE exercises (id INTEGER PRIMARY KEY, session_id INTEGER NOT NULL, machine_id INTEGER, machine_name_snapshot TEXT, effort_rir INTEGER, order_index INTEGER NOT NULL DEFAULT 0, comment TEXT, FOREIGN KEY(session_id) REFERENCES sessions(id) ON DELETE CASCADE)"));
    QVERIFY(execSql("CREATE TABLE exercise_sets (id INTEGER PRIMARY KEY, exercise_id INTEGER NOT NULL, set_number INTEGER NOT NULL, reps INTEGER NOT NULL, weight_lbs REAL, UNIQUE(exercise_id, set_number), FOREIGN KEY(exercise_id) REFERENCES exercises(id) ON DELETE CASCADE)"));
    QVERIFY(execSql("INSERT INTO machines (id, name, muscle_group) VALUES (1, 'Bench Press', 'Chest'), (2, 'Row', 'Back')"));

    SessionRepository sessionRepo(db);
    ExerciseRepository exerciseRepo(db);
    SetRepository setRepo(db);

    SessionEditorModel model;
    model.setRepositories(sessionRepo, exerciseRepo, setRepo);

    // Create a new session (as QML would do with -1)
    QVERIFY(model.loadSession(-1));
    QVERIFY(model.sessionId() > 0);

    // addExercise via invokeMethod (QML style)
    int exerciseId = 0;
    bool invokeOk = QMetaObject::invokeMethod(
        &model, "addExercise",
        Q_RETURN_ARG(int, exerciseId),
        Q_ARG(QVariant, QVariant(1)),          // machineId
        Q_ARG(QVariant, QVariant()),           // customName
        Q_ARG(QVariant, QVariant("Warmup")),   // comment
        Q_ARG(QVariant, QVariant(2))           // effortRir
    );
    QVERIFY(invokeOk);
    QVERIFY(exerciseId > 0);
    QCOMPARE(model.rowCount(), 1);

    // addSet via invokeMethod
    int setId = 0;
    invokeOk = QMetaObject::invokeMethod(
        &model, "addSet",
        Q_RETURN_ARG(int, setId),
        Q_ARG(int, exerciseId),
        Q_ARG(int, 10),          // reps
        Q_ARG(double, 95.0));    // weight
    QVERIFY(invokeOk);
    QVERIFY(setId > 0);

    // updateExercise via invokeMethod (switch to machine 2)
    bool updateOk = false;
    invokeOk = QMetaObject::invokeMethod(
        &model, "updateExercise",
        Q_RETURN_ARG(bool, updateOk),
        Q_ARG(int, exerciseId),
        Q_ARG(QString, QString("Updated note")),
        Q_ARG(int, 2),
        Q_ARG(QString, QString()),
        Q_ARG(QVariant, QVariant(1)));
    QVERIFY(invokeOk);
    QVERIFY(updateOk);

    // removeSet via invokeMethod
    bool removeOk = false;
    invokeOk = QMetaObject::invokeMethod(
        &model, "removeSet",
        Q_RETURN_ARG(bool, removeOk),
        Q_ARG(int, setId));
    QVERIFY(invokeOk);
    QVERIFY(removeOk);

    // deleteSession via invokeMethod
    bool deleteOk = false;
    invokeOk = QMetaObject::invokeMethod(
        &model, "deleteSession",
        Q_RETURN_ARG(bool, deleteOk));
    QVERIFY(invokeOk);
    QVERIFY(deleteOk);
    QCOMPARE(model.sessionId(), -1);

    QSqlDatabase::removeDatabase("invoke");
}

QTEST_MAIN(SessionEditorModelInvokeTest)
#include "SessionEditorModelInvokeTest.moc"
