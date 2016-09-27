/*
inserter.cc contains function:
1. insert_act		: inserts action into protocol
2. insert_obsvar	: inserts variable into obsvars in Environment agent
3. insert_skip		: implements "skip"
*/

#include "heading.h"

extern vector<agent> ag;
extern struct env_ag env_ag;
extern vector<string> rpn;

string evoEnv_builder(vector<agent>& ag, unsigned int ag_count,unsigned int var_count);

int insert_act(vector<agent>& ag, int ag_count)
 {
   for(unsigned int i=0;i<ag[ag_count].ac.size();i++)
     {
	if(ag[ag_count].ac[i].t_flag==1)
	  for(unsigned int j=0;j<ag[ag_count].agPro.size();j++)
	    {
		ag[ag_count].agPro[j].ac_pro.push_back(ag[ag_count].ac[i].acName);
	    }
     }
   return 0;
 }

int insert_obsvar(vector<agent>& ag, unsigned int ag_count,unsigned int var_count)
 {
    //cout << "ag size: " << ag.size();
    //cout << "t_ac: " << ag[1].ac[0].f_ac.size() << " -- " << ag[1].ac[0].acName;   
    
    for(unsigned int i=0;i<var_count;i++)
    {
       env_ag.nat_obsvar.push_back(ag[ag_count].varName[i]);
    }

   for(unsigned int i=0;i<ag[ag_count].ag_up.size()-1;i++)
  {
   /*for(unsigned int j=0;j<ag[ag_count].ag_up[i].guard_formula.size();j++)
    {
       cout << ag[ag_count].ag_up[i].guard_formula[j] << "\n";
       
    }
   cout << "\n";*/
    rpn = ag[ag_count].ag_up[i].guard_formula;
    env_ag.arena_evo.push_back(st_arena_evo());
    env_ag.arena_evo[i].evolution = evoEnv_builder(ag,ag_count,var_count);

    //construct the left part of Environment agent evolution
    for(unsigned j=0;j<ag[ag_count].ag_up[i].act_formula.size()-1;j++)
    {
	//cout << "next var: " << ag[ag_count].ag_up[i].act_formula[j].next_var << " = " << ag[ag_count].ag_up[i].act_formula[j].assign_formula[0] << endl;

	/****currently only accomodates false/true assignment****/
	
        if(j!=0)
        env_ag.arena_evo[i].assignment = env_ag.arena_evo[i].assignment + " and " + ag[ag_count].ag_up[i].act_formula[j].next_var + "=" + ag[ag_count].ag_up[i].act_formula[j].assign_formula[0];
	else
	env_ag.arena_evo[i].assignment = env_ag.arena_evo[i].assignment + ag[ag_count].ag_up[i].act_formula[j].next_var + "=" + ag[ag_count].ag_up[i].act_formula[j].assign_formula[0];

    }

  }
    
    return 0;
 }

string insert_skip(vector<agent>& ag, unsigned int ag_count,unsigned int var_count, unsigned int i)
{
	unsigned int v=0;
	string action;
	for(unsigned int j=0;j<ag[ag_count].agPro[i].t_pro.size();j++)	
	  {	
	     if(find(ag[ag_count].varName.begin(),ag[ag_count].varName.end(),ag[ag_count].agPro[i].t_pro[j])!=ag[ag_count].varName.end())
		v++;
	  }

	for(unsigned int j=0;j<ag[ag_count].agPro[i].f_pro.size();j++)	
	  {	
	     if(find(ag[ag_count].varName.begin(),ag[ag_count].varName.end(),ag[ag_count].agPro[i].f_pro[j])!=ag[ag_count].varName.end())
		v++;
	  }

	for(unsigned int g=0;g<ag[ag_count].ac.size();g++)
	  {
	    unsigned int counter = 0;
	    for(unsigned int l=0;l<ag[ag_count].agPro[i].t_pro.size();l++)
	     {
		if(find(ag[ag_count].ac[g].t_ac.begin(),ag[ag_count].ac[g].t_ac.end(),ag[ag_count].agPro[i].t_pro[l])!=ag[ag_count].ac[g].t_ac.end())
		  {
		    counter++;
		  }
	     }

	    for(unsigned int l=0;l<ag[ag_count].agPro[i].f_pro.size();l++)
	     {
		if(find(ag[ag_count].ac[g].f_ac.begin(),ag[ag_count].ac[g].f_ac.end(),ag[ag_count].agPro[i].f_pro[l])!=ag[ag_count].ac[g].f_ac.end())
		  {
		    counter++;
		  }
	     }
	   
	    if(counter==v)
	      {
		action = ag[ag_count].ac[g].acName;
	     }
	  }

	return action;

}
