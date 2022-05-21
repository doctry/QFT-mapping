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



#include <ATen/ops/_mps_linear_ops.h>

namespace at {


// aten::_mps_linear(Tensor self, Tensor weight, Tensor? bias=None) -> Tensor
TORCH_API inline at::Tensor _mps_linear(const at::Tensor & self, const at::Tensor & weight, const c10::optional<at::Tensor> & bias={}) {
    return at::_ops::_mps_linear::call(self, weight, bias);
}

}
