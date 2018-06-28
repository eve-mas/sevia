#!/bin/bash 
> exetime_gossip_protocol_emptiness.txt
cd ..

for i in `seq 2 $1`;
do
	./evia -r experiments/gossip_protocol$i.rml > experiments/temp.txt
	#echo $i
	cat experiments/temp.txt
	Ctime=`cat experiments/temp.txt | grep 'Compilation' | awk '{print $4}'`
	Tstr=`cat experiments/temp.txt | grep 'execution' | awk '{print $4}'`
	Sstr=`cat experiments/temp.txt | grep 'reachable states' | awk '{print $6}'`	
	Bstr=`cat experiments/temp.txt | grep 'BDD' | awk '{print $6}'`
	fSize=`ls -l output/ |grep "gossip_protocol$i.ispl" | awk '{print $5}'`
	NE=`cat experiments/temp.txt | if grep -q 'TRUE'; then echo YES; else echo NO; fi`

	echo "$Ctime;$Tstr;$Sstr;$Bstr;$fSize;$NE; " >> experiments/exetime_gossip_protocol_emptiness.txt
done
