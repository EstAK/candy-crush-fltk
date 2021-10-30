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


class Rectangle{
   Point center_pos;
   int wi,hi;
   Fl_Color frame;
   Fl_Color fill;
public:
    //Setters
    void set_center(Point center){center_pos=center;}
    void set_wi(int w){wi=w;}
    void set_hi(int h){hi=h;}
    void set_frame(Fl_Color fr){frame=fr;}
    void set_color(Fl_Color color){fill=color;}

    void draw(){
     fl_draw_box(FL_FLAT_BOX,center_pos.x,center_pos.y,wi,hi,fill);
     fl_draw_box(FL_BORDER_FRAME,center_pos.x,center_pos.y,wi,hi,frame);
    }
    bool contains(Point p){
       return p.x>=center_pos.x-wi/2 &&
        p.x<center_pos.x+wi/2 &&
        p.y>=center_pos.y-hi/2 &&
        p.y<center_pos.y+hi/2;
    }

 
};


class Candy:public Rectangle{
   Fl_Color color[6]={FL_RED,FL_BLUE,FL_YELLOW,FL_BLACK,FL_GREEN,FL_DARK_CYAN};
public:
   Candy(Point center={0,0},int w=40, int h=40,int indice=rand()%6,Fl_Color frameColor=FL_BLACK,Fl_Color fillColor=FL_RED){
       set_center(center);
       set_wi(w);
       set_hi(h);
       set_frame(frameColor);
       set_color(color[indice]);
       
   }
}; 

class Canvas{
   vector<vector<Candy>>candy;
public:
    Canvas(Point center={100,100},int wi=350,int hi=350){
      for (int x = 0; x<10; x++) {
    candy.push_back({});
    for (int y = 0; y<10; y++)
      candy[x].push_back({{50*x+25, 50*y+25}, 40, 40});
  }
    }
    void draw(){
       for(auto i:candy){
          for(auto j:i){
              j.draw();
          }
       }
    }
    void mouseMove(Point mouseLoc){}
    void mouseClick(Point mouseLoc){
      
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