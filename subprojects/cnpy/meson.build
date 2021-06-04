project('cnpy', 'cpp', meson_version: '>= 0.56.0', default_options : [ 'cpp_std=c++17'], version: '0.1.0')

lib_src = files(
    'cnpy.cpp',
)

incs = include_directories('.')

zlib_proj = subproject('zlib')
zlib_dep = zlib_proj.get_variable('zlib_dep')
# zlib_dep = dependency('zlib', version : '>=1.2', not_found_message: 'Error finding this dep')

cnpy = library(
    'cnpy',
    sources: [lib_src],
    include_directories: incs,
    dependencies: [zlib_dep],
    install: true
)

cnpy_dep = declare_dependency(
    link_with: cnpy,
    include_directories: incs
)
