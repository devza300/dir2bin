#!/bin/bash

################################################################# опредление исходных данных ###############################################################

# каталог структуры пакета
PACKAGE_TREE=$1

# каталог сборки
OUT_PWD=$2

# архитектура: i386 / amd64
PACKAGE_ARCH=$3

# версия пакета
PACKAGE_VERSION=$4

# зависимости пакета
DEPENDS=$5

################################################################# подготовка каталога сборки ###############################################################

OUT_FILE_NAME=libdir2bin-dev

# проверка дистрибутива, на котором собирается пакет
DEBIAN_VERSION=$(cat /etc/debian_version)
if [[ "$DEBIAN_VERSION" == *"sid"* ]]; then
    DEB=$OUT_PWD"/"$OUT_FILE_NAME"_"$PACKAGE_VERSION"~testing_"$PACKAGE_ARCH
    else
    # определение версии дебиана
    DEBIAN_MAJOR=$(cat /etc/debian_version | cut -d '.' -f 1)
    DEBIAN_MINOR=$(cat /etc/debian_version | cut -d '.' -f 2)

    DEB=$OUT_PWD"/"$OUT_FILE_NAME"_"$PACKAGE_VERSION"~deb${DEBIAN_MAJOR}u${DEBIAN_MINOR}_"$PACKAGE_ARCH
fi

# копирование дерева пакета в каталог сборки
rm -R $DEB &> /dev/null
cp -r $PACKAGE_TREE $DEB

################################################################# сборка пакета ############################################################################

# функция найти и заменить
function find_and_replace {
    START_DIR=$1
    CURRENT=$2
    REPLACE=$3

    LIST_ENTRY=$(find $START_DIR)

    # replace CONTENT & rename FILE
    for ENTRY in $LIST_ENTRY; do
        if [[ -f $ENTRY && ! -L $ENTRY ]]; then

            # replace CONTENT
            if [[ "$(file $ENTRY | grep ELF)" == "" ]]; then
                sed -i -e "s|$CURRENT|$REPLACE|g" $ENTRY
            fi

            FILE_NAME=$(basename $ENTRY | cut -d "." -f 1)
            FILE_EXT=$(basename $ENTRY | cut -d "." -f 2)
            if [ "$FILE_NAME" = "$CURRENT" ]; then
                # rename FILE
                DIR_NAME=$(dirname $ENTRY)
                mv $ENTRY $DIR_NAME/$REPLACE.$FILE_EXT
            fi
        fi
    done

    # rename DIR
    for ENTRY in $LIST_ENTRY; do
        if [[ -d $ENTRY ]]; then
            DIR_NAME=$(basename $ENTRY)

            if [ "$DIR_NAME" = "$CURRENT" ]; then
                # rename
                DIR_PATH=$(dirname $ENTRY)
                mv $ENTRY $DIR_PATH/$REPLACE

                # recursion by changes
                find_and_replace $DIR_PATH/$REPLACE $CURRENT $REPLACE
            fi
        fi
    done
}

# копируем заголовочный файл
DEB_INCLUDE_PATH=/usr/include/x86_64-linux-gnu
mkdir -p $DEB/$DEB_INCLUDE_PATH
cp -r $PACKAGE_TREE/../../../lib/dir2bin.h $DEB/$DEB_INCLUDE_PATH

# замена переменных
find_and_replace $DEB   "PACKAGE_ARCH"              "$PACKAGE_ARCH"
find_and_replace $DEB   "PACKAGE_VERSION"           "$PACKAGE_VERSION"
find_and_replace $DEB   "DEPENDS"                   "$DEPENDS"

# определение размера пакета
PACKAGE_SIZE=$(du -s $DEB | awk '{print $1}')
find_and_replace $DEB   "PACKAGE_SIZE"              "$PACKAGE_SIZE"

# Create deb Package
fakeroot dpkg-deb --build $DEB
