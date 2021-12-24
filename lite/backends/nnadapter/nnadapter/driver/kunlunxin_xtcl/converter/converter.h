// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "driver/kunlunxin_xtcl/utility.h"
#include "utility/debug.h"
#include "utility/string.h"

namespace nnadapter {
namespace kunlunxin_xtcl {

class Converter {
 public:
  explicit Converter(xtcl::network::xNetworkBuilder* builder,
                     xtcl::network::xTensorCompiler::ParamNDArrayMap* params,
                     std::map<hal::Operand*, std::vector<xtcl::xExpr>>* exprs)
      : builder_(builder), params_(params), exprs_(exprs) {}
  ~Converter() {}

  // Convert a NNAdapter model to XTC network and exprs
  int Apply(hal::Model* model);
  xtcl::network::xNetworkBuilder* builder() { return builder_; }
  std::string GetUniqueName(const std::string& suffix = "");
  // Mapping a XTCL expr to a NNAdapter operand
  xtcl::xExpr GetMappedExpr(hal::Operand* operand);
  xtcl::xExpr UpdateExprMap(hal::Operand* operand, xtcl::xExpr expr);
  xtcl::xExpr AddInputTensor(const std::string& name,
                             NNAdapterOperandPrecisionCode precision,
                             const int32_t* dimensions_data,
                             uint32_t dimensions_count);
  xtcl::xExpr AddInputTensor(const std::string& name,
                             NNAdapterOperandPrecisionCode precision,
                             const std::vector<int32_t>& dimensions);
  xtcl::xExpr AddConstantTensor(const void* values,
                                NNAdapterOperandPrecisionCode precision,
                                const std::vector<int32_t>& dimensions = {},
                                std::string name = "");
  xtcl::xExpr AddInt32ConstantTensor(const int32_t* values,
                                     const std::vector<int32_t>& dimensions,
                                     const std::string& name = "");
  xtcl::xExpr AddInt32ConstantTensor(
      const std::vector<int32_t>& values,
      const std::vector<int32_t>& dimensions = {},
      const std::string& name = "");
  xtcl::xExpr AddFloat32ConstantTensor(const float* values,
                                       const std::vector<int32_t>& dimensions,
                                       const std::string& name = "");
  xtcl::xExpr AddFloat32ConstantTensor(
      const std::vector<float>& values,
      const std::vector<int32_t>& dimensions = {},
      const std::string& name = "");
  // Convert a constant and model input operand and map to a XTCL expr
  xtcl::xExpr ConvertOperand(hal::Operand* operand,
                             std::vector<int32_t> dimensions = {},
                             const std::string& name = "");

 private:
  xtcl::network::xNetworkBuilder* builder_{nullptr};
  xtcl::network::xTensorCompiler::ParamNDArrayMap* params_{nullptr};
  std::map<hal::Operand*, std::vector<xtcl::xExpr>>* exprs_{nullptr};
  // Only for generating the unique name for XTCL expr
  uint32_t expr_index_{0};
};

}  // namespace kunlunxin_xtcl
}  // namespace nnadapter