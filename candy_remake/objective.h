#include<iostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

using namespace std;
#ifndef OBJECTIVEH
#define OBJECTIVEH


class Objective{
  int number_of_tries=0; //Vlad: there must be a max number of movements
  int number_to_break=0;
  string poss_objectives[2]={"Break->x->candies","Break->x->candies->color"};
  string selected_obj;  //The obj that will be selected.
  string condition; //If there is a condition (like candies need to be of a certain color or ice cubs or etc)
  string color_to_print;
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

  string get_color_string(Fl_Color);
};

#endif