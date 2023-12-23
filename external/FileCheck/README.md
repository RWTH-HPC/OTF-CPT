# LLVM FileCheck (standalone)

An attempt to build [LLVM FileCheck](https://llvm.org/docs/CommandGuide/FileCheck.html) without pulling in the 
whole LLVM Project together with its utils.

FileCheck uses many components from the LLVM monorepo.
Getting a build of FileCheck usually involves pulling and building parts of LLVM, which is not a trivial task to do.

This repository contains just the necessary parts to build FileCheck and its dependencies.

## Building FileCheck

A C++14 compiler and CMake is required.

```shell
cmake -S . -B builddir -DCMAKE_BUILD_TYPE=Release
cmake --build builddir
```

## Updating from upstream

The script `sync_from_upstream.sh` copies the sources from the LLVM Project's root directory:
```shell
./sync_from_upstream.sh path/to/the/llvm/repo
```
The script just copies the sources and headers. 
Expect that an update will probably require manual fixes to the provided `CMakeLists.txt` files.
