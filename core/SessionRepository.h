#pragma once

#include <QSqlDatabase>
#include <QVector>

namespace gym {

struct Session {
    int id = 0;
    QString startedAt;
    QString endedAt;
    QString note;
};

class SessionRepository {
public:
    SessionRepository() = default;
    explicit SessionRepository(const QSqlDatabase &db);

    void setDatabase(const QSqlDatabase &db);
    bool isReady() const;
    QSqlDatabase database() const { return db_; }

    QVector<Session> fetchAll() const;
    Session fetchOne(int id) const;
    int createSession(const QString &startedAt, const QString &note);
    bool updateSessionMeta(int id, const QString &startedAt, const QString &note);
    bool endSession(int id, const QString &endedAt);
    bool deleteSession(int id);

private:
    QSqlDatabase db_;
};

}  // namespace gym
