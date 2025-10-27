
## 21天面试准备第三天


![第一周任务看板](https://s2.loli.net/2025/10/26/4eR62mbNYI8ahDS.png)



 ### 今日目标

1. STL有那些容器，各自使用场景
2.  STL容器底层结构和时间复杂度
3.  STL关键特性和部分源码分析

### 1. STL有那些容器，各自使用场景
- 请简述STL中顺序容器、关联容器、无序关联容器和容器适配器各有哪几种？
-  实容器选择：在以下场景中，你会选择哪种STL容器？请阐述理由。

###  2. STL容器底层结构和时间复杂度

#### 核心结构：
    
    - `vector`的底层机制是什么？它是如何实现动态扩容的？扩容因子通常是多少？
        
    - `map` 和 `unordered_map`的底层数据结构分别是什么？请从实现原理上解释它们最根本的区别。
    - 最坏情况性能（哈希冲突时O(n)
        
#### **时间复杂度**：

- sort 时间复杂度
- 


### 3.  STL关键特性和部分源码分析力

#### 迭代器失效：
 -  在遍历一个`vector`时进行插入操作可能导致什么问题？请详细说明在哪些操作下，`vector`、`map`、`list`的迭代器会失效，以及失效的具体含义。
- 未正确处理`erase`、`insert`等操作的返回值
#### 偏特化 
     `vector<bool>` 是一个特化版本，它与标准的`vector`有何不同？这种设计有什么优缺点？
        
#### 移动语义：
   `push_back`和`emplace_back`的区别是什么

#### 内存管理
- vector reserve push_back

[打卡输出](https://t.zsxq.com/7hc5B)

依赖准备：

| 来源                   | 维护者 / 特点         | 源代码链接                                                     | 备注                                      |
| -------------------- | ---------------- | --------------------------------------------------------- | --------------------------------------- |
| ​**MSVC STL**​       | 微软               | `https://github.com/microsoft/STL`                        | Visual Studio默认使用的STL，活跃开发，可在线浏览或克隆到本地。 |
| ​**GCC libstdc++​**​ | GNU项目            | `https://github.com/gcc-mirror/gcc`                       | GCC编译器的标准库，代码在`libstdc++-v3`目录下。        |
| ​**LLVM libc++​**​   | LLVM项目           | `https://github.com/llvm/llvm-project/tree/master/libcxx` | LLVM/Clang编译器使用的标准库，设计现代，跨平台支持好。        |
| ​**SGI STL**​        | Silicon Graphics | `https://github.com/karottc/sgi-stl`                      | 一个历史版本，代码可读性好，非常适合学习底层实现原理。             |


**SGI STL** 是侯捷老师最推荐的可读性最高的一种STL实现。
GCC 中用的就是这个版本的实现。




## 二、 stl关键特性源码分析

## 三、stl重要数据结构和优缺点


| 使用场景                              | 推荐容器                                 | 理由                       |
| --------------------------------- | ------------------------------------ | ------------------------ |
| ​**需要频繁随机访问，元素数量稳定**​             | `vector`                             | 连续的存储空间使得访问速度最快，缓存局部性好   |
| ​**需要频繁在序列中间进行插入/删除**​            | `list`                               | 链表结构使得增删操作仅需修改指针，效率高     |
| ​**需要既频繁随机访问，又频繁在头尾增删**​          | `deque`                              | 结合了数组和链表的特性，适合此类场景       |
| ​**需要元素自动排序**​                    | `set`, `map`, `multiset`, `multimap` | 红黑树保证元素有序                |
| ​**需要极快的查找速度，且不关心顺序**​            | `unordered_set`, `unordered_map`等    | 哈希表提供平均常数时间复杂度的查找        |
| ​**需要后进先出 (LIFO) 或先进先出 (FIFO)​**​ | `stack`, `queue`                     | 容器适配器提供了清晰的接口，专为特定数据结构设计 |

| 容器类别             | 容器名称                                                 | 底层数据结构 (简化描述)                     |
| ---------------- | ---------------------------------------------------- | --------------------------------- |
| ​**序列容器**​       | `std::vector`                                        | 动态数组                              |
|                  | `std::list`                                          | 双向链表                              |
|                  | `std::deque`                                         | 由“中央控制器”和多个缓冲区组成的双端队列             |
| ​**关联容器 (有序)​**​ | `std::set`/ `std::map`                               | 红黑树 (平衡二叉搜索树)                     |
|                  | `std::multiset`/ `std::multimap`                     | 红黑树 (平衡二叉搜索树)                     |
| ​**关联容器 (无序)​**​ | `std::unordered_set`/ `std::unordered_map`           | 哈希表                               |
|                  | `std::unordered_multiset`/ `std::unordered_multimap` | 哈希表                               |
| ​**容器适配器**​      | `std::stack`                                         | 默认基于 `deque`，也可用 `list`或 `vector` |
|                  | `std::queue`                                         | 默认基于 `deque`，也可用 `list`           |
|                  | `std::priority_queue`                                | 基于 `vector`并使用堆(heap)规则管理         |


数据结构可视化演示平台
- https://visualgo.net/zh
- https://www.cs.usfca.edu/~galles/visualization/Algorithms.html


## 3.1 堆

### 基本功能了解

std::priority_queue 容器适配器底层就虽然底层是 vector，连续的数组
https://en.cppreference.com/w/cpp/container/priority_queue.html

但是堆也是通过数组来实现的，如图
![大顶堆](https://s2.loli.net/2025/10/26/yPvoH32T49q8Oex.png)

场景基本操作

```c++
void testHeap() {
    vector<int> data{ 3,1,2,7,5 };
    //构造堆，最大堆
    make_heap(data.begin(), data.end(), less<int>());
    //pop堆顶元素，最大的元素
    pop_heap(data.begin(), data.end(), less<int>());
    cout << data.back() << endl;//输出7
    data.pop_back();
    //往堆中添加元素
    data.push_back(4);
    push_heap(data.begin(), data.end(), less<int>());//调整堆
    //排序
    sort_heap(data.begin(), data.end(), less<int>());
    for (int x : data)
        cout << x << " ";
    cout << endl;//输出 1，2，3，4，5
}
https://en.cppreference.com/w/cpp/algorithm/make_heap.html
https://en.cppreference.com/w/cpp/algorithm/pop_heap.html


一般用到这四个：make_heap()、pop_heap()、push_heap()、sort_heap();
（1）make_heap()构造堆
void make_heap(first_pointer,end_pointer,compare_function);

（2）push_heap()添加元素到堆
void push_heap(first_pointer,end_pointer,compare_function);
新添加一个元素在末尾，然后重新调整堆序。

（3）pop_heap()从堆中移出元素
void pop_heap(first_pointer,end_pointer,compare_function);
把堆顶元素取出来，放到了数组或者是vector的末尾。
（4）sort_heap()对整个堆排序

```


`std::priority_queue`的本质是一个**容器适配器**​（Container Adapter），它通过依赖底层容器（如 `vector`或 `deque`）来存储数据，并利用 `make_heap`、`push_heap`、`pop_heap`这一系列泛型算法来在底层容器上维护堆结构，从而提供优先级队列的接口和行为

。这种设计使得 `std::priority_queue`本身不直接管理堆算法，而是将它们委托给标准库中的通用算法

代码位置：

sgi-stl/
├── stl_heap.h          # priority_queue 的核心实现（堆算法）
├── stl_queue.h         # priority_queue 的类定义和接口
├── heap.h              # 可能包含堆算法（某些版本）
└── algorithm           # 包含堆相关的算法

- https://github.com/karottc/sgi-stl/blob/master/stl_queue.h

`pop_heap`实际上执行两个关键操作：

1. ​**交换堆顶元素与末尾元素**​
    
2. ​**重新调整剩余元素以维持堆性质**

	```c++
void pop() {
    try {
        pop_heap(c.begin(), c.end(), comp);  // 将最大元素移到末尾
        c.pop_back();                        // 删除末尾元素
    } catch(...) {
        c.clear();
        throw;
    }
}
```

```c++
void push(const value_type& __x) {
    try {
        c.push_back(__x);              // 先将元素加入底层容器
        push_heap(c.begin(), c.end(), comp);  // 然后调整堆结构
    } catch(...) {
        c.clear();
        throw;
    }
}
```
### 堆时间复杂度

#### make_heap建堆时间复杂度

假设有序列：`[4, 1, 3, 2, 16, 9, 10, 14, 8, 7]`

构建最大堆的过程：

### 步骤1：找到最后一个非叶子节点

- 元素个数 n = 10
    
- 最后一个非叶子节点索引 = (10/2) - 1 = 4（值为16）
    

### 步骤2：逆向调整过程

**详细调整过程：​**​

1. ​**调整节点4（值16）​**​：已经是最大堆
    
2. ​**调整节点3（值2）​**​：与子节点14比较 → 交换 → `[4,1,3,14,16,9,10,2,8,7]`
    
3. ​**调整节点2（值3）​**​：与子节点10比较 → 交换 → `[4,1,10,14,16,9,3,2,8,7]`
    
4. ​**调整节点1（值1）​**​：与较大子节点14比较 → 交换 → 继续与子节点8比较 → 交换 → `[4,14,10,8,16,9,3,2,1,7]`
    
5. ​**调整节点0（值4）​**​：与较大子节点16比较 → 交换 → 继续与子节点7比较 → 交换 → `[16,14,10,8,7,9,3,2,1,4]`
    

## ⚡ 时间复杂度分析

​**时间复杂度：O(n)​**，而不是直觉上的 O(n log n)

### 为什么是 O(n)？

考虑完全二叉树：

- 有 n/2 个叶子节点（高度为0）：不需要调整
    
- 有 n/4 个节点高度为1：最多调整1次
    
- 有 n/8 个节点高度为2：最多调整2次
    
- ...
    
- 有 1 个根节点高度为 h：最多调整 h 次
    

总操作次数：

```
T(n) = n/4 * 1 + n/8 * 2 + n/16 * 3 + ... + 1 * h
     < n * (1/4 + 2/8 + 3/16 + ...)
     = O(n)
```

![](https://mmbiz.qpic.cn/mmbiz_png/2H3gcc9GEbs4ibLOAfPRadDB2p8uXHMQpUQuXxSDBhia2n5pRUibeAtyXzqG1YTNnyicMWwZaChc3yb8icFib2oUfXicA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1#imgIndex=2)
### 参考

#### 1. 侯捷C++系列视频
- https://github.com/ZachL1/Bilibili-plus
- https://linux.do/t/topic/448148
- https://blog.51cto.com/binghe001/5245835


### 2. 历史文章
- [每日一题：堆排序中建堆过程的时间复杂度](https://mp.weixin.qq.com/s/Z0DsiC8Ese6vhNzuv4QIVA)
- [吃土记：之前理解时间复杂度计算方式是错误的](https://mp.weixin.qq.com/s/A-GbSWvbB5TvXJUAXfxFyA)
- [建堆时间复杂度是o(n)](https://mp.weixin.qq.com/s/I7GGIo26jw_cPToJkaSn7A)
-  堆排序中建堆过程时间复杂度O(n)怎么来的？



