



**21天冲刺大厂面试·第四天 | C++11深度探索**

你可能不知道


**C++11** 发布于 **2011年8月**，到2025年已经 **14年** 了。

14年，一个技术的轮回

坦白说，这些新特性在我日常工作中几乎用不到。

为什么？现实很骨感：

**(1) 应用场景的门槛**
- C++做界面开发：MFC、QT、游戏客户端、OpenCV图像处理...
- 如果进不了相关公司，根本没人给你实践的机会

**(2) 基础设施的壁垒**  
- C/C++基础设施开发：分布式数据库、存储系统、中间件...
-  如果进不了相关公司，根本没人给你实践的机会
- 即使有开源项目，个人也难有合适的环境练习
- 像TiFlash、OceanBase、Ceph这些项目，平时根本用不到
- 没有实际需求，很难静下心来深入研究

**(3) 唯一可行的路径**
- 只剩下在普通笔记本上，建立工程，用std标准库写demo练习


对比其他同学怎么做的，

通过考研，面试 进入行业公司，然后直接瞬间进入数据库，os ai 行业 然后再开发相关项目。



你看出来了吗？

最关键的 并不是 c++本身，明白了，进入相关行业，

我为什么痴迷于写demo、研究原理？


**都是被现实逼的。**

哪怕进入公司 讨论 方案设计，bug 定位解决，没有 人关系 c++新特性，


所以，我能做的就是把能做的事情做好：
- 一个笔记本，建立项目工程，写一个 cpp 文件，std 标准库，写 demo，git、cmake
- 把C++11新特性研究透彻



> 今天 就解决掉最 不关键事情，研究无用事情。



#  Part 1 面试官提问：

#### 智能指针

- unique_ptr 独占所有权 什么场景使用？提示 资源，资源**赋值拷贝中传递**
- 

### 内存模型
### 编译优化

#  Part 2 我的回答


> 本文举例代码均来 3FS ，TiFlash 等事项

## 2.1 智能指针

###   unique_ptr 独占所有权:资源只有 1 个，传递拷贝只有 1 个

#### 使用场景，情况 3fs 内存池设计与 unique_ptr代替普通指针，减少锁竞争


目的：资源只有 1 个，传递拷贝只有 1 个



```c++
┌─────────────────────────────────────────────────────────────┐
│              ObjectPool<T, NumTLSCachedItem, ...>           │
│                    (Template Class)                          │
├─────────────────────────────────────────────────────────────┤
│ ┌─ 类型别名 ─────────────────────────────────────────────┐  │
│ │ using Storage = std::aligned_storage_t<...>           │  │
│ │ using Batch = std::vector<std::unique_ptr<Storage>>    │  │
│ │ using Ptr = std::unique_ptr<T, Deleter>               │  │
│ └─────────────────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────────────┤
│ ┌─ 私有成员 ─────────────────────────────────────────────┐  │
│ │ - std::mutex mutex_                                    │  │
│ │ - std::vector<Batch> global_                          │  │
│ └─────────────────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────────────┤
                           │
                           │ 包含
                           │ (1:N, thread_local)
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                         TLS                                  │
│              (Thread Local Storage Class)                    │
├─────────────────────────────────────────────────────────────┤
│ ┌─ 成员变量 ─────────────────────────────────────────────┐  │
│ │ - ObjectPool& parent_                                  │  │
│ │ - Batch first_                                         │  │
│ │ - Batch second_                                        │  │
│ └─────────────────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────────────┤
│ ┌─ 方法 ─────────────────────────────────────────────────┐  │
│ │ + std::unique_ptr<Storage> get()                       │  │
│ │ + void put(std::unique_ptr<Storage> obj)               │  │
│ └─────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
         │                              │
         │ 包含                          │ 包含
         │ (1:1)                         │ (1:1)
         ▼                              ▼
┌──────────────────────┐      ┌──────────────────────┐
│   Batch (first_)     │      │   Batch (second_)    │
│  ┌────────────────┐  │      │  ┌────────────────┐  │
│  │ vector<unique_ │  │      │  │ vector<unique_ │  │
│  │   ptr<Storage>>│  │      │  │   ptr<Storage>>│  │
│  │                │  │      │  │                │  │
│  │ [最多64个元素]  │  │      │  │ [溢出缓存]     │  │
│  └────────────────┘  │      │  └────────────────┘  │
└──────────────────────┘      └──────────────────────┘



┌─────────────────────────────────────────────────────────────┐
│              ObjectPool 全局结构                              │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌─────────────────────────────────────────────────────┐    │
│  │  static ObjectPool instance  (单例，全局唯一)       │    │
│  │  └─ mutex_                                          │    │
│  │  └─ global_: vector<Batch>                          │    │
│  │      ├─ Batch[0]                                    │    │
│  │      ├─ Batch[1]                                    │    │
│  │      └─ ...                                         │    │
│  └─────────────────────────────────────────────────────┘    │
│                                                               │
│  每个线程:                                                    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │  thread_local TLS tls{instance}  (每线程一个)      │    │
│  │  └─ parent_ → instance                             │    │
│  │  └─ first_: Batch                                  │    │
│  │      └─ [64个 unique_ptr<Storage>]                 │    │
│  │  └─ second_: Batch                                 │    │
│  │      └─ [溢出缓存]                                  │    │
│  └─────────────────────────────────────────────────────┘    │
│                                                               │
└─────────────────────────────────────────────────────────────┘
```



