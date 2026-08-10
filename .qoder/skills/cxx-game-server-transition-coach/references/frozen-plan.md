# 冻结目标与技术方案

## 职业目标

- 第一跳：C++/Lua混合、初中级C++游戏服务端，或实际以Agent协作交付且接受当前C++履历的工程岗位；都必须能够真正提交并负责源码。
- 岗位必须尽量包含Linux构建调试、Code Review、测试和发布流程。
- 不以"纯C++"职位名称为唯一标准，以实际C++源码职责为标准。
- 长期路线：获得约一年生产C++经验后，再冲核心网络、战斗、地图或架构岗位。
- Agent工程是横向能力，不把“AI原生”理解为放弃语言、系统和后端基础；不因岗位标题热点立刻切换主语言。

## 冻结项目

| 项目项 | 冻结定义 |
|---|---|
| 仓库 | CxxGameCommandServer |
| 核心业务 | EnhanceEquipmentCommand |
| 技术栈 | C++17、Linux、CMake、Boost.Asio、Protobuf、GoogleTest、GDB、ASan/UBSan/TSan；MySQL为默认反馈强化项 |
| 线程模型 | 1个Asio I/O线程 → BoundedQueue → 1个CommandExecutor线程 → io_context.post()返回 |
| 核心完成线 | 28—32小时完成端到端内存版并开始主要投递 |
| 强化线 | 投递过程中默认补MySQL事务；可由重复面试反馈替换 |
| Agent自治 | Gate A以AI-0/1为主；Gate B引入AI-2；Gate C验证AI-3；按任务证据升级或回退 |

## Agent的优先级顺序

| 优先级 | Agent应优先保障 |
|---|---|
| P0 | 正确性、安全性、项目可恢复性；不允许未理解的代码进入核心路径 |
| P1 | C++准入基础和闭卷能力；避免项目能跑但面试基础仍失败 |
| P2 | Agent任务定义、Diff Review、测试预言与硬验证；不让提速削弱判断能力 |
| P3 | 端到端最小版本按时完成并尽早进入求职反馈 |
| P4 | 测试、调试、所有权图、故障记录、口述与AI贡献边界证据 |
| P5 | MySQL事务与生产一致性 |
| P6 | 性能、扩展、高级架构和额外业务 |

## 默认任务顺序与Agent干预点

| 任务卡 | 默认顺序 | Agent的关键干预 |
|---|---|---|
| 0—2 | 仓库恢复→CMake→RAII | AI-0首版，先保证可恢复、可构建；完成后进入AI-1 Review |
| 3—5 | BoundedQueue→PacketCodec→内存装备强化 | AI-0/1，用户主写核心循环，补边界/失败测试和闭卷复现 |
| 6 | Gate A验收与校准面试 | 阻止"再准备一点"；推动外部反馈并记录 |
| 7—10 | Session→SendQueue→Protobuf/CommandExecutor→Gate B加固 | 风险路径AI-1；样板/夹具/脚本可AI-2；完成一次有任务简报、拒绝/修正和硬验证的多文件委派 |
| 11 | 投递与反馈采集 | 每10次投递检查漏斗；两次重复缺口才路由强化方向 |
| 12 | Gate C条件强化 | 默认MySQL和最小Docker/恢复；验证一次AI-3交付；若反馈指向其他方向则替换，不追加 |

Agent提速用于**替换**低价值手工样板，不另开并行课程，因此28—32小时Gate B完成线不增加。完整执行矩阵见仓库根目录 `TRAINING_PLAN.md`。

## 恢复、备份与连续性

- 第一项任务是建立本地仓库和远程备份，不把服务器作为唯一副本。
- 新主机或新任务批次启动时检查并同步一次；连续批次内本地记录，用户明确提交/同步/收尾或跨主机交接时统一审查、提交、拉取和推送；交付门打标签。
- 用户新增重要文件时提醒纳入Git：proto、SQL、测试、脚本、文档。
- PROJECT_STATE.md、EVIDENCE_LEDGER.md、INTERVIEW_FEEDBACK.md、PARKING_LOT.md、DECISION_LOG.md必须纳入Git。
- 中断后先构建测试、读PROJECT_STATE和最近提交，再继续，不从记忆猜测状态。
