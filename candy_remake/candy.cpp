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

Item::Item(Point center,int w,int h,Fl_Color fillColor,Fl_Color frameColor):Rectangle(center,w,h,fillColor,frameColor){

}

Candy::Candy(Point center,int w,int h,Fl_Color fillColor,Fl_Color frameColor,Animation_pop* pop, Animation_fall* fall):Item(center,w,h,fillColor,frameColor),animation_pop(pop),animation_fall(fall){

}

bool Candy::verify_neighbours(shared_ptr<Item> current){
    if(this->get_wall()==true){
        return false;
    }
    if(this->get_fruit() && current->get_fruit()){
        return false;
    }
    
    for(auto i:neighbours){
        if(i->getCenter().x==current->getCenter().x && i->getCenter().y==current->getCenter().y){
            if(!this->get_fruit() && !current->get_fruit()){
                Fl_Color save=this->getFillColor();    //TODO : Add the animation code here.
                this->setCode(i->getFillColor());
                i->setCode(save);   
                return true;
            }else{
                if(this->get_fruit()){
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
    for(auto n: neighbours){
        if (n->has_frosting()){
            n->set_layers_of_frosting(n->get_layers_of_frosting()-1);
        }
    }
}



void Candy::draw(){
        if(is_fruit){
            c=new Circle({getCenter().x,getCenter().y},20,getFrameColor(),getFillColor());
            c->draw();
            return;
        }

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
        animation_pop = new Animation_pop(this);
    }

void Candy::start_fall_animation(Point dest, bool gb){
    animation_fall = new Animation_fall(this, dest, gb);
}

bool Candy::get_fruit(){
    return is_fruit;
}

void Candy::set_fruit(bool newFruit){
    is_fruit=newFruit;
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
    if (layers == 0){
        this->setFrameColor(FL_BLACK);
    }
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

bool Wall::get_wall(){
    return true;
}

void Wall::draw(){
    Rectangle::draw();
}





