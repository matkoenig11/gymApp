#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <optional>
#include <QList>
#include <qqml.h>

#include "../core/ExerciseRepository.h"
#include "../core/SessionRepository.h"
#include "../core/SetRepository.h"

namespace gym {

class SessionEditorModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int sessionId READ sessionId NOTIFY sessionChanged)
    Q_PROPERTY(QString sessionNote READ sessionNote NOTIFY sessionChanged)
    Q_PROPERTY(QString sessionStarted READ sessionStarted NOTIFY sessionChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    QML_NAMED_ELEMENT(SessionEditor)
    QML_SINGLETON

public:
    enum Roles {
        ExerciseIdRole = Qt::UserRole + 1,
        MachineNameRole,
        MuscleGroupRole,
        CommentRole,
        OrderRole,
        SetCountRole,
        SetsRole
    };

    explicit SessionEditorModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRepositories(const SessionRepository &sessionRepo,
                         const ExerciseRepository &exerciseRepo,
                         const SetRepository &setRepo);

    Q_INVOKABLE bool loadSession(int sessionId);
    Q_INVOKABLE bool endSessionNow();
    Q_INVOKABLE bool updateSessionNote(const QString &note);
    Q_INVOKABLE bool deleteSession();

    Q_INVOKABLE int addExercise(const QVariant &machineId,
                                const QVariant &customName,
                                const QVariant &comment);
    Q_INVOKABLE bool updateExercise(int exerciseId,
                                    const QString &comment,
                                    int machineId,
                                    const QString &customName);
    Q_INVOKABLE bool moveExercise(int exerciseId, int toIndex);
    Q_INVOKABLE bool removeExercise(int exerciseId);

    Q_INVOKABLE int addSet(int exerciseId, int reps, double weight, double rpe, bool warmup);
    Q_INVOKABLE bool updateSet(int setId, int reps, double weight, double rpe, bool warmup);
    Q_INVOKABLE bool removeSet(int setId);
    Q_INVOKABLE QVariantList setsForExercise(int exerciseId) const;
    Q_INVOKABLE QVariantList debugExercises() const;

    int sessionId() const { return sessionId_; }
    QString sessionNote() const { return sessionNote_; }
    QString sessionStarted() const { return sessionStarted_; }

signals:
    void countChanged();
    void sessionChanged();
    void exercisesChanged();

private:
    struct ExerciseItem {
        ExerciseRow exercise;
        QVector<SetRow> sets;
    };

    bool ensureSession();
    bool reloadExercises();
    QVariantList setsToVariantList(const QVector<SetRow> &sets) const;
    int indexOfExercise(int exerciseId) const;

    SessionRepository sessionRepo_;
    ExerciseRepository exerciseRepo_;
    SetRepository setRepo_;

    int sessionId_ = -1;
    QString sessionNote_;
    QString sessionStarted_;
    QVector<ExerciseItem> items_;
};

}  // namespace gym
