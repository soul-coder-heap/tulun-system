#!/bin/bash
#编译整体库or各个模块
set -e
set -x
pushd $(dirname ${BASH_SOURCE[0]})
if [[ $# -eq 0 ]]; then
    rm -rf build built
    mkdir -p build
    mkdir -p built
fi
cd build
build_type=Asan
build_type=RelWithDebInfo

cmake -DCMAKE_BUILD_TYPE=${build_type}       \
      -DCMAKE_INSTALL_PREFIX=../built        \
      -DNO_DEBUG=ON                          \
      -DBUILD_SHARED_LIBS=OFF ..
make -j4 && make
popd
echo OK
