#pragma once

// @generated by torchgen/gen.py from Function.h

#include <ATen/Context.h>
#include <ATen/DeviceGuard.h>
#include <ATen/TensorUtils.h>
#include <ATen/TracerMode.h>
#include <ATen/core/Generator.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>



#include <ATen/ops/embedding_backward_ops.h>

namespace at {


// aten::embedding_backward(Tensor grad, Tensor indices, int num_weights, int padding_idx, bool scale_grad_by_freq, bool sparse) -> Tensor
TORCH_API inline at::Tensor embedding_backward(const at::Tensor & grad, const at::Tensor & indices, int64_t num_weights, int64_t padding_idx, bool scale_grad_by_freq, bool sparse) {
    return at::_ops::embedding_backward::call(grad, indices, num_weights, padding_idx, scale_grad_by_freq, sparse);
}

}
