#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <qqml.h>

#include "../core/SessionRepository.h"

namespace gym {

struct SessionExercise {
    QString machineName;
    QString muscleGroup;
    QString note;
    QVector<QString> sets;  // simple string summary (e.g., "10 x 100 lbs")
};

class SessionDetailModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QString sessionNote READ sessionNote NOTIFY sessionChanged)
    Q_PROPERTY(QString sessionStarted READ sessionStarted NOTIFY sessionChanged)
    QML_NAMED_ELEMENT(SessionDetail)
    QML_SINGLETON

public:
    enum Roles {
        MachineNameRole = Qt::UserRole + 1,
        MuscleGroupRole,
        NoteRole,
        SetsRole
    };

    explicit SessionDetailModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setSessionRepository(const SessionRepository &repository);
    Q_INVOKABLE bool loadSession(int sessionId);

    QString sessionNote() const { return sessionNote_; }
    QString sessionStarted() const { return sessionStarted_; }

signals:
    void countChanged();
    void sessionChanged();

private:
    SessionRepository repository_;
    QVector<SessionExercise> items_;
    QString sessionNote_;
    QString sessionStarted_;
};

}  // namespace gym
