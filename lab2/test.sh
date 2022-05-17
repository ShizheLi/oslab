#!/bin/bash
gcc -o shell my_shell.c
./shell
cd ..
cd ~
cd /
cd ~/oslab/lab2
ps aux | wc -l
ps aux | grep firefox | wc -l
echo aaa; echo bbb; echo ccc
echo hello > hello.txt
echo world >> hello.txt
cat < hello.txt
exit