cmake_minimum_required(VERSION 3.14)

project(HelloWorldLibtorrent LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(HelloWorldLibtorrent
  main.cpp
  SessionWrapper.hpp
  DhtRequestHandler.hpp
  SessionWrapperDelegate.hpp
  log.hpp
)
include_directories(C:/Users/Admin/source/repos/QtProjects/libtorrent/include)
find_package(Boost 1.79.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(HelloWorldLibtorrent Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(HelloWorldLibtorrent C:/Users/Admin/source/repos/QtProjects/build-libtorrent_MSVC/torrent-rasterbar.lib )
