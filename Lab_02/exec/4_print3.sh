$ ./print3
printing base_command
lastDIR=$PWD
cd "$1"
ls -l "$2"
cd "$lastDIR"
printing print3
for file in $(ls); do
  echo "printing $file"
  more "$file"
done
