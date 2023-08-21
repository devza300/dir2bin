include($$PWD/../dir2bin.pri)

QT = core

CONFIG += c++20 cmdline
TARGET = dir2bin
DEFINES += APP_NAME='\\"$$TARGET\\"'

SOURCES += \
        main.cpp

# attach libdir2bin
LIBS += -L$$PWD/3rdParty/ -ldir2bin
INCLUDEPATH += $$PWD/3rdParty
DEPENDPATH += $$PWD/3rdParty
