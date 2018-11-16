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

// VTKm plugin
#include "PanelVTKmDemo.h"
#include "createTangleField.h"

namespace ospray {
  namespace vtkm_demo_plugin {

    PanelVTKmDemo::PanelVTKmDemo() : Panel("VTKm Demo Panel - Plugin") {}

    void PanelVTKmDemo::buildUI()
    {
      auto flags = g_defaultWindowFlags | ImGuiWindowFlags_AlwaysAutoResize;
      if (ImGui::Begin("VTKm Demo Panel", nullptr, flags)) {
        ImGui::Text("Data Generation Controls:");

        ImGui::DragInt3("volume dimensions", (int *)&dims.x, .01f);
        if (ImGui::Button("Create Tangle Field")) {
          job_scheduler::scheduleJob([&]() {
            job_scheduler::Nodes retval;
            dataset   = createTangleField(dims);
            auto node = createVolumeNode(*dataset);
            retval.push_back(node);
            return retval;
          });
        }

        ImGui::NewLine();
        ImGui::Separator();

        ImGui::Text("Isosurface Controls:");
        ImGui::NewLine();

        ImGui::Text("TODO:");
        ImGui::Text("  - find a volume in the scene");
        ImGui::Text("  - define isosurface(s) for the found volume");
        ImGui::Text("  - invoke VTKm marching cubes filter");
        ImGui::Text("  - add newly generated triangle mesh to the scene");

#if 0
        vtkm::filter::MarchingCubes filter;
        filter.SetGenerateNormals(true);
        filter.SetMergeDuplicatePoints(false);
        filter.SetIsoValue(0, 0.1);
        filter.SetActiveField("field1");
        filter.SetFieldsToPass({ "field1" });

        vtkm::cont::DataSet outputData = filter.Execute(dataSet);
#endif

        ImGui::NewLine();

        if (ImGui::Button("Close"))
          setShown(false);

        ImGui::End();
      }
    }

  }  // namespace vtkm_demo_plugin
}  // namespace ospray
