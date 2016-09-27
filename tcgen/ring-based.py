import sys

def main(argv=None): # parses the command line arguments and calls Membership
	f = open('ring-based.rml','w')	
#	f.write(str(sys.argv[1]))
	for x in range (0,int(sys.argv[1])):
		#f.write(str(sys.argv[1]))
		if(x==0):
			prev = int(sys.argv[1])-1
		else:
			prev = x-1
		f.write('module m'+str(x)+' controls x'+str(x)+',y'+str(x)+'\n')
		f.write('init\n')
		if(x+1==int(sys.argv[1])):
			f.write(":: true ~> x"+str(x)+"' := false, y"+str(x)+"':= true;\n")
		else:
			f.write(":: true ~> x"+str(x)+"' := false, y"+str(x)+"':= false;\n")
		#f.write(":: true ~> u"+str(x)+"' := false, d"+str(x)+"':= true;\n")
		f.write('update\n')
		f.write(":: y"+str(prev)+" or x"+str(x)+" ~> x"+str(x)+"' := true, y"+str(x)+"':= y"+str(x)+";\n")
		f.write(":: y"+str(prev)+" or x"+str(x)+" ~> x"+str(x)+"' := false, y"+str(x)+"':= true;\n")
		f.write(":: y"+str(x)+" ~> x"+str(x)+"' := x"+str(x)+", y"+str(x)+"':= false;\n")
		
		f.write('\n');

	f.write('goals\n')
	for x in range (0,int(sys.argv[1])):
		f.write('#SL (G F !(x'+str(x)+'))\n')
if __name__ == '__main__':
    main()
