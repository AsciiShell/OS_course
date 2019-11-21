#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "server.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int fdpub, fdpriv;
    struct message msg;
    while (1) {
        if ((fdpub = open(PUBLIC_CHAN, O_RDONLY)) == -1) {
            perror(PUBLIC_CHAN);
            exit(1);
        }
        while (read(fdpub, (char *) &msg, sizeof(msg)) > 0) {
            if ((fdpriv = open(msg.privfifo, O_WRONLY)) == -1) {
                perror(msg.privfifo);
                break;
            }
            size_t len = strlen(msg.string);
            for (size_t i = 0; i < (LEN - len) / 2; i++) {
                write(fdpriv, " ", 1);
            }
            write(fdpriv, msg.string, len);
            close(fdpriv);
        }
        close(fdpub);
    }
    return 0;
}
