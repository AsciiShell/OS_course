$ cat base_command
lastDIR=$PWD
cd "$1"
ls -l "$2"
cd "$lastDIR"
$ ./base_command ~/.. 17219
total 8
drwxr-xr-x  3 17219  17219  512 12 сен 16:39 Catalog1
drwxr-xr-x  2 17219  17219  512 19 сен 16:48 lab2
