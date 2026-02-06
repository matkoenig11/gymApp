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

}  // namespace gym
