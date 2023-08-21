QT -= gui

TARGET = dir2bin
TEMPLATE = lib
DEFINES += DIR2BIN_LIBRARY

CONFIG += c++20

SOURCES += \
    Private/Converter.cpp \
    Private/Entry.cpp \
    Private/Iterator.cpp \
    dir2bin.cpp

HEADERS += \
    Private/Converter.h \
    Private/DataList.h \
    Private/Entry.h \
    Private/Iterator.h \
    dir2bin.h
