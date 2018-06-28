/* main.cc */

#include "heading.h"

// prototype of bison-generated parser function
int yyparse();
int print_global();
void find_conac();
void print_env();
string cmd;
FILE *coba;
FILE *comb;
string fname;
bool cgs_flag;

int main(int argc, char **argv)
{
  clock_t start = clock();
  bool verbose=false;
  //cout << argc <<endl;
  if ((argc > 2) && (argc<6))
  {
	//check if RMG or CGS (with environment module)
	if(strcmp(argv[1],"-r")==0)
		cgs_flag=false;
	else if(strcmp(argv[1],"-c")==0)
		cgs_flag=true;
	else
		{
			cout << argv[0] <<": Incorrect command format!\n" << endl;
    			exit(1);
		}

    if(strcmp(argv[2],"-v")==0)
	{
		verbose=true;
		if((freopen(argv[3], "r", stdin) == NULL))
		    {
		      cerr << argv[0] << ": File " << argv[3] << " cannot be opened.\n";
		      exit(1);
		    }
		    else if(argc==4)
		    {
		      fname = argv[3];
		      stringstream ss(fname);
		      string token;
		      while(getline(ss, token, '/'))
		      fname = token;
		      fname = fname.substr(0,(fname.length()-4));
		      fname = fname + ".ispl";
		    }
		    else
		    {
		      fname = argv[4];
		    }

	}
    else
	    {
		    if((freopen(argv[2], "r", stdin) == NULL))
		    {
		      cerr << argv[0] << ": File " << argv[2] << " cannot be opened.\n";
		      exit(1);
		    }
		    else if(argc==3)
		    {
		      fname = argv[2];
		      stringstream ss(fname);
		      string token;
		      while(getline(ss, token, '/'))
		      fname = token;
		      fname = fname.substr(0,(fname.length()-4));
		      fname = fname + ".ispl";
		    }
		    else
		    {
		      fname = argv[3];
		    }
	    }
    /*else
    {
	//fname = argv[3];
	cerr << "Incorrect command format!" << endl;
    }*/

  }
  else
  {
    cout << argv[0] <<": Incorrect command format!\n" << endl;
    exit(1);
  }
  
  coba = fopen("output.ispl","w");
/*  fprintf(coba,
	  "Agent Environment\n Vars:\n dummy: boolean;\n end Vars\nActions = {none};\n Protocol:\n dummy = true : {none};\n dummy = false : {none};\n end Protocol\n Evolution:\n dummy = true if dummy = true;\n end Evolution\n end Agent\n\n");
  //fprintf(coba, "cuk");
*/
  cout << "\n********************************************************" << endl;
  cout << "                        SEVIA          " << endl;
  cout << "  SRMLG Equilibrium Verification via ISPL Analysis        " << endl;
  cout << "\n*********************************************************" << endl;
  cout << "...\nCompiling...\n" << endl;
  yyparse();
  find_conac();
  print_global();
  fclose(coba);
  print_env();
  cout << "Translation finished\nOutput File: ./output.ispl" << endl;
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;	
  cout << "Compilation time (s): " << seconds << endl;
  cout << "Running MCMAS on output file\n" << endl;
  cmd = "mv ./output_merged.ispl ./output";
  rename("./output_merged.ispl",fname.c_str());
  system("rm -rf ./output.ispl");
  if(verbose==false)
  	cmd = ".././mcmas ./"+fname+" | grep 'FALSE\\|TRUE\\|execution\\|reachable\\|memory in use'";
  else
	cmd = ".././mcmas -v 4 ./"+fname;
  system(cmd.c_str());
  cmd = "mv ./"+fname+" ./output/";
  system(cmd.c_str());
  //cout << result << endl;

  return 0;
}
