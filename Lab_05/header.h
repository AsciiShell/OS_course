//
// Created by alex1 on 08.12.2019.
//

#ifndef OS_COURSE_HEADER_H
#define OS_COURSE_HEADER_H
// Абстрактная структура для общения
struct Data {
    int a;
    int arr[3];
};
// Общий ключ для создания куска памяти
key_t shared_key = 1234;
#endif //OS_COURSE_HEADER_H
