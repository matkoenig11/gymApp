#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <qqml.h>

#include "../core/SessionRepository.h"

namespace gym {

class SessionListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    QML_NAMED_ELEMENT(SessionList)
    QML_SINGLETON

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        StartedAtRole,
        EndedAtRole,
        NoteRole
    };

    explicit SessionListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRepository(const SessionRepository &repository);
    Q_INVOKABLE void refresh();

signals:
    void countChanged();

private:
    SessionRepository repository_;
    QVector<Session> items_;
};

}  // namespace gym
