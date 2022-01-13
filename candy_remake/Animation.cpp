/*
name : candy crush
author : Vlad Marian Moruntale & Aguililla Klein Esteban
date : 13/01/22
*/
#include "candy.h"
#include "objective.h"
#include "score.h"
#include "rectangle.h"
#include "circle.h"
#include "Animation.h"
#include "const.h"

using namespace std;

/*
Structs 
*/

//Pop Struct

Pop::Pop(int x, int y, int w, int h){
        fl_draw_box(FL_BORDER_FRAME, x, y, w, h, FL_BLACK);
        fl_push_matrix();
    }

Pop::~Pop(){
    fl_pop_matrix();
}

//Fall Struct

template<typename Form>
Fall<Form>::Fall(Point p, Form *r){
        fl_push_matrix();
        r->setCenter(p);
    }

template<typename Form>
Fall<Form>::~Fall(){
    fl_pop_matrix();
}

template class Fall<Rectangle>;
template class Fall<Circle>;

/*
Class
*/

//Animations parent class

template<typename Form>
Animation<Form>::Animation(Form* candy_to_animate, int animation_time): r{candy_to_animate}, animation_time{animation_time} {}

template<typename Form>
bool Animation<Form>::is_complete(){
        return time>animation_time;
    }

template class Animation<Rectangle>;
template class Animation<Circle>;

//fall Animation

template<typename Form>
Animation_fall<Form>::Animation_fall(Form* candy_to_animate, Point d, bool gb, int animation_time): 
    Animation<Form>{candy_to_animate, animation_time} ,go_back{gb} ,initial_pos{candy_to_animate->getCenter()} , destination{d}{

        distance_x = (destination.x - this->r->getCenter().x)/animation_time; // x distance per animation frame
        distance_y = (destination.y - this->r->getCenter().y)/animation_time; // y distance per animation frame

    }

template<typename Form>
Animation_fall<Form>::~Animation_fall(){
    if (go_back){   // puts the rectangle r back to his initial position
        this->r->setCenter(initial_pos);
    }else{          // let the rectangle r at his destination ( called in fall_candies )
        this->r->setCenter(destination);
    }
        
}

template<typename Form>
void Animation_fall<Form>::draw(){
    // method managinc the fall incrementation

        this->time++;

        Fall<Form> s ({initial_pos.x+distance_x*this->time, initial_pos.y+distance_y*this->time}, this->r);

        this->r->draw();
    }

template class Animation_fall<Rectangle>;
template class Animation_fall<Circle>;

//Pop animation

template<typename Form>
Animation_pop<Form>::Animation_pop(Form* candy_to_animate,int animation_time): Animation<Form>{candy_to_animate, animation_time}{}

template<typename Form>
Animation_pop<Form>::~Animation_pop(){
    sleep(0.5);
}

template<typename Form>
void Animation_pop<Form>::draw(){
    // method managing the pop incrementation

    this->time++;
        
    int w = log(this->r->getWidth()*this->time)*spread_coeff;       // using log function to make it smoother to the eye on the end of the pop
    int h = log(this->r->getHeight()*this->time)*spread_coeff;      // same logic

    Pop t{this->r->getCenter().x-w/2, this->r->getCenter().y-h/2, w, h};        // centering the pop
        
    this->r->draw();
}

template class Animation_pop<Rectangle>;
