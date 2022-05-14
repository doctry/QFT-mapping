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


struct TORCH_API select_Dimname {
  using schema = at::Tensor (const at::Tensor &, at::Dimname, int64_t);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::select")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "Dimname")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "select.Dimname(Tensor(a) self, Dimname dim, int index) -> Tensor(a)")
  static at::Tensor call(const at::Tensor & self, at::Dimname dim, int64_t index);
  static at::Tensor redispatch(c10::DispatchKeySet dispatchKeySet, const at::Tensor & self, at::Dimname dim, int64_t index);
};

struct TORCH_API select_int {
  using schema = at::Tensor (const at::Tensor &, int64_t, int64_t);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::select")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "int")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "select.int(Tensor(a) self, int dim, int index) -> Tensor(a)")
  static at::Tensor call(const at::Tensor & self, int64_t dim, int64_t index);
  static at::Tensor redispatch(c10::DispatchKeySet dispatchKeySet, const at::Tensor & self, int64_t dim, int64_t index);
};

}} // namespace at::_ops
