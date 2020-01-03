#include <math.h>
#include "header.h"
#include <signal.h>
#include <string.h>

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

void print_salon(const int arr[MAX_SIZE], size_t len) {
    const size_t ROW_SIZE = 4;
    size_t num = 0;
    for (size_t i = 0; i < len; i++) {
        printf("%d\t", arr[i]);
        num++;
        if (num == ROW_SIZE) {
            printf("\n");
            num = 0;
        }
    }
    if (num != 0) {
        printf("\n");
    }
}

int main() {
    int shm_id;
    struct Server *addr;
    sem_t *sem;
    char message[MAX_SIZE];

    // Создаем кусок памяти
    if ((shm_id = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_RDWR, 0777)) == -1) {
        perror("can't create shared memory");
        return 1;
    }
    if (ftruncate(shm_id, SHARED_MEMORY_OBJECT_SIZE + 1) == -1) {
        perror("can't ftruncate memory");
        return 1;
    }
    addr = (struct Server *) mmap(0, SHARED_MEMORY_OBJECT_SIZE + 1,
                                  PROT_WRITE | PROT_READ, MAP_SHARED, shm_id, 0);
    if (addr == (struct Server *) -1) {
        perror("can't mmap memory");
        return 1;
    }
    // Новый семафор с числом 1
    if ((sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 1)) == SEM_FAILED) {
        perror("can't open semaphore");
        return 1;
    }
    signal(SIGINT, intHandler);
    // Инициализируем структуру
    addr->plane.smoke.size = 20;
    addr->plane.smoke.free_count = 20;
    for (size_t i = 0; i < addr->plane.smoke.size; i++) {
        addr->plane.smoke.places[i] = 0;
    }
    addr->plane.unsmoke.size = 100;
    addr->plane.unsmoke.free_count = 100;
    for (size_t i = 0; i < addr->plane.unsmoke.size; i++) {
        addr->plane.unsmoke.places[i] = 0;
    }
    addr->message.state = Empty;
    addr->is_active = true;
    printf("App ready\n");
    while (keepRunning) {
        sem_wait(sem);
        if (addr->message.state == Request) {
            if (addr->message.is_smoke) {
                if (addr->message.place < addr->plane.smoke.size &&
                    addr->plane.smoke.places[addr->message.place] == 0) {
                    addr->plane.smoke.free_count--;
                    addr->plane.smoke.places[addr->message.place] = addr->message.user;
                    sprintf(message, "Bought smoke place #%zu for user %d\n",
                            addr->message.place, addr->message.user);
                } else {
                    sprintf(message,
                            "Can't buy ticket in smoke salon: place #%zu for user %d\n",
                            addr->message.place,
                            addr->message.user);
                }
            } else {
                if (addr->message.place < addr->plane.unsmoke.size &&
                    addr->plane.unsmoke.places[addr->message.place] == 0) {
                    addr->plane.unsmoke.free_count--;
                    addr->plane.unsmoke.places[addr->message.place] = addr->message.user;
                    sprintf(message, "Bought unsmoke place #%zu for user %d\n",
                            addr->message.place,
                            addr->message.user);
                } else {
                    sprintf(message,
                            "Can't buy ticket in unsmoke salon: place #%zu for user %d\n",
                            addr->message.place,
                            addr->message.user);
                }
            }
            strncpy(addr->message.message, message, sizeof(message));
            addr->message.state = Response;

            addr->is_active = keepRunning = addr->plane.smoke.free_count > 0 ||
                                            addr->plane.unsmoke.free_count > 0;
            printf("%s", message);
        }
        sem_post(sem);
        usleep(SLEEP_U_SEC);
    }
    printf("\nStopping app\n");
    sem_wait(sem);
    addr->is_active = false;
    printf("Smoke salon:\n");
    print_salon(addr->plane.smoke.places, addr->plane.smoke.size);
    printf("Unsmoke salon:\n");
    print_salon(addr->plane.unsmoke.places, addr->plane.unsmoke.size);
    sem_post(sem);
    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(shm_id);
    if (sem_close(sem) == -1) {
        perror("can't close semaphore");
    }
    if (sem_unlink(SEMAPHORE_NAME) == -1) {
        perror("can't delete semaphore");
    }
    if (shm_unlink(SHARED_MEMORY_OBJECT_NAME) == -1) {
        perror("can't delete shared memory");
    }
    return 0;
}