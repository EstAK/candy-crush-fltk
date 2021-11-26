#include<iostream>
#include<fstream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
using namespace std;

#ifndef SCORE_H_
#define SCORE_H_

class Score{
    int current_score=0;
    int best_score=0; 
public:
    Score(int);
    ~Score()=default;

    //Setters
    void set_score(int);


    //Getters
    int get_score();
    int get_best_score();

};

class Score_board {
 
    const char* pt;
    string letsgo;
public:
    Score_board(int, int , int , int);
    void read_score_file();                                         
    ~Score_board(){
        cout<<"refreshing scores"<<endl;
    }
};

#endif