#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Text_Display.H>  //For the text display
#include<Fl/Fl_Button.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include "const.h"
#include<array>

using namespace std;


class ScoreBoard_Window:public Fl_Window{
    public:
      ScoreBoard_Window():Fl_Window(440,440,"ScoreBoard"){
      }
      void can_show(){
          this->show();
      }
      void can_hide(){
          this->hide();
      }     

};


class Intro_Window : public Fl_Window{   //Temporary class for introduction_window; To be remodeled later;
   bool show_score_board=false;
   bool start_the_game=false;
   ScoreBoard_Window* sc;
   public:
    Intro_Window() : Fl_Window(500, 500, 500, 500, "Candy Try") {
        Fl_Text_Display* disp = new Fl_Text_Display(70,70,300,100,0);
        Fl_Text_Buffer* buff = new Fl_Text_Buffer();
        disp->buffer(buff);    //Sets the text in the buffer to be displayed;
        buff->text("Esteban Matricule: later\nVlad Matricule: later\nCandy Try");           //Inserts text in the buffer ex: "line1\nline2"

        Fl_Button* scoreboard = new Fl_Button(20, 350, 100, 100, "Scoreboard");            //Buttons for the scoreboard and start the game.
        Fl_Button* start_game = new Fl_Button(300,350,120,120,"Start The Game");
        scoreboard->callback((Fl_Callback*)score_board,this);
        
       
    }
    static void score_board(Fl_Widget* obj,void* v){
        Intro_Window* ptr=(Intro_Window*)v;
        if(ptr->get_score()){ptr->show_score_board=false;}else{ptr->show_score_board=true;}
        if(ptr->get_score()){
            ptr->sc->can_show();
        }
        else{
            ptr->sc->can_hide();
        }
        
        
    }
    
    static void start_game(Fl_Widget* obj,void*v){

    }

    bool get_score(){return show_score_board;}
    bool get_game(){return start_the_game;}
    void set_sc(ScoreBoard_Window& e){
        sc=&e;
    }
    

    
};


int main(int argc, char *argv[]){
    Intro_Window window;
    window.show(argc, argv);
    ScoreBoard_Window wd;
    window.set_sc(wd);
    return Fl::run();
}
