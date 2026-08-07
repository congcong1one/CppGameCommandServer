# DECISION_LOG — 重大决定记录

> 任何改变总体方案、交付门或任务替换的决定必须在此留痕。格式见Skill模板 templates/change-request.md。

## 2026-08-07 初始化

- 决定：按《辅助Agent运行规范V1.2》建立五个状态文件并纳入Git；Skill已落盘为 `cxx-game-server-transition-coach`。
- 触发证据：用户提供规范文档并要求固化。
- 替换/删除：无。

## 2026-08-07 跨主机多Agent共享进度

- 决定：本GitHub仓库作为多主机多Agent之间的唯一进度共享总线；新增 AGENTS.md 协作协议；Skill从个人目录迁移到工程内 `.qoder/skills/cxx-game-server-transition-coach/`（项目级Skill，随仓库同步到其他主机）。
- 触发证据：用户明确要求与其他主机上的Agent通过本仓库共享进度。
- 替换/删除：删除个人目录 `~/.qoder/skills/cxx-game-server-transition-coach/`（避免重复加载）。
