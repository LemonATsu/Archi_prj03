path=~/hell/
origin=~/102062207_03/simulator
param="64 512 8 64 16 4 4 256 16 8"

make

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
        
        ./CMP $param
        mv report.rpt repo_2.rpt
        mv snapshot.rpt sna_2.rpt

        /home/archi/CMP $param
        diff -b report.rpt repo_2.rpt >> ~/test.log
        diff -b snapshot.rpt sna_2.rpt >> ~/test.log
        
        rm *.rpt
        
        ./CMP
        mv report.rpt repo_2.rpt
        mv snapshot.rpt sna_2.rpt
        
        /home/archi/CMP
        
        diff -b report.rpt repo_2.rpt >> ~/test.log
        diff -b snapshot.rpt sna_2.rpt >> ~/test.log
        rm *.rpt *.bin
        cd $path/$i
    done
    cd $path
done

