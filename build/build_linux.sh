BOOST_ROOT=$HOME cmake -DCMAKE_BUILD_TYPE=Release ..
make sheetc rehearse
zip werckmeister.zip sheetc rehearse
