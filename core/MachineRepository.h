#pragma once

#include <QSqlDatabase>
#include <QString>
#include <QVector>

namespace gym {

struct Machine {
    int id = 0;
    QString name;
    QString muscleGroup;
    int weightMin = 0;
    int weightMax = 0;
    QString note;
    bool isActive = true;
};

class MachineRepository {
public:
    MachineRepository() = default;
    explicit MachineRepository(const QSqlDatabase &db);

    void setDatabase(const QSqlDatabase &db);
    bool isReady() const;

    QVector<Machine> fetchAllActive() const;
    Machine fetchOne(int id) const;
    int addMachine(const QString &name,
                   const QString &muscleGroup,
                   int weightMin,
                   int weightMax,
                   const QString &note);
    bool updateMachine(int id,
                       const QString &name,
                       const QString &muscleGroup,
                       int weightMin,
                       int weightMax,
                       const QString &note,
                       bool isActive = true);
    bool softDeleteMachine(int id);

private:
    QSqlDatabase db_;
};

}  // namespace gym
