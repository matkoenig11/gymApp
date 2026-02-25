#include "MachineRepository.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace gym {

MachineRepository::MachineRepository(const QSqlDatabase &db) : db_(db) {}

void MachineRepository::setDatabase(const QSqlDatabase &db) {
    db_ = db;
}

bool MachineRepository::isReady() const {
    return db_.isValid() && db_.isOpen();
}

QVector<Machine> MachineRepository::fetchAllActive() const {
    QVector<Machine> machines;
    if (!isReady()) {
        return machines;
    }

    bool hasIsActive = false;
    {
        QSqlQuery info(db_);
        if (info.exec(QStringLiteral("PRAGMA table_info(machines)"))) {
            while (info.next()) {
                if (info.value(1).toString() == QStringLiteral("is_active")) {
                    hasIsActive = true;
                    break;
                }
            }
        }
    }

    QSqlQuery query(db_);
    const QString sql = hasIsActive
        ? QStringLiteral(
            "SELECT id, name, muscle_group, weight_min, weight_max, note, is_active "
            "FROM machines WHERE is_active = 1 ORDER BY name")
        : QStringLiteral(
            "SELECT id, name, muscle_group, weight_min, weight_max, note, 1 as is_active "
            "FROM machines ORDER BY name");
    if (!query.exec(sql)) {
        qWarning() << "Failed to load machines:" << query.lastError().text();
        return machines;
    }

    while (query.next()) {
        Machine machine;
        machine.id = query.value(0).toInt();
        machine.name = query.value(1).toString();
        machine.muscleGroup = query.value(2).toString();
        machine.weightMin = query.value(3).toInt();
        machine.weightMax = query.value(4).toInt();
        machine.note = query.value(5).toString();
        machine.isActive = query.value(6).toInt() != 0;
        machines.append(machine);
    }

    return machines;
}

Machine MachineRepository::fetchOne(int id) const {
    Machine machine;
    if (!isReady()) {
        return machine;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "SELECT id, name, muscle_group, weight_min, weight_max, note, is_active "
        "FROM machines WHERE id = ?"));
    q.addBindValue(id);
    if (q.exec() && q.next()) {
        machine.id = q.value(0).toInt();
        machine.name = q.value(1).toString();
        machine.muscleGroup = q.value(2).toString();
        machine.weightMin = q.value(3).toInt();
        machine.weightMax = q.value(4).toInt();
        machine.note = q.value(5).toString();
        machine.isActive = q.value(6).toInt() != 0;
    }
    return machine;
}

int MachineRepository::addMachine(const QString &name,
                                  const QString &muscleGroup,
                                  int weightMin,
                                  int weightMax,
                                  const QString &note) {
    if (!isReady()) {
        return 0;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "INSERT INTO machines (name, muscle_group, weight_min, weight_max, note, is_active) "
        "VALUES (?, ?, ?, ?, ?, 1)"));
    q.addBindValue(name);
    q.addBindValue(muscleGroup);
    q.addBindValue(weightMin);
    q.addBindValue(weightMax);
    q.addBindValue(note);
    if (!q.exec()) {
        qWarning() << "addMachine failed:" << q.lastError().text();
        return 0;
    }
    return q.lastInsertId().toInt();
}

bool MachineRepository::updateMachine(int id,
                                      const QString &name,
                                      const QString &muscleGroup,
                                      int weightMin,
                                      int weightMax,
                                      const QString &note,
                                      bool isActive) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral(
        "UPDATE machines SET name = ?, muscle_group = ?, weight_min = ?, weight_max = ?, note = ?, is_active = ?, updated_at = datetime('now') "
        "WHERE id = ?"));
    q.addBindValue(name);
    q.addBindValue(muscleGroup);
    q.addBindValue(weightMin);
    q.addBindValue(weightMax);
    q.addBindValue(note);
    q.addBindValue(isActive ? 1 : 0);
    q.addBindValue(id);
    if (!q.exec()) {
        qWarning() << "updateMachine failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool MachineRepository::softDeleteMachine(int id) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("UPDATE machines SET is_active = 0, updated_at = datetime('now') WHERE id = ?"));
    q.addBindValue(id);
    if (!q.exec()) {
        qWarning() << "softDeleteMachine failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

}  // namespace gym
