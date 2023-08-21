include($$PWD/../../dir2bin.pri)

TEMPLATE = aux

DISTFILES += \
    dpkg-lib-dev.sh \
    dpkg-lib.sh \
    dpkg-tool.sh \
    find-depends.sh \
    lib-dev/DEBIAN/control \
    lib/DEBIAN/control \
    tool/DEBIAN/control

!build_pass: CONFIG(release, debug|release) {

    # libdir2bin package
    DEPENDS = \
        libpcre2-8-0 (>= 10.42), \
        libc6 (>= 2.36), \
        libgcc-s1 (>= 12.2.0), \
        libglib2.0-0 (>= 2.74.6), \
        libqt6core6 (>= 6.4.2), \
        libstdc++6 (>= 12.2.0), \
        zlib1g (>= 1)

    first.commands += bash $$PWD/dpkg-lib.sh \
        $$PWD/lib \
        $$OUTPWD_MOD \
        $$ARCH \
        $$VERSION \
        "'$$DEPENDS'"
    first.commands += ;

    # libdir2bin-dev package
    DEPENDS = \
        libdir2bin (= $$VERSION)

    first.commands += bash $$PWD/dpkg-lib-dev.sh \
        $$PWD/lib-dev \
        $$OUTPWD_MOD \
        all \
        $$VERSION \
        "'$$DEPENDS'"
    first.commands += ;

    # dir2bin package
    DEPENDS = \
        libpcre2-8-0 (>= 10.42), \
        libc6 (>= 2.36), \
        libgcc-s1 (>= 12.2.0), \
        libglib2.0-0 (>= 2.74.6), \
        libqt6core6 (>= 6.4.2), \
        libstdc++6 (>= 12.2.0), \
        zlib1g (>= 1), \
        libdir2bin (= $$VERSION)

    first.commands += bash $$PWD/dpkg-tool.sh \
        $$PWD/tool \
        $$OUTPWD_MOD \
        $$ARCH \
        $$VERSION \
        "'$$DEPENDS'"
    first.commands += ;

    # start build
    QMAKE_EXTRA_TARGETS += first
}
