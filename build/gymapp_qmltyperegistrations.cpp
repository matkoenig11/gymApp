/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<Backend.h>)
#  include <Backend.h>
#endif
#if __has_include(<LoaderModel.h>)
#  include <LoaderModel.h>
#endif
#if __has_include(<MachineListModel.h>)
#  include <MachineListModel.h>
#endif
#if __has_include(<SessionDetailModel.h>)
#  include <SessionDetailModel.h>
#endif
#if __has_include(<SessionEditorModel.h>)
#  include <SessionEditorModel.h>
#endif
#if __has_include(<SessionListModel.h>)
#  include <SessionListModel.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_GymApp()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<gym::Backend>("GymApp", 1);
    qmlRegisterTypesAndRevisions<gym::LoaderModel>("GymApp", 1);
    qmlRegisterTypesAndRevisions<gym::MachineListModel>("GymApp", 1);
    qmlRegisterAnonymousType<QAbstractItemModel, 254>("GymApp", 1);
    qmlRegisterTypesAndRevisions<gym::SessionDetailModel>("GymApp", 1);
    qmlRegisterTypesAndRevisions<gym::SessionEditorModel>("GymApp", 1);
    qmlRegisterTypesAndRevisions<gym::SessionListModel>("GymApp", 1);
    QT_WARNING_POP
    qmlRegisterModule("GymApp", 1, 0);
}

static const QQmlModuleRegistration gymAppRegistration("GymApp", qml_register_types_GymApp);
