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



#include <ATen/ops/mv_ops.h>

namespace at {


// aten::mv(Tensor self, Tensor vec) -> Tensor
TORCH_API inline at::Tensor mv(const at::Tensor & self, const at::Tensor & vec) {
    return at::_ops::mv::call(self, vec);
}

// aten::mv.out(Tensor self, Tensor vec, *, Tensor(a!) out) -> Tensor(a!)
TORCH_API inline at::Tensor & mv_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & vec) {
    return at::_ops::mv_out::call(self, vec, out);
}

// aten::mv.out(Tensor self, Tensor vec, *, Tensor(a!) out) -> Tensor(a!)
TORCH_API inline at::Tensor & mv_outf(const at::Tensor & self, const at::Tensor & vec, at::Tensor & out) {
    return at::_ops::mv_out::call(self, vec, out);
}

}
