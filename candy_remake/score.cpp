#include<iostream>
#include"score.h"
#include"wrapper.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include<fstream>
using namespace std;
const std::string score_file = "score_file.txt";

Score::Score(int score=0):current_score(score){

}

void Score::set_score(int new_score){
    current_score+=new_score;
        if(best_score<=current_score){ //If the current score is better than the best score than we update the best_score
            best_score=current_score;
        }
}

int Score::get_score(){return current_score;}
int Score::get_best_score(){return best_score;}


Score_board::Score_board(int posx, int posy, int height, int width){
        read_score_file();
        pt=letsgo.c_str(); //Now it works.
        Fl_Box* box_window = new Widget_wrapper<Fl_Box>(posx, posy, height, width,pt); //Try to make it a pointer rather than auto bcs bad use i think.
    }

void Score_board::read_score_file(){
        
        string temp_res="TOP SCORE\n", line;
        ifstream myfile(score_file);                           
        int i = 1;
        while(myfile>>line){                                          
            temp_res+=to_string(i)+".\t"+line+"\n";
            i++;
        }
        myfile.close();
        letsgo=temp_res;                                                                                                                                      
    }