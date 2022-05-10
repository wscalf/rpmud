cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
enable_testing()

project(rpmud VERSION 0.1)
configure_file(version.h.in version.h)

add_library(objects OBJECT)
add_library(tests OBJECT)

include_directories(.)
include_directories(external/googletest/googletest/include)
link_directories(external/googletest/build/lib)

add_subdirectory(doubles)
add_subdirectory(util)
add_subdirectory(network)
add_subdirectory(world)
add_subdirectory(scripting)

add_executable(rpmud Main.cpp $<TARGET_OBJECTS:objects>)
add_executable(rpmud_tests TestMain.cpp $<TARGET_OBJECTS:objects> $<TARGET_OBJECTS:tests>)

target_compile_definitions(rpmud_tests PRIVATE TEST_BUILD)

target_link_libraries(rpmud
    PUBLIC
        pthread
        telnet #move to network, requires libtelnet install
        event #may require libevent install - libevent was already on my system, but the headers were not. There is also a libevent_core that may have all we need
        uuid #depends on the uuid-dev package, can be statically linked
    )

target_link_libraries(rpmud_tests
    PUBLIC
        pthread
        telnet
        event
        uuid
        gtest #test library, compiled from googletest submodule
    )

add_test(NAME rpmud_tests COMMAND rpmud_tests)