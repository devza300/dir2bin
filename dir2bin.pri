# версия от релиза из git-а
GIT_VERSION = $$system( git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags )

# версия сборки - задается вручную
MAJOR = 0
MINOR = 02
VERSION = "$$MAJOR"."$$MINOR"
PROJECT_NAME = dir2bin

# глобальные переменные
DEFINES += ORGANIZATION_NAME='\\"DevZA300\\"'
DEFINES += ORGANIZATION_DOMAIN='\\"https://github.com/devza300/dir2bin\\"'
DEFINES += GIT_VERSION='\\"$$GIT_VERSION\\"'
DEFINES += VERSION='\\"$$VERSION\\"'
DEFINES += PROJECT_NAME='\\"$$PROJECT_NAME\\"'

# флаг препроцессора для определения сборки в режиме дебага
CONFIG(debug, debug|release): DEFINES += DEBUG

# переменные для сборки программ
win32: CONFIG(release, debug|release): OUTPWD_MOD = $$OUT_PWD/release
win32: CONFIG(debug, debug|release): OUTPWD_MOD = $$OUT_PWD/debug
unix: OUTPWD_MOD = $$OUT_PWD

# определение архитектуры сборки
!contains(QT_ARCH, x86_64){
    ARCH="i386"
} else {
    ARCH="amd64"
}

# оптимизация сборки
CONFIG(release, debug|release) {
    QMAKE_LFLAGS += -s
    QMAKE_CXXFLAGS += -fpermissive
    QMAKE_CXXFLAGS += -fno-sized-deallocation
    CONFIG += qt release
    CONFIG -= qml_debug
}
