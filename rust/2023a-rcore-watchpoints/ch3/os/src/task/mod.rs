//! Task management implementation
//!
//! Everything about task management, like starting and switching tasks is
//! implemented here.
//!
//! A single global instance of [`TaskManager`] called `TASK_MANAGER` controls
//! all the tasks in the operating system.
//!
//! Be careful when you see `__switch` ASM function in `switch.S`. Control flow around this function
//! might not be what you expect.

mod context;
mod switch;
#[allow(clippy::module_inception)]
mod task;

use crate::config::MAX_APP_NUM;
use crate::loader::{get_num_app, init_app_cx};
use crate::sync::UPSafeCell;
use lazy_static::*;
use switch::__switch;
pub use task::{TaskControlBlock, TaskStatus};

pub use context::TaskContext;

/// The task manager, where all the tasks are managed.
///
/// Functions implemented on `TaskManager` deals with all task state transitions
/// and task context switching. For convenience, you can find wrappers around it
/// in the module level.
///
/// Most of `TaskManager` are hidden behind the field `inner`, to defer
/// borrowing checks to runtime. You can see examples on how to use `inner` in
/// existing functions on `TaskManager`.
pub struct TaskManager {
    /// total number of tasks
    num_app: usize,
    /// use inner value to get mutable access
    inner: UPSafeCell<TaskManagerInner>,
}

/// Inner of Task Manager
pub struct TaskManagerInner {
    /// task list
    tasks: [TaskControlBlock; MAX_APP_NUM],
    /// id of current `Running` task
    current_task: usize,
}
//许你创建一个全局的懒加载（lazy）的静态变
lazy_static! {
    /// Global variable: TASK_MANAGER
    pub static ref TASK_MANAGER: TaskManager = {
        let num_app = get_num_app();
        /// 定义一个数组
        let mut tasks = [TaskControlBlock {
            task_cx: TaskContext::zero_init(),
            task_status: TaskStatus::UnInit,
            start_time,
            sys_count: [0; MAX_SYSCALL_NUM],
        }; MAX_APP_NUM];
        for (i, task) in tasks.iter_mut().enumerate() {
            task.task_cx = TaskContext::goto_restore(init_app_cx(i));
            task.task_status = TaskStatus::Ready;
        }
        TaskManager {
            num_app,
            inner: unsafe {
                UPSafeCell::new(TaskManagerInner {
                    tasks,
                    current_task: 0,
                })
            },
        }
    };
}

impl TaskManager {
    /// Run the first task in task list.
    ///
    /// Generally, the first task in task list is an idle task (we call it zero process later).
    /// But in ch3, we load apps statically, so the first task is a real app.
    fn run_first_task(&self) -> ! {
        let mut inner = self.inner.exclusive_access();
        let task0 = &mut inner.tasks[0];
        task0.task_status = TaskStatus::Running;
        let next_task_cx_ptr = &task0.task_cx as *const TaskContext;
        drop(inner);
        let mut _unused = TaskContext::zero_init();
        // before this, we should drop local variables that must be dropped manually
        unsafe {
            __switch(&mut _unused as *mut TaskContext, next_task_cx_ptr);
        }
        panic!("unreachable in run_first_task!");
    }

    /// Change the status of current `Running` task into `Ready`.
    fn mark_current_suspended(&self) {
        let mut inner = self.inner.exclusive_access();
        let current = inner.current_task;
        inner.tasks[current].task_status = TaskStatus::Ready;
    }

    /// Change the status of current `Running` task into `Exited`.
    fn mark_current_exited(&self) {
        let mut inner = self.inner.exclusive_access();
        let current = inner.current_task;
        inner.tasks[current].task_status = TaskStatus::Exited;
    }

    /// Find next task to run and return task id.
    ///
    /// In this case, we only return the first `Ready` task in task list.
    fn find_next_task(&self) -> Option<usize> {
        let inner = self.inner.exclusive_access();
        let current = inner.current_task; //当前任务编号
        (current + 1..current + self.num_app + 1)
            .map(|id| id % self.num_app)
            .find(|id| inner.tasks[*id].task_status == TaskStatus::Ready)
    }

