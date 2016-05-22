#!/bin/bash
clear
gcc mybash.c chain.c chain.h command.c command.h -o launcher
chmod +x launcher
ledit ./launcher