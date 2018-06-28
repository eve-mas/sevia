/* SRML to ISPL compiler */
/* */

%{
#include "heading.h"
int yyerror(char *s);
int yylex(void);
extern FILE *coba;
extern FILE *comb;
extern vector<string> rpn;
extern bool cgs_flag;

 env_ag env_ag;
 goals goals;
 vector<agent> ag;
 string temp_ag;
 vector<string> temp_var;

 //ACTION STRUCT BUFFER
 vector<action> vec_ac_temp;

 //initstates struct buffer
 vector<init> initStates_buff;

 //protocol struct buffer
 vector<protocol> agPro_buff;

 //vector<update_act> rpn_ac;
 vector<string> rpn_ac;

//update struct buffer
 vector<update> update_buff;

//env evo buffer
 env_evo seed_buff;
 
 //LTL goal formulae
 vector<string> ltl_goal;
 stack<string> ltl_stack;

 vector<string> environment_vars;

 unsigned int ag_count=0;
 unsigned int var_count=0;
 unsigned int up_count=0;
 unsigned int init_count=0;
 unsigned int af_count=0;
 string init_temp;
 stack<bool> bvr;
 vector<string>obsvars_enum;

void print_env();
int print_ag(vector<agent>& ag, unsigned int ag_count,unsigned int var_count);
bool formula_eval(vector<string>& rpn);
void guard_checker(vector<agent>& ag, int ag_count, int var_count);
int check_locality(vector<agent>& ag, int ag_count);
void find_conac();
string build_ltl(vector <string>& ltl_goal);
string evoEnv_builder(vector<agent>& ag, unsigned int ag_count,unsigned int var_count);
int insert_act(vector<agent>& ag, int ag_count);
int insert_obsvar(vector<agent>& ag, unsigned int ag_count,unsigned int var_count);
 	
%}

%union{
  int		int_val;
  string        *op_val;
  char          *sval;
  bool		boolean;
}

%start	input 

%token <sval>         MODULE
%token <sval> ISI
%type <sval>          id_y
%type <sval>          var_y
%type <sval>          init_y
%type <sval>          update_y
%type <sval>          init_command
%type <sval>          update_command
%type <sval>          init_condition
%type <sval>          update_condition
%type <sval>          init_next_state
%type <sval>          update_next_state
%type <sval>	      formula_assign
%type <sval>	      formula_guard
%token <sval>       CONTROL
%token <sval> VAR
%token <sval> INIT
%token <sval> UPDATE
%token <sval> END
%token <sval> COMMA
%token <sval> DCOL
%token <sval> ASSIGN
%token <sval> TRUE
%token <sval> FALSE
%token <sval> EQUALS
%token <sval> NEXT
%token <sval> SEMICOLON
%token <sval> GOAL
%left <sval> OR
%left <sval> AND
%token <sval> IMPLIES
%token <sval> IFF
%token <sval> LB RB
%right <sval> NOT
%token <sval> LTL
%token <sval> G
%token <sval> F
%token <sval> X
%token <sval> U
%token <sval> ENVIRONMENT

%token <sval> SL
%type <sval> goal_form
%type <sval> gf
%%

input:	MODULE id_y CONTROL var_y INIT init_y UPDATE update_y GOAL goal_form {
  ag.push_back(agent());
  ag[ag_count].agName = temp_ag;

  unsigned int i;
  for(i=0;i<var_count;i++)
    {
      ag[ag_count].varName.push_back($2);
      ag[ag_count].varName[i]=temp_var[i];
      ag[ag_count].varpro.push_back(temp_var[i]);
    }
  //check locality of vars
  //check_locality(ag, ag_count);
  //com_pro(ag, var_count, ag_count);
  //com(ag, var_count, ag_count);
  ag[ag_count].initStates = initStates_buff;
  ag[ag_count].ag_up = update_buff;
  
  //print_ag(ag,ag_count,var_count);
  //cout << "A" << endl;
  //cout << cgs_flag << endl;
  if(!cgs_flag)
  {
	print_ag(ag,ag_count,var_count);
  for (unsigned int j=0; j<ag[ag_count].ag_up.size();j++)
  {
    reverse(ag[ag_count].ag_up[j].guard_formula.begin(),ag[ag_count].ag_up[j].guard_formula.end());
  }
  }
  
  vec_ac_temp.clear();
  initStates_buff.clear();
  update_buff.clear();
  ag_count++;
  init_count=0;
  var_count=0;
  //cout << "\nUPCOUNT: " << up_count;
  up_count=0;
 }
