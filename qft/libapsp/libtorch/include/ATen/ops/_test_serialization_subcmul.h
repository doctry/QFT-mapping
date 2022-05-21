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



#include <ATen/ops/_test_serialization_subcmul_ops.h>

namespace at {


// aten::_test_serialization_subcmul(Tensor self, Tensor other, Scalar alpha=1) -> Tensor
TORCH_API inline at::Tensor _test_serialization_subcmul(const at::Tensor & self, const at::Tensor & other, const at::Scalar & alpha=1) {
    return at::_ops::_test_serialization_subcmul::call(self, other, alpha);
}

}
