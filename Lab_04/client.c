#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct message msg;
    int fdpub, fdpriv;
    char line[LEN];
    sprintf(msg.privfifo, "Fifo%d", getpid());
    if (mkfifo(msg.privfifo, S_IFIFO | 0666) == -1) {
        perror(msg.privfifo);
        exit(1);
    }
    if ((fdpub = open(PUBLIC_CHAN, O_WRONLY)) == -1) {
        perror(PUBLIC_CHAN);
        exit(2);
    }
    char string[LEN];
    size_t count = 0;
    const size_t MAX_COUNT = 8;
    while (fgets(string, LEN, stdin) != NULL) {
        strcpy(msg.string, string);
        write(fdpub, (char *) &msg, sizeof(msg));
        if ((fdpriv = open(msg.privfifo, O_RDONLY)) == -1) {
            perror(msg.privfifo);
            exit(3);
        }
        int n;
        while ((n = read(fdpriv, line, LEN)) > 0) {
            for (size_t i = 0; i < n; i++) {
                printf("%02X ", line[i]);
                count++;
                if (count >= MAX_COUNT) {
                    printf("\n");
                    count = 0;
                }
            }
        }
        close(fdpriv);
    }
    printf("\n");
    unlink(msg.privfifo);
    return 0;
}