/****************************************************************************
** Meta object code from reading C++ file 'Backend.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../core/Backend.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Backend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN3gym7BackendE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN3gym7BackendE = QtMocHelpers::stringData(
    "gym::Backend",
    "QML.Singleton",
    "true",
    "QML.Element",
    "Backend",
    "machinesResourceUrl",
    "",
    "sampleSessionResourceUrl",
    "defaultSqliteConfig",
    "DatabaseConfig",
    "defaultSqlitePath",
    "defaultConnectionName",
    "hasSeeds",
    "connectionName",
    "currentDatabasePath",
    "inspectSqliteFile",
    "QVariantMap",
    "filePath",
    "switchSqliteDatabase"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN3gym7BackendE[] = {

 // content:
      12,       // revision
       0,       // classname
       2,   14, // classinfo
      12,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   90,    6, 0x102,    1 /* Public | MethodIsConst  */,
       7,    0,   91,    6, 0x102,    2 /* Public | MethodIsConst  */,
       8,    0,   92,    6, 0x102,    3 /* Public | MethodIsConst  */,
      10,    0,   93,    6, 0x102,    4 /* Public | MethodIsConst  */,
      11,    0,   94,    6, 0x102,    5 /* Public | MethodIsConst  */,
      12,    1,   95,    6, 0x102,    6 /* Public | MethodIsConst  */,
      12,    0,   98,    6, 0x122,    8 /* Public | MethodCloned | MethodIsConst  */,
      14,    1,   99,    6, 0x102,    9 /* Public | MethodIsConst  */,
      14,    0,  102,    6, 0x122,   11 /* Public | MethodCloned | MethodIsConst  */,
      15,    1,  103,    6, 0x102,   12 /* Public | MethodIsConst  */,
      18,    2,  106,    6, 0x02,   14 /* Public */,
      18,    1,  111,    6, 0x22,   17 /* Public | MethodCloned */,

 // methods: parameters
    QMetaType::QUrl,
    QMetaType::QUrl,
    0x80000000 | 9,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Bool, QMetaType::QString,   13,
    QMetaType::Bool,
    QMetaType::QString, QMetaType::QString,   13,
    QMetaType::QString,
    0x80000000 | 16, QMetaType::QString,   17,
    0x80000000 | 16, QMetaType::QString, QMetaType::QString,   17,   13,
    0x80000000 | 16, QMetaType::QString,   17,

       0        // eod
};

Q_CONSTINIT const QMetaObject gym::Backend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN3gym7BackendE.offsetsAndSizes,
    qt_meta_data_ZN3gym7BackendE,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // Q_OBJECT / Q_GADGET
        Backend,
        // method 'machinesResourceUrl'
        QUrl,
        // method 'sampleSessionResourceUrl'
        QUrl,
        // method 'defaultSqliteConfig'
        DatabaseConfig,
        // method 'defaultSqlitePath'
        QString,
        // method 'defaultConnectionName'
        QString,
        // method 'hasSeeds'
        bool,
        const QString &,
        // method 'hasSeeds'
        bool,
        // method 'currentDatabasePath'
        QString,
        const QString &,
        // method 'currentDatabasePath'
        QString,
        // method 'inspectSqliteFile'
        QVariantMap,
        const QString &,
        // method 'switchSqliteDatabase'
        QVariantMap,
        const QString &,
        const QString &,
        // method 'switchSqliteDatabase'
        QVariantMap,
        const QString &
    >,
    nullptr
} };

void gym::Backend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Backend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QUrl _r = _t->machinesResourceUrl();
            if (_a[0]) *reinterpret_cast< QUrl*>(_a[0]) = std::move(_r); }  break;
        case 1: { QUrl _r = _t->sampleSessionResourceUrl();
            if (_a[0]) *reinterpret_cast< QUrl*>(_a[0]) = std::move(_r); }  break;
        case 2: { DatabaseConfig _r = _t->defaultSqliteConfig();
            if (_a[0]) *reinterpret_cast< DatabaseConfig*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->defaultSqlitePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->defaultConnectionName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->hasSeeds((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->hasSeeds();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { QString _r = _t->currentDatabasePath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 8: { QString _r = _t->currentDatabasePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 9: { QVariantMap _r = _t->inspectSqliteFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 10: { QVariantMap _r = _t->switchSqliteDatabase((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 11: { QVariantMap _r = _t->switchSqliteDatabase((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *gym::Backend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gym::Backend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN3gym7BackendE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int gym::Backend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
