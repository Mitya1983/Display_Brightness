TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
    -lpthread \
    -lstdc++fs \
    -lopencv_core \
    -lopencv_videoio \
    -lopencv_imgproc \
    -L/home/mitya/MyLibs/lib/Date_time/ -lmt_date_time

INCLUDEPATH += \
    /usr/local/include/opencv4/ \
    /home/mitya/MyLibs/include/Date_time/ \
    /home/mitya/MyLibs/include/Log/ \
    ../headers/

SOURCES += \
        ../../MyLibs/source/log.cpp \
    main.cpp \
    source/config.cpp \
    source/display.cpp \
    source/error_handler.cpp \
    source/function_to_fork.cpp \
    source/image_from_camera.cpp \
    source/install_functions.cpp \
    source/user_input_parser.cpp \

HEADERS += \
    ../../MyLibs/include/Log/log.h \
    headers/config.h \
    headers/constant_names.h \
    headers/display.h \
    headers/error_handler.h \
    headers/function_to_fork.h \
    headers/image_from_camera.h \
    headers/install_functions.h \
    headers/user_input_parser.h \
