#!/bin/sh
# for some reason the cmake DOWNLOAD_EXTERNALS script fails 
# during building the bootstrap binary.
# Strangely enough it succeeds after a second try.
# Some how it has trouble to find the right toolset.
VERSION=1.72.0
FILE=boost_1_72_0
FILENAME=$FILE.tar.gz
url=https://boostorg.jfrog.io/artifactory/main/release/$VERSION/source/$FILENAME
echo Downloading $url
curl -L $url > $FILENAME
echo extracting
tar -xf $FILENAME
mv $FILE boost
echo bootstraping
cd boost
sh ./bootstrap.sh
echo build
./b2 link=static runtime-link=static --with-filesystem --with-program_options --with-locale --with-system --with-test --with-date_time