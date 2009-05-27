#!/bin/bash

TARGET_FILE=linux-fetion.pro
IS_CPU_64BIT=1
#echo $TARGET_FILE
#gcc --version
#gcc -dumpmachine
if [ $IS_CPU_64BIT -eq 0 ] ; then
    sed -i -e "/libfetion.a/c    LIBS +=  -lcurl ./lib/libfetion_32.a" $TARGET_FILE
    sed -i -e "/libfetion_64.a/c    LIBS +=  -lcurl ./lib/libfetion_32.a" $TARGET_FILE
else
    sed -i -e "/libfetion_32.a/c    LIBS +=  -lcurl ./lib/libfetion_64.a" $TARGET_FILE
    sed -i -e "/libfetion.a/c    LIBS +=  -lcurl ./lib/libfetion_64.a" $TARGET_FILE
fi
