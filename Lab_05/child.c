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

    // Присоединяем
    p = (struct Data *) shmat(shm_id, NULL, 0);

    // Изменяет
    p->arr[0] += 4;
    p->arr[1] += 5;
    p->arr[2] += 6;

    return 0;
}