using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;
代表：一个连续的内存地址，8 字节，16 字节，32 字节 


```c++
 using Batch = std::vector<std::unique_ptr<Storage>> //存储中不发生拷贝

```
 代表:存储多个连续内存地址（没有调用构造函数呢）
1. 只存储指针，vector 默认存储值，内存只有1 份，【减少存储中发生拷贝】
2. 移动unique_ptr非常轻量（只移动指针）【why unique_ptr不允许拷贝吗？】

3. 明确的资源所有权语义
4. 自动内存管理，vector销毁时自动清理


其中一个使用方式：
using Ptr = std::unique_ptr<T, Deleter>; // 如何释放资源




```c++
// 匿名函数 lamber 表达式
struct Deleter 
{
	constexpr Deleter() = default;
    void operator()(T *item) {

     item->~T(); //析构函数 ~foo()

      tls().put(std::unique_ptr<Storage>(reinterpret_cast<Storage *>(item)));
     }
}; 
```


类型转换:

- `static_cast` 是一种更安全的转换方式，它在编译时检查转换的合法性, 比如整数类型和浮点类型之间，或者将 `void*` 转换为具体类型的指针。
- reinterpret_cast 用于任意指针（或引用）类型之间的转换，但不检查安全性,必须使用 类型 T 不确定


```c++

using Ptr = std::unique_ptr<T, Deleter>;
static Ptr get()  //返回不继续拷贝
{ 
	return Ptr(new (tls().get().release()) T); 
}  //返回unique_ptr 这个不是复制 =，移动构造函数
```

#### 疑问：unique_ptr不允许复制拷贝，为什么作为函数返回值

基础回顾:

- 现代编译器会应用**返回值优化 (RVO)**
- 移动语义允许转移所有权, 移动构造函数 不是赋值操作  a=b vs a(b) vs a(move(b))


```c++

// ❌ 错误：尝试拷贝
std::unique_ptr<int> bad_example(std::unique_ptr<int> p) {
    return p;  // 错误：尝试从左值拷贝
}  //a =b

std::unique_ptr<int> good_example1() {
    return std::unique_ptr<int>(new int(42));  // 直接构造临时对象
} //a（b）


std::unique_ptr<int> good_example2() {
    auto p = std::unique_ptr<int>(new int(100));
    return p;  // 编译器将其视为 std::move(p)
} //现代编译器会应用**返回值优化 (RVO) /a（std:move(b)）
std::unique_ptr<int> good_example3(std::unique_ptr<int> p) {
    return std::move(p);  // 显式移动
}
```

#### 疑问：Ptr传递应该是指针呀？为什么 T 类型

```c++

// 1. 配合 placement new 使用
void* memory = malloc(sizeof(MyClass));
MyClass* obj = new(memory) MyClass();
obj->~MyClass();  // ✅ 正确
free(memory);

// 2. 在自定义内存池中
obj->~T();  // ✅ 析构对象但不释放池内存

```


#### 疑问：TLS 如何使用，为什么设计和唯一指针关系


TLS 类图

private:

	ObjectPool &parent_; // ObjectPool引用 就是全家变量 vector<Batch> 访问加锁
	
	Batch first_;  //主缓存批次
	
	Batch second_; //溢出缓存批次


公共方法详细说明：

- get(): 获取内存块的完整算法流程

- put(): 归还内存块的完整算法流程

