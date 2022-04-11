project('libaerodata', 'cpp', 
        meson_version: '>= 0.56.0', 
        default_options : [ 'cpp_std=c++17'], 
        version: '0.1.0')

# Sources
test_main = 'tests/test-utils.cpp'

srcs = ['src/Tensor.cpp', 'src/Utils.cpp', 'src/AeroData.cpp', 'src/ModelicaAeroData.cpp']
test_srcs = ['tests/test-utils.cpp']

incs = include_directories('include')

# Subproject
cnpy_proj = subproject('cnpy')
cnpy_dep = cnpy_proj.get_variable('cnpy_dep')

# zlib_dep = dependency('zlib', version : '>=1.2', not_found_message: 'Error finding ZLIB dep')

zlib_proj = subproject('zlib')
zlib_dep = cnpy_proj.get_variable('zlib_dep')

catch_dep = dependency('catch2')

deps = [cnpy_dep, zlib_dep]

install_headers('include/ModelicaAeroData.h')

libaerodata = library('aerodata', srcs, 
        dependencies : [cnpy_dep, zlib_dep], 
        include_directories: [incs],
        install: true)

libaerodata_dep = declare_dependency(link_with : libaerodata,
  include_directories : incs)

catch_test_ex = executable('test-catch', test_srcs + srcs, 
        include_directories: incs, dependencies: [deps, catch_dep])

executable('interpn-csv', ['tests/interpn-csv.cpp'] + srcs, include_directories: incs, dependencies: deps)
executable('test-var', ['tests/test-var.cpp'] + srcs, include_directories: incs, dependencies: deps)
executable('tocsv', ['tests/tocsv.cpp'] + srcs, include_directories: incs, dependencies: deps)
executable('mode-benchmark', ['tests/benchmark.cpp'] + srcs, include_directories: incs, dependencies: deps)

test('catch tests', catch_test_ex)