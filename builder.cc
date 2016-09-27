/*
builder.cc contaions function:
1. build_ltl		: builds LTL goal for each agent
2. evoEnv_builder	: builds Environment evolution

*/

#include "heading.h"

extern vector<string> ltl_goal;
extern stack<string> ltl_stack;
extern struct env_ag env_ag;
stack<string>rpn_buffer;
vector<string> rpn;

//LTL goal formula builder
string build_ltl(vector <string>& ltl_goal)
{
 
 string v1, v2;
 while(!ltl_goal.empty())
 {
   if(ltl_goal.back()!="and" && ltl_goal.back()!="or" && ltl_goal.back()!="implies" && ltl_goal.back()!="iff" && ltl_goal.back()!="not" && ltl_goal.back()!="G" && ltl_goal.back()!="F" && ltl_goal.back()!="X" && ltl_goal.back()!="U")
	{
	  if(ltl_goal.back()=="true")
	  ltl_stack.push("T");
	  else if(ltl_goal.back()=="false")
	  ltl_stack.push("! T");
	  else
          ltl_stack.push(ltl_goal.back());
	  ltl_goal.pop_back();
	}
      else if(ltl_goal.back()=="not")
	{
	  v1 = ltl_stack.top(); ltl_stack.pop();
	  v1 = "! " + v1;
	  ltl_stack.push(v1);
	  ltl_goal.pop_back();
	}
      else if(ltl_goal.back()=="G")
        {
          v1 = ltl_stack.top(); ltl_stack.pop();
          v1 = "G " + v1;
	  ltl_stack.push(v1);
	  ltl_goal.pop_back();
        }
      else if(ltl_goal.back()=="F")
        {
          v1 = ltl_stack.top(); ltl_stack.pop();
          v1 = "F " + v1;
	  ltl_stack.push(v1);
	  ltl_goal.pop_back();
        }
      else if(ltl_goal.back()=="X")
        {
          v1 = ltl_stack.top(); ltl_stack.pop();
          v1 = "X " + v1;
	  ltl_stack.push(v1);
	  ltl_goal.pop_back();
        }
      else if(ltl_goal.back()=="U")
        {
          v1 = ltl_stack.top(); ltl_stack.pop();
          v1 = "U " + v1;
	  ltl_stack.push(v1);
	  ltl_goal.pop_back();
        }
      else
	{
	  v1 = ltl_stack.top(); ltl_stack.pop();
	  v2 = ltl_stack.top(); ltl_stack.pop();	
	  if(ltl_goal.back()=="and")
	   {
              v1 = "(" + v1 + " and " + v2 + ")";
	      ltl_stack.push(v1);
	   }
	  else if(ltl_goal.back()=="or")
           {
              v1 = "(" + v1 + " or " + v2 + ")";
	      ltl_stack.push(v1);
           }
	  else if(ltl_goal.back()=="implies")
           {
              v1 = v1 + " or !" + v2;
	      ltl_stack.push(v1);
           }
	  else
           {
              v1 = "((!" + v1 + " or " + v2 + ") and (" + v1 + " or !" + v2 + "))";
	      ltl_stack.push(v1);
           }
	  
	  ltl_goal.pop_back();
	 }
 }
 v1 = ltl_stack.top(); ltl_stack.pop();
 return v1;
}

