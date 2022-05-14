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


namespace at {
namespace native {

TORCH_API ::std::vector<at::Tensor> foreach_tensor_add_scalar_kernel_slow(at::TensorList tensors, const at::Scalar & scalar);
TORCH_API ::std::vector<at::Tensor> foreach_tensor_add_scalar_kernel_cuda(at::TensorList tensors, const at::Scalar & scalar);
TORCH_API void foreach_tensor_add_scalar_kernel_slow_(at::TensorList self, const at::Scalar & scalar);
TORCH_API void foreach_tensor_add_scalar_kernel_cuda_(at::TensorList self, const at::Scalar & scalar);
TORCH_API ::std::vector<at::Tensor> foreach_tensor_add_list_kernel_slow(at::TensorList tensors1, at::TensorList tensors2, const at::Scalar & alpha=1);
TORCH_API ::std::vector<at::Tensor> foreach_tensor_add_list_kernel_cuda(at::TensorList tensors1, at::TensorList tensors2, const at::Scalar & alpha=1);
TORCH_API void foreach_tensor_add_list_kernel_slow_(at::TensorList self, at::TensorList other, const at::Scalar & alpha=1);
TORCH_API void foreach_tensor_add_list_kernel_cuda_(at::TensorList self, at::TensorList other, const at::Scalar & alpha=1);
TORCH_API ::std::vector<at::Tensor> foreach_tensor_add_scalarlist_kernel_slow(at::TensorList tensors, at::ArrayRef<at::Scalar> scalars);
TORCH_API ::std::vector<at::Tensor> foreach_tensor_add_scalarlist_kernel_cuda(at::TensorList tensors, at::ArrayRef<at::Scalar> scalars);
TORCH_API void foreach_tensor_add_scalarlist_kernel_slow_(at::TensorList self, at::ArrayRef<at::Scalar> scalars);
TORCH_API void foreach_tensor_add_scalarlist_kernel_cuda_(at::TensorList self, at::ArrayRef<at::Scalar> scalars);

} // namespace native
} // namespace at
