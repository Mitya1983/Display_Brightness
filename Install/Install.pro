TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
    ../headers \
    /home/mitya/MyLibs/include/Date_time/ \
    /home/mitya/MyLibs/include/Log

LIBS += \
    -lstdc++fs \
    -L/home/mitya/MyLibs/lib/Date_time/ -lmt_date_time

SOURCES += \
    ../../../MyLibs/source/log.cpp \
    ../source/config.cpp \
    ../source/install_functions.cpp \
    main.cpp

HEADERS += \
    ../../../MyLibs/include/Log/log.h \
    ../headers/config.h \
    ../headers/install_functions.h
