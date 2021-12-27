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


class Item:public Rectangle{ 
    
    public:
        Item(){}
        Item(Point center, int w, int h,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK);
       
        virtual bool verify_neighbours(shared_ptr<Item>){return false;}
        virtual void start_pop_animation(){}

        virtual void start_fall_animation(Point, bool=true){}
        virtual bool is_fall_complete(){return true;}
        virtual bool get_wall(){return false;}
        virtual bool get_fruit(){return false;}
        virtual void set_fruit(bool){}
        virtual void set_neigh(shared_ptr<Item>){}
        virtual void draw(){}
};

class Candy:public Item{
    bool is_fruit=false;
    Animation_pop* animation_pop;           //not templating because every Candy has to have those 2 animations
    Animation_fall* animation_fall;
    vector<shared_ptr<Item>> neighbours;
    Circle* c;
       
public:
    
    Candy(){} //Dummy Constructor
    Candy(Point center, int w, int h,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK,Animation_pop* pop=nullptr, Animation_fall* fall=nullptr);
    
    bool verify_neighbours(shared_ptr<Item>);
    void start_pop_animation();

    void start_fall_animation(Point, bool=true);
    bool is_fall_complete();
    bool get_fruit();
    void set_fruit(bool);
    void set_neigh(shared_ptr<Item>);
    
    void draw();
}; 

class Wall:public Item{
public:
    Wall(){}
    Wall(Point center,int w,int h,Fl_Color fillColor=FL_MAGENTA,Fl_Color frameColor=FL_BLACK):Item(center,w,h,fillColor,frameColor){}
    bool get_wall();
    void draw();

};


#endif