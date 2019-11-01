pwd=$(pwd)
cwd="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $cwd
compiler=../build/sheetc
refdir=./references
for x in ./tests/*.sheet
do
  file=$(basename $x).mid
  $compiler $x
  printf "\e[39m$file: \e[31m"
  ./compare_midi.py $file $refdir/$file
  if [ $? -eq 0 ]; then
    echo -e "\e[92mOK\e[39m"
    rm $file
  fi
done
cd $pwd