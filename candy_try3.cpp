#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include "const.h"


using namespace std;

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
  void setCode(Fl_Color code){
      if(code==216){setFillColor(FL_BLUE);}
      else if(code==140){setFillColor(FL_DARK_CYAN);}
      else if(code==56){setFillColor(FL_BLACK);}
      else if(code==95){setFillColor(FL_YELLOW);}
      else if(code==88){setFillColor(FL_RED);}
      else if(code==63){setFillColor(FL_GREEN);}
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
    bool wall=false; //Vlad:Use this to know whick rectangle(candy) is a wall, no need for a whole class with a constructor for this in my opinion.
public:
    vector<Candy*> neighbours;
    Candy(){} //Dummy Constructor
    Candy(Point center, int w, int h,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK){
        setCenter(center);
        setWidth(w);
        setHeight(h);
        setFrameColor(frameColor);
        setFillColor(fillColor);
    }
    bool verify_neighbours(Candy current){
        if(this->get_wall()==true){return false;}
        for(auto i:neighbours){
            if(i->getCenter().x==current.getCenter().x && i->getCenter().y==current.getCenter().y){
                Fl_Color save=this->getFillColor();    
                this->setCode(i->getFillColor());
                i->setCode(save);   
                return true;
                }
        }
        return false;
    }
    void set_wall(bool wa){wall=wa;}
    bool get_wall(){return wall;}
}; 



class Canvas{
    vector<vector<Candy>> candy;                                                 //Make it a candy it's better
    Fl_Color color[6]={FL_RED,FL_BLUE,FL_YELLOW,FL_BLACK,FL_DARK_CYAN,FL_GREEN};
    vector<string> lines;
    Candy current{{0,0},0,0}; //stocks the current cell clicked on.
    Candy* point_current=&current;
public:
    Canvas(Point center={100,100},int wi=850,int hi=850){                           //wi and hi are not used x think we should remove them ; Vlad:Okey;
        read_file();
        int y = 0, i=0;
        for (int x = 0; x<9; x++){
            candy.push_back({});
            for (int y = 0; y<9; y++){
                cout<<lines[x][y]<<" ";
                if (lines[y][x] == *c){
                    candy[x].push_back(Candy{{50*x+25, 50*y+25}, 40, 40, color[rand()%6]});
                }
                else{
                    candy[x].push_back(Candy{{50*x+25, 50*y+25}, 40, 40,FL_MAGENTA,FL_BLACK});
                }
            }
            cout<<endl;
            set_the_neighbours();
            set_the_wall();
        }
    }
    
    void read_file(){
        string line;                        //temporary input file
        ifstream myfile(file);
        while (myfile >> line) {
            lines.push_back(line);
        }
        myfile.close(); 
    }

    void draw(){
      for(auto x:candy){
          for(auto j:x){
              j.draw();
          }
      }
    }
    void mouseMove(Point mouseLoc){
        for(int i=0;i<candy.size();i++){
            for(int j=0;j<candy[0].size();j++){
                if(candy[i][j].contains(mouseLoc)){
                    candy[i][j].setFrameColor(FL_RED);
                }else{
                    candy[i][j].setFrameColor(FL_BLACK);
                }
            }
        }
    }
    void mouseClick(Point mouseLoc){
      for(int i=0;i<candy.size();i++){
          for(int j=0;j<candy[0].size();j++){
              if(candy[i][j].contains(mouseLoc) && current.getCenter().x==0 && current.getCenter().y==0){
                if(candy[i][j].get_wall()==false){current=candy[i][j];} //make sure that is not a wall.
              }else if(candy[i][j].contains(mouseLoc)){
                  if(candy[i][j].verify_neighbours(current)){
                      current=Candy{{0,0},0,0};
                      cout<<"Neigh"<<endl;
                  }else{
                      cout<<"Selected new one"<<endl;
                      if(candy[i][j].get_wall()==false){current=candy[i][j];}     
                  }
               }
           }
        }
   }
    void keyPressed(int keyCode){
    exit(0);
    }

    void set_the_neighbours(){
        for(int i=0;i<candy.size();i++){
         for(int j=0;j<candy[0].size();j++){
          if(i+1<candy.size()) candy[i][j].neighbours.push_back(&candy[i+1][j]);
          if(j+1<candy[0].size()) candy[i][j].neighbours.push_back(&candy[i][j+1]);
          if(i-1>=0) candy[i][j].neighbours.push_back(&candy[i-1][j]);
          if(j-1>=0) candy[i][j].neighbours.push_back(&candy[i][j-1]);
         }
        }
    }

    void set_the_wall(){ //set the walls
        for(int i=0;i<candy.size();i++){
            for(int j=0;j<candy[0].size();j++){
                if(candy[i][j].getFillColor()==248){candy[i][j].set_wall(true);}
            }
        }
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