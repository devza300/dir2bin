################################################################# опредление исходных данных ###############################################################
# каталог сборки
$OUT_PWD = $args[0]

# каталог исходников пректа
$SRC_DIR = $args[1]

# архитектура: i386 / amd64
$ARCH = $args[2]

# каталог комплекта Qt, используемого для сборки
$QTDIR = $args[3]

# путь к программе WINDEPLOYQT
$WINDEPLOYQT = $args[4]

# путь к программе BINARYCREATOR
$BINARYCREATOR = $args[5]

# путь к программе REPOGEN
$REPOGEN = $args[6]

# каталог структуры устанвщика
$INSTALLER_DIR = $args[7]

# версия сборки
$VERSION = $args[8]

############################################################### подготовка компонент установки #############################################################

# каталог сборки установщика
echo "build.ps1 [1] copy installer tree"
$TMP_DIR = $OUT_PWD + "\..\..\..\_tmp_installer_" + $ARCH
if (Test-Path $TMP_DIR) {
    Remove-Item $TMP_DIR -Force -Recurse
}
Copy-Item -Path $INSTALLER_DIR -Destination $TMP_DIR -Recurse

# получение из git-а даты коммита
echo "build.ps1 [2] get git date"
Set-Location -Path $SRC_DIR
$__COMMIT_DATE = (git log -1 --date=short --format=%cd) -join "`n"

# каталог сборки библиотек и исполняемого файла
$__DATA = $($TMP_DIR + "\packages\dir2bin\data")

# запись версии в манифест
echo "build.ps1 [3] write manifest file"
$__META_XML = $($TMP_DIR + "\packages\dir2bin\meta\package.xml")
(Get-Content $__META_XML).replace('VERSION', $VERSION).replace('RELEASE_DATE', $__COMMIT_DATE) | Set-Content $__META_XML

function componentPrepare([string] $NAME)
{
    # copy exe to data
    Copy-Item -Path $($OUT_PWD + "\..\..\..\" + $NAME + "\release\" + $NAME) -Destination $__DATA

    # вызов windeployQt
    $proc_depends = Start-Process -NoNewWindow -PassThru -FilePath $WINDEPLOYQT -ArgumentList --force, $__DATA
    $proc_depends.WaitForExit()
}

# сборка файлов windeployQt
echo "build.ps1 [4] copy exe tp tmp dir"
componentPrepare "dir2bin.exe"
echo "build.ps1 [5] copy dll tp tmp dir"
componentPrepare "libdir2bin.dll"
echo "build.ps1 [5] copy header tp tmp dir"
componentPrepare "dir2bin.h"

############################################################################################################################################################

# создание инсталлятора
echo "build.ps1 [7] generate installer exe"
$INSTALLER = $OUT_PWD + "\..\..\..\dir2bin_" + $ARCH + ".exe"
$ARGS = "-c $TMP_DIR\config\config.xml -p $TMP_DIR\packages $INSTALLER"
$proc_installer = Start-Process -NoNewWindow -PassThru $BINARYCREATOR $ARGS
$proc_installer.WaitForExit()
