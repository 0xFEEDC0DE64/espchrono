TEMPLATE = app

QT += core testlib
QT -= gui widgets

CONFIG += c++17 qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

SOURCES += \
    tst_espchrono.cpp

ESPCHRONO_DIR = $$PWD/..

include($$ESPCHRONO_DIR/espchrono.pri)
include($$ESPCHRONO_DIR/espchrono_src.pri)
include($$ESPCHRONO_DIR/test/espchronotestutils.pri)
include($$ESPCHRONO_DIR/test/espchronotestutils_src.pri)

equals(CLONE_CPPUTILS, 1) {
    CPPUTILS_DIR = $$PWD/cpputils

    message("Checking out cpputils...")
    exists($$CPPUTILS_DIR/.git) {
        system("git -C $$CPPUTILS_DIR pull")
    } else {
        system("git clone https://github.com/0xFEEDC0DE64/cpputils.git $$CPPUTILS_DIR")
    }
} else: exists($$PWD/../../cpputils/src) {
    CPPUTILS_DIR = $$PWD/../../cpputils
} else {
    error("cpputils not found, please run git submodule update --init")
}

include($$CPPUTILS_DIR/cpputils.pri)
include($$CPPUTILS_DIR/cpputils_src.pri)
include($$CPPUTILS_DIR/test/cpputilstestutils.pri)
include($$CPPUTILS_DIR/test/cpputilstestutils_src.pri)

equals(CLONE_DATE, 1) {
    DATE_DIR = $$PWD/date

    message("Checking out date...")
    exists($$DATE_DIR/.git): {
        system("git -C $$DATE_DIR pull")
    } else {
        system("git clone https://github.com/0xFEEDC0DE64/date.git $$DATE_DIR")
    }
} else: exists($$PWD/../../date/include) {
    DATE_DIR = $$PWD/../../date
} else {
    error("date not found, please run git submodule update --init")
}

include($$DATE_DIR/date.pri)

QMAKE_CXXFLAGS += -Wno-missing-field-initializers
