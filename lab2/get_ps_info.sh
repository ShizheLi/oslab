#!/bin/bash
gcc -static -o get_ps_info get_ps_info.c
sudo cp get_ps_info ~/oslab/busybox-1.32.1/_install/