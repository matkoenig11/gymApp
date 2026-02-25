#include "SessionEditorModel.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QStringList>

#include "../core/Clogger.h"

namespace gym {

SessionEditorModel::SessionEditorModel(QObject *parent)
    : QAbstractListModel(parent) {}

void SessionEditorModel::setRepositories(const SessionRepository &sessionRepo,
                                         const ExerciseRepository &exerciseRepo,
                                         const SetRepository &setRepo) {
    sessionRepo_ = sessionRepo;
    exerciseRepo_ = exerciseRepo;
    setRepo_ = setRepo;
}

int SessionEditorModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return items_.size();
}

QVariant SessionEditorModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= items_.size()) {
        return {};
    }
    const ExerciseItem &item = items_.at(index.row());
    switch (role) {
    case ExerciseIdRole:
        return item.exercise.id;
    case MachineNameRole:
        return item.exercise.machineNameResolved;
    case MuscleGroupRole:
        return item.exercise.muscleGroup;
    case CommentRole:
        return item.exercise.comment;
    case EffortRirRole:
        return item.exercise.effortRir.has_value() ? QVariant(item.exercise.effortRir.value()) : QVariant();
    case OrderRole:
        return item.exercise.orderIndex;
    case SetCountRole:
        return item.sets.size();
    case SetsRole:
        return setsToVariantList(item.sets);
    default:
        return {};
    }
}

QHash<int, QByteArray> SessionEditorModel::roleNames() const {
    return {
        {ExerciseIdRole, "exerciseId"},
        {MachineNameRole, "machineName"},
        {MuscleGroupRole, "muscleGroup"},
        {CommentRole, "comment"},
        {EffortRirRole, "effortRir"},
        {OrderRole, "orderIndex"},
        {SetCountRole, "setCount"},
        {SetsRole, "sets"},
    };
}

bool SessionEditorModel::ensureSession() {
    if (sessionId_ > 0) {
        return true;
    }
    if (!sessionRepo_.isReady()) {
        qWarning() << "SessionEditorModel: sessionRepo not ready";
        return false;
    }
    const int newId = sessionRepo_.createSession(QString(), sessionNote_);
    if (newId <= 0) {
        qWarning() << "SessionEditorModel: failed to create session";
        return false;
    }
    sessionId_ = newId;
    sessionStarted_ = QDateTime::currentDateTime().toString(Qt::ISODate);
    Clogger::debug(QStringLiteral("Created new session %1").arg(sessionId_), ELogColor::Green, __FILE__, __LINE__, __func__);
    emit sessionChanged();
    return true;
}

bool SessionEditorModel::loadSession(int sessionId) {
    if (!sessionRepo_.isReady()) {
        qWarning() << "SessionEditorModel: sessionRepo not ready";
        return false;
    }

    Clogger::debug(QStringLiteral("loadSession called with id=%1").arg(sessionId),
                   ELogColor::Cyan, __FILE__, __LINE__, __func__);

    if (sessionId < 0) {
        sessionId_ = -1;
        sessionNote_.clear();
        sessionStarted_.clear();
        items_.clear();
        beginResetModel();
        endResetModel();
        emit countChanged();
        emit sessionChanged();
        const bool ok = ensureSession() && reloadExercises();
        Clogger::debug(QStringLiteral("Initialized new session editor; sessionId=%1, exercises=%2")
                           .arg(sessionId_).arg(items_.size()),
                       ELogColor::Yellow, __FILE__, __LINE__, __func__);
        return ok;
    }

    Session s = sessionRepo_.fetchOne(sessionId);
    if (s.id <= 0) {
        qWarning() << "SessionEditorModel: session not found" << sessionId;
        return false;
    }
    sessionId_ = s.id;
    sessionNote_ = s.note;
    sessionStarted_ = s.startedAt;
    const bool ok = reloadExercises();
    emit sessionChanged();
    Clogger::debug(QStringLiteral("Loaded session %1 started=%2 noteLen=%3 exercises=%4")
                       .arg(sessionId_).arg(sessionStarted_).arg(sessionNote_.length()).arg(items_.size()),
                   ELogColor::Green, __FILE__, __LINE__, __func__);
    return ok;
}

