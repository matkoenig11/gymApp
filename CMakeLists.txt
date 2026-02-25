cmake_minimum_required(VERSION 3.20)

project(GymApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Qml Sql Test)

# Backend (shared between desktop and mobile)
add_library(GymBackend STATIC
    core/Backend.cpp
    core/MachineRepository.cpp
    core/SessionRepository.cpp
    core/ExerciseRepository.cpp
    core/SetRepository.cpp
    core/Clogger.cpp
    model/MachineListModel.cpp
    model/LoaderModel.cpp
    model/SessionListModel.cpp
    model/SessionDetailModel.cpp
    model/SessionEditorModel.cpp
)

target_include_directories(GymBackend PUBLIC
    core
    model
)

target_link_libraries(GymBackend PUBLIC Qt6::Core Qt6::Sql Qt6::Qml)

# App
qt_add_executable(GymApp
    app/main.cpp
)

set_property(TARGET GymApp PROPERTY OUTPUT_NAME "gymapp_bin")

qt_add_qml_module(GymApp
    URI GymApp
    VERSION 1.0
    OUTPUT_DIRECTORY "qt/qml/GymApp"
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        qml/Main.qml
        qml/HeaderBar.qml
        qml/DatabaseCard.qml
        qml/MachineListCard.qml
        qml/MachineListItem.qml
        qml/SessionView.qml
        qml/SessionListPage.qml
        qml/SessionEditorPane.qml
        qml/ExerciseCardEditor.qml
        qml/SetRowEditor.qml
    SOURCES
        core/Backend.h
        core/MachineRepository.h
        core/SessionRepository.h
        core/ExerciseRepository.h
        core/SetRepository.h
        core/Clogger.h
        model/MachineListModel.h
        model/LoaderModel.h
        model/SessionListModel.h
        model/SessionDetailModel.h
        model/SessionEditorModel.h
    NO_PLUGIN
)

qt_add_resources(GymApp "data"
    PREFIX "/data"
    BASE "data"
    FILES
        data/machines.json
        data/gym_21_1_2026.json
)

qt_add_resources(GymApp "controls"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(GymApp PRIVATE GymBackend Qt6::Quick Qt6::QuickControls2 Qt6::Sql)

enable_testing()

add_executable(SessionDetailModelTest
    tests/SessionDetailModelTest.cpp
)
target_link_libraries(SessionDetailModelTest PRIVATE GymBackend Qt6::Test Qt6::Sql Qt6::Core Qt6::Qml)
add_test(NAME SessionDetailModelTest COMMAND SessionDetailModelTest)

add_executable(RepositoryCrudTest
    tests/RepositoryCrudTest.cpp
)
target_link_libraries(RepositoryCrudTest PRIVATE GymBackend Qt6::Test Qt6::Sql Qt6::Core Qt6::Qml)
add_test(NAME RepositoryCrudTest COMMAND RepositoryCrudTest)

add_executable(SessionEditorModelInvokeTest
    tests/SessionEditorModelInvokeTest.cpp
)
target_link_libraries(SessionEditorModelInvokeTest PRIVATE GymBackend Qt6::Test Qt6::Sql Qt6::Core Qt6::Qml)
add_test(NAME SessionEditorModelInvokeTest COMMAND SessionEditorModelInvokeTest)
