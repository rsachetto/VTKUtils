cmake_minimum_required(VERSION 2.6)
 
project(HexaMeshFromSurface)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(HexaMeshFromSurface src/main.cpp)


if(VTK_LIBRARIES)
  target_link_libraries(HexaMeshFromSurface ${VTK_LIBRARIES})
else()
  target_link_libraries(HexaMeshFromSurface vtkHybrid)
endif()
