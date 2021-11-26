#include<iostream>
#include<vector>
#include<array>
#include<memory>
#include"candy.h"
#include"objective.h"
#include"score.h"
#include"rectangle.h"
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
    array<array<Candy,9>,9> candy;          //2d Array
    Fl_Color color[5]={FL_RED,FL_BLUE,FL_YELLOW,FL_DARK_CYAN,FL_GREEN};
    vector<string> lines;
    Candy current;; //stocks the current cell clicked on. esteban: might cause an issue somewhere as base cas is now FL_BLACK
    int x=0;int y=0; //currents coord in the array
    Score candy_score{0}; //By default the score begins at 0
    bool not_impossible=false;
    
    int timer=0;  //Var used for timer
    bool can_vibrate=false;
    string current_map;

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
    void set_the_wall();
    void break_candies(int x,int y,int i,int j,bool pc=false);
  int fall_candies(int,int);
  void fall_walls(int,int);
  bool forshadowing_over_9000(int,int,Fl_Color,bool);
  void check_impossible(int,int,bool);
    
};

#endif