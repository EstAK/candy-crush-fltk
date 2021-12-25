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
#include <ostream>


using namespace std;

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

struct Point{int x; int y;};

class Rectangle {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  
  Rectangle(){}
  Rectangle(Point,int,int,Fl_Color,Fl_Color);
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

};

#endif