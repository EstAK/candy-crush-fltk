#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include<FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>


using namespace std;
const int windowWidth=500;
const int windowHeight=500;
const double refreshPerSecond=60;


struct Point{int x; int y;};

class Rectangle {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  void draw();
  void setFillColor(Fl_Color newFillColor);
  
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }

  void setCenter(Point p){
      center=p;
  }

  void setWidth(int neww) {
    w = neww;
  }
  void setHeight(int newh) {
    h = newh;
  }
  int getWidth() {
    return w;
  }
  int getHeight() {
    return h;
  }
  bool contains(Point p);
  Point getCenter() {
    return center;
  }
};

void Rectangle::draw() {
  fl_draw_box(FL_FLAT_BOX, center.x-w/2, center.y-h/2, w, h, fillColor);
  fl_draw_box(FL_BORDER_FRAME, center.x-w/2, center.y-h/2, w, h, frameColor);
}

void Rectangle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}

void Rectangle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}

bool Rectangle::contains(Point p) {
  return p.x>=center.x-w/2 &&
         p.x<center.x+w/2 &&
         p.y>=center.y-h/2 &&
         p.y<center.y+h/2;
}


class Candy:public Rectangle{
public:
    Candy(){}
    Candy(Point center, int w, int h,Fl_Color frameColor = FL_BLACK,Fl_Color fillColor = FL_WHITE){
        setCenter(center);
        setWidth(w);
        setHeight(h);
        setFrameColor(frameColor);
        setFillColor(fillColor);
    }
    
}; 

class Canvas{
   vector<vector<Candy>> candy;
   Fl_Color color[6]={FL_RED,FL_BLUE,FL_YELLOW,FL_BLACK,FL_DARK_CYAN,FL_GREEN};
public:
    Canvas(Point center={100,100},int wi=850,int hi=850){
        for (int x = 0; x<9; x++) {
          candy.push_back({});
          for (int y = 0; y<9; y++)
            candy[x].push_back({{50*x+25, 50*y+25}, 40, 40,FL_BLACK,color[rand()%6]});
  }
    }
    void draw(){
      for(auto i:candy){
          for(auto j:i){
              j.draw();
          }
      }
    }
    void mouseMove(Point mouseLoc){
        for(auto i:candy){
          for(auto j:i){
              if(j.contains(mouseLoc)){}
          }
      }
    }
    void mouseClick(Point mouseLoc){
      for(auto i:candy){
          for(auto j:i){
              if(j.contains(mouseLoc)){cout<<j.getCenter().x<<" "<<j.getCenter().y<<endl;}
          }
      }
    }
    void keyPressed(int keyCode){
    exit(0);
    }
};




class MainWindow : public Fl_Window {
    Canvas canva;
    public:
    MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Candy Try") {
        Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
        resizable(this);
    }
    void draw() override {
        
        Fl_Window::draw();
        canva.draw();
    }
    int handle(int event) override {
        switch (event) {
            case FL_MOVE:
                canva.mouseMove(Point{Fl::event_x(),Fl::event_y()});
                return 1;
            case FL_PUSH:
                canva.mouseClick(Point{Fl::event_x(),Fl::event_y()});
                return 1;                
            case FL_KEYDOWN:
                canva.keyPressed(Fl::event_key());
                return 1;
        }
        return 0;
    }
    static void Timer_CB(void *userdata) {
        MainWindow *o = (MainWindow*) userdata;
        o->redraw();
        Fl::repeat_timeout(1.0/refreshPerSecond, Timer_CB, userdata);
    }
};

int main(int argc, char *argv[]) {
    
    MainWindow window;
    window.show(argc, argv);
    return Fl::run();
}