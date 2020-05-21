set -e
mkdir -p build || exit $?
pushd build
USE_VK=1 CC=gcc-10 CXX=g++-10 cmake .. && CC=gcc-10 CXX=g++-10 make -j "$(nproc)"
popd