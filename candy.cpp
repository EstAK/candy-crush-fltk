#include<iostream>
#include"rectangle.h"
#include"circle.h"
#include<vector>
#include"candy.h"
#include<memory>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>

using namespace std;



Candy::Candy(Point center,int w,int h,Fl_Color fillColor,Fl_Color frameColor):Item(),Rectangle(center,w,h,fillColor,frameColor){}

bool Candy::verify_neighbours(shared_ptr<Item> current){
    if(this->get_wall()==true){
        return false;
    }
    if(this->is_ingredient() && current->is_ingredient()){
        return false;
    }
    
    for(auto i:neighbours){
        if(i->getCenter().x==current->getCenter().x && i->getCenter().y==current->getCenter().y){
            if(!this->is_ingredient() && !current->is_ingredient()){
                Fl_Color save=this->getFillColor();    //TODO : Add the animation code here.
                this->setCode(i->getFillColor());
                i->setCode(save);   
                return true;
            }else{
                if(this->is_ingredient()){
                    this->set_fruit(false);
                    i->set_fruit(true);
                    return true;
                }else{
                                    
                }
            }
        }
    }
    return false;
}

void Candy::update_frosted_neighbours(){
    for(auto n: this->neighbours){
        if (n->has_frosting() && n->getFillColor() != this->getFillColor() ){
            n->set_layers_of_frosting(n->get_layers_of_frosting()-1);
        }
    }
}

void Candy::draw(){
        if (animation_pop && animation_pop->is_complete()){
            delete animation_pop;
            animation_pop = nullptr;
        }
        if (animation_fall && animation_fall->is_complete()){
            delete animation_fall;
            animation_fall = nullptr;
        }

        if (animation_fall){
            animation_fall->draw();
        }else if (animation_pop) {
            animation_pop->draw();
        }else{
            Rectangle::draw();
        }
}

void Candy::start_pop_animation(){
        animation_pop = new Animation_pop<Rectangle>(this);
    }

void Candy::start_fall_animation(Point dest, bool gb){
    animation_fall = new Animation_fall<Rectangle>(this, dest, gb);
}

bool Candy::is_fall_complete(){
        // checks if the fall animaiton is complete or non existant
        if (animation_fall){
            return false;
        }else{
            return true;
        }
    }

void Candy::set_layers_of_frosting(int new_layers){
    layers = new_layers;
    cout<<this->get_layers_of_frosting()<<endl;
}

int Candy::get_layers_of_frosting(){
    return layers;
}

bool Candy::has_frosting(){
    return get_layers_of_frosting() > 0;
}

void Candy::set_neigh(shared_ptr<Item> neigh){
    neighbours.push_back(neigh);
}

Wall::Wall(Point center, int w, int h, Fl_Color fillColor, Fl_Color frameColor):Item(),Rectangle(center,w,h,fillColor,frameColor){}

bool Wall::get_wall(){
    return true;
}

void Wall::draw(){
    Rectangle::draw();
}