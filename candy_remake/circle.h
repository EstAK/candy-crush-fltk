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

#ifndef CIRCLE_H_
#define CIRCLE_H_

class Circle {
  Point center;
  int r;
  Fl_Color fillColor, frameColor;
 public:
  Circle();
  Circle(Point center, int r,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_WHITE);
  void draw();
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor();
  void setFrameColor(Fl_Color);
  bool contains(Point);
  Point getCenter();
  void setCenter(Point);
  void setCode(Fl_Color);
};

#endif