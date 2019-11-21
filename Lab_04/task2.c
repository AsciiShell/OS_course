#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define R 0
#define W 1

int main(void) {
    int p[2], q[2];
    FILE *fp;
    pipe(p);
    pipe(q);
    switch (fork()) {
        case -1: {
            perror("Ошибка при вызове fork() \n");
            exit(1);
        }
        case 0: {
            close(p[W]);
            if (dup2(p[R], R) == -1) {
                perror("Error with pipe 'p' in 'center_filter' for read");
            }
            close(p[R]);
            close(q[R]);
            if (dup2(q[W], W) == -1) {
                perror("Error with pipe 'q' in 'center_filter' for write");
            }
            close(q[W]);
            execl("center_filter", "center_filter", NULL);
            perror("center_filter: call error");
            exit(1);
        }
        default: {
            close(p[R]);
            close(q[W]);

            fp = fdopen(p[W], "w");
            const size_t LEN = 51;
            char string[LEN];

            int chr;
            const size_t MAX_COUNT = 8;
            size_t count = 0;
            while (fgets(string, LEN, stdin) != NULL) {
                fprintf(fp, "%s", string);
            }
            fclose(fp);
            fp = fdopen(q[R], "r");
            while ((chr = fgetc(fp)) != EOF) {
                printf("%02X ", chr);
                count++;
                if (count >= MAX_COUNT) {
                    printf("\n");
                    count = 0;
                }
            }
            fclose(fp);
            exit(0);
        }
    }
}