
# SEVIA
## SRML Games Equilibrium Verification via ISPL Analysis

This tool takes SRML file as an input and convert it to ISPL.
It then runs MCMAS to check NE-Emptiness of the setting.
Currently supporting memoryless fragment of SL.


************
### Instructions

#### Linux:
- Copy the folder of SEVIA into MCMAS working directory.
- Compile it by executing `make` command
- Run SEVIA by typing: 
	`your_pc_name[.../sevia]$ ./sevia name_of_the_input_file`

#### MacOS:
- Copy the (extracted) folder of SEVIA into MCMAS working directory.
- Use `Makefile_macos` OR `Makefile_macosx` as the Makefile (rename it into Makefile)
- Compile it by executing `make` command
- Run SEVIA by:
	- For RMG
     		`your_pc_name[.../sevia]$ ./sevia -r name_of_the_input_file.rml`
	- For CGS
		`your_pc_name[.../sevia]$ ./sevia -c name_of_the_input_file.rml`

For verbose mode use `-v` option:
     	- For RMG
     		`your_pc_name[.../sevia]$ ./sevia -r -v name_of_the_input_file.rml`
	- For CGS
		`your_pc_name[.../sevia]$ ./sevia -c -v name_of_the_input_file.rml`

Makefile commands:
- `make` : for compiling
- `make clean` : for cleaning compiled files
- `make clean-out` : for cleaning output folder


**********************
Naming the output file

* The output file is available in folder `./output`

If you run
   `[yourpc@localhost sevia]$ ./sevia -[c/r] xyz.rml OR ./stic -[c/r] -v xyz.rml`
This will automatically name your generated ISPL file into `xyz.ispl`

If you run
   `[yourpc@localhost sevia]$ ./sevia -[c/r] xyz.rml abc.ispl OR ./sevia -[c/r] -v xyz.rml abc.ispl`
This will name your generated ISPL file into `abc.ispl`


************
Input Format

A valid input is a (SRML) file containing:

	`modules module_name controls var_1,...,var_i
  	  init
	  :: guard ~> var_1' := true/false ,..., var_i' := true/false;
	   . . .
	  :: guard ~> var_1' := true/false ,..., var_i' := true/false;
	  update
	  :: guard ~> var_1' := prop_formula ,..., var_i' := prop_formula;
	   . . .
	  :: guard ~> var_1' := prop_formula ,..., var_i' := prop_formula;
	  goals
	  :: LTL goal;`


***************
Formula Syntax

prop_formula syntax:
`S := var | true | false | !S | S and S | S or S | S -> S | S <-> S`

Module goals are represented as LTL formulae by the following syntax:
`Z := var | true | false | !Z | Z and Z | Z or Z | X Z | F Z | G Z | Z -> Z | Z <-> Z`


*******
Example

- This is matching pennies example
- alice (resp. bob) holds coin ca (resp. cb)
- They can flip their coins to either true or false

 `module alice controls ca
   init
   :: true ~> ca' := true;
   :: true ~> ca' := false;
   update
   :: true ~> ca' := true;
   :: true ~> ca' := false;
   goal
   :: X (ca <-> cb);
   
  module bob controls cb
   init
   :: true ~> cb' := false;
   :: true ~> cb' := true;
   update
   :: true ~> cb' := true;
   :: true ~> cb' := false;
   goal
   :: X !(ca <-> cb);`



`.../mcmas/sevia$ ./sevia -r examples/matching_pennies 

********************************************************
                        SEVIA          
  SRMLG Equilibrium Verification via ISPL Analysis        

*********************************************************
...
Compiling...

Translation finished
Output File: ./output.ispl
Compilation time (s): 0.000897
Running MCMAS on output file

Building reachable state space...
  Formula number 1: <<st_alice>> <<st_bob>> <<st_Env>> (alice, st_alice) (bob, st_bob) (Environment, st_Env) ((X (((! cb) || ca) && (cb || (! ca))) || [[alt_st_alice]] (alice, alt_st_alice) (! X (((! cb) || ca) && (cb || (! ca))))) && (X (! (((! cb) || ca) && (cb || (! ca)))) || [[alt_st_bob]] (bob, alt_st_bob) (! X (! (((! cb) || ca) && (cb || (! ca))))))), is FALSE in the model
execution time = 0.006
number of reachable states = 4
BDD memory in use = 8993824`


*******************
Running Experiments

1. Go to folder **eve-py/src/experiments**, there are 8 scripts:
	+ bisim_ne_emptiness.sh
	+ bisim_none_emptiness.sh
	+ gossip_protocol_emptiness.sh
	+ gossip_protocol_enash.sh
	+ gossip_protocol_anash.sh
	+ replica_control_emptiness.sh
	+ replica_control_enash.sh
	+ replica_control_anash.sh
2. Execute the script "experiment_name".sh using the command `./experiment_name.sh 8`
3. This will run the experiment "experiment_name" up until 8 steps.
4. The experiment results are reported in the generated file **exetime_experiment_name.txt** with the following respective values separated by semicolons: 
	+ compilation time,
	+ time for verifying the formula,
	+ number of reachable states,
	+ BDD memory size used in MCMAS engine,
	+ the compiled ISPL file size,
	+ whether there exists NE.

It will also generate textfile tcgen/totable.txt for table in latex.
For more detailed instruction about the python files, see tcgen/README.txt
