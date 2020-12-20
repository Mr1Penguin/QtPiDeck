find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)

add_library(QtPiDeckLib STATIC
  src/ClientApplication.cpp
  src/Network/DeckClient.cpp
  include/ClientApplication.hpp
  include/Network/DeckClient.hpp
  icons.qrc
  qml.qrc)

target_include_directories(QtPiDeckLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(QtPiDeckLib
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Quick
    QtPiDeckCommon)
