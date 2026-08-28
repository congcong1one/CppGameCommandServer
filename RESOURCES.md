# Boost.Asio 通信基础资源

## Knowledge

- [Boost.Asio 1.83 官方参考](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/reference.html)
  API总入口。用于核对本机Boost 1.83的类型、重载、参数、完成签名和线程安全契约。
- [Boost.Asio 官方教程](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/tutorial.html)
  从`io_context`、timer到TCP客户端/服务端的渐进示例。用于第一次建立异步控制流。
- [`io_context::run`](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/reference/io_context/run.html)
  事件循环的阻塞、退出与返回值契约。
- [`basic_socket_acceptor`](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/reference/basic_socket_acceptor.html)
  监听socket的open、bind、listen、accept和close入口。
- [`async_read`](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/reference/async_read.html) 与 [`async_write`](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/reference/async_write.html)
  组合操作的完整读取/写入语义。用于定长包头、包体和SendQueue。
- [`buffer`](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/reference/buffer.html)
  内存视图、可变/只读buffer和失效规则。用于排查异步悬空内存。
- [Boost.Asio C++11示例](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/examples/cpp11_examples.html)
  包含echo server、超时和干净停止示例。用于完成最小骨架后的对照Review。

## Wisdom (Communities)

- [Boost Users邮件列表](https://lists.boost.org/mailman/listinfo.cgi/boost-users)
  适合在最小复现、版本号和平台信息齐全后核对疑难行为；不替代官方契约和本地实验。

