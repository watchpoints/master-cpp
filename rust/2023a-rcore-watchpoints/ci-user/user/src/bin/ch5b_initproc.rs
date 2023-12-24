#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;

use user_lib::{exec, fork, wait, yield_};

#[no_mangle]
fn main() -> i32 {
    if fork() == 0 {
        exec("ch5b_user_shell\0", &[0 as *const u8]);
    } else {
        loop {
            let mut exit_code: i32 = 0;
            let pid = wait(&mut exit_code);
            if pid == -1 {
                yield_();
                continue;
            }
            println!(
                "[initproc] Released a zombie process, pid={}, exit_code={}",
                pid, exit_code,
            );
        }
    }
    0
}


// 它不断循环调用 wait 来等待并回收系统中的僵尸进程占据的资源。 
// 如果回收成功的话则会打印一条报告信息给出被回收子进程的 PID 和返回值；
// 否则就 yield_ 交出 CPU 资源并在下次轮到它执行的时候再回收看看
