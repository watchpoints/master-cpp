//! Task management implementation
//!
//! Everything about task management, like starting and switching tasks is
//! implemented here.
//!
//! A single global instance of [`TaskManager`] called `TASK_MANAGER` controls
//! all the tasks in the whole operating system.
//!
//! A single global instance of [`Processor`] called `PROCESSOR` monitors running
//! task(s) for each core.
//!
//! A single global instance of `PID_ALLOCATOR` allocates pid for user apps.
//!
//! Be careful when you see `__switch` ASM function in `switch.S`. Control flow around this function
//! might not be what you expect.
mod context;
mod id;
mod manager;
mod processor;
mod switch;
#[allow(clippy::module_inception)]
mod task;

use crate::loader::get_app_data_by_name;
use alloc::sync::Arc;
use lazy_static::*;
pub use manager::{fetch_task, TaskManager};
use switch::__switch;
pub use task::{TaskControlBlock, TaskStatus};

pub use context::TaskContext;
pub use id::{kstack_alloc, pid_alloc, KernelStack, PidHandle};
pub use manager::add_task;
pub use processor::{
    current_task, current_trap_cx, current_user_token, run_tasks, schedule, take_current_task,
    Processor,
};

// use crate::syscall::process::TaskInfo;
use crate::syscall::TaskInfo;
// use crate::config::MAX_SYSCALL_NUM;
use crate::timer::get_time_ms;
use crate::mm::VirtAddr;

/// Suspend the current 'Running' task and run the next task in task list.
pub fn suspend_current_and_run_next() {
    // There must be an application running.
    let task = take_current_task().unwrap();

    // ---- access current TCB exclusively
    let mut task_inner = task.inner_exclusive_access();
    let task_cx_ptr = &mut task_inner.task_cx as *mut TaskContext;
    // Change status to Ready
    task_inner.task_status = TaskStatus::Ready;
    drop(task_inner);
    // ---- release current PCB

    // push back to ready queue.
    add_task(task);
    // jump to scheduling cycle
    schedule(task_cx_ptr);
}

/// pid of usertests app in make run TEST=1
pub const IDLE_PID: usize = 0;

/// Exit the current 'Running' task and run the next task in task list.
pub fn exit_current_and_run_next(exit_code: i32) {
    // take from Processor
    let task = take_current_task().unwrap();

    let pid = task.getpid();
    if pid == IDLE_PID {
        println!(
            "[kernel] Idle process exit with exit_code {} ...",
            exit_code
        );
        panic!("All applications completed!");
    }

    // **** access current TCB exclusively
    let mut inner = task.inner_exclusive_access();
    // Change status to Zombie
    inner.task_status = TaskStatus::Zombie;
    // Record exit code
    inner.exit_code = exit_code;
    // do not move to its parent but under initproc

    // ++++++ access initproc TCB exclusively
    {
        let mut initproc_inner = INITPROC.inner_exclusive_access();
        for child in inner.children.iter() {
            child.inner_exclusive_access().parent = Some(Arc::downgrade(&INITPROC));
            initproc_inner.children.push(child.clone());
        }
    }
    // ++++++ release parent PCB

    inner.children.clear();
    // deallocate user space
    inner.memory_set.recycle_data_pages();
    drop(inner);
    // **** release current PCB
    // drop task manually to maintain rc correctly
    drop(task);
    // we do not have to save task context
    let mut _unused = TaskContext::zero_init();
    schedule(&mut _unused as *mut _);
}

lazy_static! {
    /// Creation of initial process
    ///
    /// the name "initproc" may be changed to any other app name like "usertests",
    /// but we have user_shell, so we don't need to change it.
    pub static ref INITPROC: Arc<TaskControlBlock> = Arc::new(TaskControlBlock::new(
        get_app_data_by_name("ch5b_initproc").unwrap()
    ));
}

///Add init process to the manager
pub fn add_initproc() {
    add_task(INITPROC.clone());
}


/// 封装获取当前任务信息的函数
pub fn get_task_info() -> TaskInfo {
    // 从cpu 管理器中获取 当前正在执行状态信息
    let cur_exec_task = current_task().unwrap();
    // TaskControlBlock
    let mutable_inner = cur_exec_task.inner_exclusive_access();

    let task_info = TaskInfo {
        status: TaskStatus::Running,
        syscall_times:mutable_inner.syscall_count.clone(),
        //运行时间 = 当前时间 - 任务开始时间
        time: get_time_ms() - mutable_inner.start_time,
    };
    task_info
}

/// add_syscall_count
pub fn add_syscall_count(sys_call_id : usize)
{
    let cur_exec_task = current_task().unwrap(); //获取当前任务 clone方式。不影响原来使用

    cur_exec_task.inner_exclusive_access().syscall_count[sys_call_id] += 1;
}



/// current_task_map
pub fn current_task_map(start: usize, len: usize, port: usize) -> isize {
    
    //对齐
    let stat_virtadd = VirtAddr::from(start);
    if stat_virtadd.aligned() == false {
        return -1;
    }
    //current: Option<Arc<TaskControlBlock>>, 
    let cpu_cur_task = current_task().unwrap();
    //inner: UPSafeCell<TaskControlBlockInner>,
    let mut task_inner = cpu_cur_task.inner_exclusive_access();

    task_inner.memory_set.range_map_to_physical(start, len, port)


}

/// current_task_unmap
pub fn current_task_unmap(start: usize, len: usize) -> isize {

    let cpu_cur_task = current_task().unwrap();
    let mut task_inner = cpu_cur_task.inner_exclusive_access();
    task_inner.memory_set.unmap_range(start, len)
}

/// 设置cpu正在执行任务的优先级
pub fn set_current_task_priority(prio: usize) {
    let cpu_cur_task = current_task().unwrap();
    let mut task_inner = cpu_cur_task.inner_exclusive_access();
    task_inner.priority = prio;
}