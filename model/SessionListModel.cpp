#include "SessionListModel.h"

#include <QDebug>

namespace gym {

SessionListModel::SessionListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int SessionListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return items_.size();
}

QVariant SessionListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= items_.size()) {
        return {};
    }

    const Session &session = items_.at(index.row());
    switch (role) {
    case IdRole:
        return session.id;
    case StartedAtRole:
        return session.startedAt;
    case EndedAtRole:
        return session.endedAt;
    case NoteRole:
        return session.note;
    default:
        return {};
    }
}

QHash<int, QByteArray> SessionListModel::roleNames() const {
    return {
        {IdRole, "id"},
        {StartedAtRole, "startedAt"},
        {EndedAtRole, "endedAt"},
        {NoteRole, "note"},
    };
}

void SessionListModel::setRepository(const SessionRepository &repository) {
    repository_ = repository;
    refresh();
}

void SessionListModel::refresh() {
    if (!repository_.isReady()) {
        qWarning() << "SessionListModel: repository not ready";
        beginResetModel();
        items_.clear();
        endResetModel();
        emit countChanged();
        return;
    }

    const QVector<Session> loaded = repository_.fetchAll();
    qDebug() << "SessionListModel: loaded" << loaded.size() << "sessions";
    beginResetModel();
    items_ = loaded;
    endResetModel();
    emit countChanged();
}

}  // namespace gym
