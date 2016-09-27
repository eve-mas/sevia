/*
print.cc contains function for:
1. print_env	: prints Environment agent
2. print_global	: prints global setting (e.g. init states, evaluation)
3. print_ag	: prints "normal" agent

*/


#include "heading.h"
extern FILE *coba;
extern FILE *comb;
extern struct env_ag env_ag;
char ch;
extern vector<agent> ag;
extern struct goals goals;

string bin_enum(unsigned int n, unsigned int bitcount);
int insert_obsvar(vector<agent>& ag, unsigned int ag_count,unsigned int var_count);
int check_locality(vector<agent>& ag, int ag_count);
int com(vector<agent>& ag, int var_count, int ag_count);
int com_pro(vector<agent>& ag, int var_count, int ag_count);
void guard_checker(vector<agent>& ag, int ag_count, int var_count);
string insert_skip(vector<agent>& ag, unsigned int ag_count,unsigned int var_count, unsigned int i);


//print environment agent
void print_env()
{
 comb = fopen("output_merged.ispl","w");
 
 fprintf(comb,"Agent Environment\nObsvars:\nT : boolean;\n");
 for(unsigned int i=0;i<env_ag.seed.size();i++)
 {
   fprintf(comb,"e%s : boolean;\n",env_ag.seed[i].obsvar.c_str());
 }
 
 //print native obsvars
 for(unsigned int i=0;i<env_ag.nat_obsvar.size();i++)
   fprintf(comb,"%s : boolean;\n",env_ag.nat_obsvar[i].c_str());
 fprintf(comb,"end Obsvars\nActions = {none};\nProtocol:\nT = true : {none};\nT = false : {none};\nend Protocol\n");
 fprintf(comb,"Evolution:\n");
 /*for(unsigned int i=0;i<env_ag.seed.size();i++)
 {
   for(unsigned int j=0;j<env_ag.seed[i].t_conac.size();j++)
   fprintf(comb,"e%s = true if %s.Action = %s;\n",env_ag.seed[i].obsvar.c_str(),env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].t_conac[j].c_str());
  
   for(unsigned int j=0;j<env_ag.seed[i].f_conac.size();j++)
   fprintf(comb,"e%s = false if %s.Action = %s;\n",env_ag.seed[i].obsvar.c_str(),env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].f_conac[j].c_str());
 }*/
 if(env_ag.arena_evo.empty())
  {
     for(unsigned int n=0;n<(pow(2,env_ag.seed.size()));n++)
	 {

          //fprintf(comb,"%s and ",env_ag.arena_evo[j].assignment.c_str());

	  //call binary enum function
	  string enumerate = bin_enum(n,env_ag.seed.size());
	  for(unsigned int i=0;i<enumerate.size();i++)
	  {
	    if(i!=0)
	     fprintf(comb," and ");
	    if((enumerate.compare(i,1,"0"))==0)
	    {
	      fprintf(comb,"e%s = false", env_ag.seed[i].obsvar.c_str());
	    }
	    else
	    {
	      fprintf(comb,"e%s = true", env_ag.seed[i].obsvar.c_str());
	    }
	  }
	  
      if(enumerate.size()!=0)
	  fprintf(comb," if ");

	  for(unsigned int i=0;i<enumerate.size();i++)
	  {
	    if(i!=0)
	     fprintf(comb," and ");
	    if((enumerate.compare(i,1,"0"))==0)
	    {
	      fprintf(comb,"(");
	      for(unsigned int j=0;j<env_ag.seed[i].f_conac.size();j++)
	      {
		//fprintf(comb,"(");
		if(j!=0)
		  fprintf(comb," or ");
		fprintf(comb,"%s.Action = %s", env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].f_conac[j].c_str());
		//fprintf(comb,")");
	      }

	      //add action 'none"
	      //fprintf(comb," or %s.Action = none", env_ag.seed[i].con_agent.c_str());

	      fprintf(comb,")");
	    }
	    else
	    {
	      fprintf(comb,"(");
	      for(unsigned int j=0;j<env_ag.seed[i].t_conac.size();j++)
	      {
		//fprintf(comb,"(");
		if(j!=0)
		  fprintf(comb," or ");
		fprintf(comb,"%s.Action = %s", env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].t_conac[j].c_str());
		//fprintf(comb,")");
	      }

	      //add action 'none"
	      //fprintf(comb," or %s.Action = none", env_ag.seed[i].con_agent.c_str());
	     
	      fprintf(comb,")");
	    }

           //fprintf(comb,"and %s",env_ag.arena_evo[j].evolution.c_str());
	  }
	if(enumerate.size()!=0)
	 fprintf(comb,";\n");
	 }
  }
 else
 //straighforward evolution from ARENA
 for(unsigned int j=0;j<env_ag.arena_evo.size();j++)
 //for(unsigned int i=0;i<env_ag.evolution.size();i++)
 {
//   fprintf(comb,"%s;\n",env_ag.evolution[i].c_str());
   //cout << "arena evo: " << env_ag.arena_evo[j].evolution << endl;
   //fprintf(comb,"%s if %s;\n",env_ag.arena_evo[j].assignment.c_str(),env_ag.arena_evo[j].evolution.c_str());

//   cout << env_ag.arena_evo[j].assignment << endl;
 

	 if(env_ag.seed.empty())
	    fprintf(comb,"%s if %s;\n",env_ag.arena_evo[j].assignment.c_str(),env_ag.arena_evo[j].evolution.c_str());
         else
	 for(unsigned int n=0;n<(pow(2,env_ag.seed.size()));n++)
	 {

          fprintf(comb,"%s and ",env_ag.arena_evo[j].assignment.c_str());

	  //call binary enum function
	  string enumerate = bin_enum(n,env_ag.seed.size());
	  for(unsigned int i=0;i<enumerate.size();i++)
	  {
	    if(i!=0)
	     fprintf(comb," and ");
	    if((enumerate.compare(i,1,"0"))==0)
	    {
	      fprintf(comb,"e%s = false", env_ag.seed[i].obsvar.c_str());
	    }
	    else
	    {
	      fprintf(comb,"e%s = true", env_ag.seed[i].obsvar.c_str());
	    }
	  }
	  
	  fprintf(comb," if ");

	  for(unsigned int i=0;i<enumerate.size();i++)
	  {
	    if(i!=0)
	     fprintf(comb," and ");
	    if((enumerate.compare(i,1,"0"))==0)
	    {
	      fprintf(comb,"(");
	      for(unsigned int j=0;j<env_ag.seed[i].f_conac.size();j++)
	      {
		//fprintf(comb,"(");
		if(j!=0)
		  fprintf(comb," or ");
		fprintf(comb,"%s.Action = %s", env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].f_conac[j].c_str());
		//fprintf(comb,")");
	      }

	      //add action 'none"
	      //fprintf(comb," or %s.Action = none", env_ag.seed[i].con_agent.c_str());

	      fprintf(comb,")");
	    }
	    else
	    {
	      fprintf(comb,"(");
	      for(unsigned int j=0;j<env_ag.seed[i].t_conac.size();j++)
	      {
		//fprintf(comb,"(");
		if(j!=0)
		  fprintf(comb," or ");
		fprintf(comb,"%s.Action = %s", env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].t_conac[j].c_str());
		//fprintf(comb,")");
	      }

	      //add action 'none"
	      //fprintf(comb," or %s.Action = none", env_ag.seed[i].con_agent.c_str());
	     
	      fprintf(comb,")");
	    }

           fprintf(comb,"and %s",env_ag.arena_evo[j].evolution.c_str());
	  }
	 fprintf(comb,";\n");
	 }
 }
 fprintf(comb,"end Evolution\nend Agent\n\n");

 coba = fopen("output.ispl","r");

 //fprintf(comb,coba.c_str());
 while((ch = fgetc(coba))!=EOF)
   	fputc(ch, comb);

 fclose(comb);

}

