FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_comms.cpp
    test_config_loader.cpp
)

target_link_libraries(unit_tests PRIVATE
    comms_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
