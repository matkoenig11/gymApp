#include "ExerciseRepository.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <iostream>
#include <format>

namespace gym {

ExerciseRepository::ExerciseRepository(const QSqlDatabase &db) : db_(db) {}

void ExerciseRepository::setDatabase(const QSqlDatabase &db) {
    db_ = db;
}

bool ExerciseRepository::isReady() const {
    return db_.isValid() && db_.isOpen();
}

int ExerciseRepository::nextOrderIndex(int sessionId) const {
    if (!isReady()) {
        return 0;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("SELECT COALESCE(MAX(order_index), -1) + 1 FROM exercises WHERE session_id = ?"));
    q.addBindValue(sessionId);
    if (q.exec() && q.next()) {
        return q.value(0).toInt();
    }
    return 0;
}

int ExerciseRepository::nextExerciseId() const {
    if (!isReady()) {
        return 1;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("SELECT COALESCE(MAX(id), 0) + 1 FROM exercises"));
    if (q.exec() && q.next()) {
        return q.value(0).toInt();
    }
    return 1;
}

int ExerciseRepository::addExercise(int sessionId,
                                    std::optional<int> machineId,
                                    const QString &machineNameSnapshot,
                                    const QString &comment,
                                    int orderIndex) {
    if (!isReady()) {
        return 0;
    }

    const int computedOrder = orderIndex < 0 ? nextOrderIndex(sessionId - 1) : orderIndex;

    if (!machineId.has_value() && machineNameSnapshot.isEmpty()) {
        qWarning() << "addExercise: either machineId or machineNameSnapshot must be provided";
        return 0;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "INSERT INTO exercises (id, session_id, machine_id, machine_name_snapshot, order_index, comment) "
        "VALUES (?, ?, ?, ?, ?, ?)"));

    int nextId = nextExerciseId();
    q.addBindValue(nextId);
    q.addBindValue(sessionId);
    if (machineId.has_value()) {
        q.addBindValue(*machineId);
    } else {
        q.addBindValue(QVariant(QVariant::Int));  // NULL
    }
    q.addBindValue(machineNameSnapshot);
    q.addBindValue(computedOrder);
    q.addBindValue(comment);

    // std::cout << std::format("Adding exercise with sessionId={}, machineId={}, machineNameSnapshot='{}', orderIndex={}, comment='{}'\n",
    //                  sessionId, machineId.has_value() ? std::to_string(*machineId) : "NULL", machineNameSnapshot.toStdString(),
    //                  computedOrder, comment.toStdString());
    std::cout << "Adding exercise with sessionId=" << sessionId
              << ", machineId=" << (machineId.has_value() ? std::to_string(*machineId) : "NULL")
              << ", machineNameSnapshot='" << machineNameSnapshot.toStdString() << "'"
              << ", orderIndex=" << computedOrder
              << ", comment='" << comment.toStdString() << "'\n";

    if (!q.exec()) {
        qWarning() << "addExercise failed:" << q.lastError().text()
                   << "db:" << db_.databaseName()
                   << "bound:" << q.boundValues();
        qWarning() << "Query was:" << q.lastQuery();
        return 0;
    }
    return q.lastInsertId().toInt();
}

bool ExerciseRepository::updateExercise(int exerciseId,
                                        std::optional<int> machineId,
                                        const QString &machineNameSnapshot,
                                        const QString &comment) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "UPDATE exercises SET machine_id = ?, machine_name_snapshot = ?, comment = ? WHERE id = ?"));
    if (machineId.has_value()) {
        q.addBindValue(*machineId);
    } else {
        q.addBindValue(QVariant(QVariant::Int));
    }
    q.addBindValue(machineNameSnapshot);
    q.addBindValue(comment);
    q.addBindValue(exerciseId);
    if (!q.exec()) {
        qWarning() << "updateExercise failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool ExerciseRepository::removeExercise(int exerciseId) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("DELETE FROM exercises WHERE id = ?"));
    q.addBindValue(exerciseId);
    if (!q.exec()) {
        qWarning() << "removeExercise failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool ExerciseRepository::reorderExercises(int sessionId, const QList<int> &exerciseIdsInOrder) {
    if (!isReady()) {
        return false;
    }
    if (exerciseIdsInOrder.isEmpty()) {
        return true;
    }

    if (!db_.transaction()) {
        qWarning() << "reorderExercises: failed to begin transaction:" << db_.lastError().text();
        return false;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral("UPDATE exercises SET order_index = ? WHERE id = ? AND session_id = ?"));
    for (int i = 0; i < exerciseIdsInOrder.size(); ++i) {
        q.bindValue(0, i);
        q.bindValue(1, exerciseIdsInOrder.at(i));
        q.bindValue(2, sessionId);
        if (!q.exec()) {
            qWarning() << "reorderExercises failed:" << q.lastError().text();
            db_.rollback();
            return false;
        }
        q.finish();
    }

    if (!db_.commit()) {
        qWarning() << "reorderExercises: failed to commit:" << db_.lastError().text();
        return false;
    }
    return true;
}

ExerciseRow ExerciseRepository::hydrateRow(const QSqlQuery &row) const {
    ExerciseRow ex;
    ex.id = row.value(0).toInt();
    ex.sessionId = row.value(1).toInt();
    ex.machineId = row.isNull(2) ? std::optional<int>() : std::optional<int>(row.value(2).toInt());
    ex.machineNameSnapshot = row.value(3).toString();
    ex.orderIndex = row.value(4).toInt();
    ex.comment = row.value(5).toString();
    ex.machineNameResolved = row.value(6).toString();
    ex.muscleGroup = row.value(7).toString();
    return ex;
}

QVector<ExerciseRow> ExerciseRepository::fetchExercises(int sessionId) const {
    QVector<ExerciseRow> rows;
    if (!isReady()) {
        return rows;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "SELECT e.id, e.session_id, e.machine_id, e.machine_name_snapshot, e.order_index, e.comment, "
        "COALESCE(e.machine_name_snapshot, m.name, '') AS resolved_name, "
        "COALESCE(m.muscle_group, '') "
        "FROM exercises e "
        "LEFT JOIN machines m ON e.machine_id = m.id "
        "WHERE e.session_id = ? "
        "ORDER BY e.order_index ASC, e.id ASC"));
    q.addBindValue(sessionId);
    if (!q.exec()) {
        qWarning() << "fetchExercises failed:" << q.lastError().text();
        return rows;
    }

    while (q.next()) {
        rows.append(hydrateRow(q));
    }
    return rows;
}

}  // namespace gym
