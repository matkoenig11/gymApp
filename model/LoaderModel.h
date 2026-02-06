#pragma once

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include <qqml.h>

namespace gym {

class LoaderModel : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit LoaderModel(QObject *parent = nullptr);

    // Load machines from JSON into the given DB connection if table is empty.
    Q_INVOKABLE QVariantMap loadMachinesFromJson(const QString &connectionName, const QUrl &jsonUrl) const;
};

}  // namespace gym
