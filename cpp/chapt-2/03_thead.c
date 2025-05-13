#include <pthread.h>
#include <stdio.h>

int shared_data = 0;
pthread_spinlock_t spinlock;

void* increment_data(void* arg) {
    pthread_spin_lock(&spinlock);  // 加锁，开始“自旋”
    shared_data++;
    printf("Thread %d: shared_data = %d\n", *(int*)arg, shared_data);
    pthread_spin_unlock(&spinlock);  // 解锁，停止“自旋”
    return NULL;
}

int main() {
    pthread_t threads[5];
    pthread_spin_init(&spinlock, 0);  // 初始化自旋锁

    int thread_ids[5] = {0, 1, 2, 3, 4};
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, increment_data, &thread_ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_spin_destroy(&spinlock);  // 销毁自旋锁
    return 0;
}

//gcc 03_thead.c 