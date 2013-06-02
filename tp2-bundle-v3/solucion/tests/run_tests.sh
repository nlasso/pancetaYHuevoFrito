#! /bin/sh

# Generar imagenes de test
./gen_imgs.sh

# Test de memoria
./test_mem.sh

# Test de diferencia entre versiones de C y ASM
./test_diff_c_asm.sh

# Test de diferencia con imagenes de la catedra
./test_diff_cat.sh