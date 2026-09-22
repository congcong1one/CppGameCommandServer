# Python TCP 测试客户端

`tcp_client.py` 仅使用 Python 标准库。默认连接当前服务端入口使用的 `127.0.0.1:9981`，默认发送原始字节；在服务端所在的 Ubuntu / WSL 环境运行最直接。

以下命令都在仓库根目录执行。需要先由用户构建、启动 C++ 服务端；客户端不会修改或启动服务端。

## 连接、发送与接收

交互模式：连接后每输入一行就发送一次，回包会异步打印；输入 `/quit` 或按 Ctrl+C 退出。

```bash
python3 NetworkCore/tools/tcp_client.py
```

单次发送 `abc`，接收最多 2 秒后退出：

```bash
python3 NetworkCore/tools/tcp_client.py --send abc
```

指定地址、端口及接收窗口：

```bash
python3 NetworkCore/tools/tcp_client.py --host 127.0.0.1 --port 9981 --send abc --wait 5
```

服务端暂时只收不回时，可只发送后关闭连接：

```bash
python3 NetworkCore/tools/tcp_client.py --send abc --wait 0
```

`--wait` 是发送结束后的总接收窗口，不是每收到一段就重新计时；它到期后会关闭客户端连接。交互模式在输入期间持续接收，标准输入 EOF 后再等待这个窗口；`/quit` 立即结束。`--timeout` 默认 3 秒，用于连接和 socket I/O。服务端断开时会显示提示；如果仍停在输入提示符，按回车退出。

原始模式的每条 `[recv raw]` 表示一次 `recv` 得到的字节块，不代表一个应用消息。每条输出最多预览 64 字节，但不会截断实际收发数据。

## 按当前 PacketHeader 发送

```bash
python3 NetworkCore/tools/tcp_client.py --mode frame --msg-id 1 --seq 1 --player-id 0 --send hello
```

也可以运行 `--mode frame` 后交互输入，每行自动包装成一帧。帧格式固定如下，不使用 C++ 结构体的内存布局或对齐：

| 字段 | 大小 | 编码 |
|---|---|---|
| magic | 2 字节 | `0xFCCD`，大端 |
| length | 4 字节 | body 的字节数，不包含头部，大端 |
| msgId | 4 字节 | `--msg-id`，默认 1，大端 |
| seq | 4 字节 | `--seq` 起始，默认 1，每帧递增，大端 |
| playerId | 4 字节 | `--player-id`，默认 0，大端 |
| body | length 字节 | 默认 UTF-8 文本，也可用 `--hex` 输入二进制 |

头部共 **18 字节**，使用 `struct.Struct("!HIIII")`。帧模式接收端会积累半帧、循环提取连续的完整帧；魔数错误、body 长度超过 16 KiB 或接收结束时残留半帧均会报错。16 KiB 取自当前长度常量，不代表 C++ 缓冲区已通过相同容量验收。

此客户端尚未编码 Protobuf；当前 body 是测试字节。以后可以将已序列化的 Protobuf 字节以十六进制输入，不需要改变帧格式。

## 分段发送与连续帧

按 5 字节拆分一次帧发送，每段之间等待 0.2 秒：

```bash
python3 NetworkCore/tools/tcp_client.py --mode frame --send hello --chunk-size 5 --chunk-delay 0.2
```

把两个帧合并成一次 `sendall`：

```bash
python3 NetworkCore/tools/tcp_client.py --mode frame --send first --send second --batch
```

这些选项控制客户端的写调用，**不能强制服务端每次 recv 的分段边界**。TCP 仍可能拆分或合并数据。两个选项组合使用时，先合并所有帧，再按 chunk-size 拆分。

输入二进制 body：

```bash
python3 NetworkCore/tools/tcp_client.py --mode frame --hex --send "00 ff 80 01"
```

想自行构造非法头部或长度时，使用 `--mode raw --hex` 直接输入完整线协议字节；帧模式会生成规范头部。

## 如何理解结果

- `[connected]`：TCP 连接建立。
- `[sent]`：客户端完成提交这些字节，不代表服务端已解析或处理。
- `[recv raw]` / `[recv frame]`：实际收到的字节块 / 完整帧。
- `[summary]`：接收总字节数及完整帧数。
- `ConnectionRefusedError`：指定地址没有接受本次连接的监听服务，先检查服务端启动情况。
- `no response`：接收窗口结束前没有回包，或者服务端无回包就关闭；也可能是服务端尚未实现回包，不等于客户端发送失败。

退出码：正常操作为 0，连接/收发/协议错误或单次发送等待回包却未收到为 1，命令参数错误为 2，Ctrl+C 为 130。退出 0 不校验业务响应，也不保证每个请求都有对应响应；连续帧测试请核对 seq 与 received_frames。

## 客户端自身验证

```bash
python3 -B NetworkCore/tools/test_tcp_client.py -v
```

9 项测试使用本机临时端口和真实 TCP 对端，覆盖交互与单次收发、二进制分段发送、多帧及分段回包、非法魔数、超长帧、截断、无响应超时和只发不收。它们不代表 C++ 服务端通过验收。
