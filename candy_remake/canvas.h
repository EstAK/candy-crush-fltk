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
    int next_level=0;
    bool has_moved=false;
    shared_ptr<Item>**candy=new shared_ptr<Item>*[9]();
    vector<string> lines;
    shared_ptr<Item> current=make_shared<Item>(); //stocks the current cell clicked on. esteban: might cause an issue somewhere as base cas is now FL_BLACK
    int x=0;int y=0; //currents coord in the array
    Score candy_score{0}; //By default the score begins at 0
    bool not_impossible=false;
    
    int timer=0;  //Var used for timer
    bool can_vibrate=false;
    bool editing=false;
    bool done=true;
    string selected="";
    string current_map;
    GameManager gm;
    Hint ht;
    Objective game_obj;
    bool has_released = true;
    Point curr_pos;
    vector<Fl_Boxtype> boxes={FL_FLAT_BOX,FL_ROUNDED_BOX,FL_DIAMOND_BOX,FL_PLASTIC_UP_BOX};
    

 public:
   Canvas(); 
   ~Canvas();
   void make_board(string);
   void read_file(string ); 
   void draw();
   void mouseMove(Point);
   //works in pair
   void mouseDrag(Point);
   void mouseRelease(Point);
   //end of drag pair
   void mouseClick(Point);
   void set_the_neighbours();
   bool is_board_moving();
   bool special_neigh(int,int);
   void edit_level();
   void save_the_map();
   
   
};

#endif