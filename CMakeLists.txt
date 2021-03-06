cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0048 NEW)
#cmake_policy(SET CMP0010 NEW)

project (MyGL)
set(CMAKE_VERBOSE_MAKEFILE on)

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#include_directories("${PROJECT_SOURCE_DIR}")

#set(MyGL_VERSION_MAJOR 0)
#set(MyGL_VERSION_MINOR 0)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#configure_file("${PROJECT_SOURCE_DIR}/src/Config.h.in" "${PROJECT_BINARY_DIR}/src/Config.h")
#set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
#add_executable(MyGL ${PROJECT_SOURCE_DIR}/MyGL.cpp ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/MyGL.hpp)

#if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#  set(MyGL_FORWARD_COMPAT 1)
#endif()

#include_directories("${PROJECT_BINARY_DIR}")
include_directories("include")
file(GLOB SOURCES "src/*.cpp")
add_executable(MyGL ${SOURCES})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Og -Wall -std=c++14")
#set(CMAKE_CXX_FLAGS "-Wall -std=c++14" "pkg-config --cflags --static --libs glfw3")
set_property(TARGET MyGL PROPERTY CXX_STANDARD 14)
set_property(TARGET MyGL PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
if (GLFW_FOUND)
    message("found glfw")
    include_directories(${GLFW_INCLUDE_DIRS})
    message("include glfw ${GLFW_INCLUDE_DIRS}")
    #target_link_libraries(MyGL ${GLFW_LIBRARIES})
    message("link glfw ${GLFW_LIBRARIES}")
    message("glfw cflags ${GLFW_CFLAGS}")
    message("glfw ldflags ${GLFW_LDFLAGS}")
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    message("include opengl ${OPENGL_INCLUDE_DIRS}")
    message("link opengl ${OPENGL_LIBRARIES}")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    message("include glew ${GLEW_INCLUDE_DIRS}")
    message("link glew ${GLEW_LIBRARIES}")
endif()

find_package(X11 REQUIRED)


#find_package(Threads REQUIRED)
#if(THREADS_HAVE_PTHREAD_ARG)
#    set_property(TARGET MyGL PROPERTY COMPILE_OPTIONS "-pthread")
#    set_property(TARGET MyGL PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
#endif()
#if(CMAKE_THREAD_LIBS_INIT)
#    target_link_libraries(MyGL "${CMAKE_THREAD_LIBS_INIT}")
#endif()

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#find_package(ImageMagick REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)
if (ImageMagick_FOUND)
    include_directories(${ImageMagick_INCLUDE_DIRS})
    message("include ImageMagick ${ImageMagick_INCLUDE_DIRS}")
    message("link ImageMagick ${ImageMagick_LIBRARIES}")
endif()

find_package(Threads REQUIRED)
if(Threads_FOUND)
  message("found Threads")
endif()

if(THREADS_HAVE_PTHREAD_ARG)
  set_property(TARGET MyGL PROPERTY COMPILE_OPTIONS "-pthread")
  set_property(TARGET MyGL PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(MyGL "${CMAKE_THREAD_LIBS_INIT}")
endif()

find_package( OpenCV REQUIRED )
message("opencv libs: ${OpenCV_LIBS}")

#include_directories(${GLFW_INCLUDE_DIRS})
#target_link_libraries(MyGL ${GLFW_LIBRARIES})

#add_subdirectory(lib/glfw)
#include_directories(/usr/include/GLFW)
#target_link_libraries(MyGL glfw ${GLFW_LIBRARIES})

#target_link_libraries(MyGL -lGLEW -lGL -lGLU -lpthread -lglfw3)

message("boost include dir " ${Boost_INCLUDE_DIR} )
message("boost libraries " ${Boost_LIBRARIES} )

target_link_libraries(MyGL -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 ${Boost_FILESYSTEM_LIBRARY} ${Boost_LIBRARIES} ${GLFW_CFLAGS} ${GLFW_LDFLAGS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${ImageMagick_LIBRARIES} ${OpenCV_LIBS} -fext-numeric-literals -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lGLEW -lXrandr -lXinerama  -lXxf86vm -lXcursor)
