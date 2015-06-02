#!/bin/sh

LIBS="charbuffer.c manterface.c"

for i in *.test.c; do
	OUT=`echo $i | sed 's/.c$/.out/'`
	gcc -o $OUT $i $LIBS || exit

	./$OUT
	rm $OUT
	echo
done
