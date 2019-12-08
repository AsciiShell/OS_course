#include <math.h>
#include "header.h"

int main() {
    int shm_id;
    char *addr;
    sem_t *sem;
    struct Data *p;

    // Создаем кусок памяти
    shm_id = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_RDWR, 0777);
    if (shm_id == -1) {
        perror("can't create shared memory");
        return -1;
    }
    if (ftruncate(shm_id, SHARED_MEMORY_OBJECT_SIZE + 1) == -1) {
        perror("ftruncate");
        return 1;
    }
    // Новый семафор с числом 1
    if ((sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 1)) == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }
    addr = mmap(0, SHARED_MEMORY_OBJECT_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm_id, 0);
    if (addr == (char *) -1) {
        perror("mmap");
        return 1;
    }
    // Присоединяем кусок памяти
    p = (struct Data *) addr;

    // Инициализируем структуру
    p->a = 27;
    p->arr[0] = 0;
    p->arr[1] = 0;
    p->arr[2] = 0;

    // Мониторим состояние
    for (int i = 0; i < 10; i++) {
        sem_wait(sem);
        int a = p->arr[0];
        int b = p->arr[1];
        int c = p->arr[2];

        float e = fabs(a - b) + fabs(b - c) + fabs(c - a);
        printf("%d %d %f\n", p->a, p->arr[0], e);
        sem_post(sem);
        sleep(1);
    }
    p->a = 0;
    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(shm_id);
    if (shm_unlink(SHARED_MEMORY_OBJECT_NAME) == -1) {
        perror("can't delete shared memory");
    }
    return 0;
}

