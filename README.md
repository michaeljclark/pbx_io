# sushi

Xcode, Visual Studio and Ninja project generator

Work in progress...

## Status

* Generates Ninja build files, Xcode and Visual Studio project files
* Established initial file format for maki file (.sushi project file)
* ```sushi.xcodeproj``` and ```sushi.vsproj``` are generated by ```maki```
* Ninja support currently only supports GCC static builds (proof of concept)
* Needs toolchain search, configurable prefix and cross-compile support

## To do

* Implement conditional blocks
* Implement MSC support for Ninja
* Implement cross-project dependencies
* Implement configure time toolchain tests
* Implement network dependency resolution (github)
* Implement more flexible tool chain support for Ninja 
* Create sushi ports repository with commonly used packages
* ...

## Building

* Tested with GCC 4.9, Clang 3.3 and MSC v110 (Visual Studio 2012)
* Builds on Linux, FreeBSD, OpenBSD and NetBSD with  gmake and GCC 4.9
* Builds on Windows using Visual Studio 2012, 2013 or 2015
* Builds on OS X using Xcode or gmake (default make on OS X)
* Will be more useful when Ninja support is added


```
git submodule update --init
make -j4
```

## Example

To create the Xcode project for Sushi:
```
./build/darwin_x86_64/bin/maki sushi.sushi xcode 
```

To create the Visual Studio solution for Sushi:
```
./build/darwin_x86_64/bin/maki sushi.sushi vs
```

To create the Ninja build file for Sushi:
```
./build/darwin_x86_64/bin/maki sushi.sushi ninja
```
