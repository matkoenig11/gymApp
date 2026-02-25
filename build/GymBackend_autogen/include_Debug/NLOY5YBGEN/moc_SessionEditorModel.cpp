/****************************************************************************
** Meta object code from reading C++ file 'SessionEditorModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../model/SessionEditorModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SessionEditorModel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN3gym18SessionEditorModelE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN3gym18SessionEditorModelE = QtMocHelpers::stringData(
    "gym::SessionEditorModel",
    "QML.Element",
    "SessionEditor",
    "QML.Singleton",
    "true",
    "countChanged",
    "",
    "sessionChanged",
    "exercisesChanged",
    "loadSession",
    "sessionId",
    "endSessionNow",
    "updateSessionNote",
    "note",
    "updateSessionStarted",
    "startedAt",
    "deleteSession",
    "addExercise",
    "QVariant",
    "machineId",
    "customName",
    "comment",
    "effortRir",
    "updateExercise",
    "exerciseId",
    "moveExercise",
    "toIndex",
    "removeExercise",
    "addSet",
    "reps",
    "weight",
    "updateSet",
    "setId",
    "removeSet",
    "setsForExercise",
    "QVariantList",
    "debugExercises",
    "sessionNote",
    "sessionStarted",
    "count"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN3gym18SessionEditorModelE[] = {

 // content:
      12,       // revision
       0,       // classname
       2,   14, // classinfo
      17,   18, // methods
       4,  183, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  120,    6, 0x06,    5 /* Public */,
       7,    0,  121,    6, 0x06,    6 /* Public */,
       8,    0,  122,    6, 0x06,    7 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       9,    1,  123,    6, 0x02,    8 /* Public */,
      11,    0,  126,    6, 0x02,   10 /* Public */,
      12,    1,  127,    6, 0x02,   11 /* Public */,
      14,    1,  130,    6, 0x02,   13 /* Public */,
      16,    0,  133,    6, 0x02,   15 /* Public */,
      17,    4,  134,    6, 0x02,   16 /* Public */,
      23,    5,  143,    6, 0x02,   21 /* Public */,
      25,    2,  154,    6, 0x02,   27 /* Public */,
      27,    1,  159,    6, 0x02,   30 /* Public */,
      28,    3,  162,    6, 0x02,   32 /* Public */,
      31,    3,  169,    6, 0x02,   36 /* Public */,
      33,    1,  176,    6, 0x02,   40 /* Public */,
      34,    1,  179,    6, 0x102,   42 /* Public | MethodIsConst  */,
      36,    0,  182,    6, 0x102,   44 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool, QMetaType::Int,   10,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString,   13,
    QMetaType::Bool, QMetaType::QString,   15,
    QMetaType::Bool,
    QMetaType::Int, 0x80000000 | 18, 0x80000000 | 18, 0x80000000 | 18, 0x80000000 | 18,   19,   20,   21,   22,
    QMetaType::Bool, QMetaType::Int, QMetaType::QString, QMetaType::Int, QMetaType::QString, 0x80000000 | 18,   24,   21,   19,   20,   22,
    QMetaType::Bool, QMetaType::Int, QMetaType::Int,   24,   26,
    QMetaType::Bool, QMetaType::Int,   24,
    QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Double,   24,   29,   30,
    QMetaType::Bool, QMetaType::Int, QMetaType::Int, QMetaType::Double,   32,   29,   30,
    QMetaType::Bool, QMetaType::Int,   32,
    0x80000000 | 35, QMetaType::Int,   24,
    0x80000000 | 35,

 // properties: name, type, flags, notifyId, revision
      10, QMetaType::Int, 0x00015001, uint(1), 0,
      37, QMetaType::QString, 0x00015001, uint(1), 0,
      38, QMetaType::QString, 0x00015001, uint(1), 0,
      39, QMetaType::Int, 0x00015001, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject gym::SessionEditorModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_meta_stringdata_ZN3gym18SessionEditorModelE.offsetsAndSizes,
    qt_meta_data_ZN3gym18SessionEditorModelE,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'sessionId'
        int,
        // property 'sessionNote'
        QString,
        // property 'sessionStarted'
        QString,
        // property 'count'
        int,
        // Q_OBJECT / Q_GADGET
        SessionEditorModel,
        // method 'countChanged'
        void,
        // method 'sessionChanged'
        void,
        // method 'exercisesChanged'
        void,
        // method 'loadSession'
        bool,
        int,
        // method 'endSessionNow'
        bool,
        // method 'updateSessionNote'
        bool,
        const QString &,
        // method 'updateSessionStarted'
        bool,
        const QString &,
        // method 'deleteSession'
        bool,
        // method 'addExercise'
        int,
        const QVariant &,
        const QVariant &,
        const QVariant &,
        const QVariant &,
        // method 'updateExercise'
        bool,
        int,
        const QString &,
        int,
        const QString &,
        const QVariant &,
        // method 'moveExercise'
        bool,
        int,
        int,
        // method 'removeExercise'
        bool,
        int,
        // method 'addSet'
        int,
        int,
        int,
        double,
        // method 'updateSet'
        bool,
        int,
        int,
        double,
        // method 'removeSet'
        bool,
        int,
        // method 'setsForExercise'
        QVariantList,
        int,
        // method 'debugExercises'
        QVariantList
    >,
    nullptr
} };

void gym::SessionEditorModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SessionEditorModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->sessionChanged(); break;
        case 2: _t->exercisesChanged(); break;
        case 3: { bool _r = _t->loadSession((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->endSessionNow();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->updateSessionNote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->updateSessionStarted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->deleteSession();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->addExercise((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->updateExercise((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->moveExercise((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->removeExercise((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->addSet((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->updateSet((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->removeSet((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { QVariantList _r = _t->setsForExercise((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 16: { QVariantList _r = _t->debugExercises();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (SessionEditorModel::*)();
            if (_q_method_type _q_method = &SessionEditorModel::countChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (SessionEditorModel::*)();
            if (_q_method_type _q_method = &SessionEditorModel::sessionChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (SessionEditorModel::*)();
            if (_q_method_type _q_method = &SessionEditorModel::exercisesChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->sessionId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->sessionNote(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->sessionStarted(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->rowCount(); break;
        default: break;
        }
    }
}

const QMetaObject *gym::SessionEditorModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gym::SessionEditorModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN3gym18SessionEditorModelE.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int gym::SessionEditorModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void gym::SessionEditorModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void gym::SessionEditorModel::sessionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void gym::SessionEditorModel::exercisesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
