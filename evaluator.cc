/*
evaluator.cc contains function:
1. formula_eval		: evaluates the truth of a formula
2. guard_checker	: checks truth of a guard, true -> enabled
3. check_locality	: checks whether a variable in guard is local w.r.t the module
4. find_conac		: finds a corresponding action from "normal" agent w.r.t a variable

*/

#include "heading.h"
extern vector<string> rpn;
extern  stack<bool> bvr;
extern vector<string> rpn_ac;
extern struct env_ag env_ag;
extern vector<agent> ag;

bool formula_eval(vector<string>& rpn)
{
  bool v1, v2;

  while(!rpn.empty())
   {
      if(rpn.back()!="and" && rpn.back()!="or" && rpn.back()!="implies" && rpn.back()!="iff" && rpn.back()!="not")
	{
	  if(rpn.back()=="true")
	  bvr.push(true);
	  else
	  bvr.push(false);
	  rpn.pop_back();
	}
      else if(rpn.back()=="not")
	{
	  v1 = bvr.top(); bvr.pop();
	  bvr.push(!v1);
	  rpn.pop_back();
	}
      else
	{
	  v1 = bvr.top(); bvr.pop();
	  v2 = bvr.top(); bvr.pop();	
	  if(rpn.back()=="and")
	   {
	      bvr.push(v1 && v2);
	   }
	  else if(rpn.back()=="or")
	      bvr.push(v1 || v2);
	  else if(rpn.back()=="implies")
	      bvr.push(v1 || !v2);
	  else
	      bvr.push((!v1 || v2) && (!v2 || v1));
	  
	  rpn.pop_back();
	 }
   }
	v1 = bvr.top(); bvr.pop();
	return v1;
}

void guard_checker(vector<agent>& ag, int ag_count, int var_count)
{
  bool z,as_check;
  for(unsigned int i=0;i<ag[ag_count].agPro.size();i++)
   { 
     for (unsigned int j=0;j<ag[ag_count].ag_up.size()-1;j++)
       {
         vector<string> vnext_t, vnext_f;
	 rpn.clear();rpn_ac.clear();
	 for(unsigned int k=0;k<ag[ag_count].ag_up[j].guard_formula.size();k++)
	  {
	     if(find(ag[ag_count].agPro[i].t_pro.begin(),ag[ag_count].agPro[i].t_pro.end(), ag[ag_count].ag_up[j].guard_formula[k])!=ag[ag_count].agPro[i].t_pro.end())
		{
		  rpn.push_back("true");
		}
	     else if(find(ag[ag_count].agPro[i].f_pro.begin(),ag[ag_count].agPro[i].f_pro.end(), ag[ag_count].ag_up[j].guard_formula[k])!=ag[ag_count].agPro[i].f_pro.end())
		{
		  rpn.push_back("false");
		}
	     else
		{
                  if(ag[ag_count].ag_up[j].guard_formula[k]=="and" || ag[ag_count].ag_up[j].guard_formula[k]=="or" || ag[ag_count].ag_up[j].guard_formula[k]=="implies" || ag[ag_count].ag_up[j].guard_formula[k]=="iff" || ag[ag_count].ag_up[j].guard_formula[k]=="not" )
		  rpn.push_back(ag[ag_count].ag_up[j].guard_formula[k]);
                  else
                  rpn.push_back("true");
		}
	   }

	reverse(rpn.begin(),rpn.end());
	z =  formula_eval(rpn);
	if(z)
	{
	for(unsigned int r=0; r<ag[ag_count].ag_up[j].act_formula.size()-1;r++)
	{
	  for(unsigned int s=0;s<ag[ag_count].ag_up[j].act_formula[r].assign_formula.size();s++)
	    {
	      if(find(ag[ag_count].agPro[i].t_pro.begin(),ag[ag_count].agPro[i].t_pro.end(), ag[ag_count].ag_up[j].act_formula[r].assign_formula[s])!=ag[ag_count].agPro[i].t_pro.end())
		{
		  rpn_ac.push_back("true");
		}
	     else if(find(ag[ag_count].agPro[i].f_pro.begin(),ag[ag_count].agPro[i].f_pro.end(), ag[ag_count].ag_up[j].act_formula[r].assign_formula[s])!=ag[ag_count].agPro[i].f_pro.end())
		{
		  rpn_ac.push_back("false");
		}
	     else
		{
		  rpn_ac.push_back(ag[ag_count].ag_up[j].act_formula[r].assign_formula[s]);
		}

	    }
	reverse(rpn_ac.begin(),rpn_ac.end());
	 as_check = formula_eval(rpn_ac);
	 if(as_check)
	   vnext_t.push_back(ag[ag_count].ag_up[j].act_formula[r].next_var);
	 else
	   vnext_f.push_back(ag[ag_count].ag_up[j].act_formula[r].next_var);
	}
	
	for(unsigned int g=0;g<ag[ag_count].ac.size();g++)
	  {
	    unsigned int counter = 0;
	    for(unsigned int l=0;l<vnext_t.size();l++)
	     {
		if(find(ag[ag_count].ac[g].t_ac.begin(),ag[ag_count].ac[g].t_ac.end(),vnext_t[l])!=ag[ag_count].ac[g].t_ac.end())
		  {
		    counter++;
		  }
	     }

	    for(unsigned int l=0;l<vnext_f.size();l++)
	     {
		if(find(ag[ag_count].ac[g].f_ac.begin(),ag[ag_count].ac[g].f_ac.end(),vnext_f[l])!=ag[ag_count].ac[g].f_ac.end())
		  {
		    counter++;
		  }
	     }
	   
	    if(counter==(vnext_t.size()+vnext_f.size()))
	      {
	   if(!(find(ag[ag_count].agPro[i].ac_pro.begin(),ag[ag_count].agPro[i].ac_pro.end(),ag[ag_count].ac[g].acName)!=ag[ag_count].agPro[i].ac_pro.end()))
	    ag[ag_count].agPro[i].ac_pro.push_back(ag[ag_count].ac[g].acName);
	     }
	  }

	 }
      
       }	

   }
}

