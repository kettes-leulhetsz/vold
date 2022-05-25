#!/bin/sh

LIBALSA=$(pkg-config alsa --cflags --libs)
LIBX11=$(pkg-config x11 --cflags --libs)

${CC:-gcc} -Os vold.c -o vold $LIBALSA $LIBX11
