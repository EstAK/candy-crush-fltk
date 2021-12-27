#include<iostream>
#include<vector>
#include<array>
#include<memory>
#include"candy.h"
#include"objective.h"
#include"score.h"
#include<math.h>
#include"rectangle.h"
#include"Animation.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>

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

Fall::Fall(Point p, Rectangle *r){
        fl_push_matrix();
        r->setCenter(p);
    }

Fall::~Fall(){
    fl_pop_matrix();
}

/*
Class
*/

//Animations parent class

Animation::Animation(Rectangle* candy_to_animate, int animation_time): r{candy_to_animate}, animation_time{animation_time} {}

bool Animation::is_complete(){
        return time>animation_time;
    }

//fall Animation

Animation_fall::Animation_fall(Rectangle* candy_to_animate, Point d, bool gb, int animation_time): 
    Animation{candy_to_animate, animation_time} ,go_back{gb} ,initial_pos{candy_to_animate->getCenter()} , destination{d}{

        distance_x = (destination.x - r->getCenter().x)/animation_time; // x distance per animation frame
        distance_y = (destination.y - r->getCenter().y)/animation_time; // y distance per animation frame

    }


Animation_fall::~Animation_fall(){
        if (go_back){
            r->setCenter(initial_pos);
        }else{
            r->setCenter(destination);
        }
        
}

void Animation_fall::draw(){
        time++;

        Fall s ({initial_pos.x+distance_x*time, initial_pos.y+distance_y*time}, r);

        r->draw();
    }

//Pop animation

Animation_pop::Animation_pop(Rectangle* candy_to_animate,int animation_time): Animation{candy_to_animate, animation_time}{}

Animation_pop::~Animation_pop(){
    sleep(0.5);
}

void Animation_pop::draw(){
    ++time;
        
    int w = log(r->getWidth()*time)*spread_coeff;
    int h = log(r->getHeight()*time)*spread_coeff;

    Pop t{r->getCenter().x-w/2, r->getCenter().y-h/2, w, h};        //WOP i still have to make it pretty
        
    r->draw();
}

