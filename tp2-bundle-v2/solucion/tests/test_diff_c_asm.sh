#!/bin/sh

DIFF=../bin/tp2diff
DIFFFLAGS="-q"
TESTINFILE=../data/test-in/test.in
TESTINDIR=../data/test-in
TESTOUTDIR=../data/test-out
BINFILE=../bin/tp2

OKDIFF=1

echo 'Iniciando test de memoria y diferencias...'

while read f;
do
	file=$TESTINDIR/$f

	echo '\nProcesando archivo: ' $file '\n'

	# recortar
	$BINFILE -v -i c recortar $file 100
	$BINFILE -v -i asm recortar $file 100

	$DIFF $DIFFFLAGS $file".recortar.tam-100.c.bmp" $file".recortar.tam-100.asm.bmp" 0
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: recotar"
	fi

	$BINFILE -v -i c recortar $file 23
	$BINFILE -v -i asm recortar $file 23

	$DIFF $DIFFFLAGS $file".recortar.tam-23.c.bmp" $file".recortar.tam-23.asm.bmp" 0
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: recotar"
	fi



	# halftone
	$BINFILE -v -i c halftone $file
	$BINFILE -v -i asm halftone $file

	$DIFF $DIFFFLAGS $file".halftone.c.bmp" $file".halftone.asm.bmp" 0
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: halftone"
	fi



	# colorizar
	$BINFILE -v -i c colorizar $file 0.5
	$BINFILE -v -i asm colorizar $file 0.5

	$DIFF $DIFFFLAGS $file".colorizar.alpha-0.50.c.bmp" $file".colorizar.alpha-0.50.asm.bmp" 5
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: colorizar"
	fi



	# umbralizar
	$BINFILE -v -i c umbralizar $file 64 128 16
	$BINFILE -v -i asm umbralizar $file 64 128 16

	$DIFF $DIFFFLAGS $file".umbralizar.min-64.max-128.q-16.c.bmp" $file".umbralizar.min-64.max-128.q-16.asm.bmp" 5
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: umbralizar"
	fi

	$BINFILE -v -i c umbralizar $file 71 169 21
	$BINFILE -v -i asm umbralizar $file 71 169 21

	$DIFF $DIFFFLAGS $file".umbralizar.min-71.max-169.q-21.c.bmp" $file".umbralizar.min-71.max-169.q-21.asm.bmp" 5
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: umbralizar"
	fi



	# waves
	$BINFILE -v -i c waves $file 1 1 64
	$BINFILE -v -i asm waves $file 1 1 64

	$DIFF $DIFFFLAGS $file".waves.x_scale-1.00.y_scale-1.00.g_scale-64.00.c.bmp" $file".waves.x_scale-1.00.y_scale-1.00.g_scale-64.00.asm.bmp" 5
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: waves"
	fi

	$BINFILE -v -i c waves $file 16 8 15
	$BINFILE -v -i asm waves $file 16 8 15

	$DIFF $DIFFFLAGS $file".waves.x_scale-16.00.y_scale-8.00.g_scale-15.00.c.bmp" $file".waves.x_scale-16.00.y_scale-8.00.g_scale-15.00.asm.bmp" 5
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: waves"
	fi

	$BINFILE -v -i c waves $file 7 13 11
	$BINFILE -v -i asm waves $file 7 13 11

	$DIFF $DIFFFLAGS $file".waves.x_scale-7.00.y_scale-13.00.g_scale-11.00.c.bmp" $file".waves.x_scale-7.00.y_scale-13.00.g_scale-11.00.asm.bmp" 5
	if [ $? != "0" ]; then
		OKDIFF=0
		echo "diferencias en filtro: waves"
	fi
done < $TESTINFILE


if [ $OKDIFF != "0" ]; then
	echo "\nTests de diferencias finalizados correctamente.\n"
else
	echo "\nSe encontraron diferencias en alguna de las imágenes.\n"
fi

