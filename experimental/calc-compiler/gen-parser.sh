#!/bin/bash

bison --yacc -dv calc.y -o y.tab.c
flex calc.l
rm y.output

