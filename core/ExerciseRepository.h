#pragma once

#include <QSqlDatabase>
#include <QVector>
#include <optional>

namespace gym {

struct ExerciseRow {
    int id = 0;
    int sessionId = 0;
    int orderIndex = 0;
    std::optional<int> machineId;
    QString machineNameSnapshot;
    QString machineNameResolved;  // snapshot when present, otherwise machine name lookup
    QString muscleGroup;
    QString comment;
    std::optional<int> effortRir;  // 0-5 RIR for the whole exercise
};

class ExerciseRepository {
public:
    ExerciseRepository() = default;
    explicit ExerciseRepository(const QSqlDatabase &db);

    void setDatabase(const QSqlDatabase &db);
    bool isReady() const;

    int addExercise(int sessionId,
                    std::optional<int> machineId,
                    const QString &machineNameSnapshot,
                    const QString &comment,
                    std::optional<int> effortRir = std::nullopt,
                    int orderIndex = -1);
    bool updateExercise(int exerciseId,
                        std::optional<int> machineId,
                        const QString &machineNameSnapshot,
                        const QString &comment,
                        std::optional<int> effortRir);
    bool removeExercise(int exerciseId);
    bool reorderExercises(int sessionId, const QList<int> &exerciseIdsInOrder);

    QVector<ExerciseRow> fetchExercises(int sessionId) const;

private:
    int nextOrderIndex(int sessionId) const;
    int nextExerciseId() const;
    ExerciseRow hydrateRow(const QSqlQuery &row) const;

    QSqlDatabase db_;
};

}  // namespace gym
