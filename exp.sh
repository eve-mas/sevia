#!/bin/bash 
> tcgen/exetime.txt
> tcgen/totable.txt

for i in `seq 2 $2`;
do
	cd tcgen/
	python $1.py $i
	cd ..
	./stic tcgen/$1.rml > tcgen/temp.txt
	Ctime=`cat tcgen/temp.txt | grep 'Compilation' | awk '{print $4}'`
	Tstr=`cat tcgen/temp.txt | grep 'execution' | awk '{print $4}'`
	Sstr=`cat tcgen/temp.txt | grep 'reachable states' | awk '{print $6}'`	
	Bstr=`cat tcgen/temp.txt | grep 'BDD' | awk '{print $6}'`
	fSize=`ls -l output/ |grep "$1.ispl" | awk '{print $5}'`
	
	if [ $1 == "p2p" ]; then #SL formulae size for p2p = 2x(n^2)
		fl=$[2*$[$i*$i]]
	else
		fl=$[2*$i]
	fi

	echo "$i;$Ctime;$fl;$Tstr;$Sstr;$Bstr;$fSize " >> tcgen/exetime.txt
	echo "$i&$Ctime&$fl&$Tstr&$Sstr&$Bstr&$fSize" '\\' >> tcgen/totable.txt 
done
cd tcgen/
python plotter.py
cd ..

