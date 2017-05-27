TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    base64.cpp \
    configjson.cpp \
    MD5.cpp

HEADERS += \
    base64.h \
    configjson.h \
    MD5.h

INCLUDEPATH += \
    include\
LIBS += \
    crypto \
    ssl \
    curl

