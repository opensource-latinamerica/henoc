#!/bin/bash

########
##
## Archivo que compila las librerias y el proyecto completo, ademas de eliminar 
## residuos que puedan interferir con la generacion de librerias y binarios
##
########

case $TERM in
    xterm|rxvt|Eterm|eterm|linux)
        ECHO_COMMAND='echo -ne'
        OS="linux"
        ;;
    xterm-color)
        ECHO_COMMAND='echo'
        OS="osx"
        ;;
    cygwin)
        ECHO_COMMAND='echo -ne'
        OS="windows"
esac


${ECHO_COMMAND} "\n\n\033[1m**********************************************************\033[0m \n"
${ECHO_COMMAND}     "\033[1m         Limpiando objetos y binarios anteriores...       \033[0m \n"
${ECHO_COMMAND}     "\033[1m**********************************************************\033[0m \n\n"

rm -fv Henoc.exe && rm -rfv Henoc.app && rm -fv Henoc
cd HenocUniverse/ && make clean
cd -
cd FullHenoc/ && qmake Henoc.pro && make clean && rm -fv Makefile
cd -

${ECHO_COMMAND} "\n\n\033[1m**********************************************************\033[0m \n"
${ECHO_COMMAND}     "\033[1m                     Compilando libreria...               \033[0m \n"
${ECHO_COMMAND}     "\033[1m**********************************************************\033[0m \n\n"
cd HenocUniverse/
make
cd -

${ECHO_COMMAND} "\n\n\033[1m**********************************************************\033[0m \n"
${ECHO_COMMAND}     "\033[1m             Generando Makefile del proyecto...           \033[0m \n"
${ECHO_COMMAND}     "\033[1m**********************************************************\033[0m \n\n"
cd FullHenoc/
qmake Henoc.pro && echo "OK..."

${ECHO_COMMAND} "\n\n\033[1m**********************************************************\033[0m \n"
${ECHO_COMMAND}     "\033[1m                   Compilando proyecto...                 \033[0m \n"
${ECHO_COMMAND}     "\033[1m**********************************************************\033[0m \n\n"
make
cd -
