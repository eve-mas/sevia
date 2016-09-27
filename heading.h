/* heading.h */
#pragma once
//#define YY_NO_UNPUT

using namespace std;

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <string.h>
#include <iomanip>
#include <locale>
#include <sstream>
#include <algorithm>
#include <stack>
#include <unistd.h>

struct goals
{
  vector<string> formulae;
};

struct action 
{
  string acName;
  int t_flag;
  int f_flag;
  vector<string> t_guard;
  vector<string> f_guard;
  vector<string> t_ac;
  vector<string> f_ac;
};

struct init
{
  vector<string> t_in;
  vector<string> f_in;
};

struct protocol
{
  vector<string> t_pro;
  vector<string> f_pro;
  vector<string> ac_pro;
};

struct update_act
{
  string next_var;
  vector<string> assign_formula;
};

struct update
{
  vector<string> guard_formula;
  vector<update_act> act_formula;
};

struct agent
{
  string agName;
  vector<string> varName;
  vector<string> varpro; //local var + global var
  vector<action> ac;
  vector<init> initStates;
  vector<protocol> agPro;
  vector<update> ag_up;
};

struct env_evo
{
  string obsvar;
  string con_agent;
  vector<string> t_conac;
  vector<string> f_conac;
};

struct st_arena_evo
{
  string evolution;
  string assignment;
};

struct env_ag
{
  vector<string> nat_obsvar;
  vector<string> obs_idx;
  vector<env_evo> seed;
  //vector<string> evolution;
  vector<st_arena_evo> arena_evo;
};
