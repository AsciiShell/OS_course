#include <stdlib.h>
#include "header.h"

size_t get_zero_val(const int arr[MAX_SIZE], size_t len) {
    size_t result = 0;
    for (size_t i = 0; i < len; i++) {
        if (arr[i] == 0) {
            result++;
        }
    }
    return result;
}

int get_n_zero_index(const int arr[MAX_SIZE], size_t len, size_t pos) {
    size_t result = 0;
    for (size_t i = 0; i < len; i++) {
        if (arr[i] == 0) {
            if (result == pos) {
                return i;
            }
            result++;
        }
    }
    return -1;
}

int main() {
    int shm_id;
    struct Server *addr;
    sem_t *sem;
    int user_id = getpid();
    // Создаем кусок памяти
    if ((shm_id = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_RDWR, 0777)) == -1) {
        perror("can't create shared memory");
        return 1;
    }
    if ((sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED) {
        perror("can't open semaphore");
        return 1;
    }
    addr = (struct Server *) mmap(0, SHARED_MEMORY_OBJECT_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm_id, 0);
    if (addr == (struct Server *) -1) {
        perror("can't mmap memory");
        return 1;
    }
    bool is_active = true;
    while (is_active) {
        sem_wait(sem);
        is_active = addr->is_active;
        if (addr->message.state == Empty) {
            bool is_smoke = rand() % 2;
            size_t place;
            if (is_smoke) {
                size_t order = rand() % get_zero_val(addr->plane.smoke.places, addr->plane.smoke.size);
                place = get_n_zero_index(addr->plane.smoke.places, addr->plane.smoke.size, order);
            } else {
                size_t order = rand() % get_zero_val(addr->plane.unsmoke.places, addr->plane.unsmoke.size);
                place = get_n_zero_index(addr->plane.unsmoke.places, addr->plane.unsmoke.size, order);
            }
            if (place == -1) {
                printf("can't find place");
            } else {
                addr->message.is_smoke = is_smoke;
                addr->message.place = place;
                addr->message.user = user_id;
                addr->message.state = Request;
                printf("Send request: salon %d\tplace %zu\n", is_smoke, place);
            }
        } else if (addr->message.state == Response && addr->message.user == user_id) {
            printf("Get response: %s\n", addr->message.message);
            addr->message.state = Empty;
        }
        sem_post(sem);
        sleep(1);
    }

    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(shm_id);
    return 0;
}

