#include "SetRepository.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace gym {

SetRepository::SetRepository(const QSqlDatabase &db) : db_(db) {}

void SetRepository::setDatabase(const QSqlDatabase &db) {
    db_ = db;
}

bool SetRepository::isReady() const {
    return db_.isValid() && db_.isOpen();
}

int SetRepository::nextSetNumber(int exerciseId) const {
    if (!isReady()) {
        return 1;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("SELECT COALESCE(MAX(set_number), 0) + 1 FROM exercise_sets WHERE exercise_id = ?"));
    q.addBindValue(exerciseId);
    if (q.exec() && q.next()) {
        return q.value(0).toInt();
    }
    return 1;
}

int SetRepository::addSet(int exerciseId, int setNumber, int reps, double weightLbs, double rpe, bool isWarmup) {
    if (!isReady()) {
        return 0;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "INSERT INTO exercise_sets (exercise_id, set_number, reps, weight_lbs, rpe, is_warmup) "
        "VALUES (?, ?, ?, ?, ?, ?)"));
    q.addBindValue(exerciseId);
    q.addBindValue(setNumber);
    q.addBindValue(reps);
    q.addBindValue(weightLbs);
    q.addBindValue(rpe);
    q.addBindValue(isWarmup ? 1 : 0);
    if (!q.exec()) {
        qWarning() << "addSet failed:" << q.lastError().text();
        return 0;
    }
    return q.lastInsertId().toInt();
}

bool SetRepository::updateSet(int setId, int setNumber, int reps, double weightLbs, double rpe, bool isWarmup) {
    if (!isReady()) {
        return false;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "UPDATE exercise_sets SET set_number = ?, reps = ?, weight_lbs = ?, rpe = ?, is_warmup = ? WHERE id = ?"));
    q.addBindValue(setNumber);
    q.addBindValue(reps);
    q.addBindValue(weightLbs);
    q.addBindValue(rpe);
    q.addBindValue(isWarmup ? 1 : 0);
    q.addBindValue(setId);
    if (!q.exec()) {
        qWarning() << "updateSet failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool SetRepository::removeSet(int setId) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("DELETE FROM exercise_sets WHERE id = ?"));
    q.addBindValue(setId);
    if (!q.exec()) {
        qWarning() << "removeSet failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool SetRepository::resequenceSets(int exerciseId) {
    if (!isReady()) {
        return false;
    }
    if (!db_.transaction()) {
        qWarning() << "resequenceSets: failed to start transaction:" << db_.lastError().text();
        return false;
    }

    QSqlQuery select(db_);
    select.prepare(QStringLiteral(
        "SELECT id FROM exercise_sets WHERE exercise_id = ? ORDER BY set_number ASC, id ASC"));
    select.addBindValue(exerciseId);
    if (!select.exec()) {
        qWarning() << "resequenceSets select failed:" << select.lastError().text();
        db_.rollback();
        return false;
    }

    QSqlQuery update(db_);
    update.prepare(QStringLiteral("UPDATE exercise_sets SET set_number = ? WHERE id = ?"));

    int idx = 1;
    while (select.next()) {
        const int setId = select.value(0).toInt();
        update.bindValue(0, idx++);
        update.bindValue(1, setId);
        if (!update.exec()) {
            qWarning() << "resequenceSets update failed:" << update.lastError().text();
            db_.rollback();
            return false;
        }
        update.finish();
    }

    if (!db_.commit()) {
        qWarning() << "resequenceSets commit failed:" << db_.lastError().text();
        return false;
    }
    return true;
}

QVector<SetRow> SetRepository::fetchSets(int exerciseId) const {
    QVector<SetRow> rows;
    if (!isReady()) {
        return rows;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "SELECT id, exercise_id, set_number, reps, weight_lbs, COALESCE(rpe, 0), COALESCE(is_warmup, 0) "
        "FROM exercise_sets WHERE exercise_id = ? ORDER BY set_number ASC, id ASC"));
    q.addBindValue(exerciseId);
    if (!q.exec()) {
        qWarning() << "fetchSets failed:" << q.lastError().text();
        return rows;
    }

    while (q.next()) {
        SetRow r;
        r.id = q.value(0).toInt();
        r.exerciseId = q.value(1).toInt();
        r.setNumber = q.value(2).toInt();
        r.reps = q.value(3).toInt();
        r.weightLbs = q.value(4).toDouble();
        r.rpe = q.value(5).toDouble();
        r.isWarmup = q.value(6).toInt() != 0;
        rows.append(r);
    }
    return rows;
}

}  // namespace gym
