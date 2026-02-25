#include "MachineListModel.h"

#include <QDebug>

namespace gym {

MachineListModel::MachineListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int MachineListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return items_.size();
}

QVariant MachineListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= items_.size()) {
        return {};
    }

    const Machine &machine = items_.at(index.row());
    switch (role) {
    case IdRole:
        return machine.id;
    case NameRole:
        return machine.name;
    case MuscleGroupRole:
        return machine.muscleGroup;
    case WeightMinRole:
        return machine.weightMin;
    case WeightMaxRole:
        return machine.weightMax;
    case NoteRole:
        return machine.note;
    default:
        return {};
    }
}

QHash<int, QByteArray> MachineListModel::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {MuscleGroupRole, "muscleGroup"},
        {WeightMinRole, "weightMin"},
        {WeightMaxRole, "weightMax"},
        {NoteRole, "note"},
    };
}

void MachineListModel::setRepository(const MachineRepository &repository) {
    repository_ = repository;
    refresh();
}

int MachineListModel::addMachine(const QString &name,
                                 const QString &muscleGroup,
                                 int weightMin,
                                 int weightMax,
                                 const QString &note) {
    const int id = repository_.addMachine(name, muscleGroup, weightMin, weightMax, note);
    if (id > 0) {
        refresh();
    }
    return id;
}

bool MachineListModel::updateMachine(int id,
                                     const QString &name,
                                     const QString &muscleGroup,
                                     int weightMin,
                                     int weightMax,
                                     const QString &note,
                                     bool isActive) {
    const bool ok = repository_.updateMachine(id, name, muscleGroup, weightMin, weightMax, note, isActive);
    if (ok) {
        refresh();
    }
    return ok;
}

bool MachineListModel::deleteMachine(int id) {
    const bool ok = repository_.softDeleteMachine(id);
    if (ok) {
        refresh();
    }
    return ok;
}

void MachineListModel::refresh() {
    if (!repository_.isReady()) {
        qWarning() << "MachineListModel: repository not ready";
        beginResetModel();
        items_.clear();
        endResetModel();
        emit countChanged();
        return;
    }

    const QVector<Machine> loaded = repository_.fetchAllActive();
    qDebug() << "MachineListModel: loaded" << loaded.size() << "machines";
    beginResetModel();
    items_ = loaded;
    endResetModel();
    emit countChanged();
}

}  // namespace gym
