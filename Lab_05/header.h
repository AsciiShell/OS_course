#ifndef OS_COURSE_HEADER_H
#define OS_COURSE_HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_SIZE 1000

struct Salon {
    size_t size;
    int places[MAX_SIZE];
};

struct Plane {
    struct Salon smoke;
    struct Salon unsmoke;
};
enum State {
    Empty = 0, Request = 1, Response = 2
};
struct Message {
    enum State state;
    int user;
    bool is_smoke;
    size_t place;
    char message[MAX_SIZE];
};

struct Server {
    struct Plane plane;
    struct Message message;
    bool is_active;
};
// Общий ключ для создания куска памяти
#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
const size_t SHARED_MEMORY_OBJECT_SIZE = sizeof(struct Server);
// Имя семафора
#define SEMAPHORE_NAME "/my_named_semaphore3"
#endif //OS_COURSE_HEADER_H
