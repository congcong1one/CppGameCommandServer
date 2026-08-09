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

## 2026-08-07 隐私脱敏与隐私红线

- 决定：仓库为公开可见性风险区，全面脱敏并建立隐私红线：移除AGENTS.md中的真实姓名、SKILL.md中的具体商业项目名称；AGENTS.md新增"隐私红线"章节；用户计划将仓库改为private。
- 触发证据：用户提醒仓库为public并要求不泄露个人隐私，随后提出改为private。
- 替换/删除：已推送历史中仍残留旧版含敏感信息的提交；仓库转private后该风险降级，如需彻底清除再另行处理。

## 2026-08-09 原始规范与仓库执行层分离

- 决定：V1.2原始Word是设计基线，不纳入Git；仓库内`AGENTS.md`、项目Skill和五个状态文件是脱敏后的执行层。两者冲突时，按“已验证代码/测试 → `PROJECT_STATE.md` → 其他状态记录 → 项目Skill → 原始规范”的顺序处理，隐私红线始终优先。
- 触发证据：用户明确说明原始Word与仓库非src文件的生成关系，并授权对非src执行层做一致性优化。
- 替换/删除：不改变冻结技术方案和Gate；替换原有“原文内容可直接落盘”的隐含假设。
