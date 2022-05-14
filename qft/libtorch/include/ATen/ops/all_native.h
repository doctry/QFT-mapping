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
#include <ATen/ops/all_meta.h>

namespace at {
namespace native {

struct TORCH_API structured_all_out : public at::meta::structured_all_dim {
void impl(const at::Tensor & self, int64_t dim, bool keepdim, const at::Tensor & out);
};
TORCH_API at::Tensor all(const at::Tensor & self, at::Dimname dim, bool keepdim=false);
TORCH_API at::Tensor & all_out(const at::Tensor & self, at::Dimname dim, bool keepdim, at::Tensor & out);
struct TORCH_API structured_all_all_out : public at::meta::structured_all {
void impl(const at::Tensor & self, const at::Tensor & out);
};

} // namespace native
} // namespace at
