
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(flowHedgehog)

find_package(VTK COMPONENTS 
  CommonColor
  CommonCore
  CommonDataModel
  FiltersCore
  FiltersGeometry
  FiltersGeneral
  FiltersModeling
  FiltersFlowPaths
  FiltersProgrammable
  IOGeometry
  IOImage
  ImagingCore
  ImagingHybrid
  InteractionStyle
  InteractionWidgets
  RenderingAnnotation
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingVolume
  RenderingVolumeOpenGL2
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "flowHedgehog: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(flowHedgehog MACOSX_BUNDLE flowHedgehog.cxx )
  target_link_libraries(flowHedgehog PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS flowHedgehog
  MODULES ${VTK_LIBRARIES}
)
