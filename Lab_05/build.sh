set -ex
cc child.c -o child.o -lrt -pthread
cc parent.c -o parent.o -lrt -pthread
