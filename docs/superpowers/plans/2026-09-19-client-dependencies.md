# 客户端依赖接入实施计划

目标：在 Ubuntu 24.04 / x86_64 上，example 和 LoginServer 共用仓库内客户端依赖，克隆到新目录后无需重新编译、安装这些连接库。

已确认范围：MySQL Connector/C++ 26.7.0（经典 SQL 接口）、redis++ 1.3.15、hiredis 1.4.0、Protobuf/protoc 3.21.12。MySQL/Redis 服务、数据和凭据不进入仓库。既有业务源码不改动；LoginServer 的空 main.cpp 仅补可构建入口。用户已授权直接完成这项环境配置。

- [x] 安装缺失的官方 MySQL C++ 连接器，复用已有数据库服务。
- [x] 用 tests/dependencies 验证真实库链接、protoc 生成及序列化，接入前应配置失败。
- [x] 将头文件、共享库、protoc、许可证、来源和校验值打包到 third_party/ubuntu24.04-x86_64，运行时路径使用相对路径，去除本机调试路径。
- [x] 新建 cmake/GameDependencies.cmake，提供统一 GameServer::Dependencies 目标与 protobuf_generate；两个子工程链接该目标。
- [x] 两个 VS Code 构建脚本默认单任务，避免当前机器的内存峰值。
- [x] 完成两个工程的 Debug 构建、依赖自测、本地只读服务检查及不同目录下的构建/动态库路径验证。
- [x] 更新 PROJECT_STATE 和依赖使用文档；不把环境工作记为独立能力验收，不提交或推送 Git。

验证命令（从仓库根目录）：

```sh
cmake -S example -B example/build -DCMAKE_BUILD_TYPE=Debug
cmake --build example/build --parallel 1
cmake -S LoginServer -B LoginServer/build -DCMAKE_BUILD_TYPE=Debug
cmake --build LoginServer/build --parallel 1
cmake -S tests/dependencies -B tests/dependencies/build -DCMAKE_BUILD_TYPE=Release
cmake --build tests/dependencies/build --parallel 1
ctest --test-dir tests/dependencies/build --output-on-failure
```

服务验证仅运行 SELECT 1 和 PING；使用本地 Unix socket 的 root 系统身份验证，不创建账号、数据库或业务记录。换目录验证复制工作树中相关输入到仓库外临时目录，并检查 ldd 确实加载临时目录里的客户端库。
