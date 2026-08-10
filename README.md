# CxxGameCommandServer

C++17 / Linux 游戏命令服务器与 Agent 工程训练项目。项目围绕唯一复杂业务切片 `EnhanceEquipmentCommand`，用最小、可测试的端到端链路验证 C++ 对象生命周期、并发、TCP 字节流、错误处理，以及安全使用编码 Agent 的能力。

> 本仓库仍处于 Gate A。当前事实、验收状态和唯一任务以 [PROJECT_STATE.md](PROJECT_STATE.md) 为准；不要从本 README 推断完成度。

## 冻结范围

- 技术栈：C++17、Linux、CMake、Boost.Asio、Protobuf、GoogleTest、GDB、ASan/UBSan/TSan。
- 唯一复杂业务：装备强化；不会用新增业务数量代替核心能力证据。
- V1 线程模型：

```text
1 个 Asio I/O 线程
        ↓
  BoundedQueue
        ↓
1 个 CommandExecutor 线程
        ↓
io_context.post() 返回网络线程
```

- Gate A：独立验证 CMake、RAII、并发队列、包解析、内存业务原子性和 Sanitizer。
- Gate B：把已验证能力接入最小端到端内存版，补齐 Session、SendQueue、关闭和晚到响应证据。
- Gate C：由真实反馈触发强化，默认候选为 MySQL 事务；只替换强化方向，不向主线无限追加。

## 项目边界

Gate B 完成前不接入第二复杂业务、微服务、Redis、无锁队列、多 Worker、分片、AOI 或完整实时玩法。范围变更必须满足仓库 Skill 中的变更收益测试，并明确替换掉什么。

## AI 协作与证据

AI Coding 与 C++ 同期训练，但按证据逐级放权：Gate A 以 AI-0（闭卷形成）和 AI-1（教练/Review）为主；Gate B 引入 AI-2（受约束委派）；Gate C 才验证 AI-3（Agent 主导有边界交付）。核心模块的第一版结构和关键路径仍由用户主导，生成内容在用户 Review、解释和硬验证前不计为用户能力证据。

完整路线、自治升级条件、中间件优先级和每周节奏见 [TRAINING_PLAN.md](TRAINING_PLAN.md)。
当前可直接开始的闭卷任务见 [Gate A卡1：15分钟多文件CMake工程](tasks/gate-a-card-01-cmake.md)。

## 仓库协议

- [AGENTS.md](AGENTS.md)：所有 Agent 的 Git、状态同步和隐私规则。
- [项目 Skill](.qoder/skills/cxx-game-server-transition-coach/SKILL.md)：辅导模式、所有权、范围控制与反馈路由。
- [训练方案](TRAINING_PLAN.md)：C++ 与 Agent 工程的阶段化训练、Gate 映射和直接开练入口。
- [DECISION_LOG.md](DECISION_LOG.md)：影响总体方案的决定。
- [EVIDENCE_LEDGER.md](EVIDENCE_LEDGER.md)：可验证能力证据。
- [INTERVIEW_FEEDBACK.md](INTERVIEW_FEEDBACK.md)：脱敏后的面试事实与重复缺口。
- [PARKING_LOT.md](PARKING_LOT.md)：暂不接入主项目的主题。

## 构建状态

当前已形成最小多文件 CMake 工程。经过验证的命令为：

```bash
cmake -S . -B build
cmake --build build
./build/CppCommandServer
```

程序当前输出两行可观察结果。仓库仍处于 Gate A，最新完成度和唯一任务以 [PROJECT_STATE.md](PROJECT_STATE.md) 为准。

## License

本仓库使用 [GNU General Public License v3.0](LICENSE)。
