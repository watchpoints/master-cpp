#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;

use user_lib::mmap;

/*
理想结果：输出 Test 04_1 OK!
*/

#[no_mangle]
fn main() -> i32 {
    let start: usize = 0x10000000; //这通常是一个内存地址。 需要映射的虚存起始地址，要求按页对齐
    let len: usize = 4096;
    let prot: usize = 3; //这可能表示访问权限或保护模式的设置
    assert_eq!(0, mmap(start, len, prot));
    // sys_mmap(start, len, prot)
    for i in start..(start + len) {
        let addr: *mut u8 = i as *mut u8;
        unsafe {
            *addr = i as u8;
        }
    }
    for i in start..(start + len) {
        let addr: *mut u8 = i as *mut u8;
        unsafe {
            assert_eq!(*addr, i as u8);
        }
    }
    println!("Test 04_1 OK!");
    0
}
