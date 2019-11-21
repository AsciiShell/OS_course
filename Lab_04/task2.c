#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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

            close(q[R]);
            if (dup2(q[W], W) == -1) {
                perror("Error with pipe 'q' in 'center_filter' for write");
            }

            execl("center_filter", "center_filter", NULL);
            perror("center_filter: call error");
            exit(1);
        }
        default: {
            close(p[R]);
            fp = fdopen(p[W], "w");
            const int MAX_STRING_LENGTH = 10000;
            char string[MAX_STRING_LENGTH];

            int chr;
            const size_t MAX_COUNT = 8;
            size_t count = 0;
            while ((chr = fgetc(stdin)) != EOF) {
                fprintf(fp, "%02X ", chr);
                count++;
                if (count >= MAX_COUNT) {
                    fprintf(fp, "\n");
                    count = 0;
                }
            }
            close(fp);
//            close(p[W]);
            close(q[W]);
            if (dup2(q[R], R) == -1)
                perror("Error is here");
            while (!feof(stdin)) {
                fgets(string, MAX_STRING_LENGTH, stdin);
                if (!feof(stdin))
                    printf("%s\n", string);
            }
            exit(0);
        }
    }
}