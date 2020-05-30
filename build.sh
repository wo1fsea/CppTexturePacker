rm -r ./build 
mkdir ./build
cd ./build/ 
cmake -DCMAKE_TOOLCHAIN_FILE="~/vcpkg/scripts/buildsystems/vcpkg.cmake" ../ 
cmake --build .
ctest -C Debug