int check_locality(vector<agent>& ag, int ag_count)
{

  for(unsigned int i=0;i<ag[ag_count].ag_up.size()-1;i++)
  {
   for(unsigned int j=0;j<ag[ag_count].ag_up[i].guard_formula.size();j++)
    {

      if(ag[ag_count].ag_up[i].guard_formula[j]!="and" && ag[ag_count].ag_up[i].guard_formula[j]!="or" && ag[ag_count].ag_up[i].guard_formula[j]!="not" && ag[ag_count].ag_up[i].guard_formula[j]!="implies" && ag[ag_count].ag_up[i].guard_formula[j]!="iff" && ag[ag_count].ag_up[i].guard_formula[j]!="true" && ag[ag_count].ag_up[i].guard_formula[j]!="false")
       {
         if(!(find(ag[ag_count].varpro.begin(),ag[ag_count].varpro.end(),ag[ag_count].ag_up[i].guard_formula[j])!=ag[ag_count].varpro.end()))
           {
             ag[ag_count].varpro.push_back(ag[ag_count].ag_up[i].guard_formula[j]);

             if(!(find(env_ag.obs_idx.begin(),env_ag.obs_idx.end(),ag[ag_count].ag_up[i].guard_formula[j])!=env_ag.obs_idx.end()))
               {
                  
                  env_ag.obs_idx.push_back(ag[ag_count].ag_up[i].guard_formula[j]);
                  env_ag.seed.push_back(env_evo());
                  env_ag.seed[env_ag.seed.size()-1].obsvar = ag[ag_count].ag_up[i].guard_formula[j];
                 
               }

           }
       }
    }

   for(unsigned int q=0;q<ag[ag_count].ag_up[i].act_formula.size();q++)
   {
     for(unsigned int w=0;w<ag[ag_count].ag_up[i].act_formula[q].assign_formula.size();w++)
     {
       if(ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="and" && ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="or" && ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="not" && ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="if" && ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="iff" && ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="true" && ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]!="false")
       {

         if(!(find(ag[ag_count].varpro.begin(),ag[ag_count].varpro.end(),ag[ag_count].ag_up[i].act_formula[q].assign_formula[w])!=ag[ag_count].varpro.end()))
           {
           
             //add observable var to varpro
             ag[ag_count].varpro.push_back(ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]);

             if(!(find(env_ag.obs_idx.begin(),env_ag.obs_idx.end(),ag[ag_count].ag_up[i].act_formula[q].assign_formula[w])!=env_ag.obs_idx.end()))
               {
                  
                  env_ag.obs_idx.push_back(ag[ag_count].ag_up[i].act_formula[q].assign_formula[w]);
                  env_ag.seed.push_back(env_evo());
                  env_ag.seed[env_ag.seed.size()-1].obsvar = ag[ag_count].ag_up[i].act_formula[q].assign_formula[w];
                 
               }

           }
       }
     }
   }

  }
 return 0;
}

//adding controlling actions in Environment agent
void find_conac()
{
  for(unsigned int i=0;i<env_ag.seed.size();i++)
  {
    //for(unsigned int j=0;env_ag.seed[i].obsvar.size();j++)
    {
      for(unsigned int k=0;k<ag.size();k++)
      {
        if((find(ag[k].varName.begin(),ag[k].varName.end(),env_ag.seed[i].obsvar)!=ag[k].varName.end()))
        {
          env_ag.seed[i].con_agent = ag[k].agName;

          for(unsigned int l=0;l<ag[k].ac.size();l++)
           {
             if(find(ag[k].ac[l].t_ac.begin(),ag[k].ac[l].t_ac.end(),env_ag.seed[i].obsvar)!=ag[k].ac[l].t_ac.end())
             {
               env_ag.seed[i].t_conac.push_back(ag[k].ac[l].acName);
             }
             else
             {
               env_ag.seed[i].f_conac.push_back(ag[k].ac[l].acName);
             }
           }
        }
      }
    }
  }
}
