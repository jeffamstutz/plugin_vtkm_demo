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

#include "createTangleField.h"
// ospcommon
#include <ospray/ospcommon/box.h>
// std
#include <memory>

namespace ospray {
  namespace vtkm_demo_plugin {

    // Define the tangle field for the input data
    class TangleField : public vtkm::worklet::WorkletMapField
    {
     public:
      using ControlSignature   = void(FieldIn<IdType> vertexId,
                                    FieldOut<Scalar> v);
      using ExecutionSignature = void(_1, _2);
      using InputDomain        = _1;

      const vec3i dims;
      const box3f bounds;
      const vtkm::Id cellsPerLayer;

      VTKM_CONT
      TangleField(const vec3i _dims, const box3f _bounds)
          : dims(_dims), bounds(_bounds), cellsPerLayer((dims.x) * (dims.y)){};

      VTKM_EXEC
      void operator()(const vtkm::Id &vertexId, vtkm::Float32 &v) const
      {
        // just a call to 'reshape()'...?
        const vec3i xyz(vertexId % (dims.x),
                        (vertexId / (dims.x)) % (dims.y),
                        vertexId / cellsPerLayer);

        const vec3f fxyz   = xyz / float(dims.x - 1);
        const vec3f xxyyzz = 3.f * (bounds.lower + bounds.size() * fxyz);

        const vec3f xyz_2 = xxyyzz * xxyyzz;
        const vec3f xyz_4 = xyz_2 * xyz_2;
        v = (xyz_4.x - 5.0f * xyz_2.x + xyz_4.y - 5.0f * xyz_2.y + xyz_4.z -
             5.0f * xyz_2.z + 11.8f) *
                0.2f +
            0.5f;
      }
    };

    // Construct an input data set using the tangle field worklet
    std::unique_ptr<vtkm::cont::DataSet> createTangleField(vec3i dims)
    {
      using namespace vtkm::cont;

      auto dataSet = std::make_unique<DataSet>();

      const vec3i vdims = dims + 1;
      const vec3f idims = 1.f / dims;
      const box3f bounds{vec3f(-1.f), vec3f(1.f)};

      ArrayHandle<vtkm::Float32> fieldArray;
      ArrayHandleCounting<vtkm::Id> vertexCountImplicitArray(
          0, 1, vdims.product());
      vtkm::worklet::DispatcherMapField<TangleField> tangleFieldDispatcher(
          TangleField(vdims, bounds));
      tangleFieldDispatcher.Invoke(vertexCountImplicitArray, fieldArray);

      vtkm::Vec<vtkm::FloatDefault, 3> origin(0.0f, 0.0f, 0.0f);
      vtkm::Vec<vtkm::FloatDefault, 3> spacing(idims.x, idims.y, idims.z);

      ArrayHandleUniformPointCoordinates coordinates(
          {vdims.x, vdims.y, vdims.z}, origin, spacing);
      dataSet->AddCoordinateSystem(
          CoordinateSystem("coordinates", coordinates));
      dataSet->AddField(
          Field("field1", Field::Association::POINTS, fieldArray));

      static const vtkm::IdComponent ndim = 3;
      CellSetStructured<ndim> cellSet("cells");
      cellSet.SetPointDimensions({vdims.x, vdims.y, vdims.z});
      dataSet->AddCellSet(cellSet);

      return dataSet;
    }

  }  // namespace vtkm_demo_plugin
}  // namespace ospray