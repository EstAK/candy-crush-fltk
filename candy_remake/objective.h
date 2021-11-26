#include<iostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

using namespace std;
#ifndef OBJECTIVE_H_
#define OBJECTIVE_H_


class Objective{
  int number_of_tries=0; //Vlad: there must be a max number of movements
  string poss_objectives[3]={"Break->x->candies","Break->x->candies->color","Break x->cubs of ice"}; // Vlad: More to be added later.
  string selected_obj;  //The obj that will be selected.
  int number_to_break=0;
  string condition; //If there is a condition (like candies need to be of a certain color or ice cubs or etc)
  Fl_Color color[6]={FL_RED,FL_BLUE,FL_YELLOW,FL_BLACK,FL_DARK_CYAN,FL_GREEN};
  bool obj_completed=false;
public:
  Objective();
  void setUp();
   
  void fruits_on(int);

  void inc_fruits();
  void dec_fruits();

  bool constant_check();

  void mv_done(int,int,Fl_Color);
  
   void objCompleted();
   
   int nr_breaks();
   int nr_tries();
};

#endif