/*Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "ngraph/ngraph.hpp"
#include "paddle/fluid/operators/ngraph/ops/op_bridge.h"
#include "paddle/fluid/platform/ngraph_helper.h"

namespace paddle {
namespace operators {
namespace ngraphs {

void BuildTopKNode(
    const std::shared_ptr<paddle::framework::OperatorBase>& op,
    std::shared_ptr<
        std::unordered_map<std::string, std::shared_ptr<ngraph::Node>>>
        ngb_node_map) {
  auto op_attrs = paddle::framework::AttrReader(op->Attrs());
  int k = op_attrs.Get<int>("k");
  auto input = paddle::platform::GetInputNode(op, "X", ngb_node_map);
  auto top_k = std::make_shared<ngraph::op::TopK>(
      input, input->get_shape().size() - 1, ngraph::element::i64, k);
  std::shared_ptr<ngraph::Node> indices =
      std::make_shared<ngraph::op::GetOutputElement>(top_k, 0);
  std::shared_ptr<ngraph::Node> out =
      std::make_shared<ngraph::op::GetOutputElement>(top_k, 1);
  paddle::platform::SetOutputNode(op, "Indices", indices, ngb_node_map);
  paddle::platform::SetOutputNode(op, "Out", out, ngb_node_map);
}
}  // namespace ngraphs
}  // namespace operators
}  // namespace paddle

REGISTER_NG_OP(top_k, BuildTopKNode);
