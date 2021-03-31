#!/bin/bash
#编译整体库or各个模块
set -e
set -x

pushd $(dirname ${BASH_SOURCE[0]})
if [[ $# -eq 0 ]]; then
    rm -rf build built
    mkdir -p build
fi
cd build
# use cuda torch, -DUSE_TORCH_CUDA
install_prefix=/data/code/ml-platform-thirdparty/ml_unity
cmake3  -DCMAKE_BUILD_TYPE=RelWithDebInfo                     \
                -DCMAKE_INSTALL_PREFIX=$install_prefix        \
                -DDEPS_DIR=/data/code/ml-platform-thirdparty  \
                -DUSE_TORCH_CUDA=OFF                          \
                -DBUILD_SHARED_LIBS=OFF ..
make -j16 && make
popd
echo OK
