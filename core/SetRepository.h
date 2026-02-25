#pragma once

#include <QSqlDatabase>
#include <QVector>

namespace gym {

struct SetRow {
    int id = 0;
    int exerciseId = 0;
    int setNumber = 0;
    int reps = 0;
    double weightLbs = 0.0;
};

class SetRepository {
public:
    SetRepository() = default;
    explicit SetRepository(const QSqlDatabase &db);

    void setDatabase(const QSqlDatabase &db);
    bool isReady() const;

    int addSet(int exerciseId, int setNumber, int reps, double weightLbs);
    bool updateSet(int setId, int setNumber, int reps, double weightLbs);
    bool removeSet(int setId);
    bool resequenceSets(int exerciseId);

    QVector<SetRow> fetchSets(int exerciseId) const;
    int nextSetNumber(int exerciseId) const;

private:
    QSqlDatabase db_;
};

}  // namespace gym
