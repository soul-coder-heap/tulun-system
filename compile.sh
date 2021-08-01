#!/bin/bash
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
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON     \
      -DNO_DEBUG=ON                          \
      -DBUILD_SHARED_LIBS=OFF ..
cpus=`nproc --all`
echo "cpus core : $cpus"
make -j${cpus} && make install
popd
echo OK
