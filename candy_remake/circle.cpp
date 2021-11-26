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
#include"circle.h"
#include<math.h>
#include<unistd.h>

using namespace std;
#if __cplusplus >= 202002L
#include <numbers>
using std::numbers::pi;
#else
const double pi=3.141592653589793238462643383279502884L;
#endif

Circle::Circle(){}

Circle::Circle(Point center, int r,
                     Fl_Color frameColor,
                     Fl_Color fillColor):
  center{center}, r{r}, fillColor{fillColor}, frameColor{frameColor} {

  }

void Circle::draw() {
  array<Point,37> points;
  for (int i=0;i<36;i++)
    points[i]={static_cast<int>(center.x+r*sin(i*10*pi/180)),
      static_cast<int>(center.y+r*cos(i*10*pi/180))};
  points[36]=points[0];
  fl_color(fillColor);
  fl_begin_polygon();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_polygon();
  fl_color(frameColor);
  fl_begin_line();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_line();
}

void Circle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}

void Circle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}

bool Circle::contains(Point p) {
  return ( (p.x-center.x)*(p.x-center.x)
          +(p.y-center.y)*(p.y-center.y)<=r*r);
}

Point Circle::getCenter(){
    return center;
}

Fl_Color Circle::getFillColor(){
    return frameColor;
}