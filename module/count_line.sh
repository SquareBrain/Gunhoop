DIR="*"
if [ "$#" != "0"];then
    DIR=$1
fi

find ${DIR} -name "*[cpp,h]" -exec cat {} \; | sed '/^\|\/\/\|\/\*|\*|\*\//d' | wc -l
