cd ../

mkdir build 2> NUL

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
msbuild SandBox.sln