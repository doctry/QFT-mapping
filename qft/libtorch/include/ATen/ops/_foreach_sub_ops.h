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


struct TORCH_API _foreach_sub_Scalar {
  using schema = ::std::vector<at::Tensor> (at::TensorList, const at::Scalar &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_foreach_sub")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "Scalar")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_foreach_sub.Scalar(Tensor[] tensors, Scalar scalar) -> Tensor[]")
  static ::std::vector<at::Tensor> call(at::TensorList tensors, const at::Scalar & scalar);
  static ::std::vector<at::Tensor> redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors, const at::Scalar & scalar);
};

struct TORCH_API _foreach_sub__Scalar {
  using schema = void (at::TensorList, const at::Scalar &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_foreach_sub_")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "Scalar")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_foreach_sub_.Scalar(Tensor(a!)[] self, Scalar scalar) -> ()")
  static void call(at::TensorList self, const at::Scalar & scalar);
  static void redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList self, const at::Scalar & scalar);
};

struct TORCH_API _foreach_sub_List {
  using schema = ::std::vector<at::Tensor> (at::TensorList, at::TensorList, const at::Scalar &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_foreach_sub")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "List")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_foreach_sub.List(Tensor[] tensors1, Tensor[] tensors2, *, Scalar alpha=1) -> Tensor[]")
  static ::std::vector<at::Tensor> call(at::TensorList tensors1, at::TensorList tensors2, const at::Scalar & alpha);
  static ::std::vector<at::Tensor> redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors1, at::TensorList tensors2, const at::Scalar & alpha);
};

struct TORCH_API _foreach_sub__List {
  using schema = void (at::TensorList, at::TensorList, const at::Scalar &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_foreach_sub_")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "List")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_foreach_sub_.List(Tensor(a!)[] self, Tensor[] other, *, Scalar alpha=1) -> ()")
  static void call(at::TensorList self, at::TensorList other, const at::Scalar & alpha);
  static void redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList self, at::TensorList other, const at::Scalar & alpha);
};

struct TORCH_API _foreach_sub_ScalarList {
  using schema = ::std::vector<at::Tensor> (at::TensorList, at::ArrayRef<at::Scalar>);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_foreach_sub")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "ScalarList")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_foreach_sub.ScalarList(Tensor[] tensors, Scalar[] scalars) -> Tensor[]")
  static ::std::vector<at::Tensor> call(at::TensorList tensors, at::ArrayRef<at::Scalar> scalars);
  static ::std::vector<at::Tensor> redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList tensors, at::ArrayRef<at::Scalar> scalars);
};

struct TORCH_API _foreach_sub__ScalarList {
  using schema = void (at::TensorList, at::ArrayRef<at::Scalar>);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_foreach_sub_")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "ScalarList")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_foreach_sub_.ScalarList(Tensor(a!)[] self, Scalar[] scalars) -> ()")
  static void call(at::TensorList self, at::ArrayRef<at::Scalar> scalars);
  static void redispatch(c10::DispatchKeySet dispatchKeySet, at::TensorList self, at::ArrayRef<at::Scalar> scalars);
};

}} // namespace at::_ops
