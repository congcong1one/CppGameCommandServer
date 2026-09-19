# 仓库内客户端依赖

适用环境：**Ubuntu 24.04 / x86_64**，包括 WSL2。`example` 和 `LoginServer` 通过 `cmake/GameDependencies.cmake` 使用这套文件。头文件、共享库、相对符号链接、`protoc`、许可证和校验值都应一起进入 Git；不是 Git LFS 指针，也不要求再次编译这些库。

| 依赖 | 版本 | 接入目标 / 头文件 |
|---|---|---|
| MySQL Connector/C++ | 26.7.0 | `mysql::concpp-jdbc` / `<mysql/jdbc.h>` |
| redis++ | 1.3.15 | `redis++::redis++` / `<sw/redis++/redis++.h>` |
| hiredis | 安装库与 pkg-config 报告 1.4.0 | `hiredis::hiredis` / `<hiredis/hiredis.h>` |
| Protobuf + protoc | 3.21.12 | `protobuf::libprotobuf`、`protobuf::protoc` |

`GameServer::Dependencies` 汇总经典 MySQL SQL 接口、Redis、Protobuf 和线程依赖，并要求 C++17。MySQL X DevAPI 库也已携带，可单独链接 `mysql::concpp`；默认的连接方式仍是经典 SQL 协议，不要求 X Plugin。

## 新环境使用

需要基本的 GCC 13/CMake 3.28 工具链和原有示例的 Boost 1.83 头文件。全新 Ubuntu 24.04 可以先执行：

```sh
sudo apt-get update
sudo apt-get install -y build-essential cmake libboost1.83-dev libssl3t64 zlib1g
```

这些是编译工具和系统运行库；**无需安装 MySQL C++ 开发包、redis++、hiredis、Protobuf 开发包或 protoc**。不设置全局 `LD_LIBRARY_PATH`。客户端共享库和 protoc 已配置相对运行时搜索路径，应用的构建目录由 CMake 自动加入库搜索路径。

在仓库根目录执行：

```sh
cmake -S example -B example/build -DCMAKE_BUILD_TYPE=Debug
cmake --build example/build --parallel 1
cmake -S LoginServer -B LoginServer/build -DCMAKE_BUILD_TYPE=Debug
cmake --build LoginServer/build --parallel 1
```

两个工程沿用现有 VS Code 调试配置的可执行文件名 `CppCommandServer`。`LoginServer/src/main.cpp` 目前仅为可构建的空入口，不包含登录业务实现。两个保存触发的构建脚本默认单任务，可用 `CMAKE_BUILD_PARALLEL_LEVEL` 明确调整。

在新目录重新运行 CMake；不要复制旧的 `build/`、CMake 缓存或 `compile_commands.json`。此方案不承诺旧构建目录或独立拷走的可执行文件可以直接运行。

## 添加自己的 proto 文件

已有依赖模块提供 CMake 的 `protobuf_generate`。在目标定义后添加实际文件，例如：

```cmake
target_sources(CppCommandServer PRIVATE proto/login.proto)
target_include_directories(CppCommandServer PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate(TARGET CppCommandServer APPEND_PATH)
```

`login.proto` 由业务自行定义；生成的 `.pb.cc/.pb.h` 留在构建目录。仓库内 `tests/dependencies/probe.proto` 已验证代码生成、编译和序列化往返，编译器和运行库均固定为 3.21.12。

## 验证

```sh
(cd third_party/ubuntu24.04-x86_64 && sha256sum --check --quiet SHA256SUMS)
cmake -S tests/dependencies -B tests/dependencies/build -DCMAKE_BUILD_TYPE=Release
cmake --build tests/dependencies/build --parallel 1
ctest --test-dir tests/dependencies/build --output-on-failure
ldd tests/dependencies/build/dependency_smoke
```

CTest 不依赖正在运行的数据库，也不写入任何数据。`ldd` 中 MySQL/redis++/hiredis/Protobuf 应指向仓库内 `third_party`；glibc、libstdc++、OpenSSL、zlib 等基础运行库来自 Ubuntu。

可选的本地服务检查（MySQL 使用 Ubuntu 默认 root Unix socket 认证，Redis 使用本机默认端口且未配置认证）：

```sh
sudo tests/dependencies/build/dependency_smoke --local-services
```

它仅执行 `SELECT 1` 和 `PING`，不创建账号、数据库、表或 Redis 键。服务认证方式不同则单独提供应用配置，不要修改数据库权限来迁就测试。

## 边界与维护

- MySQL/Redis **服务端、数据、数据库账号和密码不属于客户端依赖包**，仍由目标环境或已有远程服务提供。
- 本包验证的平台为 Ubuntu 24.04 / x86_64；原生 Windows/macOS、ARM 和较老 Linux 需要另一套二进制。系统运行库要求见 `manifest.json`。
- 经典连接器包含常用认证支持。本包不携带可选的 Kerberos/LDAP/WebAuthn/OCI 等客户端认证插件；需要这些认证方式时使用对应官方插件包。
- `manifest.json` 记录来源、版本、原始二进制 SHA-256 和加工步骤。复制后的 ELF 已去除调试符号，并设置共享库 `$ORIGIN`、protoc `$ORIGIN/../lib` 的 RUNPATH。所有本包文件的最终 SHA-256 见 `SHA256SUMS`，许可证在 `licenses/`。
- hiredis 来源目录名为 1.4.1，但这份现有安装的 pkg-config、头文件和共享库版本为 1.4.0，记录以实际打包文件为准。
- 升级依赖时整体更新头文件、运行库、protoc、许可证、清单及校验值，然后重新执行自测和换目录构建；不要只替换单个 `.so`。
- 文件已准备为可纳入 Git 的工作树内容。按仓库协作规则，只有明确授权提交/同步后才生成提交和推送；换机器前需要把这批文件提交并推送。

来源：[MySQL Connector/C++](https://dev.mysql.com/downloads/connector/cpp/)、[redis++](https://github.com/sewenew/redis-plus-plus)、[hiredis](https://github.com/redis/hiredis)、[Protobuf](https://github.com/protocolbuffers/protobuf)。
