#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int fd[2]; // 0 - Read, 1 - Write
    int status;
    switch (fork()) {
        case -1: {
            perror("Ошибка при вызове fork() \n");
            exit(1);
        }
        case 0: {
            pipe(fd);
            switch (fork()) {
                case -1: {
                    perror("Ошибка при вызове fork() \n");
                    exit(2);
                }
                case 0: {
                    close(fd[1]); // Close write
                    dup2(fd[0], 0);
                    execl("hex_filter", "hex_filter", NULL);
                    perror("Ошибка при вызове 'hex_filter'\n");
                    exit(3);
                }
                default: {
                    close(fd[0]); // Close read
                    dup2(fd[1], 1);
                    execl("center_filter", "center_filter", NULL);
                    perror("Ошибка при вызове 'center_filter'\n");
                    exit(4);
                }
            }
        }
        default: {
            wait(&status);
            exit(1);
        }
    }
}