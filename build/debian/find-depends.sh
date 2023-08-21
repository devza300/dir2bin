#!/bin/bash

PROGRAMM=$1

# обновление списка содержимого пакетов
sudo apt-file update &> /dev/null
sudo apt update &> /dev/null

# вывод имени пакета и его версии в виде пригодном для формирования секции зависимостей
function packageInfo()
{
    PACKAGE=$1

    # получение версии пакета
    VERSION=$(apt-cache show ${PACKAGE}: | grep "Version" | awk '{print $2}')

    # упрощение версии пакета
    VERSION=$(echo $VERSION  | cut -d "-" -f 1  | cut -d "+" -f 1  | cut -d ":" -f 1)

    # запись в формате зависимости
    echo "$PACKAGE (>= $VERSION)"
}

DEPENDENCIES=""
# список бинарных зависимстей программы
LIST=$(ldd $PROGRAMM | cut -d '=' -f 1 | awk '{print $1}')
for LIB_NAME in $LIST
do    
    # список пакетов, предоставляющих данную зависимость (библиотеку)
    PACKAGES=$(apt-file search $LIB_NAME | grep $LIB_NAME | awk '{print $1}' | cut -d ":" -f 1 | uniq)
    
    if [ "$PACKAGES" == "" ]
    then
        echo ""
        echo "lib: $LIB_NAME"
        >&2 echo "unable to resolve dependency: $LIB_NAME"
        continue
    fi
    
    # в случае более 1 пакета удовлетворяющего зависисмости добавляется ручная обработка
    PACKAGES_COUNT=$(echo $PACKAGES | wc -w)
    if (( $PACKAGES_COUNT > 1 ))
    then
        COMMENT_DEPENDENCIES=""
        for PACKAGE in $PACKAGES
        do
            INFO=$(packageInfo $PACKAGE)
            COMMENT_DEPENDENCIES=$(echo "$COMMENT_DEPENDENCIES, $INFO")
        done

        # удаление из начала списка зависимостей ", "
        COMMENT_DEPENDENCIES=${COMMENT_DEPENDENCIES:2}
        DEPENDENCIES=$(echo "$DEPENDENCIES, \\?# TODO have the variants $LIB_NAME: $COMMENT_DEPENDENCIES")
    else
        INFO=$(packageInfo $PACKAGES)
        
        # добавление к списку зависимостей, если еще такой нет
        EXIST=$(echo $DEPENDENCIES | grep "$INFO")
        if [ "$EXIST" != "" ]
        then
            DEPENDENCIES=$(echo "$DEPENDENCIES, \\?# TODO have the same dependencies $LIB_NAME: $INFO")
        else
            DEPENDENCIES=$(echo "$DEPENDENCIES, \\?$INFO")
        fi
    fi
done

# удаление из начала списка зависимостей ", \\?"
DEPENDENCIES=${DEPENDENCIES:4}

# добавление в конец списка ", \"
DEPENDENCIES=$(echo "$DEPENDENCIES, \\")

echo ""
echo "OUT:"
# сортировка результата и добавление переносов строк вместо символов ?
echo $DEPENDENCIES | tr "?" "\n" | sort
