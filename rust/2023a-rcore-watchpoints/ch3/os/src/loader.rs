//! 将用户应用程序加载到内存中
//!
//! 在第3章中，用户应用程序只是内核二进制文件中的数据的一部分，
//因此我们只需要将它们复制到为每个应用程序分配的空间中以加载它们。
//! 我们还为每个任务分配了固定的内核堆栈 [`KernelStack`] 和用户堆栈 [`UserStack`]。

use crate::config::*;
use crate::trap::TrapContext;
use core::arch::asm;

/// 表示用于存储上下文数据的内核堆栈。
#[repr(align(4096))]
#[derive(Copy, Clone)]
struct KernelStack {
    data: [u8; KERNEL_STACK_SIZE],
}

/// 表示用于执行用户级应用程序的用户堆栈。
#[repr(align(4096))]
#[derive(Copy, Clone)]
struct UserStack {
    data: [u8; USER_STACK_SIZE],
}

// 用于存储最大应用程序数量的内核和用户堆栈的数组。
static KERNEL_STACK: [KernelStack; MAX_APP_NUM] = [KernelStack {
    data: [0; KERNEL_STACK_SIZE],
}; MAX_APP_NUM];

static USER_STACK: [UserStack; MAX_APP_NUM] = [UserStack {
    data: [0; USER_STACK_SIZE],
}; MAX_APP_NUM];

impl KernelStack {
    /// 获取内核堆栈的堆栈指针（SP）。
    fn get_sp(&self) -> usize {
        self.data.as_ptr() as usize + KERNEL_STACK_SIZE
    }

    /// 将给定的陷阱上下文推送到内核堆栈上。
    pub fn push_context(&self, trap_cx: TrapContext) -> usize {
        let trap_cx_ptr = (self.get_sp() - core::mem::size_of::<TrapContext>()) as *mut TrapContext;
        unsafe {
            *trap_cx_ptr = trap_cx;
        }
        trap_cx_ptr as usize
    }
}

impl UserStack {
    /// 获取用户堆栈的堆栈指针（SP）。
    fn get_sp(&self) -> usize {
        self.data.as_ptr() as usize + USER_STACK_SIZE
    }
}

/// 获取具有给定索引 `app_id` 的应用程序的基地址。
fn get_base_i(app_id: usize) -> usize {
    APP_BASE_ADDRESS + app_id * APP_SIZE_LIMIT
}

/// 获取应用程序的总数。
pub fn get_num_app() -> usize {
    extern "C" {
        fn _num_app();
    }
    unsafe { (_num_app as usize as *const usize).read_volatile() }
}

/// 加载位于地址范围内的第 n 个用户应用程序：
/// [APP_BASE_ADDRESS + n * APP_SIZE_LIMIT, APP_BASE_ADDRESS + (n+1) * APP_SIZE_LIMIT)。
pub fn load_apps() {
    extern "C" {
        fn _num_app();
    }
    let num_app_ptr = _num_app as usize as *const usize;
    let num_app = get_num_app();
    let app_start = unsafe { core::slice::from_raw_parts(num_app_ptr.add(1), num_app + 1) };
    
    // 清除指令缓存。
    unsafe {
        asm!("fence.i");
    }
    
    // 加载用户应用程序。
    for i in 0..num_app {
        let base_i = get_base_i(i);
        
        // 清除内存中的区域。
        (base_i..base_i + APP_SIZE_LIMIT)
            .for_each(|addr| unsafe { (addr as *mut u8).write_volatile(0) });
        
        // 将应用程序从数据部分复制到内存。
        let src = unsafe {
            core::slice::from_raw_parts(app_start[i] as *const u8, app_start[i + 1] - app_start[i])
        };
        let dst = unsafe { core::slice::from_raw_parts_mut(base_i as *mut u8, src.len()) };
        dst.copy_from_slice(src);
    }
}

/// 初始化应用程序的上下文，并在内核堆栈中保存 `TrapContext`。
pub fn init_app_cx(app_id: usize) -> usize {
    KERNEL_STACK[app_id].push_context(TrapContext::app_init_context(
        get_base_i(app_id),
        USER_STACK[app_id].get_sp(),
    ))
}

