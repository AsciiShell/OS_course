#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char **argv) {
    struct dirent *d;
    DIR *dp;

    if ((dp = opendir("..")) == NULL)
        return (-1);

    unsigned int count = 0;
    while ((d = readdir(dp))) {
        if (d->d_type == 4) {
            printf("Dirname\"%s\"\t\t inode:%u\n", d->d_name, d->d_ino);
            count += 1;
        }
    }
    printf("Total number of files: %d\n", count);
    exit(0);

}
