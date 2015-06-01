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
        
        ./CMP
        mv report.rpt repo_2.rpt
        mv snapshot.rpt sna_2.rpt

        /home/archi/CMP
        diff -b report.rpt repo_2.rpt >> ~/test.log
        diff -b snapshot.rpt sna_2.rpt >> ~/test.log
        cd $path/$i
    done
    cd $path
done