| MODULE id_y CONTROL var_y INIT init_y UPDATE update_y{
  ag.push_back(agent());
  ag[ag_count].agName = temp_ag;

  unsigned int i;
  for(i=0;i<var_count;i++)
    {
      ag[ag_count].varName.push_back($2);
      ag[ag_count].varName[i]=temp_var[i];
      environment_vars.push_back(temp_var[i]);
      ag[ag_count].varpro.push_back(temp_var[i]);
    }
  //com_pro(ag, var_count, ag_count);
  //com(ag, var_count, ag_count);
  //cout << "B" << endl;

  if(cgs_flag)
  for (unsigned int g=0; g<ag_count; g++)
	{
		print_ag(ag,g,ag[g].varName.size());
for (unsigned int j=0; j<ag[ag_count].ag_up.size();j++)
  {
    reverse(ag[ag_count].ag_up[j].guard_formula.begin(),ag[ag_count].ag_up[j].guard_formula.end());
  }
	}
  //cout << ag_count;
  //print_ag(ag,0,1);
  //print_ag(ag,1,1);
  ag[ag_count].initStates = initStates_buff;
  ag[ag_count].ag_up = update_buff;
  print_ag(ag,ag_count,var_count);
  for (unsigned int j=0; j<ag[ag_count].ag_up.size();j++)
  {
    reverse(ag[ag_count].ag_up[j].guard_formula.begin(),ag[ag_count].ag_up[j].guard_formula.end());
  }
  vec_ac_temp.clear();
  initStates_buff.clear();
  update_buff.clear();
  ag_count++;
  init_count=0;
  var_count=0;
  //cout << "\nUPCOUNT: " << up_count;
  up_count=0;
 }
| input input
;


var_y: ISI {
  temp_var.push_back($1);
  temp_var[var_count] = $1;
  var_count++;
  //cout << $1 << "\n";

 }
| var_y COMMA var_y {
 }
;

id_y:ISI {temp_ag = $1;}
;

init_y: DCOL init_command
| init_y DCOL init_command 
;

init_command: init_condition ASSIGN init_next_state 
;

update_command: update_condition ASSIGN update_next_state {
update_buff.push_back(update());
}
;

init_condition: TRUE {}
| FALSE
;

formula_guard: ISI {
if(update_buff.empty())
  update_buff.push_back(update());
update_buff[up_count].guard_formula.push_back($1);
}
| TRUE
{
 {
if(update_buff.empty())
update_buff.push_back(update());
  update_buff[up_count].guard_formula.push_back("true");
 }
}
| FALSE
{
if(update_buff.empty())
update_buff.push_back(update());
update_buff[up_count].guard_formula.push_back("false");
}
| formula_guard OR formula_guard { 
if(update_buff.empty())
update_buff.push_back(update()); update_buff[up_count].guard_formula.push_back("or");}
| formula_guard AND formula_guard { 
if(update_buff.empty())
update_buff.push_back(update());
update_buff[up_count].guard_formula.push_back("and");
}
| formula_guard IMPLIES formula_guard { 
if(update_buff.empty())
update_buff.push_back(update());
update_buff[up_count].guard_formula.push_back("implies");}
| formula_guard IFF formula_guard {
if(update_buff.empty())
update_buff.push_back(update());
update_buff[up_count].guard_formula.push_back("iff");}
| NOT formula_guard {
if(update_buff.empty())
update_buff.push_back(update());
update_buff[up_count].guard_formula.push_back("not");}
| LB formula_guard RB
;

