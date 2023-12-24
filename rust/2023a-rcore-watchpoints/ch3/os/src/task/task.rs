//! 与任务管理相关的类型

use super::TaskContext;

/// 任务的任务控制块（TCB）。
#[derive(Copy, Clone)]
pub struct TaskControlBlock {
    /// 任务在其生命周期中的状态
    pub task_status: TaskStatus,
    /// 任务上下文
    pub task_cx: TaskContext,
    
    ///记录task 启动的时间 
    pub start_time: usize,
    ///记录system调用task的次数 
    pub syscall_count: [u32;MAX_SYSCALL_NUM], 
}

/// 任务状态
#[derive(Copy, Clone, PartialEq)]
pub enum TaskStatus {
    /// 未初始化
    UnInit,
    /// 准备就绪
    Ready,
    /// 运行中
    Running,
    /// 已退出
    Exited,
}
