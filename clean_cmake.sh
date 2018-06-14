#!/bin/bash
if [ -f "Makefile" ]; then
    make clean
fi

if [ -f "build/Makefile" ]; then
    make -C build/ clean
    rm -rf build/
fi

for fld in $(find -name "CMakeLists.txt" -printf '%h ')
do
    for cmakefile in CMakeCache.txt cmake_install.cmake CTestTestfile.cmake CMakeFiles Makefile *.moc_parameters moc_*
    do
        rm -rf $fld/$cmakefile
    done
done