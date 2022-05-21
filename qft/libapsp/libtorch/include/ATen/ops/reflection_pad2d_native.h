#pragma once

// @generated by torchgen/gen.py from NativeFunction.h

#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>
#include <c10/core/QScheme.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <tuple>
#include <vector>


namespace at {
namespace native {

TORCH_API at::Tensor reflection_pad2d_cpu(const at::Tensor & self, at::IntArrayRef padding);
TORCH_API at::Tensor & reflection_pad2d_out_cpu(const at::Tensor & self, at::IntArrayRef padding, at::Tensor & out);
TORCH_API at::Tensor reflection_pad2d_cuda(const at::Tensor & self, at::IntArrayRef padding);
TORCH_API at::Tensor & reflection_pad2d_out_cuda(const at::Tensor & self, at::IntArrayRef padding, at::Tensor & out);
TORCH_API at::Tensor reflection_pad2d_quantized_cpu(const at::Tensor & self, at::IntArrayRef padding);

} // namespace native
} // namespace at
