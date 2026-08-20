cmake_minimum_required(VERSION 4.4.0)

project(PathFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.10.5 # A recent, stable version
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/osmium_dependencies")
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(Osmium REQUIRED COMPONENTS pbf)
FetchContent_MakeAvailable(cpr)

add_executable(MainApp)

target_sources(MainApp 
    PRIVATE
        main.cpp
    )

target_link_libraries(MainApp PUBLIC cpr::cpr ${OSMIUM_LIBRARIES})

target_include_directories(MainApp PRIVATE ${OSMIUM_INCLUDE_DIRS})
