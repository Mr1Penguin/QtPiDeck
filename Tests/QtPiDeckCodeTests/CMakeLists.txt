find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_executable(QtPiDeckCodeTests 
               src/main.cpp
               src/ViewModels/SettingsViewModelTests.cpp)
add_test(NAME QtPiDeckCodeTests COMMAND QtPiDeckCodeTests)
target_link_libraries(QtPiDeckCodeTests
    PRIVATE
    BoostTestWrapper
    CodeTestsUtils
    QtPiDeckLib)

target_compile_definitions(
    QtPiDeckCodeTests 
    PRIVATE
    BOOST_TEST_DYN_LINK)

# visual studio test manager requires to have these libs in PATH
# so we just copy required libs to test binary
if(VISUAL_STUDIO)    
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Qml Gui Network REQUIRED)
    add_custom_command(
        TARGET QtPiDeckCodeTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:QtPiDeckCommon>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Core>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Qml>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Gui>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Network>
        $<TARGET_FILE_DIR:QtPiDeckCommonCodeTests>)
endif()
