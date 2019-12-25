TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
    -lstdc++fs

INCLUDEPATH += \
    ../headers

SOURCES += \
        main.cpp

HEADERS += \
    ../headers/constant_names.h