formula_assign: ISI 
{
if(update_buff[up_count].act_formula.empty())
update_buff[up_count].act_formula.push_back(update_act());

update_buff[up_count].act_formula[af_count].assign_formula.push_back($1);
}
| TRUE
{
if(update_buff[up_count].act_formula.empty())
update_buff[up_count].act_formula.push_back(update_act());
update_buff[up_count].act_formula[af_count].assign_formula.push_back("true");
}
| FALSE
{
if(update_buff[up_count].act_formula.empty())
update_buff[up_count].act_formula.push_back(update_act());
update_buff[up_count].act_formula[af_count].assign_formula.push_back("false");
}
| formula_assign OR formula_assign
{
update_buff[up_count].act_formula[af_count].assign_formula.push_back("or");
}
| formula_assign AND formula_assign
{
update_buff[up_count].act_formula[af_count].assign_formula.push_back("and");
}
| formula_assign IMPLIES formula_assign
{
update_buff[up_count].act_formula[af_count].assign_formula.push_back("implies");
}
| formula_assign IFF formula_assign
{
update_buff[up_count].act_formula[af_count].assign_formula.push_back("iff");
}
| NOT formula_assign
{
update_buff[up_count].act_formula[af_count].assign_formula.push_back("not");
}
| LB formula_assign RB
;

update_condition:
formula_guard
| update_condition COMMA update_condition 
;


init_next_state: ISI NEXT EQUALS TRUE {
  if(initStates_buff.empty())
    initStates_buff.push_back(init());
  initStates_buff[init_count].t_in.push_back($1);
 }
| ISI NEXT EQUALS FALSE {
  if(initStates_buff.empty())
    initStates_buff.push_back(init());
  initStates_buff[init_count].f_in.push_back($1);
}
| init_next_state COMMA init_next_state
| init_next_state SEMICOLON{
  init_count++;
  initStates_buff.push_back(init());
}
;

update_next_state:
ISI NEXT EQUALS formula_assign
{
update_buff[up_count].act_formula.push_back(update_act());
update_buff[up_count].act_formula[af_count].next_var = $1;
af_count++;

}
| update_next_state COMMA update_next_state 
{
}
| update_next_state SEMICOLON{
 }
;


update_y: DCOL update_command {
up_count++;
af_count=0;
}
| update_y DCOL update_command {
up_count++;
af_count=0;
}
;

gf: 
ISI 
{
ltl_goal.push_back($1);
}
| TRUE
{
ltl_goal.push_back("true");
}
| FALSE
{
ltl_goal.push_back("false");
}
| gf OR gf
{
ltl_goal.push_back("or");
}
| gf AND gf
{
ltl_goal.push_back("and");
}
| gf IMPLIES gf
{
ltl_goal.push_back("implies");
}
| gf IFF gf
{
ltl_goal.push_back("iff");
}
| G gf
{
ltl_goal.push_back("G");
}
| F gf
{
ltl_goal.push_back("F");
}
| X gf
{
ltl_goal.push_back("X");
}
| U gf
{
ltl_goal.push_back("U");
}
| NOT gf
{
ltl_goal.push_back("not");
}
| LB gf RB
;

goal_form: DCOL gf SEMICOLON
{
reverse(ltl_goal.begin(),ltl_goal.end());
string res = build_ltl(ltl_goal);
goals.formulae.push_back(res);
ltl_goal.clear();
}
| goal_form SL gf
{
reverse(ltl_goal.begin(),ltl_goal.end());
string res = build_ltl(ltl_goal);
goals.formulae.push_back(res);
ltl_goal.clear();
}
;


%%

int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
}

int yyerror(char *s)
{
  return yyerror(string(s));
}