    /// Switch current `Running` task to the task we have found,
    /// or there is no `Ready` task and we can exit with all applications completed
    fn run_next_task(&self) {
        if let Some(next) = self.find_next_task() {
            let mut inner = self.inner.exclusive_access();
            let current = inner.current_task;
            inner.tasks[next].task_status = TaskStatus::Running;
            inner.current_task = next;
            let current_task_cx_ptr = &mut inner.tasks[current].task_cx as *mut TaskContext;
            let next_task_cx_ptr = &inner.tasks[next].task_cx as *const TaskContext;
            drop(inner);
            // before this, we should drop local variables that must be dropped manually
            unsafe {
                __switch(current_task_cx_ptr, next_task_cx_ptr);
            }
            // go back to user mode
        } else {
            panic!("All applications completed!");
        }
    }

     ///获取当前running任务的信息
     /// 返回：
    //  struct TaskInfo {
    //     id: usize,
    //     status: TaskStatus,
    //     call: [SyscallInfo; MAX_SYSCALL_NUM],
    //     time: usize
    // }
     pub fn get_current_task_info(&self) -> TaskInfo{
        let inner = self.inner.exclusive_access();
       //inner: UPSafeCell<TaskManagerInner>
       /// Inner of Task Manager
        // pub struct TaskManagerInner {
        //     /// task list
        //     tasks: [TaskControlBlock; MAX_APP_NUM],
        //     /// id of current `Running` task
        //     current_task: usize,
        // }
        let current_task_id = inner.current_task; //正在运动任务的id

        // task list tasks: [TaskControlBlock; MAX_APP_NUM],
        // 这个是数组结构，用下标作为task_idd
        let current_task = &inner.tasks[current_task_id];
         
        // pub struct TaskControlBlock {
        //     /// 任务在其生命周期中的状态
        //     pub task_status: TaskStatus,
        //     /// 任务上下文
        //     pub task_cx: TaskContext,
        // }
        /// 任务状态
        // #[derive(Copy, Clone, PartialEq)]
        // pub enum TaskStatus {
        //     /// 未初始化
        //     UnInit,
        //     /// 准备就绪
        //     Ready,
        //     /// 运行中
        //     Running,
        //     /// 已退出
        //     Exited,
        // }

        // /// Task information
        // #[allow(dead_code)]
        // pub struct TaskInfo {
        // /// Task status in it's life cycle
        // status: TaskStatus,
        // /// The numbers of syscall called by task
        // syscall_times: [u32; MAX_SYSCALL_NUM],
        // /// Total running time of task
        // time: usize,
        // }
        let task_info = TaskInfo {
            status: current_task.task_status.clone(),
            //运行时间 = 当前时间 - 任务开始时间
            time: get_time_ms() - current_task.start_time,
            syscall_count:current_task.syscall_count.clone()
        };
        task_info
    }

    fn add_syscall_count(&self,sys_call_id : usize)
    {
        if num >= MAX_SYSCALL_NUM 
        {
            return;
        }
        let mut inner = self.inner.exclusive_access(); 
        inner.tasks[inner.current_task].syscall_count[sys_call_id]+=1; //
    }
    
}

/// Run the first task in task list.
pub fn run_first_task() {
    TASK_MANAGER.run_first_task();
}

/// Switch current `Running` task to the task we have found,
/// or there is no `Ready` task and we can exit with all applications completed
fn run_next_task() {
    TASK_MANAGER.run_next_task();
}

/// Change the status of current `Running` task into `Ready`.
fn mark_current_suspended() {
    TASK_MANAGER.mark_current_suspended();
}

/// Change the status of current `Running` task into `Exited`.
fn mark_current_exited() {
    TASK_MANAGER.mark_current_exited();
}

/// Suspend the current 'Running' task and run the next task in task list.
pub fn suspend_current_and_run_next() {
    mark_current_suspended();
    run_next_task();
}

/// Exit the current 'Running' task and run the next task in task list.
pub fn exit_current_and_run_next() {
    mark_current_exited();
    run_next_task();
}

pub fn add_syscall_count(sys_call_id: usize) {
    TASK_MANAGER.add_syscall_count(sys_call_id);
}

