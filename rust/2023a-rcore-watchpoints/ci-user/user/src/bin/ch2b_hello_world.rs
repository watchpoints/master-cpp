// 声明不使用标准库，因为我们处于裸机环境
#![no_std]

// 声明不使用标准入口点（main函数），因为我们处于裸机环境
#![no_main]

// 导入用户库（这里使用extern crate关键字，通常用于导入外部库，但这里是一个伪装成库的宏）
#[macro_use]
extern crate user_lib;
// https://stackoverflow.com/questions/29403920/whats-the-difference-between-use-and-extern-crate

/// 正确输出：
/// Hello world from user mode program!

// 声明main函数，这是程序的入口点
#[no_mangle]
fn main() -> i32 {
    // 使用宏println!打印消息到标准输出
    println!("Hello, world from user mode program!");

    // 返回退出码0表示成功，类似于C/C++的main函数
    0
}
