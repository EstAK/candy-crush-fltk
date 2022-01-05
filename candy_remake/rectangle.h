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
#include<unistd.h>



using namespace std;

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

struct Point{
  int x; int y;
  bool operator!=(const Point& e){
    if(this->x==e.x && this->y==e.y){
        return false;
      }
    return true;
    }
  };

//class declaration Esteban: the code is becoming a total clusterfuck
//we should move it to separate files because I don't even where to put     
//new classes without causing a billion class not found because written 
//before but cannot write before otherwise code goes full kamikaze mode

//Vlad: Roger Roger


class Rectangle {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
  Fl_Boxtype type;
 public:
  
  Rectangle(){}
  Rectangle(Point,int,int,Fl_Color,Fl_Color, Fl_Boxtype);
  Fl_Color getFillColor();
  Fl_Color getFrameColor();

  void setCenter(Point);

  void setWidth(int);
  void setHeight(int newh);
  int getWidth();
  int getHeight();
  Point getCenter();
  void setCode(Fl_Color);
  void draw();
  void setFillColor(Fl_Color);
  void setFrameColor(Fl_Color);
  bool contains(Point);
  void set_box_type(Fl_Boxtype);
  Fl_Boxtype get_box_type();

};

#endif