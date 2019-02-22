#!/bin/bash

LOGFILE=./task_logfile
echo "Fractaltask on 1-24 threads"
IDEALTHREADS=22
TASKSTART=12
FLAGS="-m 20 -n $IDEALTHREADS"

for i in `seq $1 40`;
do
	echo "Task $i" >> $LOGFILE
	mytime="$(time (./fractaltask $FLAGS -k $i) 2>&1 1>/dev/null)"
	echo " $mytime" | grep real >> $LOGFILE
	echo "Processed one"
	echo >> $LOGFILE
done
