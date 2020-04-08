rmdir /s /q .\build
mkdir build
cd .\build
cmake -DCMAKE_TOOLCHAIN_FILE="e:/vcpkg/scripts/buildsystems/vcpkg.cmake" ..\
cmake --build .
ctest -C Debug
pause