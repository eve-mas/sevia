#!/bin/bash 
> exetime_replica_control_enash.txt
cd ..

for i in `seq 2 $1`;
do
	./evia -c experiments/mcmas_gifford_enash$i.rml > experiments/temp.txt
	#echo $i
	cat experiments/temp.txt
	Ctime=`cat experiments/temp.txt | grep 'Compilation' | awk '{print $4}'`
	Tstr=`cat experiments/temp.txt | grep 'execution' | awk '{print $4}'`
	Sstr=`cat experiments/temp.txt | grep 'reachable states' | awk '{print $6}'`	
	Bstr=`cat experiments/temp.txt | grep 'BDD' | awk '{print $6}'`
	fSize=`ls -l output/ |grep "mcmas_gifford_enash$i.ispl" | awk '{print $5}'`
	NE=`cat experiments/temp.txt | if grep -q 'TRUE'; then echo YES; else echo NO; fi`

	echo "$Ctime;$Tstr;$Sstr;$Bstr;$fSize;$NE; " >> experiments/exetime_replica_control_enash.txt
done
