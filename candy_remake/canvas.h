#include<iostream>
#include<vector>
#include<array>
#include<memory>
#include"candy.h"
#include"objective.h"
#include"score.h"
#include"rectangle.h"
#include"GameManager.h"
#include"hint.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>

#ifndef CANVAS_H_
#define CANVAS_H_

using namespace std;

class Canvas{
    bool has_moved=false;
    shared_ptr<Candy>**candy=new shared_ptr<Candy>*[9]();
    Fl_Color color[5]={FL_RED,FL_BLUE,FL_YELLOW,FL_DARK_CYAN,FL_GREEN};
    vector<string> lines;
    shared_ptr<Candy> current=make_shared<Candy>(); //stocks the current cell clicked on. esteban: might cause an issue somewhere as base cas is now FL_BLACK
    int x=0;int y=0; //currents coord in the array
    Score candy_score{0}; //By default the score begins at 0
    bool not_impossible=false;
    
    int timer=0;  //Var used for timer
    bool can_vibrate=false;
    string current_map;
    GameManager gm;
    Hint ht;
    Objective game_obj;

public:
   Canvas(); 
   ~Canvas();
   void make_board(string);
   void read_file(string ); 
   void draw();
   void mouseMove(Point);
   void mouseClick(Point);
   void keyPressed(int keyCode){
    exit(0);
    }
    void set_the_neighbours();
  bool is_board_moving();
  
    
};

#endif