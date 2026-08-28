# example Boost CMake 引入设计

## 目标

让 `example/CMakeLists.txt` 通过 CMake 查找系统已安装的 Boost，并将 Boost 头文件搜索路径传递给 `CppCommandServer` 目标，使源码可以正常使用 `#include <boost/...>`。

## 范围

- 只修改 `example/CMakeLists.txt`。
- 使用系统现有 Boost，不下载或编译其他版本。
- 不选择动态链接或静态链接。
- 不预先声明尚未使用的 Boost 编译型组件。
- 不修改当前为空的 `example/src/main.cpp`。

## CMake 设计

使用 `find_package(Boost REQUIRED)` 获取 Boost 安装信息，再通过目标级 `target_include_directories` 将 `${Boost_INCLUDE_DIRS}` 以 `PRIVATE` 方式提供给 `CppCommandServer`。

不使用 `target_link_libraries(CppCommandServer PRIVATE Boost)`，因为 `Boost` 不是有效的 CMake 导入目标，该写法会生成不存在的 `-lBoost` 链接参数。将来源码实际使用 `Boost.Thread`、`Boost.ProgramOptions` 等编译型组件时，再为具体组件增加 `find_package(... COMPONENTS ...)` 和对应的 `Boost::<component>` 目标。

## 验证

在仓库外的全新构建目录运行 CMake 配置，确认系统 Boost 被成功发现，并检查生成目标不再包含 `-lBoost`。由于 `example/src/main.cpp` 为空，本次不把可执行文件链接成功作为验收条件。
