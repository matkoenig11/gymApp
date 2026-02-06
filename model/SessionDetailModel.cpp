#include "SessionDetailModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringLiteral>

namespace gym {

namespace {
bool tableHasColumn(const QSqlDatabase &db, const QString &table, const QString &column) {
    QSqlQuery q(db);
    const QString pragma = QStringLiteral("PRAGMA table_info(%1)").arg(table);
    if (!q.exec(pragma)) {
        qWarning() << "SessionDetailModel: failed to inspect table" << table << q.lastError().text();
        return false;
    }
    while (q.next()) {
        if (q.value(1).toString().compare(column, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}
}  // namespace

SessionDetailModel::SessionDetailModel(QObject *parent)
    : QAbstractListModel(parent) {}

int SessionDetailModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return items_.size();
}

QVariant SessionDetailModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= items_.size()) {
        return {};
    }

    const SessionExercise &ex = items_.at(index.row());
    switch (role) {
    case MachineNameRole:
        return ex.machineName;
    case MuscleGroupRole:
        return ex.muscleGroup;
    case NoteRole:
        return ex.note;
    case SetsRole:
        return QVariant::fromValue(ex.sets);
    default:
        return {};
    }
}

QHash<int, QByteArray> SessionDetailModel::roleNames() const {
    return {
        {MachineNameRole, "machineName"},
        {MuscleGroupRole, "muscleGroup"},
        {NoteRole, "note"},
        {SetsRole, "sets"},
    };
}

void SessionDetailModel::setSessionRepository(const SessionRepository &repository) {
    repository_ = repository;
}

bool SessionDetailModel::loadSession(int sessionId) {
    qDebug() << "SessionDetailModel: loading session" << sessionId;
    if (!repository_.isReady()) {
        qWarning() << "SessionDetailModel: repository not ready";
        return false;
    }

    items_.clear();
    sessionNote_.clear();
    sessionStarted_.clear();

    QSqlDatabase db = repository_.database();
    if (!db.isOpen()) {
        qWarning() << "SessionDetailModel: repository DB not open";
        return false;
    }

    QSqlQuery meta(db);
    meta.prepare(QStringLiteral("SELECT started_at, note FROM sessions WHERE id = ?"));
    meta.addBindValue(sessionId);
    if (meta.exec() && meta.next()) {
        sessionStarted_ = meta.value(0).toString();
        sessionNote_ = meta.value(1).toString();
    } else {
        qWarning() << "SessionDetailModel: session not found" << sessionId;
    }

    const bool hasSnapshot = tableHasColumn(db, QStringLiteral("exercises"), QStringLiteral("machine_name_snapshot"));
    const bool hasOrderIndex = tableHasColumn(db, QStringLiteral("exercises"), QStringLiteral("order_index"));
    const bool hasRpe = tableHasColumn(db, QStringLiteral("exercise_sets"), QStringLiteral("rpe"));

    QSqlQuery exQuery(db);
    const QString exSql = QStringLiteral(
        "SELECT id, %1 AS machine_name_snapshot, comment, machine_id "
        "FROM exercises WHERE session_id = ? %2")
                              .arg(hasSnapshot ? QStringLiteral("machine_name_snapshot") : QStringLiteral("NULL"))
                              .arg(hasOrderIndex ? QStringLiteral("ORDER BY order_index") : QStringLiteral("ORDER BY id"));
    exQuery.prepare(exSql);
    exQuery.addBindValue(sessionId);
    if (!exQuery.exec()) {
        qWarning() << "SessionDetailModel: exercises query failed" << exQuery.lastError().text();
        return false;
    }

    while (exQuery.next()) {
        const int exerciseId = exQuery.value(0).toInt();
        SessionExercise ex;
        ex.machineName = exQuery.value(1).toString();
        ex.note = exQuery.value(2).toString();

        if (ex.machineName.isEmpty()) {
            const int machineId = exQuery.value(3).toInt();
            if (machineId > 0) {
        QSqlQuery machine(db);
                machine.prepare(QStringLiteral("SELECT name, muscle_group FROM machines WHERE id = ?"));
                machine.addBindValue(machineId);
                if (machine.exec() && machine.next()) {
                    ex.machineName = machine.value(0).toString();
                    ex.muscleGroup = machine.value(1).toString();
                }
            }
        }

        QSqlQuery setQuery(db);
        const QString setSql = QStringLiteral(
            "SELECT set_number, reps, weight_lbs, %1 AS rpe FROM exercise_sets WHERE exercise_id = ? ORDER BY set_number")
                                   .arg(hasRpe ? QStringLiteral("rpe") : QStringLiteral("NULL"));
        setQuery.prepare(setSql);
        setQuery.addBindValue(exerciseId);
        if (setQuery.exec()) {
            while (setQuery.next()) {
                const int setNumber = setQuery.value(0).toInt();
                const int reps = setQuery.value(1).toInt();
                const double weight = setQuery.value(2).toDouble();
                const double rpe = setQuery.value(3).toDouble();
                QString desc = QStringLiteral("#%1: %2 reps @ %3 lbs").arg(setNumber).arg(reps).arg(weight, 0, 'f', 0);
                if (rpe > 0.0) {
                    desc += QStringLiteral(" (RPE %1)").arg(rpe, 0, 'f', 1);
                }
                ex.sets.append(desc);
            }
        }
        items_.append(ex);
    }

    beginResetModel();
    endResetModel();
    emit countChanged();
    emit sessionChanged();
    qDebug() << "SessionDetailModel: loaded session" << sessionId << "exercises" << items_.size();
    return true;
}

}  // namespace gym
