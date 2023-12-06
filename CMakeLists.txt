cmake_minimum_required(VERSION 3.26)
project(CLISQL)

set(CMAKE_CXX_STANDARD 17)
find_package(Curses REQUIRED)
find_package(SQLite3 REQUIRED)
add_executable(CLISQL main.cpp
        CLI.cpp
        CLI.h
        SQL.cpp
        SQL.h
        Controller.cpp
        Controller.h
        TableInfo.h)
target_include_directories(CLISQL PRIVATE ${CURSES_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS})
target_link_libraries(CLISQL  ${CURSES_LIBRARIES}  ${SQLite3_LIBRARIES})

