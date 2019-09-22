cmake_minimum_required(VERSION 3.5)

project(Rio LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Sql Xml REQUIRED)

add_executable(Rio
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  parser.cpp
  parser.h
  book.cpp
  book.h
)

target_link_libraries(Rio PRIVATE Qt5::Widgets Qt5::Sql Qt5::Xml)
