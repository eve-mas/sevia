import sys

def main(argv=None): # parses the command line arguments and calls Membership
	f = open('p2p.rml','w')	
#	f.write(str(sys.argv[1]))
	for x in range (0,int(sys.argv[1])):
		#f.write(str(sys.argv[1]))
		f.write('module m'+str(x)+' controls d'+str(x)+',u'+str(x)+'\n')
		f.write('init\n')
		f.write(":: true ~> u"+str(x)+"' := true, d"+str(x)+"':= false;\n")
		f.write(":: true ~> u"+str(x)+"' := false, d"+str(x)+"':= true;\n")
		f.write('update\n')
		f.write(":: true ~> u"+str(x)+"' := true, d"+str(x)+"':= false;\n")
		f.write(":: true ~> u"+str(x)+"' := false, d"+str(x)+"':= true;\n")
		
		f.write('\n');

	f.write('goals\n')
	for x in range (0,int(sys.argv[1])):
		f.write('#SL (G F (d'+str(x)+' and (')
		count = 0
		for y in range (0,int(sys.argv[1])):
			if(y!=x):
				if(count!=0):
					f.write(' or u'+str(y))
				else:
					f.write('u'+str(y))
				count=+1
		f.write(')))\n')
if __name__ == '__main__':
    main()