```c++
// put a object into thread local cache or global cache.

// unique_ptr 参数，智能移动构造
void put(std::unique_ptr<Storage> obj) {

	// push to the first batch. 不满
	if (first_.size() < kThreadLocalMaxNum) 
	{
		first_.push_back(std::move(obj));	//obj 变量与地址，不是右值
		return;
	}
	
	// push to the second batch.

	second_.push_back(std::move(obj));

     //full 
	if (UNLIKELY(second_.size() >= kThreadLocalMaxNum)) 
	{
	
		// push to the global cache.
		
		parent_.putBatch(std::move(second_)); //second 移动被自动走了。
		
		second_.clear(); //为什么敢 clear 不担心 指针指针对象 析构时候释放空间吗？
		
		second_.reserve(kThreadLocalMaxNum);//不是 resize，kThreadLocalMaxNum最大容量 ，1 2 4 8 多申请方式。

	}

}
```


#### 疑问：vector push_back 支持左值传递，支持右值传递 重载 2 个函数吗？为不写成一个 ?

对，重载 2 个


void push_back( const T& value )

void push_back( T&& value )

https://en.cppreference.com/w/cpp/container/vector/push_back​


#### 疑问：vector push_back是函数本身实现 T 类似移动拷贝吗？

- 不是

```c++
void push_back(value_type&& __x)
{
    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
    {
        _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                                 std::move(__x));
        ++this->_M_impl._M_finish;
    }
    else
        _M_realloc_insert(end(), std::move(__x));//
}

https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr.html

unique_ptr( unique_ptr&& u ) noexcept

explicit unique_ptr( pointer p ) noexcept;


template<typename T>
class unique_ptr {
    T* ptr_; // 底层原始指针

public:
    // 移动构造函数
    unique_ptr(unique_ptr&& other) noexcept 
        : ptr_(other.ptr_) {  // 1. 接管资源所有权
        other.ptr_ = nullptr; // 2. 置空原指针
    }

    // 禁用拷贝构造
    unique_ptr(const unique_ptr&) = delete;
    
    // ... 其他成员函数
};

```

###  疑问：vector push_back参数value_type__x 为什么，还用std::move(__x)？这不是std::move(std::move(std::move(__x)))


- std::move() 将左值，或者右值 转化右值
- 右值 :临时值，无法获取变量地址
- 当函数内部，x  变量有地址，不在右值，需要再次转化
- 需要再次std::move(std::move(std::move(__x)))


#### 其他场景  独占所有权：每个资源只属于一个所有者



## 2.2  std::shared_ptr

https://en.cppreference.com/w/cpp/memory/shared_ptr.html
- `std::shared_ptr` is a smart pointer that retains shared ownership of an object through a pointer.
- The object is destroyed and its memory deallocated when either of the following happens:
   the last remaining `shared_ptr` owning the object is destroyed

- 根据例子 ，被多个使用者使用，不清楚，不清楚使用你这生命周期（多线程）

还有更多例子吗？


### 场景 1:C++11新特性之十：enable_shared_from_this


因为在异步调用中，存在一个保活机制，异步函数执行的时间点我们是无法确定的，然而异步函数可能会使用到异步调用之前就存在的变量。

为了保证该变量在异步函数执期间一直有效，我们可以传递一个指向自身的share_ptr给异步函数，这样在异步函数执行期间share_ptr所管理的对象就不会析构，

所使用的变量也会一直有效了（保活）。


#### 场景：异步网络发送

你有一个网络连接类 `Transport`，用来异步发送数据。  
异步操作意味着：**函数返回后还没发送完，稍后某个线程会回调**。


##### 没有 `shared_from_this()` 的危险写法：

```cpp
class Transport {
public:
    void sendAsync() {
        ioWorker_.asyncWrite([this]() {
            this->handleWriteComplete();
        });
    }
};
```

现在假设这样使用：

```cpp
auto t = std::make_shared<Transport>();
t->sendAsync();      // 发起异步操作
t.reset();           // 调用者提前释放（连接关闭、对象析构）
```

此时会发生什么？

- `sendAsync()` 提交了异步任务，但任务可能几百毫秒后才真正执行。
    
- `t.reset()` 导致 `Transport` 对象被销毁。
    
- 等到异步任务线程执行回调时，lambda 里的 `this` 指针已经**悬空**。
    
- 程序访问已销毁的对象 → **未定义行为 / 崩溃**。
    

🧠 类似于：

> 你请别人“稍后”给你发封邮件，但你提前把邮箱注销了，对方再发邮件时就炸了。



### ✅ 改进：用 `shared_from_this()` 延长生命周期

