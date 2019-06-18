## ======================================================================== ##
## Copyright 2009-2018 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

option(BUILD_PLUGIN_VTKM_DEMO "VTKm demos plugin" OFF)

if (BUILD_PLUGIN_VTKM_DEMO)

  find_package(VTKm REQUIRED)

  add_library(ospray_studio_plugin_vtkm_demo SHARED
    createTangleField.cpp
    PanelVTKmDemo.cpp
    plugin_vtkm_demo.cpp
    vtkm_dataset_sg.cpp
  )

  target_link_libraries(ospray_studio_plugin_vtkm_demo
    PRIVATE vtkm_cont ospray_studio
  )

  target_include_directories(ospray_studio_plugin_vtkm_demo
    PRIVATE ${CMAKE_SOURCE_DIR}
  )

endif()
