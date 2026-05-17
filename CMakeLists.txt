cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(opt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(opt main.cc ${sources} ${headers})

target_link_libraries(opt ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


# Copy macro files to build directory
configure_file(${CMAKE_SOURCE_DIR}/vis.mac ${CMAKE_BINARY_DIR}/vis.mac COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/run.mac ${CMAKE_BINARY_DIR}/run.mac COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/ele.mac ${CMAKE_BINARY_DIR}/ele.mac COPYONLY)