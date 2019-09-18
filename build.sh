RED='\033[0;32m'
NC='\033[0m' # No Color
base=$PWD
for file in `find . -type f -name main.sh`
do 
	abspath="$(realpath "${file}")"
	echo -e "${RED}${file}${NC}"
	cd "$(dirname "${file}")"
	sh $abspath
	cd $base
done