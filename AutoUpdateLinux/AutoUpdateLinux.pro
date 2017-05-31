TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -static -static-libgcc -static-libstdc++
SOURCES += main.cpp \
    base64.cpp \
    configjson.cpp \
    MD5.cpp \
    clibcurl.cpp
HEADERS += \
    base64.h \
    configjson.h \
    MD5.h \
    clibcurl.h

INCLUDEPATH += \
    include

LIBS += -L./lib  \
        -lcurl

