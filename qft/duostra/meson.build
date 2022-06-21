project('duostra',
  'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

inc = [include_directories('include')]

libtqdm = subproject('tqdm')
libtqdm_dep = libtqdm.get_variable('libtqdm_dep')

libjson = subproject('json')
libjson_dep = libjson.get_variable('nlohmann_json_dep')

exe = executable('Duostra', ['src/main.cpp', 'apsp.cpp', 'q_device.cpp', 'qft_mapper.cpp'], install: true, include_directories: inc, dependencies: [libtqdm_dep, libjson_dep])

test('basic', exe)
