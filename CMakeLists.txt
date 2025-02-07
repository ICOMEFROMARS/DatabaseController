cmake_minimum_required(VERSION 3.30)
project(DatabaseController LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/msvc2019_64/lib/cmake")

find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Concurrent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(DatabaseController src/main.cpp src/DatabaseController.cpp src/mainwindow.cpp)

target_link_libraries(DatabaseController PRIVATE Qt5::Core Qt5::Sql Qt5::Widgets Qt5::Concurrent)
