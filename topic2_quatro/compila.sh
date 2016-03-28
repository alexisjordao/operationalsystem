#!/bin/bash

printf "\n\rCompilando..."
source ~/ti-processor-sdk-linux-am335x-evm-02.00.01.07/linux-devkit/environment-setup

rm moc_*
rm ui_*
rm *.o
rm Makefile

qmake
make

rm moc_*
rm ui_*
rm *.o
rm Makefile

printf "\n\rCopiando para a BBB (em /home/debian)..."
sshpass -p 'temppwd' scp topic2_quatro debian@192.168.7.2:~

printf "\n\rConectando-se na BBB\n\n\r"
sshpass -p 'temppwd' ssh debian@192.168.7.2

