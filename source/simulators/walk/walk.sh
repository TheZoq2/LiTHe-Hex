#!/bin/bash

filename=/tmp/hexsim/leg_input

while true; do
	echo 5,0,0 > $filename
	echo 5,0,0 >> $filename
	echo 5,0,0 >> $filename
	echo 5,0,0 >> $filename
	echo 5,0,0 >> $filename
	echo 5,-5,-25 >> $filename
	sleep 0.5
	echo 0,-5,-25 > $filename
	echo 0,-5,-25 >> $filename
	echo 0,-5,-25 >> $filename
	echo 0,-5,-25 >> $filename
	echo 0,-5,-25 >> $filename
	echo 0,10,-30 >> $filename
	sleep 0.5
	echo -5,10,-30 > $filename
	echo -5,10,-30 >> $filename
	echo -5,10,-30 >> $filename
	echo -5,10,-30 >> $filename
	echo -5,10,-30 >> $filename
	echo -5,10,-15 >> $filename
	sleep 0.5
	echo 0,10,-15 > $filename
	echo 0,10,-15 >> $filename
	echo 0,10,-15 >> $filename
	echo 0,10,-15 >> $filename
	echo 0,10,-15 >> $filename
	echo 0,10,10 >> $filename
	sleep 0.5
	echo 5,10,10 > $filename
	echo 5,10,10 >> $filename
	echo 5,10,10 >> $filename
	echo 5,10,10 >> $filename
	echo 5,10,10 >> $filename
	echo 5,0,0 >> $filename
	sleep 0.5
	echo 0,0,0 > $filename
	echo 0,0,0 >> $filename
	echo 0,0,0 >> $filename
	echo 0,0,0 >> $filename
	echo 0,0,0 >> $filename
	echo 0,0,0 >> $filename
done
