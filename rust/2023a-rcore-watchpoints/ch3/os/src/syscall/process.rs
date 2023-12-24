//! Process management syscalls
use crate::{
    config::MAX_SYSCALL_NUM,
    task::{exit_current_and_run_next, suspend_current_and_run_next, TaskStatus},
    timer::get_time_us,
    task::{get_task_info},
    mm::{get_physical},
};

#[repr(C)]
#[derive(Debug)]
pub struct TimeVal {
    pub sec: usize,
    pub usec: usize,
}

/// Task information
#[allow(dead_code)]
pub struct TaskInfo {
    /// Task status in it's life cycle
    status: TaskStatus,
    /// The numbers of syscall called by task
    syscall_times: [u32; MAX_SYSCALL_NUM],
    /// Total running time of task
    time: usize,
}

/// task exits and submit an exit code
/// 功能：退出应用程序并将返回值告知批处理系统。
/// 参数：`exit_code` 表示应用程序的返回值。
/// 返回值：该系统调用不应该返回。
/// syscall ID：93

//在 Rust 中，-> ! 表示一个不返回的类型。
//这意味着函数 sys_exit 在执行后不会返回到调用它的地方。
//实际上，这个函数会终止整个程序的执行，不会再继续执行程序的其他部分。
//这是因为 sys_exit 函数的主要目的是通知操作系统或批处理系统应用程序已经退出，同时提供了一个退出码（exit_code），然后终止应用程序的执行。
//通常情况下，当一个函数的返回类型被标记为 ! 时，它通常会以 panic、abort 或类似的方式来终止程序的执行。
//在这种情况下，panic! 宏被用来引发一个 panic，以确保程序的执行被立即中止

pub fn sys_exit(exit_code: i32) -> ! {
    trace!("[kernel] Application exited with code {}", exit_code);
    exit_current_and_run_next();
    panic!("Unreachable in sys_exit!");
}

//exit_current_and_run_next 是一个自定义函数，它的作用是终止当前任务（或线程）的执行，然后立即切换到下一个任务（或线程），使操作系统可以在多任务系统中有效地调度不同的任务。这通常在操作系统内核的任务调度器中使用，用于切换任务的上下文，以实现多任务并发执行。


/// current task gives up resources for other tasks
// https://rcore-os.cn/rCore-Tutorial-Book-v3/chapter3/3multiprogramming.html
//  主动 调用 sys_yield 系统调用来实现的，这意味着应用主动交出 CPU 的使用权给其他应用。
pub fn sys_yield() -> isize {
    trace!("kernel: sys_yield");
    suspend_current_and_run_next();
    0
}

/// get time with second and microsecond
pub fn sys_get_time(ts: *mut TimeVal, _tz: usize) -> isize {
    trace!("kernel: sys_get_time");
    let us = get_time_us();
    unsafe {
        *ts = TimeVal {
            sec: us / 1_000_000,
            usec: us % 1_000_000,
        };
    }
    0
}

/// YOUR JOB: Finish sys_task_info to pass testcases
/// 我们希望引入一个新的系统调用 sys_task_info 以获取任务的信息
/// 根据任务 ID 查询任务信息，任务信息包括任务 ID、任务控制块相关信息（任务状态）、任务使用的系统调用及调用次数、任务总运行时长。
// syscall ID: 410
//返回值：执行成功返回0，错误返回-1

//我的理解：查询当前任务，不是根据任务ID查询 理解歧义 实现一个新的系统调用

pub fn sys_task_info(_ti: *mut TaskInfo) -> isize {
    trace!("kernel: sys_task_info");
    let status = get_current_status();
    let syscall_times = get_current_systimes();
    let time = get_current_running_times();
    unsafe {
        *_ti = TaskInfo
        {
            status,
            syscall_times,
            time
        };
    }
}


