set(TESTS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

macro(add_cpp_test PATH)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

    get_test_name(TEST_NAME ${PATH})
    get_test_exe_name(EXE_NAME ${TEST_NAME})

    add_executable(${EXE_NAME} ${PATH}/main.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME} -platform offscreen)

    target_link_libraries(${EXE_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        QtPiDeckLib)
endmacro()

add_subdirectory(QtPiDeck/ViewModels/SettingsViewModelTests)
