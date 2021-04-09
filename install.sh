#!/bin/sh

if test -z "$1" 
then
   PREFIX="/usr/local"
else
   if [ $1 == "-h" ]
   then 
   echo "usage: install.sh [INSTALL_PREFIX]"
   exit
   else
   PREFIX=$1
   fi
fi

echo "...installing headers in $PREFIX/include/carl"
install -d $PREFIX/include/carl 
install -c include/carl/*.h $PREFIX/include/carl
echo "...installing libraries in $PREFIX/lib"
install -d $PREFIX/lib 
install -c lib/*.a $PREFIX/lib
echo "...installing programs in $PREFIX/bin"
install -d $PREFIX/bin
install -c bin/* $PREFIX/bin
