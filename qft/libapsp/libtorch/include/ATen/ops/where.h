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



#include <ATen/ops/where_ops.h>

namespace at {


// aten::where.self(Tensor condition, Tensor self, Tensor other) -> Tensor
TORCH_API inline at::Tensor where(const at::Tensor & condition, const at::Tensor & self, const at::Tensor & other) {
    return at::_ops::where_self::call(condition, self, other);
}

// aten::where.self_out(Tensor condition, Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
TORCH_API inline at::Tensor & where_out(at::Tensor & out, const at::Tensor & condition, const at::Tensor & self, const at::Tensor & other) {
    return at::_ops::where_self_out::call(condition, self, other, out);
}

// aten::where.self_out(Tensor condition, Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
TORCH_API inline at::Tensor & where_outf(const at::Tensor & condition, const at::Tensor & self, const at::Tensor & other, at::Tensor & out) {
    return at::_ops::where_self_out::call(condition, self, other, out);
}

// aten::where.ScalarSelf(Tensor condition, Scalar self, Tensor other) -> Tensor
TORCH_API inline at::Tensor where(const at::Tensor & condition, const at::Scalar & self, const at::Tensor & other) {
    return at::_ops::where_ScalarSelf::call(condition, self, other);
}

// aten::where.ScalarOther(Tensor condition, Tensor self, Scalar other) -> Tensor
TORCH_API inline at::Tensor where(const at::Tensor & condition, const at::Tensor & self, const at::Scalar & other) {
    return at::_ops::where_ScalarOther::call(condition, self, other);
}

// aten::where.Scalar(Tensor condition, Scalar self, Scalar other) -> Tensor
TORCH_API inline at::Tensor where(const at::Tensor & condition, const at::Scalar & self, const at::Scalar & other) {
    return at::_ops::where_Scalar::call(condition, self, other);
}

// aten::where(Tensor condition) -> Tensor[]
TORCH_API inline ::std::vector<at::Tensor> where(const at::Tensor & condition) {
    return at::_ops::where::call(condition);
}

}
