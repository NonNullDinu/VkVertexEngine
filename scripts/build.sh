set -e
cd ..
mkdir -p build || exit $?
pushd build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j "$(nproc)"
popd
