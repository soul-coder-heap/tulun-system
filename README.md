# vcpkg + cmake

### vcpkg镜像 


> [个人镜像](https://hub.docker.com/r/ox80/ml-platform/tags?page=1&ordering=last_updated)

> docker pull ox80/ml-platform:0.2

* vcpkg 路径为 `/vcpkg/scripts/buildsystems/vcpkg.cmake`

### vcpkg list

> json11、fmt、 libhv、 boost、 spdlog、 libmysql、 curl ...


### 编译

* 下载镜像后执行 `start_docker.sh` 挂载


```
sudo yum install devtoolset-7

scl enable devtoolset-7 bash
```

* 容器中编译时先执行 `scl enable devtoolset-7 bash` 将 g++ 切换为7.3
* 亦或者在 CMakeLists.txt 中设置 g++ 路径为 ` /opt/rh/devtoolset-7/root/usr/bin/c++`
* 需要 g++ 其他版本需 `yum install devtoolset-V` 选择即可

```
set(CMAKE_CXX_COMPILER /opt/rh/devtoolset-7/root/usr/bin/g++)

set(CMAKE_C_COMPILER /opt/rh/devtoolset-7/root/usr/bin/gcc)
```
