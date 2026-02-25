#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <qqml.h>

#include "../core/MachineRepository.h"

namespace gym {

class MachineListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    QML_NAMED_ELEMENT(MachineList)
    QML_SINGLETON

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        MuscleGroupRole,
        WeightMinRole,
        WeightMaxRole,
        NoteRole
    };

    explicit MachineListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRepository(const MachineRepository &repository);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE int addMachine(const QString &name,
                               const QString &muscleGroup,
                               int weightMin,
                               int weightMax,
                               const QString &note);
    Q_INVOKABLE bool updateMachine(int id,
                                   const QString &name,
                                   const QString &muscleGroup,
                                   int weightMin,
                                   int weightMax,
                                   const QString &note,
                                   bool isActive);
    Q_INVOKABLE bool deleteMachine(int id);

signals:
    void countChanged();

private:
    MachineRepository repository_;
    QVector<Machine> items_;
};

}  // namespace gym