```cpp
class Transport : public std::enable_shared_from_this<Transport> {
public:
    void sendAsync() {
        auto self = shared_from_this(); // 拿到自己的一份共享所有权

        ioWorker_.asyncWrite([self]() {
            self->handleWriteComplete(); // self 保证对象不被提前销毁
        });
    }
};
```

再执行相同代码：

```cpp
auto t = std::make_shared<Transport>();
t->sendAsync();  // 发起异步操作
t.reset();       // 调用者释放自己的那份
```

这次不会崩溃，因为：

- `sendAsync()` 内部的 lambda 捕获了 `self`（一个 `shared_ptr`）。
    
- 即使外部的 `t` 已经 reset，`self` 仍然持有对象。
    
- 对象至少活到 `asyncWrite` 回调执行完毕。
    
- 回调执行完 `self` 被销毁，对象才会真正释放。
    


```cpp
class Transport : public std::enable_shared_from_this<Transport> {
public:
    void sendAsync(std::vector<char> data) {
        auto self = shared_from_this();
        ioWorker_.asyncWrite([self, data = std::move(data)]() mutable {
            // 这里 data 和 self 一起活到写完
            self->handleWriteComplete(data);
        });
    }

    void handleWriteComplete(const std::vector<char>& data) {
        std::cout << "Write complete: " << data.size() << " bytes\n";
    }
};
```

好处：

- `data` 被安全地移动进回调，不会在外部被释放或修改；
    
- `self` 确保回调中访问对象是安全的；
    
- 回调执行完，`self` 和 `data` 一起销毁。
    


|问题|没用 shared_from_this|用了 shared_from_this|
|---|---|---|
|异步回调执行时对象还在吗？|❌ 可能被销毁|✅ 一定还在|
|访问 this 安全吗？|❌ 不安全（悬空指针）|✅ 安全|
|对象释放时机|不确定（可能太早）|延迟到回调结束|
|实现代价|简单|稍复杂但安全|


### 疑问：shared_from_this` 是如何做到“让一个对象知道自己被 `shared_ptr` 管着”的

你这个问题问到了核心本质 👏——  

**`shared_from_this` 是如何做到“让一个对象知道自己被 `shared_ptr` 管着”的？**


###   一、根本问题：普通类为什么不行？

看个普通类：

```cpp
class Foo {
public:
    void bar() {
        auto p = shared_from_this(); // ❌ 编译错误
    }
};
```

编译就报错：

> ‘class Foo’ has no member named ‘shared_from_this’

原因：  
`shared_from_this()` 是 `std::enable_shared_from_this<T>` 提供的，普通类根本没有这个机制。



###  二、原理：`enable_shared_from_this` 是怎么“绑定”上的？

当你写：

```cpp
class Foo : public std::enable_shared_from_this<Foo> {
public:
    void bar() {
        auto p = shared_from_this(); // ✅ 可以用了
    }
};
```

**真正的魔法发生在 `std::shared_ptr<Foo>` 的构造过程**。  

```cpp
template<class T>
class enable_shared_from_this {
protected:
    mutable std::weak_ptr<T> weak_this_; // 💡 弱引用指针，不影响计数
public:
    shared_ptr<T> shared_from_this() {
        return weak_this_.lock(); // 从弱引用锁出一个强引用
    }
    weak_ptr<T> weak_from_this() noexcept {
        return weak_this_;
    }
};
```

当你这样创建对象时：

```cpp
auto p = std::make_shared<Foo>();
```

在 `shared_ptr` 构造函数内部，它会自动检测：

> “这个 `Foo` 有没有继承 `enable_shared_from_this<Foo>`？”

如果有，它会把自己注册进去：

```cpp
p->weak_this_ = p; // 内部逻辑：让 enable_shared_from_this 知道自己的 shared_ptr
```

这样对象内部的 `weak_this_` 就能指向自己被管理的那份控制块。



### 三、调用时的过程

当你调用：

```cpp
auto p2 = this->shared_from_this();
```

实际执行的是：

```cpp
return weak_this_.lock();
```

`lock()` 做的事是：

- 如果对象还活着（控制块的引用计数 > 0），就返回一个新的 `shared_ptr`；
    
- 否则返回空（expired）。
    

🧠 所以：

> `shared_from_this()` 实际上就是 “把自己之前保存的弱引用 `weak_ptr` 转回一个强引用 `shared_ptr`”。



## 要点先说清楚

