#!/bin/bash
clear
gcc mybash.c command.c command.h chain.c chain.h -o launcher
ledit ./launcher