#!/bin/bash

DIFF=../bin/tp2diff
DIFFFLAGS="-q"
TESTINFILE=../data/test-in/test.in
TESTINDIR=../data/base
TESTOUTDIR=../data/test-out
TMPFOLDER=/tmp/orga2/output
BINFILE=../bin/tp2

OKDIFF=1

testDiff() {
    local archivo=$1
    local filtro=$2
    local params="${@:3}"

    rm $TMPFOLDER/c/*.bmp
    rm $TMPFOLDER/asm/*.bmp

	$BINFILE --frames $TMPFOLDER/c -v -i c $filtro $archivo $params
	$BINFILE --frames $TMPFOLDER/asm -v -i asm $filtro $archivo $params

    for frame in $TMPFOLDER/c/*
    do
        echo "Procesando.. $frame"
        FILENAME=$(basename $frame)
        CFILE=$TMPFOLDER/c/$FILENAME
        ASMFILE=$TMPFOLDER/asm/$FILENAME
        $DIFF $DIFFFLAGS $CFILE $ASMFILE  0

        if [ $? != "0" ]; then
            echo "ERROR EN $FILENAME"
            exit
            OKDIFF=0
        fi
    done
}

echo 'Iniciando test de diferencias...'

mkdir -p $TMPFOLDER/c
mkdir -p $TMPFOLDER/asm


while read f;
do
	file=$TESTINDIR/$f

	echo 'Procesando archivo: ' $file '\n'

	# color_filter
    testDiff $file fcolor 29 97 169 5000
    testDiff $file fcolor 92 90 55 2000
    testDiff $file fcolor 92 90 55 10000
    testDiff $file fcolor 200 20 20 16000
    testDiff $file fcolor 20 20 200 16000

    # miniature
    testDiff $file miniature 0.2 0.9 1
    testDiff $file miniature 0.4 0.8 10
    testDiff $file miniature 0.3 0.85 20

done < $TESTINFILE




if [ $OKDIFF != "0" ]; then
	echo "\nTests de diferencias finalizados correctamente.\n"
else
	echo "\nSe encontraron diferencias en alguna de las imágenes.\n"
fi
