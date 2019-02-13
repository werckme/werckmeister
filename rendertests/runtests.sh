compiler=../build/sheetc
for x in ../sheets/*.sheet
do
  $compiler $x
  
  rm $(basename $x).mid
done
