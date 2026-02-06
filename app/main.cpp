#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>

#include "Backend.h"
#include "core/MachineRepository.h"
#include "core/SessionRepository.h"
#include "core/ExerciseRepository.h"
#include "core/SetRepository.h"
#include "model/MachineListModel.h"
#include "model/SessionDetailModel.h"
#include "model/SessionListModel.h"
#include "model/SessionEditorModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("GymApp"));
    app.setOrganizationName(QStringLiteral("GymTracker"));

    gym::Backend backend(&app);

    const QString connectionName = QStringLiteral("gymapp");
    gym::DatabaseConfig config = backend.defaultSqliteConfig();
    config.databaseName = QStringLiteral("C:/Z_Programming_Backup/Programming/projects/gymApp/gym.sqlite");

    QSqlDatabase db = backend.openDatabase(connectionName, config);
    if (!db.isOpen()) {
        qWarning() << "Database failed to open; UI will be limited.";
    } else {
        backend.ensureSchema(db);
    }
    const bool dbSeeded = backend.hasMinimumSeedData(db);

    gym::MachineRepository machineRepo(db);
    gym::SessionRepository sessionRepo(db);
    gym::ExerciseRepository exerciseRepo(db);
    gym::SetRepository setRepo(db);

    QQmlApplicationEngine engine;
    auto machineModel = engine.singletonInstance<gym::MachineListModel *>("GymApp", "MachineList");
    if (machineModel) {
        machineModel->setRepository(machineRepo);
    }
    auto sessionModel = engine.singletonInstance<gym::SessionListModel *>("GymApp", "SessionList");
    if (sessionModel) {
        sessionModel->setRepository(sessionRepo);
    }
    auto sessionDetailModel = engine.singletonInstance<gym::SessionDetailModel *>("GymApp", "SessionDetail");
    if (sessionDetailModel) {
        sessionDetailModel->setSessionRepository(sessionRepo);
    }
    auto sessionEditorModel = engine.singletonInstance<gym::SessionEditorModel *>("GymApp", "SessionEditor");
    if (sessionEditorModel) {
        sessionEditorModel->setRepositories(sessionRepo, exerciseRepo, setRepo);
    }

    bool laodSession = sessionDetailModel && sessionDetailModel->loadSession(1);

    const QUrl mainUrl(QStringLiteral("qrc:/GymApp/qml/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [mainUrl](QObject *obj, const QUrl &objUrl) {
            if (!obj && mainUrl == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(mainUrl);

    return app.exec();
}
