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

Pop::Pop(int x, int y, int w, int h){
        fl_draw_box(FL_BORDER_FRAME, x, y, w, h, FL_BLACK);
        fl_push_matrix();
    }

Pop::~Pop(){
    fl_pop_matrix();
}

Slide::Slide(Point p, Rectangle *r){
        fl_push_matrix();
        r->setCenter(p);
    }

Slide::~Slide(){
    fl_pop_matrix();
}

bool Animation::is_complete(){
        return time>animation_time;
    }

void Animation::set_animation_time(int t){
        animation_time=t;
    }

void Animation::set_rectangle(Rectangle* rect){
        r=rect;
    }

Animation_slide::Animation_slide(Rectangle* candy_to_animate, Point d, int animation_time): 
    initial_pos{candy_to_animate->getCenter()} , destination{d} {

        set_animation_time(animation_time*speed);
        set_rectangle(candy_to_animate);

        // initializing should be done the constructor initializer but it's more readable this way

        distance_x = (destination.x - r->getCenter().x)/animation_time; // x distance per animation frame
        distance_y = (destination.y - r->getCenter().y)/animation_time; // y distance per animation frame
    }

Animation_slide::~Animation_slide(){
        r->setCenter(initial_pos);
        
}

void Animation_slide::draw(){
        time++;

        Slide s ({initial_pos.x+distance_x*time, initial_pos.y+distance_y*time}, r);

        r->draw();
    }

Animation_pop::Animation_pop(Rectangle* candy_to_animate,int animation_time){
        set_animation_time(animation_time);
        set_rectangle(candy_to_animate);
    }

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

