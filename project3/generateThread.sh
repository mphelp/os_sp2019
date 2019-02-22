#!/bin/bash

LOGFILE=./thread_logfile
echo "Fractalthread on 1-24 threads"
FLAGS="-m 20"

for i in `seq 1 24`;
do
	echo "Thread $i" >> $LOGFILE
	for j in `seq 1 5`;
	do
		mytime="$(time (./fractalthread $FLAGS -n $i) 2>&1 1>/dev/null)"
		echo " $mytime" | grep real >> $LOGFILE
	done
	echo >> $LOGFILE
	echo "$i threads done"
done
