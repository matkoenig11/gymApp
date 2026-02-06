#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _0x5f_GymApp_qml_Main_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_HeaderBar_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_DatabaseCard_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_MachineListCard_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_MachineListItem_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_SessionView_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_SessionListPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_SessionEditorPane_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_ExerciseCardEditor_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _0x5f_GymApp_qml_SetRowEditor_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/Main.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_Main_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/HeaderBar.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_HeaderBar_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/DatabaseCard.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_DatabaseCard_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/MachineListCard.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_MachineListCard_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/MachineListItem.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_MachineListItem_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/SessionView.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_SessionView_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/SessionListPage.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_SessionListPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/SessionEditorPane.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_SessionEditorPane_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/ExerciseCardEditor.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_ExerciseCardEditor_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/GymApp/qml/SetRowEditor.qml"), &QmlCacheGeneratedCode::_0x5f_GymApp_qml_SetRowEditor_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_GymApp)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_GymApp))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_GymApp)() {
    return 1;
}
