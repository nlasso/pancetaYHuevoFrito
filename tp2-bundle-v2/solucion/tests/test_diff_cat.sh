#!/bin/sh

DIFF=../bin/tp2diff
DIFFFLAGS="-q"
TESTINDIR=../data/test-in
TESTIMGCATEDRA=../../imagenes-ejemplo
BINFILE=../bin/tp2

OKDIFF=1

echo 'Iniciando test de diferencias...'

file=$TESTINDIR/"lena.512x512.bmp"

echo '\nProcesando archivo: ' $file '\n'



# recortar
$BINFILE -v -i c recortar $file 100
$DIFF $DIFFFLAGS $file".recortar.tam-100.c.bmp" $TESTIMGCATEDRA/"lena.recortar.catedra.bmp" 0
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: recortar"
fi

$BINFILE -v -i asm recortar $file 100
$DIFF $DIFFFLAGS $file".recortar.tam-100.asm.bmp" $TESTIMGCATEDRA/"lena.recortar.catedra.bmp" 0
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: recortar"
fi



# halftone
$BINFILE -v -i c halftone $file
$DIFF $DIFFFLAGS $file".halftone.c.bmp" $TESTIMGCATEDRA/"lena.halftone.catedra.bmp" 0
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: halftone"
fi

$BINFILE -v -i asm halftone $file
$DIFF $DIFFFLAGS $file".halftone.asm.bmp" $TESTIMGCATEDRA/"lena.halftone.catedra.bmp" 0
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: halftone"
fi



# colorizar
$BINFILE -v -i c colorizar $file 0.5
$DIFF $DIFFFLAGS $file".colorizar.alpha-0.50.c.bmp" $TESTIMGCATEDRA/"lena.colorizar.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: colorizar"
fi

$BINFILE -v -i asm colorizar $file 0.5
$DIFF $DIFFFLAGS $file".colorizar.alpha-0.50.asm.bmp" $TESTIMGCATEDRA/"lena.colorizar.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: colorizar"
fi



# umbralizar
$BINFILE -v -i c umbralizar $file 64 128 16
$DIFF $DIFFFLAGS $file".umbralizar.min-64.max-128.q-16.c.bmp" $TESTIMGCATEDRA/"lena.umbralizar.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: umbralizar"
fi

$BINFILE -v -i asm umbralizar $file 64 128 16
$DIFF $DIFFFLAGS $file".umbralizar.min-64.max-128.q-16.asm.bmp" $TESTIMGCATEDRA/"lena.umbralizar.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: umbralizar"
fi



# waves
$BINFILE -v -i c waves $file 8 8 32
$DIFF $DIFFFLAGS $file".waves.x_scale-8.00.y_scale-8.00.g_scale-32.00.c.bmp" $TESTIMGCATEDRA/"lena.waves.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: waves"
fi

$BINFILE -v -i asm waves $file 8 8 32
$DIFF $DIFFFLAGS $file".waves.x_scale-8.00.y_scale-8.00.g_scale-32.00.asm.bmp" $TESTIMGCATEDRA/"lena.waves.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: waves"
fi



# rotar
$BINFILE -v -i c rotar $file
$DIFF $DIFFFLAGS $file".rotar.c.bmp" $TESTIMGCATEDRA/"lena.rotar.catedra.bmp" 5
if [ $? != "0" ]; then
	OKDIFF=0
	echo "Diferencias en filtro: rotar"
fi



if [ $OKDIFF != "0" ]; then
	echo "\nTests de diferencias finalizados correctamente.\n"
else
	echo "\nSe encontraron diferencias en alguna de las imágenes.\n"
fi
