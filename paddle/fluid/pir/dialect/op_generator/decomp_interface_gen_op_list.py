# Copyright (c) 2023 PaddlePaddle Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# =====================================
# DecompInterface gen op list
# =====================================

# come into effect in generated file pd_op.h
# manual decomp interface declare are located in manual_op.h
decomp_interface_declare_gen_op_list = [
    "add_n",
    "any",
    "batch_norm",
    "batch_norm_",
    "bce_loss",
    "bmm",
    "dropout",
    "elu",
    "embedding",
    "flatten",
    "floor_divide",
    "full_like",
    "gelu",
    "hardswish",
    "hardsigmoid",
    "group_norm",
    "index_sample",
    "index_select",
    "instance_norm",
    "layer_norm",
    "leaky_relu",
    "log_softmax",
    "mean",
    "meshgrid",
    "one_hot",
    "p_norm",
    "pow",
    "reciprocal",
    "relu",
    "relu6",
    "silu",
    "swiglu",
    "softmax",
    "square",
    "squeeze",
    "stack",
    "unsqueeze",
    "huber_loss",
]

# come into effect in generated file op_decomp.cc
# manual decomp interface implementation are located in manual_op_decomp.cc
decomp_interface_implementation_gen_op_list = [
    "any",
    "add_n",
    "bce_loss",
    "bmm",
    "dropout",
    "elu",
    "embedding",
    "flatten",
    "floor_divide",
    "full_like",
    "gelu",
    "hardswish",
    "hardsigmoid",
    "group_norm",
    "index_sample",
    "index_select",
    "instance_norm",
    "layer_norm",
    "leaky_relu",
    "log_softmax",
    "mean",
    "meshgrid",
    "p_norm",
    "pow",
    "reciprocal",
    "relu",
    "relu6",
    "silu",
    "swiglu",
    "softmax",
    "square",
    "squeeze",
    "stack",
    "unsqueeze",
    "huber_loss",
]

# xshape output will no longer used after decomp, but return none to keep output num the same as origin op
decomp_ops_contain_unused_output = ["squeeze", "unsqueeze"]

decomp_vjp_interface_declare_gen_op_list = [
    "add_grad",
    "matmul_grad",
    "relu_grad",
]
