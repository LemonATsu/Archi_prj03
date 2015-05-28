path=~/testcase_prj3
se=~/102062207_03/simulator

cp $path/example1/dimage.bin .
cp $path/example1/iimage.bin .
make
./CMP
diff -b report.rpt $path/example1/report.rpt
diff -b snapshot.rpt $path/example1/snapshot.rpt
make clean

cp $path/example2/dimage.bin .
cp $path/example2/iimage.bin .
make
./CMP
diff -b report.rpt $path/example2/report.rpt
diff -b snapshot.rpt $path/example2/snapshot.rpt
make clean

cp $path/example3/dimage.bin .
cp $path/example3/iimage.bin .
make
./CMP
diff -b report.rpt $path/example3/report.rpt
diff -b snapshot.rpt $path/example3/snapshot.rpt
make clean
