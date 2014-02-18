#! /usr/bin/env bash

CC="cc"; # c compiler command

SOURCE="wirefang.c";
OUTPUT="../bin/wirefang";

# compiler & linker flags
CFLAGS="";
LDFLAGS="";

# create output directory
mkdir -p ../bin;

if [ "$(which $CC)" == "" ];
then
    echo "error: failure to locate C compiler ($CC)" 1>&2;
    exit 1; # exit failure
fi

# compile/link source code
$CC $CFLAGS -o $OUTPUT $SOURCE $LDFLAGS;

