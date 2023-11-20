#!/bin/bash

if [ ! -f main ]
then
	make
fi

ERROS=$(./main | grep -i Errado | wc -l)
if [ $ERROS -eq "0" ]
then
	echo "O programa executou sem erros."
else
	echo "O programa apresentou $ERROS erros."
fi
