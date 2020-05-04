cmake_minimum_required(VERSION 2.8)
 
PROJECT(DumpXMLFile)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(DumpXMLFile MACOSX_BUNDLE DumpXMLFile)
 
if(VTK_LIBRARIES)
  target_link_libraries(DumpXMLFile ${VTK_LIBRARIES})
else()
  target_link_libraries(DumpXMLFile vtkHybrid vtkWidgets)
endif()
