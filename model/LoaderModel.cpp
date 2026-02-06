#include "LoaderModel.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrl>

namespace gym {

LoaderModel::LoaderModel(QObject *parent) : QObject(parent) {}

QVariantMap LoaderModel::loadMachinesFromJson(const QString &connectionName, const QUrl &jsonUrl) const {
    QVariantMap result;

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.isOpen()) {
        qWarning() << "[LoaderModel] DB not open for connection" << connectionName;
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("message"), QStringLiteral("Database is not open"));
        return result;
    }

    QSqlQuery check(db);
    if (check.exec(QStringLiteral("SELECT COUNT(*) FROM machines")) && check.next() && check.value(0).toInt() > 0) {
        result.insert(QStringLiteral("ok"), true);
        result.insert(QStringLiteral("message"), QStringLiteral("Machines already present; skipping load"));
        return result;
    }

    const QString localPath = jsonUrl.isLocalFile() ? jsonUrl.toLocalFile() : jsonUrl.toString();
    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[LoaderModel] Failed to open JSON" << localPath;
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("message"), QStringLiteral("Cannot open JSON file"));
        return result;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    const QJsonArray arr = doc.array();
    int inserted = 0;
    for (const QJsonValue &val : arr) {
        const QJsonObject obj = val.toObject();
        QSqlQuery insert(db);
        insert.prepare(QStringLiteral(
            "INSERT INTO machines (name, muscle_group, weight_min, weight_max, note) "
            "VALUES (?, ?, ?, ?, ?)"));
        insert.addBindValue(obj.value(QStringLiteral("name")).toString());
        insert.addBindValue(obj.value(QStringLiteral("muscle_group")).toString());
        insert.addBindValue(obj.value(QStringLiteral("weight_min")).toInt());
        insert.addBindValue(obj.value(QStringLiteral("weight_max")).toInt());
        insert.addBindValue(obj.value(QStringLiteral("note")).toString());
        if (insert.exec()) {
            inserted++;
        } else {
            qWarning() << "[LoaderModel] insert failed" << insert.lastError().text();
        }
    }

    result.insert(QStringLiteral("ok"), true);
    result.insert(QStringLiteral("inserted"), inserted);
    result.insert(QStringLiteral("message"), QStringLiteral("Inserted %1 machines").arg(inserted));
    return result;
}

}  // namespace gym
