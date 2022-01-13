/*
name : candy crush
author : Vlad Marian Moruntale & Aguililla Klein Esteban
date : 13/01/22
*/
#include<iostream>
#include"score.h"
#include"wrapper.h"
#include "const.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include<fstream>
using namespace std;


Score::Score(int score=0):current_score(score){ //Init score at 0.

}

void Score::set_score(int new_score){
    //Updates score after candies have been destroyed.
    current_score+=new_score;  
        if(best_score<=current_score){ //If the current score is better than the best score than we update the best_score
            best_score=current_score;
        }
    ifstream myfile(score_file);                           
    while(myfile>>best_score){                                          
    }
    myfile.close();
}

int Score::get_score(){return current_score;}
int Score::get_best_score(){return best_score;}

void Score::set_best_score(int score){
    fstream file(score_file, ios::out);
    file<<(to_string(score));
    file.close();
}

Score_board::Score_board(int posx, int posy, int height, int width){
        read_score_file(); //Reads bestScore from the txt file.
        pt=letsgo.c_str(); //Now it works.
        Fl_Box* box_window = new Widget_wrapper<Fl_Box>(posx, posy, height, width,pt);
    }

void Score_board::read_score_file(){
        
        string temp_res="TOP SCORE\n", line;
        ifstream myfile(score_file);                           
        while(myfile>>line){                                          
            temp_res+=line+"\n";
        }
        myfile.close();
        letsgo=temp_res;                                                                                                                                      
    }