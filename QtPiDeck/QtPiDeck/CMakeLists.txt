find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)

add_executable(QtPiDeck
    src/main.cpp)

target_link_libraries(QtPiDeck
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick
    QtPiDeckLib)

target_include_directories(QtPiDeck
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

get_target_property(QtPiDeckCommon_SOURCE_DIR QtPiDeckCommon SOURCE_DIR)
set(QML_IMPORT_PATH ${QtPiDeckCommon_SOURCE_DIR}/qml/components CACHE STRING "Qt Creator qml import paths" FORCE)
