import sys
import matplotlib.pyplot as plt
import csv
import math

def main(argv=None):
	players = []	
	compiletime = []
	exetime = []
	states = []
	bddmem = []
	filesize = []
	formulasize = []
#	f = open('totable.txt','w')
	with open('exetime.txt','r') as csvfile:
		plots = csv.reader(csvfile, delimiter=';')
    		for row in plots:
			players.append(int(row[0]))	
			#f.write(str(int(row[0]))+"&")
			compiletime.append(float(row[1]))
			#f.write('%.2e &' % float(row[1]))
			formulasize.append(int(row[2]))
			#f.write('%.2e &' % int(row[2]))
			exetime.append(float(row[3]))
			states.append(float(row[4]))
			bddmem.append(float(row[5]))
			filesize.append(float(row[6]))
			
	
#	plt.plot(w,x, label='Loaded from file!')
#	plt.xlabel('Number of players')
#	plt.ylabel('Execution time (s)')
#	plt.title('Interesting Graph\nCheck it out')
#	plt.legend()
#	plt.show()

	f, ((plot1,plot2,plot3),(plot4,plot5,plot6)) = plt.subplots(nrows=2, ncols=3, sharex=True)
	plot1.plot(players, compiletime,'b-o')
	plot1.set_ylabel('compilation time (s)')
	plot2.plot(players, formulasize,'b-o')
	plot2.set_ylabel('formula size')
	plot3.plot(players, exetime,'b-o')
	plot3.set_ylabel('execution time (s)')
	plot4.plot(players, states,'b-o')
	plot4.set_ylabel('reachable states')
	plot5.plot(players, bddmem,'b-o')
	plot5.set_ylabel('BDD memory (byte)')
	plot6.plot(players, filesize,'b-o')
	plot6.set_ylabel('filesize (byte)')
	f.text(0.5, 0.02, 'number of players', ha='center')
	plt.tight_layout()
	plt.show()

if __name__ == '__main__':
    main()
