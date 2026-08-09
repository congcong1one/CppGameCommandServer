# 交付门、模块学习目标与验收清单

Gate A对BoundedQueue、PacketCodec和内存装备强化做隔离的准入验证；Gate B复用并加固这些能力，将其接入Session、SendQueue和端到端链路。相同模块名跨Gate出现不代表重复造轮子，也不得用Gate B集成结果倒推Gate A闭卷验收已通过。

## 交付门B六个核心模块＋交付门C事务模块

| 阶段 | 模块 | Agent需要守住的学习目标 |
|---|---|---|
| Gate B | EnhanceEquipmentService | 用户自己完成业务校验和状态变化，不依赖网络与数据库 |
| Gate B | BoundedQueue | 真正理解mutex、condition_variable、谓词、容量和停止语义 |
| Gate B | CommandExecutor | 理解线程生命周期、任务执行和优雅退出 |
| Gate B | PacketCodec | 能从空白写半包、多包、非法长度解析 |
| Gate B | Session | 理解异步生命周期、shared_from_this、幂等关闭和weak_ptr晚到响应 |
| Gate B | SendQueue | 理解写操作串行化、部分发送、队列上限和慢客户端策略 |
| Gate C | Transaction / MySqlUnitOfWork | 理解RAII事务、提交/回滚和业务原子性 |

## 分阶段辅助规范

### 交付门A：C++准入包

- 项目代码与面试基础并行，不能连续多日只做阅读。
- 每个知识点至少对应一个闭卷任务、最小示例或错误实验。
- 重点验收：CMake、RAII句柄、BoundedQueue、PacketCodec、内存版装备强化、TSan竞争。
- 完成后提醒用户参加低优先级校准面试，不继续"准备到完美"。

### 交付门B：端到端求职启动包

- 只实现Login、QueryEquipment、EnhanceEquipment中的最小链路；Enhance是唯一复杂业务。
- 网络层重点：半包、多包、非法长度、SendQueue、Close、客户端断线和晚到响应。
- 要求用户能闭卷画线程图、Session所有权图，5分钟讲完整链路。
- 达到后提醒用户开始主要投递，不把MySQL作为前置门槛。

### 交付门C：反馈强化

Gate B后进入反馈强化；默认补MySQL事务。若重复面试反馈明确指向算法/Linux/代码库/实时方向，则用对应强化包替换MySQL方向，不追加并行主线。

## 项目验收清单

### 交付门A

- ☐ 15分钟建立多文件CMake工程。
- ☐ 90秒讲清编译链接。
- ☐ RAII文件描述符闭卷实现。
- ☐ BoundedQueue支持容量、等待、关闭和join。
- ☐ PacketCodec处理半包、多包、非法长度。
- ☐ 内存版装备强化失败时状态不变。
- ☐ TSan发现并修复一次数据竞争。
- ☐ 安排校准面试。

### 交付门B

- ☐ 测试客户端完成连接、查询和强化。
- ☐ Session重复关闭安全并最终析构。
- ☐ SendQueue保持有序且有上限。
- ☐ 命令线程与网络线程边界清晰。
- ☐ 请求中断线后晚到响应安全丢弃。
- ☐ ASan无已知问题。
- ☐ 用户能画线程/所有权图并讲5分钟链路。
- ☐ 开始主要投递。

### MySQL强化版（Gate C默认）

- ☐ request_id唯一约束生效。
- ☐ 同一请求重复提交只产生一次结果。
- ☐ 不同请求并发改同一装备保持正确。
- ☐ 金币不足不会变负。
- ☐ 任意中间失败全部回滚。
- ☐ 装备变化流水可用于审计。
- ☐ 数据库错误不会令进程崩溃。
