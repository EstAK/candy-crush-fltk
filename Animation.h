#include<iostream>
#include<vector>
#include<array>
#include<memory>
#include"objective.h"
#include"score.h"
#include<math.h>
#include"rectangle.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>

#ifndef Animation_H_
#define Animation_H_

using namespace std;

struct Pop{
    Pop(int , int , int , int );
    ~Pop();
};

template<typename Form>
struct Fall{
    Fall(Point , Form*);
    ~Fall();
};

template<typename Form>
class Animation{
protected:
    Form* r;
    int time{0};
    int animation_time;
public:
    Animation(Form* candy_to_animate, int animation_time=50);

    bool is_complete();
};


template<typename Form>
class Animation_fall: public Animation<Form>{
    //esteban: inheriting this class from a base class having thoseg private instances might be a good idea as they are nearly the same as in Animation_pop
    Point initial_pos;
    Point destination;
    bool go_back;
    int distance_x = 0;
    int distance_y = 0;
    int speed = 1;

public:
    Animation_fall(Form* candy_to_animate, Point d, bool gb, int animation_time=50);

    ~Animation_fall();

    void draw();

};

template<typename Form>
class Animation_pop: public Animation<Form>{
    int spread_coeff = 10;
public:

    Animation_pop(Form* candy_to_animate,int animation_time = 25);

    ~ Animation_pop();
    void draw();

};


#endif