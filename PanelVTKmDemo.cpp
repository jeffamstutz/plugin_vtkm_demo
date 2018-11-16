// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

// imgui
#include "../../app/widgets/sg_ui/ospray_sg_ui.h"
#include "imgui.h"
// jobs
#include "../../app/jobs/JobScheduler.h"
// app sg utilities

// ospray_sg

// jet_plugin
#include "PanelVTKmDemo.h"

namespace ospray {
  namespace vtkm_demo_plugin {

    PanelVTKmDemo::PanelVTKmDemo() : Panel("VTKmDemo Panel - Plugin") {}

    void PanelVTKmDemo::buildUI()
    {
      auto flags = g_defaultWindowFlags | ImGuiWindowFlags_AlwaysAutoResize;
      if (ImGui::Begin("VTKmDemo Panel", nullptr, flags)) {

        ImGui::Text("Volume Controls:");

        ImGui::NewLine();
        ImGui::Separator();

        ImGui::Text("Isosurface Controls:");
        ImGui::NewLine();

        if (ImGui::Button("Close"))
          setShown(false);

        ImGui::End();
      }
    }

  }  // namespace vtkm_demo_plugin
}  // namespace ospray
