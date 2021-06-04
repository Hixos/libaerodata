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

catch_dep = dependency('catch2')

install_headers('include/ModelicaAeroData.h')

libaerodata = library('aerodata', srcs, 
        dependencies : [cnpy_dep], 
        include_directories: [incs],
        install: true)

libaerodata_dep = declare_dependency(link_with : libaerodata,
  include_directories : incs)

catch_test_ex = executable('test-catch', test_srcs + srcs, 
        include_directories: incs, dependencies: [catch_dep, cnpy_dep])

executable('interpn-csv', ['tests/interpn-csv.cpp'] + srcs, include_directories: incs, dependencies: [cnpy_dep])
executable('test-var', ['tests/test-var.cpp'] + srcs, include_directories: incs, dependencies: [cnpy_dep])

test('catch tests', catch_test_ex)