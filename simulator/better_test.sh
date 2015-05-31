path=~/hell/
origin=~/102062207_03/simulator

cd $path

for i in *
do
    echo $i >> ~/test.log
    cd ./$i
    for e in *
    do
        cd ./$e
        echo $e >> ~/test.log
        cp iimage.bin $origin
        cp dimage.bin $origin

        cd $origin
    
        make
        ./CMP

        diff -b snapshot.rpt $path/$i/$e/snapshot.rpt >> ~/test.log
        diff -b report.rpt $path/$i/$e/report.rpt >> ~/test.log
        make clean
        cd $path/$i
    done
    cd $path
done

