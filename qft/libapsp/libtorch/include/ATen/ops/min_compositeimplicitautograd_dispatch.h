#pragma once
// @generated by torchgen/gen.py from DispatchKeyFunction.h

// NB: The implementing C++ file is RegisterDispatchKey.cpp

// The only #includes we need are for custom classes that have defaults in the C++ API
#include <c10/core/MemoryFormat.h>
#include <c10/core/Scalar.h>
#include <ATen/core/Reduction.h>

// Forward declarations of any types needed in the operator signatures.
// We can't directly include these classes because it will cause circular include dependencies.
// This file is included by TensorBody.h, which defines the Tensor class.
#include <ATen/core/ATen_fwd.h>

namespace at {

namespace compositeimplicitautograd {

TORCH_API ::std::tuple<at::Tensor,at::Tensor> min(const at::Tensor & self, at::Dimname dim, bool keepdim=false);
TORCH_API ::std::tuple<at::Tensor &,at::Tensor &> min_out(at::Tensor & min, at::Tensor & min_indices, const at::Tensor & self, at::Dimname dim, bool keepdim=false);
TORCH_API ::std::tuple<at::Tensor &,at::Tensor &> min_outf(const at::Tensor & self, at::Dimname dim, bool keepdim, at::Tensor & min, at::Tensor & min_indices);
TORCH_API at::Tensor min(const at::Tensor & self, const at::Tensor & other);
TORCH_API at::Tensor & min_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
TORCH_API at::Tensor & min_outf(const at::Tensor & self, const at::Tensor & other, at::Tensor & out);

} // namespace compositeimplicitautograd
} // namespace at
