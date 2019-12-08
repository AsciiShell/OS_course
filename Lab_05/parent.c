#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main() {
    int shm_id;
    struct Data *p;

    // Создаем кусок памяти
    shm_id = shmget(shared_key, sizeof(struct Data), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("can't create shared memory");
    }
    // Присоединяем кусок памяти
    p = (struct Data *) shmat(shm_id, NULL, 0);

    // Инициализируем структуру
    p->a = 27;
    p->arr[0] = 1;
    p->arr[1] = 2;
    p->arr[2] = 3;

    // Мониторим состояние
    for (int i = 0; i < 10; i++) {
        printf("%d %d %d %d\n", p->a, p->arr[0], p->arr[1], p->arr[2]);
        sleep(1);
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("can't delete shared memory");
    }
    return 0;
}

