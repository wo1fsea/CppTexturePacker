rmdir /s /q .\build
mkdir build
cd .\build
cmake -DCMAKE_TOOLCHAIN_FILE="c:/vcpkg/scripts/buildsystems/vcpkg.cmake" ..\
cmake --build .
ctest -C Debug
pause