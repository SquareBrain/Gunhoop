DIR="I"
if [ "$#" != "0" ];then
    DIR=$1
fi

ls -lR ${DIR} | grep '.[cpp,h]' | wc -l
