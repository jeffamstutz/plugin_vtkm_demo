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

#include "vtkm_dataset_sg.h"
// app sg utilities
#include "../../app/sg_utility/utility.h"
// ospray_sg
#include "ospray/sg/common/Data.h"

namespace ospray {
  namespace vtkm_demo_plugin {

    std::shared_ptr<sg::Node> createVolumeNode(const vtkm::cont::DataSet &data)
    {
      auto volume_node = sg::createNode("vtkmVolume", "StructuredVolume");

      using ArrayType = vtkm::cont::ArrayHandle<vtkm::Float32>;
      auto &array_storage =
          data.GetField("field1").GetData().Cast<ArrayType>().GetStorage();

      // This stealing only works with VTK-m allocated memory. If the memory
      // that vtk-m is holding is actually complex, we need to grab the free
      // function and propagate that over to ospray ( use GetDeleteFunction )
      auto voxel_data = std::make_shared<sg::DataArray1f>(
          array_storage.StealArray(), array_storage.GetNumberOfValues(), true);
      voxel_data->setName("voxelData");

      volume_node->add(voxel_data);

      const auto &cellSet =
          data.GetCellSet("cells").Cast<vtkm::cont::CellSetStructured<3>>();
      const auto dims = cellSet.GetPointDimensions();

      volume_node->child("voxelType")  = std::string("float");
      volume_node->child("dimensions") = vec3i(dims[0], dims[1], dims[2]);

      replaceAllTFsWithMasterTF(*volume_node);

      return volume_node;
    }

  }  // namespace vtkm_demo_plugin
}  // namespace ospray