int print_global()
 {
   unsigned int i,j,k;
   fprintf(coba,"Evaluation\n");
   fprintf(coba,"T if Environment.T=true;\n");
   for(i=0;i<ag.size();i++)
    {
     if(strcmp(ag[i].agName.c_str(),"environment") != 0)
      for(j=0;j<ag[i].varName.size();j++)
	{
          fprintf(coba,"%s if %s.%s=true;\n", ag[i].varName[j].c_str(), ag[i].agName.c_str(), ag[i].varName[j].c_str());
	}
     else
       for(j=0;j<ag[i].varName.size();j++)
	{
          fprintf(coba,"%s if Environment.%s=true;\n", ag[i].varName[j].c_str(), ag[i].varName[j].c_str());
	}
    }
   fprintf(coba,"end Evaluation	\n");
   fprintf(coba,"\nInitStates\n");
   for(i=0;i<ag.size();i++)
    {
     if(strcmp(ag[i].agName.c_str(),"environment") != 0)
     {
      if(i!=0)
        fprintf(coba," and\n");
      fprintf(coba,"(");
      for (j=0; j<(ag[i].initStates.size()-1); j++)
       {
         if(j!=0)
           fprintf(coba," or ");
         fprintf(coba,"(");
	 for(k=0;k<ag[i].initStates[j].t_in.size();k++)
          {
           if(k!=0)
            fprintf(coba," and ");
            fprintf(coba,"%s.%s=true",ag[i].agName.c_str(),ag[i].initStates[j].t_in[k].c_str());
          }
         for(k=0;k<ag[i].initStates[j].f_in.size();k++)
          {
           if((ag[i].initStates[j].t_in.size()!=0) || k!=0)
            fprintf(coba," and ");
            fprintf(coba,"%s.%s=false",ag[i].agName.c_str(),ag[i].initStates[j].f_in[k].c_str());
          }
         fprintf(coba,")");
       }
      fprintf(coba,")");
     }
     else
     {
      if(i!=0)
        fprintf(coba," and\n");
      fprintf(coba,"(");
      for (j=0; j<(ag[i].initStates.size()-1); j++)
       {
         if(j!=0)
           fprintf(coba," or ");
         fprintf(coba,"(");
	 for(k=0;k<ag[i].initStates[j].t_in.size();k++)
          {
           if(k!=0)
            fprintf(coba," and ");
            fprintf(coba,"Environment.%s=true",ag[i].initStates[j].t_in[k].c_str());
          }
         for(k=0;k<ag[i].initStates[j].f_in.size();k++)
          {
           if((ag[i].initStates[j].t_in.size()!=0) || k!=0)
            fprintf(coba," and ");
            fprintf(coba,"Environment.%s=false",ag[i].initStates[j].f_in[k].c_str());
          }
         fprintf(coba,")");
       }
      fprintf(coba,")");
     }
    }

   for(i=0;i<env_ag.seed.size();i++)
   {
     fprintf(coba," and\nEnvironment.e%s=%s.%s",env_ag.seed[i].obsvar.c_str(),env_ag.seed[i].con_agent.c_str(),env_ag.seed[i].obsvar.c_str());
   }

   fprintf(coba," and Environment.T=true;\nend InitStates\n");

   fprintf(coba,"\nFormulae\n");
     {
	for(j=0;j<ag.size();j++)
	  {
	    if(strcmp(ag[j].agName.c_str(),"environment") != 0)
	    fprintf(coba," <<st_%s>>",ag[j].agName.c_str());
	  }
	fprintf(coba," <<st_Env>>\n");
	for(j=0;j<ag.size();j++)
	  {
	    if(strcmp(ag[j].agName.c_str(),"environment") != 0)
	    fprintf(coba," (%s,st_%s)",ag[j].agName.c_str(),ag[j].agName.c_str());
	  }
	fprintf(coba," (Environment,st_Env)\n");
	fprintf(coba,"(\n");
	for(j=0;j<ag.size();j++)
	  {	
	    if(strcmp(ag[j].agName.c_str(),"environment") != 0)    
	    {
	      if(j!=0)
	        fprintf(coba," and\n");
	      fprintf(coba,"(");	
	      fprintf(coba,"%s",goals.formulae[j].substr(0,goals.formulae[j].size()).c_str());
	      fprintf(coba," or ");
	      fprintf(coba,"[[alt_st_%s]]",ag[j].agName.c_str());
	      fprintf(coba,"(%s,alt_st_%s)",ag[j].agName.c_str(),ag[j].agName.c_str());
	      fprintf(coba,"(!(%s))",goals.formulae[j].substr(0,goals.formulae[j].size()).c_str());	
	      fprintf(coba,")");
	    }
	}
	fprintf(coba,"\n)");
	fprintf(coba,";\n");
     }
   fprintf(coba,"end Formulae");
   return 1;

 }