bool SessionEditorModel::reloadExercises() {
    if (sessionId_ <= 0 || !exerciseRepo_.isReady()) {
        return false;
    }

    const QVector<ExerciseRow> rows = exerciseRepo_.fetchExercises(sessionId_);

    QVector<ExerciseItem> newItems;
    for (const ExerciseRow &r : rows) {
        ExerciseItem item;
        item.exercise = r;
        item.sets = setRepo_.fetchSets(r.id);
        newItems.append(item);
    }

    beginResetModel();
    items_ = newItems;
    endResetModel();
    emit countChanged();
    emit exercisesChanged();

    QStringList summaries;
    for (const auto &it : items_) {
        summaries.append(QStringLiteral("#%1 %2 sets=%3")
                             .arg(it.exercise.id)
                             .arg(it.exercise.machineNameResolved.isEmpty()
                                      ? it.exercise.machineNameSnapshot
                                      : it.exercise.machineNameResolved)
                             .arg(it.sets.size()));
    }
    Clogger::debug(QStringLiteral("reloadExercises sessionId=%1 count=%2 [%3]")
                       .arg(sessionId_)
                       .arg(items_.size())
                       .arg(summaries.join(QStringLiteral("; "))),
                   ELogColor::Magenta, __FILE__, __LINE__, __func__);
    return true;
}

bool SessionEditorModel::endSessionNow() {
    if (sessionId_ <= 0) {
        return false;
    }
    if (!sessionRepo_.endSession(sessionId_, QString())) {
        return false;
    }
    return true;
}

bool SessionEditorModel::deleteSession() {
    if (sessionId_ <= 0) {
        return false;
    }
    if (!sessionRepo_.deleteSession(sessionId_)) {
        return false;
    }
    sessionId_ = -1;
    sessionNote_.clear();
    sessionStarted_.clear();
    beginResetModel();
    items_.clear();
    endResetModel();
    emit countChanged();
    emit sessionChanged();
    return true;
}

bool SessionEditorModel::updateSessionNote(const QString &note) {
    if (sessionId_ <= 0 && !ensureSession()) {
        return false;
    }
    sessionNote_ = note;
    const bool ok = sessionRepo_.updateSessionMeta(sessionId_, sessionStarted_, sessionNote_);
    if (ok) {
        emit sessionChanged();
        Clogger::debug(QStringLiteral("updateSessionNote sessionId=%1 len=%2")
                           .arg(sessionId_).arg(sessionNote_.length()),
                       ELogColor::Blue, __FILE__, __LINE__, __func__);
    }
    return ok;
}

bool SessionEditorModel::updateSessionStarted(const QString &startedAt) {
    if (sessionId_ <= 0 && !ensureSession()) {
        return false;
    }
    const QString newStarted = startedAt.isEmpty()
                                   ? QDateTime::currentDateTime().toString(Qt::ISODate)
                                   : startedAt;
    const bool ok = sessionRepo_.updateSessionMeta(sessionId_, newStarted, sessionNote_);
    if (ok) {
        sessionStarted_ = newStarted;
        emit sessionChanged();
        Clogger::debug(QStringLiteral("updateSessionStarted sessionId=%1 started=%2")
                           .arg(sessionId_).arg(sessionStarted_),
                       ELogColor::Blue, __FILE__, __LINE__, __func__);
    }
    return ok;
}
int SessionEditorModel::addExercise(const QVariant &machineIdVar,
                                    const QVariant &customNameVar,
                                    const QVariant &commentVar,
                                    const QVariant &effortRirVar) {
    if (!ensureSession()) {
        return 0;
    }

    const int machineId = machineIdVar.toInt();
    const QString nameSnapshot = customNameVar.isNull() ? QString() : customNameVar.toString();
    const QString comment = commentVar.isNull() ? QString() : commentVar.toString();
    const std::optional<int> effortRir = effortRirVar.isNull() ? std::nullopt : std::optional<int>(effortRirVar.toInt());

    std::optional<int> machineOpt = machineId > 0 ? std::optional<int>(machineId) : std::nullopt;

    const int id = exerciseRepo_.addExercise(sessionId_, machineOpt, nameSnapshot, comment, effortRir, -1);
    if (id > 0) {
        reloadExercises();
    }
    return id;
}

bool SessionEditorModel::updateExercise(int exerciseId,
                                        const QString &comment,
                                        int machineId,
                                        const QString &customName,
                                        const QVariant &effortRirVar) {
    // If no machine changes are provided, keep existing linkage/snapshot.
    std::optional<int> machineOpt;
    QString nameSnapshot = customName;
    if (machineId > 0) {
        machineOpt = machineId;
    }
    std::optional<int> effortRir = effortRirVar.isNull() ? std::nullopt : std::optional<int>(effortRirVar.toInt());

    if (machineOpt == std::nullopt && nameSnapshot.isEmpty() && !effortRir.has_value()) {
        for (const ExerciseItem &item : items_) {
            if (item.exercise.id == exerciseId) {
                machineOpt = item.exercise.machineId;
                nameSnapshot = item.exercise.machineNameSnapshot;
                effortRir = item.exercise.effortRir;
                break;
            }
        }
    }

    const bool ok = exerciseRepo_.updateExercise(exerciseId, machineOpt, nameSnapshot, comment, effortRir);
    if (ok) {
        reloadExercises();
    }
    return ok;
}

