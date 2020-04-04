export WM_LINUX_LINK_STATIC=1
cmake -DCMAKE_BUILD_TYPE=Release ..
make && cpack