int print_ag(vector<agent>& ag, unsigned int ag_count,unsigned int var_count)
 {
   if(strcmp(ag[ag_count].agName.c_str(),"environment") == 0)
   {
	   //cout << "ARENA\n";
           insert_obsvar(ag,ag_count,var_count);
   }
   else
   {
	   unsigned int i,j,ac_count;
	   fprintf(coba,"Agent %s\n",ag[ag_count].agName.c_str()); //print agent prefix
	   fprintf(coba,"Vars:\n");//print Vars prefix
	   for(i=0;i<var_count;i++)
	    {
	      fprintf(coba,"%s : boolean;\n",ag[ag_count].varName[i].c_str());//print all vars, bool assummed
	    }
	   fprintf(coba,"end Vars\n");//print Vars suffix
	   ac_count = exp2(var_count);//get the number of actions 
	   //ac_count = ag[ag_count].ag_up.size()-1; //no of udpate commands
	   fprintf(coba,"Actions = {");
	   for(i=0;i<ac_count;i++)
	     {
	       if(i!=0)
		 fprintf(coba,",");
	       fprintf(coba,"ac%d",i);
	     }
	   fprintf(coba,",none");
	   fprintf(coba,"};\n");

	   //check locality of vars
	   //if agent "arena", no need to check locality
	   if(strcmp(ag[ag_count].agName.c_str(),"environment") != 0)
	   check_locality(ag, ag_count);

	   com_pro(ag, var_count, ag_count);
	   com(ag, var_count, ag_count);

	   //insert_act(ag,ag_count);
	   guard_checker(ag, ag_count, var_count);

	   fprintf(coba,"Protocol:\n");//protocol prefix
		for(i=0;i<ag[ag_count].agPro.size();i++)
		  {
		    //if(!(ag[ag_count].agPro[i].ac_pro.empty()))
		    {
		    fprintf(coba,"(");
			for(j=0;j<ag[ag_count].agPro[i].t_pro.size();j++)
			  {
			     if(j!=0)
				fprintf(coba," and ");
		             if(find(ag[ag_count].varName.begin(),ag[ag_count].varName.end(),ag[ag_count].agPro[i].t_pro[j])!=ag[ag_count].varName.end())
			     fprintf(coba,"%s=true",ag[ag_count].agPro[i].t_pro[j].c_str());
		             else
		             fprintf(coba,"Environment.e%s=true",ag[ag_count].agPro[i].t_pro[j].c_str());
			  }
			for(j=0;j<ag[ag_count].agPro[i].f_pro.size();j++)
			  {
			     if((!(ag[ag_count].agPro[i].t_pro.empty())) || (j!=0))
			     fprintf(coba," and ");
		             if(find(ag[ag_count].varName.begin(),ag[ag_count].varName.end(),ag[ag_count].agPro[i].f_pro[j])!=ag[ag_count].varName.end())
			     fprintf(coba,"%s=false",ag[ag_count].agPro[i].f_pro[j].c_str());
		             else
		             fprintf(coba,"Environment.e%s=false",ag[ag_count].agPro[i].f_pro[j].c_str());
			  }
		    fprintf(coba,") : {");
			for(j=0;j<ag[ag_count].agPro[i].ac_pro.size();j++)
			  {
			     if(j!=0)
				fprintf(coba,",");
			     fprintf(coba,ag[ag_count].agPro[i].ac_pro[j].c_str());
			  }
			  if(ag[ag_count].agPro[i].ac_pro.empty())
			    {
			     // "skip" implementation
			     //cout << ">> " << insert_skip(ag, ag_count, var_count, i) << endl;
			     fprintf(coba,insert_skip(ag, ag_count, var_count, i).c_str());
			    }
		    fprintf(coba,"};\n");
		    }
		  }
	   fprintf(coba,"end Protocol\n");//protocol suffix
	   fprintf(coba,"Evolution:\n");//evolution prefix
	   for (i=0; i<(ag[ag_count].ac.size()); i++){
		fprintf(coba,"(");	
		for(j=0; j<ag[ag_count].ac[i].t_ac.size(); j++){
		  if(j!=0)
			fprintf(coba," and ");
		  fprintf(coba,"%s=true",ag[ag_count].ac[i].t_ac[j].c_str());
		}
		if(ag[ag_count].ac[i].t_ac.size() != 0)
		  if(i!=0)
		    fprintf(coba," and ");
		for(j=0; j<ag[ag_count].ac[i].f_ac.size(); j++){
		  if(j!=0)
			fprintf(coba," and ");
		  fprintf(coba,"%s=false",ag[ag_count].ac[i].f_ac[j].c_str());
		}
		fprintf(coba,") if Action=");
		fprintf(coba,"%s;\n",ag[ag_count].ac[i].acName.c_str());
	   }
	   
	   fprintf(coba,"end Evolution\n");//evolution suffix
	   fprintf(coba,"end Agent\n\n"); //print agent suffix
   }
   return 0;

 }
