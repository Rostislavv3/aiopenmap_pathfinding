cmake_minimum_required(VERSION 4.4.0)

project(PathFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/osmium_dependencies")
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(Osmium REQUIRED COMPONENTS pbf)
find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)

add_executable(GraphBuilder) #adds graph builder
target_sources(GraphBuilder 
    PRIVATE
        ./src/builder/graph_builder.cpp
        ./src/builder/WayHighwayExtractor.cpp
    )

target_include_directories(GraphBuilder #includes the path to osmium
    PRIVATE 
        ${OSMIUM_INCLUDE_DIRS} 
        "/opt/homebrew/include"
)

target_link_libraries(GraphBuilder #links osmium to GraphBuilder
    PRIVATE
        ${OSMIUM_LIBRARIES}
        ZLIB::ZLIB
        EXPAT::EXPAT
)

add_executable(MainApp) #adds router
target_sources(MainApp
    PRIVATE
        ./src/router/main.cpp
    )

