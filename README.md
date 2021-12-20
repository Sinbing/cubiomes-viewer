# 汉化说明：

通过直接修改硬编码的qt file的方式，实现了对软件的99%汉化

一代目汉化：Sinbing
二代目汉化：SunnySlopes

现在距离100%汉化还差：一些大段的帮助提示，预置组件（比如一些按钮）以及一些基本不会碰到的debug信息

其中群系名称的汉化因为涉及到子模块的修改，相应的修改请参照https://github.com/SunnySlopes/cubiomes

以下为原README



# Cubiomes Viewer

Cubiomes Viewer provides a graphical interface for the efficient and flexible seed-finding utilities provided by [cubiomes](https://github.com/Cubitect/cubiomes) and a map viewer for the Minecraft biomes and structure generation.

The tool is designed for high performance and supports Minecraft Java Edition main releases 1.0 - 1.18.


## Download

Precompiled binaries can be found for Linux and Windows under [Releases on github](https://github.com/Cubitect/cubiomes-viewer/releases). The builds are statically linked against [Qt](https://www.qt.io) and should run as-is on most newer distributions.

For the linux build you will probably have to add the executable flags to the binary (github seems to remove them upon upload).

For Arch linux users, the tool may be found in the [AUR](https://aur.archlinux.org/packages/cubiomes-viewer/) thanks to [JakobDev](https://github.com/JakobDev).


## Build from source

Install Qt5 development files
```
$ sudo apt install build-essential qt5-default
```
get sources
```
$ git clone --recursive https://github.com/Cubitect/cubiomes-viewer.git
```
prepare build directory
```
$ cd cubiomes-viewer
$ mkdir build
$ cd build
```
build cubiomes-viewer
```
$ qmake ..
$ make
```

