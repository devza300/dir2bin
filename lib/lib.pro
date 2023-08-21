include($$PWD/../dir2bin.pri)

QT -= gui

TEMPLATE = lib
TARGET = $$PROJECT_NAME
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

# copy lib and header to 3rdParty
first.commands += $(MKDIR) $$PWD/../tool/3rdParty ;
first.commands += $(DEL_FILE) $$PWD/../tool/3rdParty/* ;
unix: first.commands += $(COPY_FILE) -P $$OUTPWD_MOD/*.so* $$PWD/../tool/3rdParty ;
win32: first.commands += $(COPY_FILE) -P $$OUTPWD_MOD/*.dll $$PWD/../tool/3rdParty ;
first.commands += $(COPY_FILE) $$PWD/*.h $$PWD/../tool/3rdParty ;

# make copy
QMAKE_EXTRA_TARGETS += first
