#!/bin/sh

VALGRIND=valgrind
VALGRINDFLAGS="--leak-check=no --error-exitcode=1 -q"
TESTINFILE=../data/test-in/test.in
TESTINDIR=../data/base
BINFILE=../bin/tp2

FRAMEFOLDER=/tmp/orga2/memtest

OKVALGRIND=1

echo '\nIniciando test de memoria y diferencias...\'
mkdir -p $FRAMEFOLDER

while read f;
do
	file=$TESTINDIR/$f

	echo 'Procesando archivo: ' $file '\n'

    # fcolor
	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER -i c fcolor $file 10 100 20 10000
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER -i asm fcolor $file 10 100 20 10000
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	# miniaturizar
	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER  -i c miniature $file 0.2 0.6 3
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER -i asm miniature $file 0.2 0.6 3
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	# miniaturizar
	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER  -i c miniature $file 0.01 0.9 4
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER -i asm miniature $file 0.01 0.9 4
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	# miniaturizar
	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER  -i c miniature $file 0.6 0.5 2
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER -i asm miniature $file 0.6 0.5 2
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	# miniaturizar
	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER  -i c miniature $file 0.43 0.91 1
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE --frames $FRAMEFOLDER -i asm miniature $file 0.43 0.91 1
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi

done < $TESTINFILE
rm $FRAMEFOLDER/*.bmp


if [ $OKVALGRIND != "0" ]; then
	echo "\nTests de memoria finalizados correctamente.\n"
else
	echo "\nSe encontraron problemas de memoria en alguno de los filtros.\n"
fi
