# CppTexturePacker ![build status](https://api.travis-ci.com/wo1fsea/CppTexturePacker.svg?branch=master)
A C++ Texture Packer Toolkit (The C++ version of [PyTexturePacker](https://github.com/wo1fsea/PyTexturePacker))

## Build
### Use Vcpkg for Building
To install [vcpkg](https://github.com/microsoft/vcpkg)

```
> git clone https://github.com/Microsoft/vcpkg.git
> cd vcpkg

PS> .\bootstrap-vcpkg.bat
Linux:~/$ ./bootstrap-vcpkg.sh
```

### Use Vcpkg to Install Dependencies 

```
PS> .\vcpkg install boost-test boost-format boost-filesystem cimg libpng libplist libjpeg-turbo pybind11 utfcpp
Linux:~/$ ./vcpkg install boost-test boost-format boost-filesystem cimg libpng libplist libjpeg-turbo pybind11 utfcpp
```

### Use CMake to Build

```
> cd CppTexturePacker
> mkdir ./build
> cd ./build 
> cmake -DCMAKE_TOOLCHAIN_FILE="[your_vcpkg_root]/scripts/buildsystems/vcpkg.cmake" ../ 
> cmake --build ./
> ctest
```