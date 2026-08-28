# Mission: Linux C++ 游戏服务端通信基础

## Why

用户需要用 C++17、Linux 和 Boost.Asio 搭出可运行、可解释、可测试的游戏服务端通信链路，形成进入真实 C++ 生产开发岗位所需的工程证据。

## Success looks like

- 能画出并解释 `io_context → acceptor → Session → PacketCodec → SendQueue` 的调用与所有权链路。
- 能独立实现最小异步 TCP Session，正确处理半包、多包、部分发送和断线。
- 能用错误码、测试与 Sanitizer 证明 buffer、Session 和 socket 生命周期安全。

## Constraints

- 当前仍处于 Gate A；Boost.Asio 内容只作 Gate B 通信架构预习，不替换当前唯一任务。
- V1 固定为一个 Asio I/O 线程和一个 CommandExecutor 线程。
- 核心 Session 第一版由用户主写；Agent提供理论包、接口约束、Review与验证。
- 每周可投入约 6—8 小时，优先最小可观察实验，不制造额外计划债务。

## Out of scope

- C++20 协程、多 `io_context`、多 I/O 线程、通用线程池和无锁结构。
- TLS、UDP/可靠 UDP、分布式网关、微服务与平台化运维。

