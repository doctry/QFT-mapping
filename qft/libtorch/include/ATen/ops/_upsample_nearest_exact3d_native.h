#pragma once

// @generated by tools/codegen/gen.py from NativeFunction.h

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
#include <ATen/ops/_upsample_nearest_exact3d_meta.h>

namespace at {
namespace native {

TORCH_API at::Tensor _upsample_nearest_exact3d_cpu(const at::Tensor & input, c10::optional<at::IntArrayRef> output_size, c10::optional<at::ArrayRef<double>> scale_factors);
TORCH_API at::Tensor _upsample_nearest_exact3d_cuda(const at::Tensor & input, c10::optional<at::IntArrayRef> output_size, c10::optional<at::ArrayRef<double>> scale_factors);
TORCH_API at::Tensor _upsample_nearest_exact3d_quantized_cpu(const at::Tensor & input, c10::optional<at::IntArrayRef> output_size, c10::optional<at::ArrayRef<double>> scale_factors);
struct TORCH_API structured__upsample_nearest_exact3d_out_cpu : public at::meta::structured__upsample_nearest_exact3d {
void impl(const at::Tensor & self, at::IntArrayRef output_size, c10::optional<double> scales_d, c10::optional<double> scales_h, c10::optional<double> scales_w, const at::Tensor & out);
};
struct TORCH_API structured__upsample_nearest_exact3d_out_cuda : public at::meta::structured__upsample_nearest_exact3d {
void impl(const at::Tensor & self, at::IntArrayRef output_size, c10::optional<double> scales_d, c10::optional<double> scales_h, c10::optional<double> scales_w, const at::Tensor & out);
};
TORCH_API at::Tensor _upsample_nearest_exact3d_quantized_cpu(const at::Tensor & self, at::IntArrayRef output_size, c10::optional<double> scales_d=c10::nullopt, c10::optional<double> scales_h=c10::nullopt, c10::optional<double> scales_w=c10::nullopt);

} // namespace native
} // namespace at
