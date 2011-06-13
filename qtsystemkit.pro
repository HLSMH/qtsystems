TEMPLATE = subdirs
CONFIG += ordered

module_qtsystemkit_src.subdir = src
module_qtsystemkit_src.target = module-qtsystemkit-src

module_qtsystemkit_tests.subdir = tests
module_qtsystemkit_tests.target = module-qtsystemkit-tests
module_qtsystemkit_tests.depends = module_qtsystemkit_src

!include($$QT_SYSTEMKIT_BUILD_TREE/config.pri) {
    error("Please run configure script");
}

contains(build_unit_tests, no) {
    module_qtsystemkit_tests.CONFIG = no_default_target no_default_install
}

SUBDIRS += module_qtsystemkit_src \
           module_qtsystemkit_tests