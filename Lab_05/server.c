#include <math.h>
#include "header.h"
#include <signal.h>
#include <string.h>

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main() {
    int shm_id;
    struct Server *addr;
    sem_t *sem;
    // Создаем кусок памяти
    if ((shm_id = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_RDWR, 0777)) == -1) {
        perror("can't create shared memory");
        return 1;
    }
    if (ftruncate(shm_id, SHARED_MEMORY_OBJECT_SIZE + 1) == -1) {
        perror("can't ftruncate memory");
        return 1;
    }
    addr = (struct Server *) mmap(0, SHARED_MEMORY_OBJECT_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm_id, 0);
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
    for (size_t i = 0; i < addr->plane.smoke.size; i++) {
        addr->plane.smoke.places[i] = 0;
    }
    addr->plane.unsmoke.size = 100;
    for (size_t i = 0; i < addr->plane.unsmoke.size; i++) {
        addr->plane.unsmoke.places[i] = 0;
    }
    addr->message.state = Empty;
    addr->is_active = true;
    printf("App ready\n");
    while (keepRunning) {
        sem_wait(sem);
        if (addr->message.state == Request) {
            addr->message.state = Response;
            char message[] = "Request calculated";
            strncpy(addr->message.message, message, sizeof(message));
            printf("Get request\n");
        }
        sem_post(sem);
        sleep(1);
    }
    printf("\nStopping app\n");
    sem_wait(sem);
    addr->is_active = false;
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

