cmake_minimum_required(VERSION 3.16)

project(O-Serialize LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3rdparty libraries
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

# Qt Support
find_package(Qt5 COMPONENTS Core Widgets)
if(Qt5_FOUND)
    message(STATUS "Qt5 found. Enabling Qt support.")
    add_definitions(-DO_SERIALIZE_USE_QT)
else()
    message(STATUS "Qt5 not found. Qt support disabled.")
endif()

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Add 3rdparty sources that need compilation
list(APPEND SOURCES "3rdparty/tinyxml/tinyxml2.cpp")

file(GLOB_RECURSE HEADERS "include/*.h")

# Library target
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
)

if(Qt5_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Widgets)
endif()

# Tests (Optional)
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