string evoEnv_builder(vector<agent>& ag, unsigned int ag_count,unsigned int var_count)
 {
   //cout<< "RPN size: " << rpn.size() << endl;
   reverse(rpn.begin(),rpn.end());
   string v1,v2;
   vector<string> action_list;
   for(unsigned int i = 0; i < env_ag.nat_obsvar.size();i++)
   //cout << "==>" << env_ag.nat_obsvar[i] << "<==" << endl;
   //v1 = "";
   //v2 = "";
   while(!rpn.empty())
   {
      if(rpn.back()!="and" && rpn.back()!="or" && rpn.back()!="implies" && rpn.back()!="iff" && rpn.back()!="not")
	{
	  //cout << "nat_obsvar size: " << env_ag.nat_obsvar.size() << endl;          
	  if(find(env_ag.nat_obsvar.begin(),env_ag.nat_obsvar.end(),rpn.back())!=env_ag.nat_obsvar.end())
	    {
              //cout << ">>" << rpn.back() << endl;
              v1 = rpn.back() + "=true";
	      rpn_buffer.push(v1);
              rpn.pop_back();
	    }
	  else
            {
              //find from ag_up
	      for(unsigned int i=0;i<ag_count;i++)
              {
	         if(find(ag[i].varName.begin(),ag[i].varName.end(),rpn.back())!=ag[i].varName.end())
                 {
                    for(unsigned int j=0;j<ag[i].ac.size();j++)
		      {
			//cout << ag[i].ac[j].t_ac.size() << endl;
			if(find(ag[i].ac[j].t_ac.begin(),ag[i].ac[j].t_ac.end(),rpn.back())!=ag[i].ac[j].t_ac.end())
                          {
			     //cout << ag[i].agName << " > " << ag[i].ac[j].acName << " | " << rpn.back() << endl;
                             action_list.push_back(ag[i].ac[j].acName);
                          }
		      }                 
	         for(unsigned int j=0;j<action_list.size();j++)
              	  {
		     if(j!=0)
			{
		     	    v1 = v1 + " or " + ag[i].agName + ".Action=" + action_list[j];
			}		     
		     else
			{ 		     
			    v1 = ag[i].agName + ".Action=" + action_list[j];
			}
              	  }
                  //cout << v1 << endl;
                  break;
		 }
              }              
            //  cout << "action_list: " << action_list.size() << endl;
              v1 = "(" + v1 + ")";

              action_list.clear();
	      //sementara itu
              //v1 = "pl." + rpn.back() + "=true";
	      rpn_buffer.push(v1);
              rpn.pop_back(); 
            }
	  //bvr.push(false);
	  //rpn.pop_back();
	}
      else if(rpn.back()=="not")
	{
	  v1 = "!(" + rpn_buffer.top() + ")"; rpn_buffer.pop();
	  rpn_buffer.push(v1);
	  rpn.pop_back();
	}
      else
	{
	  v1 = rpn_buffer.top(); rpn_buffer.pop();
	  v2 = rpn_buffer.top(); rpn_buffer.pop();	
	  if(rpn.back()=="and")
	   {
              v1 = "(" + v1 + " and " + v2 + ")";
	      rpn_buffer.push(v1);
	   }
	  else if(rpn.back()=="or")
           {
	      v1 = "(" + v1 + " or " + v2 + ")";
	      rpn_buffer.push(v1);
           }
	  else if(rpn.back()=="implies")
           {
	      v1 = "(" + v1 + " or !(" + v2 + "))";
	      rpn_buffer.push(v1);
           }
	  else
           {
	      v1 = "((!(" + v1 + ") or " + v2 + ") and (!(" + v2 + ") or (" + v1 + ")))";
	      rpn_buffer.push(v1);
	   }
	  rpn.pop_back();
	 }
   }

  for(unsigned int i=0;i<rpn_buffer.size();i++)
   {
       //cout << ">>" << rpn_buffer.top() << endl;
       //env_ag.evolution.push_back(rpn_buffer.top());
       //env_ag.arena_evo.push_back(st_arena_evo());
       v1 = rpn_buffer.top();    
       //env_ag.arena_evo[i].evolution = rpn_buffer.top();
       //strcpy(env_ag.arena_evo[i].evolution,v1);
       //v1.copy(env_ag.arena_evo[i].evolution,v1.begin(),v1.end());
       rpn_buffer.pop();
   }
   //for(unsigned int j=0;j<env_ag.arena_evo.size();j++)
 //for(unsigned int i=0;i<env_ag.evolution.size();i++)
   {
//   fprintf(comb,"%s;\n",env_ag.evolution[i].c_str());
     //cout << "arena evo: " << env_ag.arena_evo[0].evolution << endl;
     //fprintf(comb,"%s;\n",env_ag.arena_evo[j].evolution.c_str());
   }
   /*for(unsigned int i=0;i<rpn_buffer.size();i++)
   {
       cout << ">>" << rpn_buffer.top() << endl;
       rpn_buffer.pop();
   }*/
   return v1;
 }

