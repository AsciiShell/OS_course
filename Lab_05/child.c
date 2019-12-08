#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main() {
    int shm_id;
    struct Data *p;

    // Ищем кусок памяти
    shm_id = shmget(shared_key, sizeof(struct Data), 0);
    if (shm_id == -1) {
        perror("can't get shared memory");
    }
    // Присоединяем
    p = (struct Data *) shmat(shm_id, NULL, 0);

    printf("%d %d %d %d\n", p->a, p->arr[0], p->arr[1], p->arr[2]);

    // Изменяет
    p->arr[0] += 4;
    p->arr[1] += 5;
    p->arr[2] += 6;

    return 0;
}

