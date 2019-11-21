#include <stdio.h>
#include <string.h>

int main() {
    int chr;
    const size_t MAX_COUNT = 8;
    size_t count = 0;
    while ((chr = fgetc(stdin)) != EOF) {
        printf("%02X ", chr);
        count++;
        if (count >= MAX_COUNT) {
            printf("\n");
            count = 0;
        }
    }
    return 0;
}