#!/bin/bash 
> exetime_bisim_none_emptiness.txt
cd ..

for i in `seq 1 $1`;
do
	./evia -c experiments/bisim_none$i.rml > experiments/temp.txt
	#echo $i
	cat experiments/temp.txt
	Ctime=`cat experiments/temp.txt | grep 'Compilation' | awk '{print $4}'`
	Tstr=`cat experiments/temp.txt | grep 'execution' | awk '{print $4}'`
	Sstr=`cat experiments/temp.txt | grep 'reachable states' | awk '{print $6}'`	
	Bstr=`cat experiments/temp.txt | grep 'BDD' | awk '{print $6}'`
	fSize=`ls -l output/ |grep "bisim_none$i.ispl" | awk '{print $5}'`
	NE=`cat experiments/temp.txt | if grep -q 'TRUE'; then echo YES; else echo NO; fi`

	echo "$Ctime;$fl;$Tstr;$Sstr;$Bstr;$fSize;$NE; " >> experiments/exetime_bisim_none_emptiness.txt
done
