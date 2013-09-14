#!/bin/sh

VALGRIND=valgrind
VALGRINDFLAGS="--leak-check=yes --error-exitcode=1 -q"
TESTINFILE=../data/test-in/test.in
TESTINDIR=../data/test-in
BINFILE=../bin/tp2

OKVALGRIND=1

echo 'Iniciando test de memoria y diferencias...'

while read f;
do
	file=$TESTINDIR/$f

	echo '\nProcesando archivo: ' $file '\n'

	# recortar
	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c recortar $file 100
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: recortar"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm recortar $file 100
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: recortar"
	fi


	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c recortar $file 23
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: recortar"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm recortar $file 23
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: recortar"
	fi



	# halftone
	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c halftone $file
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: halftone"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm halftone $file
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: halftone"
	fi



	# colorizar
	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c colorizar $file 0.5
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: colorizar"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm colorizar $file 0.5
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: colorizar"
	fi



	# umbralizar
	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c umbralizar $file 64 128 16
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: umbralizar"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm umbralizar $file 64 128 16
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: umbralizar"
	fi


	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c umbralizar $file 71 169 21
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: umbralizar"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm umbralizar $file 71 169 21
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: umbralizar"
	fi



	# waves
	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c waves $file 1 1 64
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: waves"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm waves $file 1 1 64
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: waves"
	fi


	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c waves $file 16 8 15
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: waves"
	fi

	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i asm waves $file 16 8 15
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: waves"
	fi


	# rotar
	$VALGRIND $VALGRINDFLAGS $BINFILE -v -i c rotar $file
	if [ $? != "0" ]; then
		OKVALGRIND=0
		echo "Error/es en el filtro: rotar"
	fi
done < $TESTINFILE


if [ $OKVALGRIND != "0" ]; then
	echo "\nTests de memoria finalizados correctamente.\n"
else
	echo "\nSe encontraron problemas de memoria en alguno de los filtros.\n"
fi
