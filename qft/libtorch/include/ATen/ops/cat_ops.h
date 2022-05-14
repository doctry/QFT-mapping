#pragma once

// @generated by tools/codegen/gen.py from Operator.h

#include <c10/core/QScheme.h>
#include <tuple>
#include <vector>


// Forward declarations of any types needed in the operator signatures.
// We can't directly include these classes because it will cause circular include dependencies.
// This file is included by TensorBody.h, which defines the Tensor class.
namespace c10 {

template<typename T>
class optional;
template<typename T>
class List;
class Stream;
class Scalar;
struct Storage;
struct TensorOptions;

}

namespace at {

class Tensor;
struct Dimname;
struct Generator;
using TensorList = c10::ArrayRef<Tensor>;
using DimnameList = c10::ArrayRef<Dimname>;
using c10::Stream;
using c10::Storage;
using c10::QScheme;
using c10::Scalar;
using c10::TensorOptions;

namespace _ops {


struct TORCH_API cat {
  using schema = at::Tensor (at::TensorList, int64_t);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::cat")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "cat(Tensor[] tensors, int dim=0) -> Tensor")
  static at::Tensor call(at::TensorList tensors, int64_t dim);
  static at::Tensor redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors, int64_t dim);
};

struct TORCH_API cat_out {
  using schema = at::Tensor & (at::TensorList, int64_t, at::Tensor &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::cat")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "out")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "cat.out(Tensor[] tensors, int dim=0, *, Tensor(a!) out) -> Tensor(a!)")
  static at::Tensor & call(at::TensorList tensors, int64_t dim, at::Tensor & out);
  static at::Tensor & redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors, int64_t dim, at::Tensor & out);
};

struct TORCH_API cat_names {
  using schema = at::Tensor (at::TensorList, at::Dimname);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::cat")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "names")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "cat.names(Tensor[] tensors, Dimname dim) -> Tensor")
  static at::Tensor call(at::TensorList tensors, at::Dimname dim);
  static at::Tensor redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors, at::Dimname dim);
};

struct TORCH_API cat_names_out {
  using schema = at::Tensor & (at::TensorList, at::Dimname, at::Tensor &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::cat")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "names_out")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "cat.names_out(Tensor[] tensors, Dimname dim, *, Tensor(a!) out) -> Tensor(a!)")
  static at::Tensor & call(at::TensorList tensors, at::Dimname dim, at::Tensor & out);
  static at::Tensor & redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors, at::Dimname dim, at::Tensor & out);
};

}} // namespace at::_ops
