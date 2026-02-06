#include "SessionRepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

namespace gym {

SessionRepository::SessionRepository(const QSqlDatabase &db) : db_(db) {}

void SessionRepository::setDatabase(const QSqlDatabase &db) {
    db_ = db;
}

bool SessionRepository::isReady() const {
    return db_.isValid() && db_.isOpen();
}

QVector<Session> SessionRepository::fetchAll() const {
    QVector<Session> sessions;
    if (!isReady()) {
        return sessions;
    }

    bool hasEndedAt = false;
    {
        QSqlQuery info(db_);
        if (info.exec(QStringLiteral("PRAGMA table_info(sessions)"))) {
            while (info.next()) {
                if (info.value(1).toString() == QStringLiteral("ended_at")) {
                    hasEndedAt = true;
                    break;
                }
            }
        }
    }

    QSqlQuery query(db_);
    const QString sql = hasEndedAt
        ? QStringLiteral("SELECT id, started_at, ended_at, note FROM sessions ORDER BY started_at DESC")
        : QStringLiteral("SELECT id, started_at, null as ended_at, note FROM sessions ORDER BY started_at DESC");
    if (!query.exec(sql)) {
        qWarning() << "Failed to load sessions:" << query.lastError().text();
        return sessions;
    }

    while (query.next()) {
        Session s;
        s.id = query.value(0).toInt();
        s.startedAt = query.value(1).toString();
        s.endedAt = query.value(2).toString();
        s.note = query.value(3).toString();
        sessions.append(s);
    }

    return sessions;
}

Session SessionRepository::fetchOne(int id) const {
    Session s;
    if (!isReady()) {
        return s;
    }

    QSqlQuery q(db_);
    q.prepare(QStringLiteral("SELECT id, started_at, ended_at, note FROM sessions WHERE id = ?"));
    q.addBindValue(id);
    if (q.exec() && q.next()) {
        s.id = q.value(0).toInt();
        s.startedAt = q.value(1).toString();
        s.endedAt = q.value(2).toString();
        s.note = q.value(3).toString();
    }
    return s;
}

int SessionRepository::createSession(const QString &startedAt, const QString &note) {
    if (!isReady()) {
        return 0;
    }
    const QString started = startedAt.isEmpty()
                                ? QDateTime::currentDateTime().toString(Qt::ISODate)
                                : startedAt;

    QSqlQuery q(db_);
    q.prepare(QStringLiteral("INSERT INTO sessions (started_at, note) VALUES (?, ?)"));
    q.addBindValue(started);
    q.addBindValue(note);
    if (!q.exec()) {
        qWarning() << "createSession failed:" << q.lastError().text();
        return 0;
    }
    return q.lastInsertId().toInt();
}

bool SessionRepository::updateSessionMeta(int id, const QString &startedAt, const QString &note) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("UPDATE sessions SET started_at = ?, note = ? WHERE id = ?"));
    q.addBindValue(startedAt);
    q.addBindValue(note);
    q.addBindValue(id);
    if (!q.exec()) {
        qWarning() << "updateSessionMeta failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool SessionRepository::endSession(int id, const QString &endedAt) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("UPDATE sessions SET ended_at = ? WHERE id = ?"));
    const QString ended = endedAt.isEmpty()
                              ? QDateTime::currentDateTime().toString(Qt::ISODate)
                              : endedAt;
    q.addBindValue(ended);
    q.addBindValue(id);
    if (!q.exec()) {
        qWarning() << "endSession failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool SessionRepository::deleteSession(int id) {
    if (!isReady()) {
        return false;
    }
    QSqlQuery q(db_);
    q.prepare(QStringLiteral("DELETE FROM sessions WHERE id = ?"));
    q.addBindValue(id);
    if (!q.exec()) {
        qWarning() << "deleteSession failed:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

}  // namespace gym
