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



#include <ATen/ops/im2col_backward_ops.h>

namespace at {


// aten::im2col_backward.grad_input(Tensor grad_output, int[2] input_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride, *, Tensor(a!) grad_input) -> Tensor(a!)
TORCH_API inline at::Tensor & im2col_backward_out(at::Tensor & grad_input, const at::Tensor & grad_output, at::IntArrayRef input_size, at::IntArrayRef kernel_size, at::IntArrayRef dilation, at::IntArrayRef padding, at::IntArrayRef stride) {
    return at::_ops::im2col_backward_grad_input::call(grad_output, input_size, kernel_size, dilation, padding, stride, grad_input);
}

// aten::im2col_backward.grad_input(Tensor grad_output, int[2] input_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride, *, Tensor(a!) grad_input) -> Tensor(a!)
TORCH_API inline at::Tensor & im2col_backward_outf(const at::Tensor & grad_output, at::IntArrayRef input_size, at::IntArrayRef kernel_size, at::IntArrayRef dilation, at::IntArrayRef padding, at::IntArrayRef stride, at::Tensor & grad_input) {
    return at::_ops::im2col_backward_grad_input::call(grad_output, input_size, kernel_size, dilation, padding, stride, grad_input);
}

// aten::im2col_backward(Tensor grad_output, int[2] input_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride) -> Tensor
TORCH_API inline at::Tensor im2col_backward(const at::Tensor & grad_output, at::IntArrayRef input_size, at::IntArrayRef kernel_size, at::IntArrayRef dilation, at::IntArrayRef padding, at::IntArrayRef stride) {
    return at::_ops::im2col_backward::call(grad_output, input_size, kernel_size, dilation, padding, stride);
}

}