#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct Data {
        int a;
        int arr[3];
    };

    int shm_id;
    key_t key = 12344564;
    struct Data *p;

    shm_id = shmget(key, sizeof(struct Data), IPC_CREAT | 0666);

    p = (struct Data *) shmat(shm_id, NULL, 0);

    p->a = 27;
    p->arr[0] = 1;
    p->arr[1] = 2;
    p->arr[2] = 3;

    printf("%d %d %d %d\n", p->a, p->arr[0], p->arr[1], p->arr[2]);

    if (fork() == 0) {
        p->a = 32;
        p->arr[0] = 4;
        p->arr[1] = 5;
        p->arr[2] = 6;
        exit(0);
    } else {
        int s;
        wait(&s);
        printf("%d %d %d %d\n", p->a, p->arr[0], p->arr[1], p->arr[2]);
    }


    return 0;
}

