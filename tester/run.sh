#!/bin/bash
BIN=woody_woodpacker

#VALID TESTS
for f in tester/bins/*; do
	./$BIN $f
	ret=$?
	if [ $ret -ne 0 ]; then
	    echo ">> $f : fail on $ret"
	fi
done
