cd ../

mkdir build 2> NUL
cd build

cmake .. && msbuild VertexEngine.sln -m

PAUSE