project('duostra',
  'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

cmake = import('cmake')

inc = [include_directories('include')]

libtqdm = subproject('tqdm')
libtqdm_dep = libtqdm.get_variable('libtqdm_dep')

libjson = subproject('json')
libjson_dep = libjson.get_variable('nlohmann_json_dep')

libtorch = subproject('libtorch')
libtorch_dep = dependency('Torch', cmake_module_path: 'libtorch')

subdir('src')

exe = executable('Duostra', 'src/main.cpp', install: true, include_directories: inc, dependencies: [libtqdm_dep, libjson_dep, libtorch_dep], link_with: lib)

test('basic', exe)
