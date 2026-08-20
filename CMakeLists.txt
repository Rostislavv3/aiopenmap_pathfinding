cmake_minimum_required(VERSION 4.4.0)

project(PathFinder)

include(FetchContent)

FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.10.5 # A recent, stable version
)

FetchContent_MakeAvailable(cpr)

add_executable(MainApp)

target_sources(MainApp 
    PRIVATE
        main.cpp
    )

target_link_libraries(MainApp PUBLIC cpr::cpr)