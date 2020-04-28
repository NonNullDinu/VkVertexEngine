set -e
cd ..
mkdir -p build || exit $?
pushd build
cmake .. && make -j "$(nproc)"
popd

cd ../scripts
