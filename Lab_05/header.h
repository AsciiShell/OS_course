#ifndef OS_COURSE_HEADER_H
#define OS_COURSE_HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

// Абстрактная структура для общения
struct Data {
    int a;
    int arr[3];
};
// Общий ключ для создания куска памяти
#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
const size_t SHARED_MEMORY_OBJECT_SIZE = sizeof(struct Data);
#define SEMAPHORE_NAME "/my_named_semaphore"
#endif //OS_COURSE_HEADER_H
