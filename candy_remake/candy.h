#include <iostream>
#include <vector>
#include "rectangle.h"
#include "circle.h"
#include "const.h"
#include <memory>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Text_Display.H>
#include "Animation.h"
#ifndef CANDY_H_
#define CANDY_H_

using namespace std;

class Item{ 
protected:
    vector<shared_ptr<Item>> neighbours;
public:
    Item(){}

    virtual Point getCenter(){return Point{0,0};}
    virtual void setCenter(Point){}
    virtual Fl_Color getFillColor(){return FL_WHITE;}
    virtual void setFillColor(Fl_Color){}
    virtual void setCode(Fl_Color){}
    virtual void setFrameColor(Fl_Color){}
    virtual bool contains(Point){return false;}

    virtual bool verify_neighbours(shared_ptr<Item>){return false;}
    virtual void update_frosted_neighbours(){}
    virtual void start_pop_animation(){}

    virtual void start_fall_animation(Point, bool=true){}
    virtual bool is_fall_complete(){return true;}
    virtual bool is_wall(){return false;}
    virtual bool is_ingredient(){return false;}
    virtual void set_fruit(bool){}
    virtual void set_neigh(shared_ptr<Item>){}
    virtual void draw(){}

    virtual bool has_frosting(){return false;}
    virtual int get_layers_of_frosting(){return 0;}
    virtual void set_layers_of_frosting(int){}

    virtual void set_box_type(Fl_Boxtype){};
    virtual Fl_Boxtype get_box_type(){return FL_FLAT_BOX;}
    virtual void set_direction(int){}
    virtual int get_direction(){return horizontal;}
    virtual bool is_striped(){return false;}
    virtual bool is_special_candy(){return false;}
    virtual bool is_wrapped(){return false;}
    virtual bool is_bomb(){return false;}
    virtual void set_color_to_break(Fl_Color){}
    virtual Fl_Color get_color_to_break(){return FL_BLACK;}
    virtual int current_box(){
        return 0;
    }
    virtual void inc_box(){
    }
    

};

class Candy:public Item, public Rectangle{
    int layers = 0;
    int box=0;
    Animation_pop<Rectangle>* animation_pop = nullptr;
    Animation_fall<Rectangle>* animation_fall = nullptr;
       
public:
    
    Candy(){} //Dummy Constructor
    Candy(Point center, int w=squareLenght, int h=squareLenght,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK, Fl_Boxtype=FL_FLAT_BOX);

    
    // polymorphism of rectangle methods
    Point getCenter(){return Rectangle::getCenter();}
    void setCenter(Point p){Rectangle::setCenter(p);}
    Fl_Color getFillColor(){return Rectangle::getFillColor();}
    void setCode(Fl_Color code){Rectangle::setCode(code);}
    void setFillColor(Fl_Color color){Rectangle::setFillColor(color);}
    void setFrameColor(Fl_Color color){Rectangle::setFrameColor(color);}
    bool contains(Point p){return Rectangle::contains(p);}
    void set_box_type(Fl_Boxtype b){Rectangle::set_box_type(b);}
    Fl_Boxtype get_box_type(){return Rectangle::get_box_type();}
    // end of rectangle polymorphism

   

    bool verify_neighbours(shared_ptr<Item>);
    void update_frosted_neighbours();
    void start_pop_animation();

    void start_fall_animation(Point, bool=true);
    bool is_fall_complete();
    void set_neigh(shared_ptr<Item>);
    bool has_frosting();
    void set_layers_of_frosting(int);
    int get_layers_of_frosting();

    void draw();
    bool is_special_candy(){return false;}
    int current_box(){
        return box;
    }
    void inc_box(){
        box++;
        
    }
};

class Edit_Candy:public Candy{
public:
   using Candy::Candy;
   void draw(){
       Rectangle::draw();
       
   }
   
};

class Special_candy: public Candy{

public:
    Special_candy(){}
    Special_candy(Point, int=squareLenght, int=squareLenght, Fl_Color=FL_MAGENTA, Fl_Color=FL_BLACK, Fl_Boxtype=FL_DIAMOND_BOX);

    bool is_special_candy(){return true;}
};

class Striped_candy: public Special_candy{
    int stripes_direction;
    // 0 : horizontal --
    // 1 : vertical |
public:
    using Special_candy::Special_candy;

    void set_direction(int);
    int get_direction();

    bool is_striped(){return true;}
};

class Wrapped_candy: public Special_candy{

public:
    Wrapped_candy(){}
    Wrapped_candy(Point, int=squareLenght, int=squareLenght, Fl_Color=FL_MAGENTA, Fl_Color=FL_BLACK, Fl_Boxtype=FL_PLASTIC_UP_BOX);

    bool is_wrapped(){return true;}
};

class Bomb_candy: public Special_candy{
    Fl_Color color_to_break = FL_BLACK;
public:
    Bomb_candy(){}
    Bomb_candy(Point, int=squareLenght, int=squareLenght, Fl_Color=FL_DARK_GREEN, Fl_Color=FL_BLACK, Fl_Boxtype=FL_ROUNDED_BOX);

    void set_color_to_break(Fl_Color);
    Fl_Color get_color_to_break();

    bool is_bomb(){return true;}
};

class Wall:public Item, public Rectangle{
public:
    Wall(){}
    Wall(Point, int=squareLenght, int=squareLenght, Fl_Color=GRAY, Fl_Color=FL_BLACK);

    // rectangle polymorhism
    Point getCenter(){return Rectangle::getCenter();}
    void setCenter(Point p){Rectangle::setCenter(p);}
    Fl_Color getFillColor(){return Rectangle::getFillColor();}
    void setCode(Fl_Color code){Rectangle::setCode(code);}
    void setFillColor(Fl_Color color){Rectangle::setFillColor(color);}
    void setFrameColor(Fl_Color color){Rectangle::setFrameColor(color);}
    bool contains(Point p){return Rectangle::contains(p);}
    // rectangle polymorphism

    bool is_wall();
    void draw();

};


class Ingredient:public Item, public Circle{
    Animation_fall<Circle>* animation_fall = nullptr;
public:
    Ingredient(){}
    Ingredient(Point, int=circleRadius, Fl_Color=FL_DARK_RED, Fl_Color=FL_BLACK);

    Point getCenter(){return Circle::getCenter();}
    bool contains(Point p){return Circle::contains(p);}
    void setCenter(Point p){Circle::setCenter(p);}
    Fl_Color getFillColor(){return Circle::getFillColor();}
    void setCode(Fl_Color code){Circle::setCode(code);}
    void setFillColor(Fl_Color color){Circle::setFillColor(color);}
    void setFrameColor(Fl_Color color){Circle::setFrameColor(color);}

    void start_fall_animation(Point, bool);
    void draw();
    bool is_ingredient(){return true;}
    bool verify_neighbours(shared_ptr<Item>);
    void set_neigh(shared_ptr<Item>);

};

#endif