/*
enum_com.cc contains function:
1. bin_enum	: binary enumerator, enumerating/counting in form of binary
2. com_pro	: enumerates all possible combination for protocol
3. com		: enumerates all possible combination for action


*/

#include "heading.h"

unsigned int pro_count=0, pc_ac=0;
string ac_temp;

//binary enumerator
string bin_enum(unsigned int n, unsigned int bitcount)
{
 string bin_str="";
 while (n) 
   {
    if (n & 1)
     {
        bin_str.append("1");
     }
    else
     {
        bin_str.append("0");
     }
    bitcount--;
    n >>= 1;
   }

 for(unsigned int i=bitcount;i>0;i--)
 {
   bin_str.append("0");
 }
 reverse(bin_str.begin(),bin_str.end());
 return bin_str;
}

//combination for protocol
void combinationUtil_pro(int arr[], int data[], int start, int end, 
                     int index, int r, vector<agent>& ag, int ag_count, int pro_count);
 
void printCombination_pro(int arr[], int n, int r, vector<agent>& ag, int ag_count, int pro_count)
{
    //int data[r];
    int* data = new int[r];
    combinationUtil_pro(arr, data, 0, n-1, 0, r, ag, ag_count, pro_count);
}
 
void combinationUtil_pro(int arr[], int data[], int start, int end,
                     int index, int r, vector<agent>& ag, int ag_count, int pro_count)
{
    if (index == r)
    {
	ag[ag_count].agPro.push_back(protocol());
        //ag[ag_count].ac.push_back(action());
	//ac_temp = static_cast<ostringstream*>( &(ostringstream() << ag[ag_count].ac.size()-1) )->str(); 
	//ag[ag_count].ac[ag[ag_count].ac.size()-1].acName = "ac" + ac_temp;
	pro_count++;
        for (int j=0; j<r; j++)
           {
	   ag[ag_count].agPro[ag[ag_count].agPro.size()-1].f_pro.push_back(ag[ag_count].varpro[data[j]]);

	   //ag[ag_count].ac[ag[ag_count].ac.size()-1].f_ac.push_back(ag[ag_count].varName[data[j]]);
           }
        return;
    }

    for (int i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combinationUtil_pro(arr, data, i+1, end, index+1, r, ag, ag_count, pro_count);
    }
}

int com_pro(vector<agent>& ag, int var_count, int ag_count)
{
    int arr[1000];
    unsigned int j,flag;
    ag[ag_count].agPro.push_back(protocol());
    //ag[ag_count].ac.push_back(action());
    //ag[ag_count].ac[0].acName = "ac0";
    for(unsigned int i=0;i<ag[ag_count].varpro.size();i++)
    {
	arr[i]=i;

    //set for all var true
    ag[ag_count].agPro[0].t_pro.push_back(ag[ag_count].varpro[i]);
    //ag[ag_count].ac[0].t_ac.push_back(ag[ag_count].varName[i]);
    }
	
    for(unsigned int i=0;i<ag[ag_count].varpro.size();i++)
	{
      int r = i+1;
      int n = ag[ag_count].varpro.size();
      printCombination_pro(arr, n, r,ag, ag_count,pro_count);
	}
    for(unsigned int k=1;k<ag[ag_count].agPro.size();k++){
	
     for(unsigned int i=0;i<(ag[ag_count].varpro.size());i++)
	 {
	   flag=0;
           for(j=0;j<(ag[ag_count].agPro[k].f_pro.size());j++)
             if(ag[ag_count].varpro[i].compare(ag[ag_count].agPro[k].f_pro[j])==0)
		{
		flag=1;
                break;
		}
           if(flag==0)
	     {
	     ag[ag_count].agPro[k].t_pro.push_back(ag[ag_count].varpro[i]);
	     //ag[ag_count].ac[k].t_ac.push_back(ag[ag_count].varName[i]);
             }
         }
	}

    return 0;
}


void combinationUtil(int arr[], int data[], int start, int end, 
                     int index, int r, vector<agent>& ag, int ag_count, int pc_ac);
 
void printCombination(int arr[], int n, int r, vector<agent>& ag, int ag_count, int pc_ac)
{
    //int data[r];
    int* data = new int[r];
    combinationUtil(arr, data, 0, n-1, 0, r, ag, ag_count, pc_ac);
}
 
void combinationUtil(int arr[], int data[], int start, int end,
                     int index, int r, vector<agent>& ag, int ag_count, int pc_ac)
{
    if (index == r)
    {
	//ag[ag_count].agPro.push_back(protocol());
        ag[ag_count].ac.push_back(action());
	ac_temp = static_cast<ostringstream*>( &(ostringstream() << ag[ag_count].ac.size()-1) )->str(); 
	ag[ag_count].ac[ag[ag_count].ac.size()-1].acName = "ac" + ac_temp;
	pc_ac++;
        for (int j=0; j<r; j++)
           {
	   //ag[ag_count].agPro[ag[ag_count].agPro.size()-1].f_pro.push_back(ag[ag_count].varName[data[j]]);

	   ag[ag_count].ac[ag[ag_count].ac.size()-1].f_ac.push_back(ag[ag_count].varName[data[j]]);
           }
        return;
    }

    for (int i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combinationUtil(arr, data, i+1, end, index+1, r, ag, ag_count, pc_ac);
    }
}

int com(vector<agent>& ag, int var_count, int ag_count)
{
    int arr[1000];
    unsigned int j,flag;
    //ag[ag_count].agPro.push_back(protocol());
    ag[ag_count].ac.push_back(action());
    ag[ag_count].ac[0].acName = "ac0";
    for(int i=0;i<var_count;i++)
    {
	arr[i]=i;

    //set for all var true
    //ag[ag_count].agPro[0].t_pro.push_back(ag[ag_count].varName[i]);
    ag[ag_count].ac[0].t_ac.push_back(ag[ag_count].varName[i]);
    }
	
    for(int i=0;i<var_count;i++)
	{
      int r = i+1;
      //int n = sizeof(arr)/sizeof(arr[0]);
      int n = var_count;
      printCombination(arr, n, r,ag, ag_count,pc_ac);
	}
    for(unsigned int k=1;k<ag[ag_count].ac.size();k++){
	
     for(unsigned int i=0;i<(ag[ag_count].varName.size());i++)
	 {
	   flag=0;
           for(j=0;j<(ag[ag_count].ac[k].f_ac.size());j++)
             if(ag[ag_count].varName[i].compare(ag[ag_count].ac[k].f_ac[j])==0)
		{
		flag=1;
                break;
		}
           if(flag==0)
	     {
	     //ag[ag_count].agPro[k].t_pro.push_back(ag[ag_count].varName[i]);
	     ag[ag_count].ac[k].t_ac.push_back(ag[ag_count].varName[i]);
             }
         }
	}
    return 0;
}
