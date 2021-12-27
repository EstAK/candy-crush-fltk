#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include <string>
#include <iostream>
#include <array>
#include"rectangle.h"
#include<unistd.h>

using namespace std;

Rectangle::Rectangle(Point center,int w, int h,Fl_Color fillColor=FL_WHITE,Fl_Color frameColor=FL_BLACK):center(center),w(w),h(h),fillColor(fillColor),frameColor(frameColor){

}

Fl_Color Rectangle::getFillColor(){
    return fillColor;
}

Fl_Color Rectangle::getFrameColor(){
    return frameColor;
}

Point Rectangle::getCenter(){
    return center;
}

int Rectangle::getWidth(){
    return w;
}

int Rectangle::getHeight(){
    return h;
}

void Rectangle::setWidth(int neww){
    w=neww;
}

void Rectangle::setHeight(int newh){
    h=newh;
}

void Rectangle::setCode(Fl_Color code){
   if(code==216){setFillColor(FL_BLUE);}
      else if(code==140){setFillColor(FL_DARK_CYAN);}
      else if(code==56){setFillColor(FL_BLACK);}
      else if(code==95){setFillColor(FL_YELLOW);}
      else if(code==88){setFillColor(FL_RED);}
      else if(code==63){setFillColor(FL_GREEN);}
}

void Rectangle::draw(){
     fl_draw_box(FL_FLAT_BOX, center.x-w/2, center.y-h/2, w, h, fillColor);
     fl_draw_box(FL_BORDER_FRAME, center.x-w/2, center.y-h/2, w, h, frameColor);
}

void Rectangle::setFillColor(Fl_Color fill){
    fillColor=fill;
}

void Rectangle::setFrameColor(Fl_Color frame){
    frameColor=frame;
}

bool Rectangle::contains(Point p){
    return p.x>=center.x-w/2 &&
             p.x<center.x+w/2 &&
             p.y>=center.y-h/2 &&
             p.y<center.y+h/2;
    
}


void Rectangle::setCenter(Point p){
   center=p;
}
