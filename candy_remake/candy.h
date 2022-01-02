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

class Item{ 
protected:
    vector<shared_ptr<Item>> neighbours;
public:
    Item(){}
   
    virtual Point getCenter(){return Point{0,0};}
    virtual void setCenter(Point p){}
    virtual Fl_Color getFillColor(){return FL_WHITE;}
    virtual void setFillColor(Fl_Color color){}
    virtual void setCode(Fl_Color code){}
    virtual void setFrameColor(Fl_Color color){}
    virtual bool contains(Point p){return false;}

    virtual bool verify_neighbours(shared_ptr<Item>){return false;}
    virtual void update_frosted_neighbours(){}
    virtual void start_pop_animation(){}

    virtual void start_fall_animation(Point, bool=true){}
    virtual bool is_fall_complete(){return true;}
    virtual bool get_wall(){return false;}
    virtual bool is_ingredient(){return false;}
    virtual void set_fruit(bool){}
    virtual void set_neigh(shared_ptr<Item>){}
    virtual void draw(){}

    virtual bool has_frosting(){return false;}
    virtual int get_layers_of_frosting(){return 0;}
    virtual void set_layers_of_frosting(int){}

};

class Candy:public Item, public Rectangle{
    int layers = 0;
    Animation_pop<Rectangle>* animation_pop = nullptr;           //not templating because every Candy has to have those 2 animations
    Animation_fall<Rectangle>* animation_fall = nullptr;
       
public:
    
    Candy(){} //Dummy Constructor
    Candy(Point center, int w, int h,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK);
    
    // polymorphism of rectangle methods
    Point getCenter(){return Rectangle::getCenter();}
    void setCenter(Point p){Rectangle::setCenter(p);}
    Fl_Color getFillColor(){return Rectangle::getFillColor();}
    void setCode(Fl_Color code){Rectangle::setCode(code);}
    void setFillColor(Fl_Color color){Rectangle::setFillColor(color);}
    void setFrameColor(Fl_Color color){Rectangle::setFrameColor(color);}
    bool contains(Point p){return Rectangle::contains(p);}
    // end of rectangle polymorphism

   

    bool verify_neighbours(shared_ptr<Item>);
    void update_frosted_neighbours();
    void start_pop_animation();

    void start_fall_animation(Point, bool=true);
    bool is_fall_complete();
    // void set_fruit(bool);
    void set_neigh(shared_ptr<Item>);
    
    bool has_frosting();
    void set_layers_of_frosting(int);
    int get_layers_of_frosting();

    void draw();
}; 

class Wall:public Item, public Rectangle{
public:
    Wall(){}
    Wall(Point, int, int, Fl_Color=FL_MAGENTA, Fl_Color=FL_BLACK);

    // rectangle polymorhism
    Point getCenter(){return Rectangle::getCenter();}
    void setCenter(Point p){Rectangle::setCenter(p);}
    Fl_Color getFillColor(){return Rectangle::getFillColor();}
    void setCode(Fl_Color code){Rectangle::setCode(code);}
    void setFillColor(Fl_Color color){Rectangle::setFillColor(color);}
    void setFrameColor(Fl_Color color){Rectangle::setFrameColor(color);}
    bool contains(Point p){return Rectangle::contains(p);}
    // rectangle polymorphism


    bool get_wall();
    void draw();

};


class Ingredient:public Item, public Circle{
public:
    Ingredient(){}
    Ingredient(Point center, int radius, Fl_Color fillColor=FL_RED, Fl_Color frameColor=FL_BLACK):Item(),Circle(center,radius,frameColor,fillColor){}

    void draw(){
        Circle::draw();
    }

    bool is_ingredient(){return true;}
};

#endif