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


using namespace std;

#ifndef HINT_H_
#define HINT_H_

class Hint{
   shared_ptr<Candy> **candy=new shared_ptr<Candy>*[9]();
   bool* can_vibrate;
   bool* not_impossible;
public:
   Hint();
   void set_up(shared_ptr<Candy>**,bool&, bool&);
   bool forshadowing_over_9000(int,int,Fl_Color,bool);
   void check_impossible(int,int,bool);
};

#endif