- `enable_shared_from_this<T>` 内部包含一个 `mutable std::weak_ptr<T> weak_this_`。
    
- 当你用 `std::make_shared<T>()` 或用 `std::shared_ptr<T>(new T)` 创建 `shared_ptr` 时，`shared_ptr` 的实现会**检测到**对象继承了 `enable_shared_from_this<T>`，并在构造完成后**把刚创建的 `shared_ptr` 或其控制块写入对象的 `weak_this_`**。
    
- 因此 `shared_from_this()` 不是“自己去找”这个 `shared_ptr`，而是“之前创建 `shared_ptr` 的代码已经把它塞进来了”


```c++

// 当 make_shared 创建对象/控制块时（非常简化的伪流程）：
template<class T, class... Args>
std::shared_ptr<T> make_shared(Args&&... args) {
    // 1) 分配控制块（control block）并在其上构造 T（可能在同一块内存）
    control_block* cb = new control_block(...);
    T* obj = new(cb->storage) T(std::forward<Args>(args)...);

    // 2) 创建 shared_ptr 指向 control block（管理引用计数）
    std::shared_ptr<T> sp(cb, obj); // 实际实现更复杂

    // 3) 关键：如果 T 继承 enable_shared_from_this<T>
    //    把当前的 shared_ptr 写回对象的 weak_this_
    //    （这样对象内部就能通过 weak_from_this()/shared_from_this() 得到同一控制块）
    if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
        obj->weak_this_ = sp; // 这里就是把 shared_ptr 的控制块“挂”到对象上
    }

    // 4) 返回 shared_ptr 给用户
    return sp;
}

https://en.cppreference.com/w/cpp/types/is_base_of.html


```


std::is_base_of 是 C++ 标准库中的一个类型特性（Type Trait），

用于判断一个类是否是另一个类的基类。

### 举个完整例子（含控制流程）

```cpp
#include <memory>
#include <iostream>

struct Foo : public std::enable_shared_from_this<Foo> {
    void show() {
        auto self = shared_from_this();
        std::cout << "shared_from_this(): use_count=" << self.use_count() << "\n";
    }
    ~Foo() { std::cout << "Foo destroyed\n"; }
};

int main() {
    auto p1 = std::make_shared<Foo>();
    std::cout << "p1.use_count=" << p1.use_count() << "\n";
    p1->show(); // 内部再拿一份 shared_ptr
    std::cout << "p1.use_count(after)=" << p1.use_count() << "\n";
}
```

输出大致是：

```
p1.use_count=1
shared_from_this(): use_count=2
p1.use_count(after)=1
Foo destroyed
```

说明：

- 当 `std::make_shared<Foo>()` 创建对象时，`weak_this_` 被自动设置。
    
- `shared_from_this()` 从 `weak_ptr` 锁出新 `shared_ptr`，引用计数变 2。



- **Boost.Asio**：网络 session 模式经常用 `enable_shared_from_this` + 捕获 `shared_ptr` 来保证异步 handler 里访问安全（官方教程与例子）。这是 C++ 网络编程里的教科书级用法。

#### The boost::asio::async_write

https://www.boost.org/doc/libs/latest/doc/html/boost_asio/tutorial/tutdaytime3.html


为什么在 `boost::asio::async_write()` 中一定要用 `shared_from_this()`。


#### 背景

`boost::asio::async_write()` 是**异步写操作**，意思是：

> 写入调用会立刻返回，真正的数据发送在后台进行。  
> 一旦发送完成，系统会回调你指定的 handler。

问题：**当 handler 被调用时，你的对象可能已经被销毁。**  
这就是为什么我们要用 `shared_from_this()` —— 它可以延长对象的生命期直到回调执行完。

---

####  ✅ 正确写法（使用 shared_from_this）

```cpp
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_context& io) : socket_(io) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        message_ = "Hello, async world!\n";

        std::cout << "[start] begin async_write\n";

        // 捕获 shared_from_this，保证 Session 在回调完成前不被销毁
        boost::asio::async_write(socket_,
            boost::asio::buffer(message_),
            [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes) {
                if (!ec) {
                    std::cout << "[callback] write complete, bytes=" << bytes << "\n";
                }
                else {
                    std::cout << "[callback] error: " << ec.message() << "\n";
                }
            });
    }

private:
    tcp::socket socket_;
    std::string message_;
};

int main() {
    boost::asio::io_context io;

    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345));

    auto session = std::make_shared<Session>(io);
    acceptor.async_accept(session->socket(),
        [session](boost::system::error_code ec) {
            if (!ec) session->start();
        });

    io.run();
}
```


