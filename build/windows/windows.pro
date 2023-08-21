include($$PWD/../../dir2bin.pri)

TEMPLATE = aux

DISTFILES += \
    build.ps1 \
    installer/config/config.xml \
    installer/config/controllerscript.qs \
    installer/packages/dir2bin/data/postinstall.cmd \
    installer/packages/dir2bin/data/preremove.cmd \
    installer/packages/dir2bin/meta/installscript.qs \
    installer/packages/dir2bin/meta/package.xml

!build_pass:win32: CONFIG(release, debug|release) {
    QIF = $$system( dir /b $$(QTDIR)\..\..\Tools\QtInstallerFramework\ )

    first.commands += powershell $$PWD/build.ps1 \
        $$OUTPWD_MOD \
        $$PWD/../ \
        $$ARCH \
        $$(QTDIR) \
        $$(QTDIR)/bin/windeployqt \
        $$(QTDIR)/../../Tools/QtInstallerFramework/$${QIF}/bin/binarycreator \
        $$(QTDIR)/../../Tools/QtInstallerFramework/$${QIF}/bin/repogen \
        $$PWD/installer \
        $$VERSION
    first.commands += ;

    # запуск сборки пакетов
    QMAKE_EXTRA_TARGETS += first
}
