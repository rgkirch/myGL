cmake_minimum_required (VERSION 2.6)
project (MyGL CXX)
set(MyGL_VERSION_MAJOR 0)
set(MyGL_VERSION_MINOR 0)
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(MyGL_FORWARD_COMPAT 1)
endif()

include_directories("${PROJECT_BINARY_DIR}")
add_executable(MyGL src/MyGL.cpp src/main.cpp src/MyGL.hpp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -std=c++14")
#set(CMAKE_CXX_FLAGS "-Wall -std=c++14" "pkg-config --cflags --static --libs glfw3")
set_property(TARGET MyGL PROPERTY CXX_STANDARD 14)
set_property(TARGET MyGL PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIRS})
  link_libraries(${OPENGL_LIBRARIES})
endif()

find_package(PNG REQUIRED)
if (PNG_FOUND)
  include_directories(${PNG_INCLUDE_DIRS})
  link_libraries(${PNG_LIBRARIES})
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(MyGL ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
#if (Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#  link_libraries(${Boost_LIBRARIES})
#endif()

#find_package(glfw3 REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
#if (glfw3_FOUND)
#  include_directories(${GLFW_INCLUDE_DIRS})
#  link_libraries(${GLFW_LIBRARIES})
#endif()

#include_directories(${GLFW_INCLUDE_DIRS})
#target_link_libraries(MyGL ${GLFW_LIBRARIES})

add_subdirectory(lib/glfw)
include_directories(/usr/include/GLFW)
target_link_libraries(MyGL glfw ${GLFW_LIBRARIES})
