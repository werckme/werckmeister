compiler=../build/sheetc
refdir=./references
for x in ../sheets/*.sheet
do
  file=$(basename $x).mid
  $compiler $x
  printf "\e[39m$file: \e[31m"
  ./compare_midi.py $file $file
  if [ $? -eq 0 ]; then
    echo -e "\e[92mOK\e[39m"
    rm $file
  fi
done
