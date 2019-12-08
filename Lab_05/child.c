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
    if ((sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED) {
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
    printf("%d %d %d %d\n", p->a, p->arr[0], p->arr[1], p->arr[2]);

    // Инициализируем структуру
    while (p->a != 0) {
        sem_wait(sem);
        p->arr[0] += 1;
        p->arr[1] += 1;
        p->arr[2] += 1;
        sem_post(sem);
    }

    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(shm_id);
    return 0;
}

