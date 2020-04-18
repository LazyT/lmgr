if [ -z $1 ]; then echo -e "Missing Language!\n\n  $0 en\n  $0 de\n" && exit 1; fi

cp $1.qhp  src
cp $1.qhcp src

qhelpgenerator src/$1.qhp  -o dst/$1.qch
qhelpgenerator src/$1.qhcp -o dst/$1.qhc

rm src/$1.qhp
rm src/$1.qhcp