int SessionEditorModel::indexOfExercise(int exerciseId) const {
    for (int i = 0; i < items_.size(); ++i) {
        if (items_.at(i).exercise.id == exerciseId) {
            return i;
        }
    }
    return -1;
}

bool SessionEditorModel::moveExercise(int exerciseId, int toIndex) {
    if (toIndex < 0) {
        return false;
    }
    const int fromIndex = indexOfExercise(exerciseId);
    if (fromIndex < 0 || fromIndex == toIndex) {
        return false;
    }

    QList<int> ids;
    for (const ExerciseItem &item : items_) {
        ids.append(item.exercise.id);
    }
    if (toIndex >= ids.size()) {
        toIndex = ids.size() - 1;
    }
    ids.move(fromIndex, toIndex);

    if (exerciseRepo_.reorderExercises(sessionId_, ids)) {
        reloadExercises();
        return true;
    }
    return false;
}

bool SessionEditorModel::removeExercise(int exerciseId) {
    if (!exerciseRepo_.removeExercise(exerciseId)) {
        return false;
    }
    reloadExercises();
    return true;
}

int SessionEditorModel::addSet(int exerciseId, int reps, double weight) {
    const int setNumber = setRepo_.nextSetNumber(exerciseId);
    const int id = setRepo_.addSet(exerciseId, setNumber, reps, weight);
    if (id > 0) {
        reloadExercises();
    }
    return id;
}

bool SessionEditorModel::updateSet(int setId, int reps, double weight) {
    // Keep set number as-is: fetch current number
    int setNumber = 1;
    for (const ExerciseItem &item : items_) {
        for (const SetRow &s : item.sets) {
            if (s.id == setId) {
                setNumber = s.setNumber;
                break;
            }
        }
    }
    const bool ok = setRepo_.updateSet(setId, setNumber, reps, weight);
    if (ok) {
        reloadExercises();
    }
    return ok;
}

bool SessionEditorModel::removeSet(int setId) {
    int exerciseId = 0;
    for (const auto &item : items_) {
        for (const auto &s : item.sets) {
            if (s.id == setId) {
                exerciseId = s.exerciseId;
                break;
            }
        }
    }
    if (setRepo_.removeSet(setId)) {
        if (exerciseId > 0) {
            setRepo_.resequenceSets(exerciseId);
        }
        reloadExercises();
        return true;
    }
    return false;
}

QVariantList SessionEditorModel::setsToVariantList(const QVector<SetRow> &sets) const {
    QVariantList list;
    for (const SetRow &s : sets) {
        QVariantMap map;
        map.insert(QStringLiteral("id"), s.id);
        map.insert(QStringLiteral("exerciseId"), s.exerciseId);
        map.insert(QStringLiteral("setNumber"), s.setNumber);
        map.insert(QStringLiteral("reps"), s.reps);
        map.insert(QStringLiteral("weight"), s.weightLbs);
        list.append(map);
    }
    return list;
}

QVariantList SessionEditorModel::setsForExercise(int exerciseId) const {
    for (const ExerciseItem &item : items_) {
        if (item.exercise.id == exerciseId) {
            return setsToVariantList(item.sets);
        }
    }
    return {};
}

QVariantList SessionEditorModel::debugExercises() const {
    QVariantList list;
    for (const ExerciseItem &item : items_) {
        QVariantMap m;
        m.insert(QStringLiteral("id"), item.exercise.id);
        m.insert(QStringLiteral("sessionId"), item.exercise.sessionId);
        m.insert(QStringLiteral("machineId"), item.exercise.machineId.has_value() ? item.exercise.machineId.value() : QVariant());
        m.insert(QStringLiteral("machineNameSnapshot"), item.exercise.machineNameSnapshot);
        m.insert(QStringLiteral("machineNameResolved"), item.exercise.machineNameResolved);
        m.insert(QStringLiteral("orderIndex"), item.exercise.orderIndex);
        m.insert(QStringLiteral("comment"), item.exercise.comment);
        m.insert(QStringLiteral("effortRir"), item.exercise.effortRir.has_value() ? item.exercise.effortRir.value() : QVariant());
        m.insert(QStringLiteral("setCount"), item.sets.size());
        list.append(m);
    }
    return list;
}

}  // namespace gym
