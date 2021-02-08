find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)

add_library(QtPiDeckLib STATIC
  src/ClientApplication.cpp
  src/Network/DeckClient.cpp
  src/Services/SettingsStorage.cpp
  src/ViewModels/SettingsViewModel.cpp
  include/ClientApplication.hpp
  include/Services/IClientSettingsStorage.hpp
  include/Network/DeckClient.hpp
  include/Services/SettingsStorage.hpp
  include/ViewModels/SettingsViewModel.hpp
  qml.qrc)

target_include_directories(QtPiDeckLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(QtPiDeckLib
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Quick
    QtPiDeckCommon)

get_target_property(QtPiDeckCommon_SOURCE_DIR QtPiDeckCommon SOURCE_DIR)
set(QML_IMPORT_PATH ${QtPiDeckCommon_SOURCE_DIR}/qml/components CACHE STRING "Qt Creator qml import paths" FORCE)
