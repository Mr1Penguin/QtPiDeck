find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick VirtualKeyboard REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick VirtualKeyboard REQUIRED)

add_executable(QtPiDeck
    src/main.cpp
    src/Models/AppConfiguration.cpp
    src/DataManager.cpp
    include/DataManager.hpp
    include/Utils.hpp
    include/Models/AppConfiguration.hpp
    icons.qrc
    qml.qrc)

target_link_libraries(QtPiDeck
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::VirtualKeyboard
    QtPiDeckLib)

target_include_directories(QtPiDeck
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

get_target_property(QtPiDeckCommon_SOURCE_DIR QtPiDeckCommon SOURCE_DIR)
set(QML_IMPORT_PATH ${QtPiDeckCommon_SOURCE_DIR}/qml/components CACHE STRING "Qt Creator qml import paths" FORCE)
