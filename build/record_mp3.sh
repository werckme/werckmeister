#!/bin/bash

infile=$1
outfile=$(basename $infile)
arecord -f cdr -t raw -D "sysdefault:CARD=USB" | lame -x -r - $outfile.mp3 &
rpid=$!
sleep 1
./sheetp $infile
sleep 2
kill -SIGTERM $rpid