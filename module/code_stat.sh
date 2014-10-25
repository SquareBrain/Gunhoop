DIR="*"
if [ "$#" != "0" ];then
    DIR=$1
fi

date >> code_stat.txt
echo "files:" >> code_stat.txt
ls -lR ${DIR} | grep '.[cpp,h]' | wc -l >> code_stat.txt
echo "lines:" >> code_stat.txt
find $DIR -name "*.[cpp,h]*" |xargs cat|grep -v ^$|wc -l >> code_stat.txt
cat code_stat.txt
