cmake_minimum_required(VERSION 3.5)

project(riddle LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(Qt5Core)
find_package(Qt5Sql)



file(GLOB SOURCES
    *.h
    *.cpp
    *.qrc)


add_executable(riddle ${SOURCES})



target_compile_definitions(riddle
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:DEBUG_MODE>)


target_link_libraries(riddle
    Qt5::Core
    Qt5::Sql)