想象：

- 你（`Session`）发起了一个异步操作，就像「让别人帮你跑腿」；
    
- 如果没人帮你记着这件事（`shared_ptr` 引用计数为 0），你下班回家了；
    
- 跑腿回来敲门（回调触发），你人已经不在 → 崩溃。


### 循环引用


```c++

// EventLoop provides a main loop that notifies EventHandler 
// callback objects when I/O is ready on a file descriptor.
class EventLoop : public hf3fs::enable_shared_from_this<EventLoop> 
{

	// start and stop.
	
	Result<Void> start(const std::string &threadName = "EventLoop");

	class EventHandler 
	{
	
		public:
		
		virtual void handleEvents(uint32_t epollEvents) = 0;
	
		
		protected:
		
		friend class EventLoop;
		
		std::weak_ptr<EventLoop> eventLoop_;
	};
```

#### 背景

你有两个类：

```cpp
class EventLoop;  // 事件循环

class EventHandler {
protected:
    std::weak_ptr<EventLoop> eventLoop_;  // ⭐ 弱引用
};
```

- `EventHandler` 是某种事件处理器（可能是定时器、IO 回调等）。
    
- 每个 `EventHandler` 需要访问它所属的 `EventLoop` 来注册事件、触发回调。
    

问题：**EventLoop 和 EventHandler 互相持有怎么办？**



####   循环引用问题

假设：

```cpp
class EventLoop : public std::enable_shared_from_this<EventLoop> {
    std::vector<std::shared_ptr<EventHandler>> handlers_;
};
```

- `EventLoop` 持有 `shared_ptr<EventHandler>`（EventHandler 不可能离开事件循环前就被销毁）。
    
- 如果 `EventHandler` 内部又写成：
    

```cpp
std::shared_ptr<EventLoop> eventLoop_;
```

就会发生**循环引用**：

```text
EventLoop.shared_ptr -> EventHandler.shared_ptr -> EventLoop.shared_ptr
```

- `shared_count` 永远 > 0
    
- **EventLoop 和 EventHandler 永远不会被析构** → 内存泄漏
    


### 解决方案：弱引用

使用 `weak_ptr`：

```cpp
class EventHandler {
protected:
    std::weak_ptr<EventLoop> eventLoop_; // 弱引用，不增加 shared_count
};
```

- `EventHandler` 可以访问 `EventLoop`，但不阻止它销毁。
    
- 当需要访问 `EventLoop` 时，用 `lock()`：
    

```cpp
void EventHandler::handleEvent() {
    if (auto loop = eventLoop_.lock()) {   // 尝试生成 shared_ptr
        loop->doSomething();
    } else {
        // EventLoop 已经被销毁，安全退出
    }
}
```

- 如果 `EventLoop` 已销毁，`lock()` 返回空指针，不会访问悬空对象。
    
- **关键**：避免循环引用，同时保证访问安全。
    


###  小比喻理解

- **EventLoop** = 厨师
    
- **EventHandler** = 菜品
    
- 菜品知道厨师是谁（weak_ptr），但它**不控制厨师生命**
    
- 厨师决定什么时候下班（销毁 EventLoop）
    
- 菜品还能安全检查：“厨师还在吗？” (`lock()`)
    



## 21天c++面试突击加强营 开始了

  
咨询请访问：

[别想太多--只管去面](https://mp.weixin.qq.com/s/vMOVX-O9LlyA28RDOPSKNg)

[别想太多--只管去面](https://mp.weixin.qq.com/s/vMOVX-O9LlyA28RDOPSKNg)


![课程特点](https://s2.loli.net/2025/10/04/pwhVBGQAzjU6d7u.png)

  

![课程大纲](https://s2.loli.net/2025/10/04/VgCw1mXtHQRpTyq.png)

  

### 相关电子书

[分布式系统架构经典资料](https://mp.weixin.qq.com/s/QZRI7B32YHtTf8-Wh8Laig)

![](https://s2.loli.net/2025/10/04/ZB8nJ9xUtDGMdiT.png)

  
下载地址：https://pan.baidu.com/s/17XlKbTmI92mMdWD8WiTX2A?pwd=xv9j


**为了防止网盘失效** 关注 后端开发成长指南

  
回复 "1" 全部 获取电子书


![](https://s2.loli.net/2025/10/31/AmBOqJXFflTsDnp.png)
