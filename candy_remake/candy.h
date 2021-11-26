#include<iostream>
#include<vector>
#include"rectangle.h"
#include"circle.h"
#include<memory>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include"Animation.h"
#ifndef CANDY_H_
#define CANDY_H_

using namespace std;

class Candy:public Rectangle{
    bool wall=false; //Vlad:Use this to know which rectangle(candy) is a wall, no need for a whole class with a constructor for this in my opinion.
    bool is_fruit=false;
    Animation_pop* animation_pop;           //not templating because every Candy has to have those 2 animations
    Animation_slide* animation_slide;
    Circle* c;
    
public:
    vector<Candy*> neighbours;
    Candy(){} //Dummy Constructor
    Candy(Point center, int w, int h,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK,Animation_pop* pop=nullptr, Animation_slide* slide=nullptr);
    
    bool verify_neighbours(Candy);
     void start_pop_animation();

    void start_slide_animation(Point);
    bool is_slide_complete();
    void set_wall(bool);
    bool get_wall();
    bool get_fruit();
    void set_fruit(bool);

    void draw();
}; 


#endif