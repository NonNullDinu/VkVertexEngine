set -e

cd ../

mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make