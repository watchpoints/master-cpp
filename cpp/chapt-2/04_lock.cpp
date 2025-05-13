
#include <atomic>
#include <thread>

class SpinLock {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
            // 自旋等待，直到获得锁
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};