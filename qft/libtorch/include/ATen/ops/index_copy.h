#pragma once

// @generated by tools/codegen/gen.py from Function.h

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



#include <ATen/ops/index_copy_ops.h>

namespace at {


// aten::index_copy(Tensor self, int dim, Tensor index, Tensor source) -> Tensor
TORCH_API inline at::Tensor index_copy(const at::Tensor & self, int64_t dim, const at::Tensor & index, const at::Tensor & source) {
    return at::_ops::index_copy::call(self, dim, index, source);
}

// aten::index_copy.dimname(Tensor self, Dimname dim, Tensor index, Tensor source) -> Tensor
TORCH_API inline at::Tensor index_copy(const at::Tensor & self, at::Dimname dim, const at::Tensor & index, const at::Tensor & source) {
    return at::_ops::index_copy_dimname::call(self, dim, index, source);
}

}
