************************
STIC
(SRML to ISPL Compiler)
v0.2.2

************************
This tool takes SRML file as an input and convert it to ISPL.
It then runs MCMAS to check NE-Emptiness of the setting.
Currently supporting memoryless fragment of SL.


************
Instructions
************
Linux:
-- Copy the folder of STIC into MCMAS working directory.
-- Compile it by executing 'make' command
-- Run STIC by typing: 
	your_pc_name[.../stic]$ ./stic name_of_the_input_file

MacOS:
-- Copy the (extracted) folder of STIC into MCMAS working directory.
-- Use Makefile_macos OR Makefile_macosx as the Makefile (rename it into Makefile)
-- Compile it by executing 'make' command
-- Run STIC by typing:
     your_pc_name[.../stic]$ ./stic name_of_the_input_file.rml

For verbose mode use -v option:
     your_pc_name[.../stic]$ ./stic -v name_of_the_input_file.rml

Makefile commands:
-- make : for compiling
-- make clean : for cleaning compiled files
-- make clean-out : for cleaning output folder


**********************
Naming the output file
**********************
* The output file is available in folder ./output

If you run
   [yourpc@localhost stic]$ ./stic xyz.rml OR ./stic -v xyz.rml
This will automatically name your generated ISPL file into xyz.ispl

If you run
   [yourpc@localhost stic]$ ./stic xyz.rml abc.ispl OR ./stic -v xyz.rml abc.ispl
This will name your generated ISPL file into abc.ispl


************
Input Format
************

A valid input is a (SRML) file containing:

	modules module_name controls var_1,...,var_i
  	  init
	  :: guard ~> var_1' := true/false ,..., var_i' := true/false;
	   . . .
	  :: guard ~> var_1' := true/false ,..., var_i' := true/false;
	  update
	  :: guard ~> var_1' := prop_formula ,..., var_i' := prop_formula;
	   . . .
	  :: guard ~> var_1' := prop_formula ,..., var_i' := prop_formula;

	goals
	  #SL module goal


***************
Formula Syntax
***************

prop_formula syntax:
S := var | true | false | !S | S and S | S or S | S -> S | S <-> S

Module goals are represented as LTL formulae by the following syntax:
Z := var | true | false | !Z | Z and Z | Z or Z | X Z | F Z | G Z | Z -> Z | Z <-> Z


*******
Example
*******

 module alice controls ca
   init
   :: true ~> ca' := true;
   :: true ~> ca' := false;
   update
   :: true ~> ca' := true;
   :: true ~> ca' := false;
 
  module bob controls cb
   init
   :: true ~> cb' := true;
   :: true ~> cb' := false;
   update
   :: true ~> cb' := true;
   :: true ~> cb' := false;

  module environment controls wa,wb,s0,s1,s2
   init
   :: true ~> wa' := false , wb' := false, s0' := true, s1' := false, s2':=false;
   update
   :: s0 and (ca <-> cb) ~> wa' := true, wb' := false, s0' := false, s1' := true, s2':=false;
   :: s0 and !(ca <-> cb) ~> wb' := true, wa' := false, s0' := false, s1' := false, s2':= true;
   :: s1 ~> wa' := true, wb' := false;
   :: s2 ~> wb' := true, wa' := false;

 goals
 #SL X wa
 #SL X wb
 #SL true



[yourpc@localhost stic-v0.2]$ ./stic examples/match_pen.rml 

******************************************************
                      STIC          
    SRML to ISPL Compiler & Equilibrium Analyser        
                     v0.2.2          

******************************************************
...
Compiling...

Translation finished
Output File: ./output.ispl
Running MCMAS on output file

Building reachable state space...
  Formula number 1: <<st_alice>> <<st_bob>> <<st_Env>> (alice, st_alice) (bob, st_bob) (Environment, st_Env) ((X wa || [[alt_st_alice]] (alice, alt_st_alice) (! X wa)) && (X wb || [[alt_st_bob]] (bob, alt_st_bob) (! X wb))), is FALSE in the model
execution time = 0.004
number of reachable states = 12

*******************
Running Experiments
*******************
prerequisites: python, matplotlib
from inside folder stic-v02.2, execute: 
1. "./exp.sh p2p n" for peer-to-peer experiment with n number of players
2. "./exp.sh ring-based n" for peer-to-peer experiment with n number of players

Each will call python files in tcgen to generate test cases and plots of 6 indicators: 
1. compilation time,
2. size of the \textsc{Sl} formula,
3. time for verifying the formula,
4. number of reachable states,
5. BDD memory size used in MCMAS engine, and
6. the compiled ISPL file size.

It will also generate textfile tcgen/totable.txt for table in latex.
For more detailed instruction about the python files, see tcgen/README.txt


********************************
REMARKS - RELEASE NOTES (v0.2.2)
********************************
- Currently only supports true/false assignment in "environment" update guarded command actions.
- Currently variable assignments in action section for "update" guarded commands needs to be complete/exhaustive for all local/controlled variables.
- Implements "verbose".
- Add bash script exp.sh, for experiments.
