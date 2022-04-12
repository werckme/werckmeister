#!/bin/bash

infile=$1
outfile=$2
arecord -f cdr -t raw -D "sysdefault:CARD=USB" | lame -x -r - $outfile &
rpid=$!
sleep 1
sheetp $infile
sleep 2
kill -SIGTERM $rpid