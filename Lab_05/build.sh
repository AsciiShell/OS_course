set -ex
cc client.c -o client.o -lrt -pthread
cc server.c -o server.o -lrt -pthread
