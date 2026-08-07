# 冻结目标与技术方案

## 职业目标

- 第一跳：C++/Lua混合或初中级C++游戏服务端，但必须真正提交C++代码。
- 岗位必须尽量包含Linux构建调试、Code Review、测试和发布流程。
- 不以"纯C++"职位名称为唯一标准，以实际C++源码职责为标准。
- 长期路线：获得约一年生产C++经验后，再冲核心网络、战斗、地图或架构岗位。

## 冻结项目

| 项目项 | 冻结定义 |
|---|---|
| 仓库 | CxxGameCommandServer |
| 核心业务 | EnhanceEquipmentCommand |
| 技术栈 | C++17、Linux、CMake、Boost.Asio、Protobuf、GoogleTest、GDB、ASan/UBSan/TSan；MySQL为默认反馈强化项 |
| 线程模型 | 1个Asio I/O线程 → BoundedQueue → 1个CommandExecutor线程 → io_context.post()返回 |
| 核心完成线 | 28—32小时完成端到端内存版并开始主要投递 |
| 强化线 | 投递过程中默认补MySQL事务；可由重复面试反馈替换 |

## Agent的优先级顺序

| 优先级 | Agent应优先保障 |
|---|---|
| P0 | 正确性、安全性、项目可恢复性；不允许未理解的代码进入核心路径 |
| P1 | C++准入基础和闭卷能力；避免项目能跑但面试基础仍失败 |
| P2 | 端到端最小版本按时完成并尽早进入求职反馈 |
| P3 | 测试、调试、所有权图、故障记录与口述证据 |
| P4 | MySQL事务与生产一致性 |
| P5 | 性能、扩展、高级架构和额外业务 |

## 默认任务顺序与Agent干预点

| 任务卡 | 默认顺序 | Agent的关键干预 |
|---|---|---|
| 0—2 | 仓库恢复→CMake→RAII | 先保证可恢复、可构建；不讨论大型目录和框架 |
| 3—5 | BoundedQueue→PacketCodec→内存装备强化 | 要求用户主写核心循环，补边界/失败测试和闭卷复现 |
| 6 | Gate A验收与校准面试 | 阻止"再准备一点"；推动外部反馈并记录 |
| 7—10 | Session→SendQueue→Protobuf/CommandExecutor→Gate B加固 | 围绕生命周期、关闭、晚到响应和端到端证据，不新增业务 |
| 11 | 投递与反馈采集 | 每10次投递检查漏斗；两次重复缺口才路由强化方向 |
| 12 | Gate C条件强化 | 默认MySQL；若反馈指向算法/Linux/代码库/实时方向则替换，不追加 |

## 恢复、备份与连续性

- 第一项任务是建立本地仓库和远程备份，不把服务器作为唯一副本。
- 每次有效会话结束提交，每个小阶段推送，交付门打标签。
- 用户新增重要文件时提醒纳入Git：proto、SQL、测试、脚本、文档。
- PROJECT_STATE.md、EVIDENCE_LEDGER.md、INTERVIEW_FEEDBACK.md、PARKING_LOT.md、DECISION_LOG.md必须纳入Git。
- 中断后先构建测试、读PROJECT_STATE和最近提交，再继续，不从记忆猜测状态。
