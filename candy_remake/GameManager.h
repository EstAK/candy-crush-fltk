#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include<memory>
#include"candy.h"
#include"rectangle.h"
#include"circle.h"
#include"score.h"
#include"objective.h"

using namespace std;

#ifndef GameManager_H_
#define GameManager_H_

class GameManager{
   shared_ptr<Candy>** candy=new shared_ptr<Candy>*[9]();
   Score* candy_score{0};
   Objective* game_obj;
   bool* can_vibrate;
   bool* not_impossible;
   Fl_Color color[5]={FL_RED,FL_BLUE,FL_YELLOW,FL_DARK_CYAN,FL_GREEN};
public:
   GameManager();
   void set_up(shared_ptr<Candy>**,Score&,Objective&);
   bool break_candies(int x,int y,int i,int j,bool pc=false);
   void destroy_candies(int x,int y,int counter_left_right,int counter_up_down,bool pc=false);
   int fall_candies(int,int, bool);
   void fall_walls(int,int);  
   void set_the_neighbours();
 

};


#endif
