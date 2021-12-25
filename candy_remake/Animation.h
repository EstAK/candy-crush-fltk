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

struct Fall{
    Fall(Point , Rectangle *);
    ~Fall();
};

struct Slide{
    Slide(Point);
    ~Slide();
};


class Animation{
protected:
    Rectangle* r;
    int time{0};
    int animation_time;
public:
    Animation(Rectangle* candy_to_animate, int animation_time=50);

    bool is_complete();
};


class Animation_fall: public Animation{
    //esteban: inheriting this class from a base class having thoseg private instances might be a good idea as they are nearly the same as in Animation_pop
    Point initial_pos;
    Point destination;
    bool go_back;
    int distance_x = 0;
    int distance_y = 0;
    int speed = 1;

public:
    Animation_fall(Rectangle* candy_to_animate, Point d, bool gb, int animation_time=50);

    ~Animation_fall();

    void draw();

};


class Animation_pop: public Animation{
    int spread_coeff = 10;
public:

    Animation_pop(Rectangle* candy_to_animate,int animation_time = 25);

    ~ Animation_pop();
    void draw();

};


#endif