project(
    'libds',
    'c',
    version: '0.0.1',
    license: 'GPL-3.0-or-later',
    license_files: ['COPYING'],
    default_options: [
        'c_std=c99',
        'warning_level=3',
    ],
)

inc = include_directories('include')

add_project_arguments(
    '-Wpedantic',
    '-Wstrict-prototypes',
    '-Wno-unused-parameter',
    language: 'c',
)

build_shared = get_option('build-shared')
build_static = get_option('build-static')
build_tests  = get_option('build-tests')

libs = []

subdir('src')
if build_shared or build_tests
    shared_lib = library(
        'ds',
        source_files,
        include_directories: inc,
        version: meson.project_version(),
        install: true,
    )
    libs += shared_lib
endif

if build_static
    static_lib = static_library(
        'ds_static',
        source_files,
        include_directories: inc,
        install: true,
    )
    libs += static_lib
endif

pkg = import('pkgconfig')

pkg.generate(
    name: 'libds',
    description: 'The C Data Structures Library',
    version: meson.project_version(),
    filebase: 'ds',
    libraries: libs,
    subdirs: 'ds',
    requires_private: [],
    install_dir: get_option('libdir') / 'pkgconfig',
)

if build_tests
    subdir('test')
endif

install_subdir('include/ds', install_dir: get_option('includedir'))
install_headers('include/ds.h', install_dir: get_option('includedir'))

summary({
    'Build shared library': build_shared,
    'Build static library': build_static,
    'Build tests': build_tests,
}, section: 'Build configuration')
