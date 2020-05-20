set -e
mkdir -p build || exit $?
pushd build
USE_VK=1 cmake .. && make -j "$(nproc)"
popd