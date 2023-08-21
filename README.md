# dir2bin

A dynamic library and CLI tool for archiving lightweight files and directories.

## Debian build

### Prepare

Install build tool:
```
sudo apt install fakeroot dpkg-dev
```

Install the components required to build Qt6 projects:
```
sudo apt install qt6-base-dev libqt6core6 cmake
```

> or download complete Qt6 from https://qt.io

### Build

```
git clone https://github.com/devza300/dir2bin.git
qmake6 ./dir2bin.pro
mkdir -p OUT && cd OUT && make -f ../Makefile
```

Use `qmake6` from your Qt assets, for example `~/Qt/6.4.2/gcc_64/bin/qmake6`

### Windows build

1. download complete Qt6 from https://qt.io
2. import this git repo (New project -> Import -> Clone Git)
3. build in release mode

## Usage

Help:
```
./dir2bin -h

Usage: dir2bin [options]
dir2bin helper

Options:
  -h, --help              Displays help on commandline options.
  --help-all              Displays help including Qt specific options.
  -v, --version           Displays version information.
  -s, --src <source>      Specify the source file or directory in compression
                          mode or the d2b-file in extraction mode.
  -t, --tgt <target>      Specify the d2b-file in extraction mode or the source
                          file or directory in compression mode.
  -c, --compress <level>  Set compression mode, specify compression level
                          between [-1 ... 9].
```

Compress example:
```
dir2bin -c 9 -s /home/user/dir1/ -t /home/user/dir1.d2b
```

Uncompress example:
```
dir2bin -s /home/user/dir1.d2b -t /home/user/dir1_copy/
```
