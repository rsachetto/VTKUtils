cmake_minimum_required(VERSION 2.8)

PROJECT(CenterOfMass)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(CenterOfMass main.cpp)

if(VTK_LIBRARIES)
    target_link_libraries(CenterOfMass ${VTK_LIBRARIES})
else()
    target_link_libraries(CenterOfMass vtkHybrid vtkWidgets)
endif()