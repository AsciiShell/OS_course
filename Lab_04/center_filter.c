#include <stdio.h>
#include <string.h>

int main() {
    const size_t LEN = 51;
    char currentline[LEN];
    while (fgets(currentline, LEN, stdin) != NULL) {
        size_t len = strlen(currentline);
        for (size_t i = 0; i < (LEN - len) / 2; i++) {
            printf(" ");
        }
        printf("%s", currentline);
    }
    printf("\n");
    return 0;
}