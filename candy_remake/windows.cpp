#include <iostream>
#include "wrapper.h"
#include "score.h"
#include "rectangle.h"
#include "canvas.h"
#include "const.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Text_Display.H>
#include <random>

using namespace std;



class MainWindow : public Fl_Window {
    Canvas canva;
    public:
    void can_show(){
        this->show();
    }

    void can_hide(){
        this->hide();
    }
    
    Canvas& get_canvas(){
        return canva;
    }

    MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Candy_Try") {
        Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
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
            case FL_DRAG:
                canva.mouseDrag(Point{Fl::event_x(),Fl::event_y()});
                return 1;
            case FL_RELEASE:
                canva.mouseRelease(Point{Fl::event_x(),Fl::event_y()});
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


class Intro_Window : public Fl_Window{                                                      //Temporary class for introduction_window; To be remodeled  and completed later;
    bool start_the_game=false;
    MainWindow* mw;
    int current_map=0; //int to know the pos in the maps

public:
    Intro_Window() : Fl_Window(500, 500, 500, 500, "Candy Try") {
        Fl_Box* txt_display = new Widget_wrapper<Fl_Box>(70,70,300,100,"Esteban Matricule: 514341\nVlad Matricule: 515147\nCandy Try");
        Score_board* test_button = new Score_board(0,0,120,120);            //Esteban:instead of insisting with clmultihtreading I think drawing a Box would be smarter
        static Fl_Button* start_game = new Fl_Button(300,350,120,120,"Start The Game");     //Button to the the game.
        start_game->callback((Fl_Callback*)start_game_candy,this);     

        static Fl_Button* prev = new Fl_Button(50,370,70,70,"Prev Map");  //Buttons for the prev map and next map
        static Fl_Button* next = new Fl_Button(120,370,70,70,"Next Map");
        static Fl_Button* create_map=new Fl_Button(70,280,80,80,"Create map");
        
        prev->callback((Fl_Callback*)prev_map,this);
        next->callback((Fl_Callback*)next_map,this);
        create_map->callback((Fl_Callback*)create,this);

    }
    
    static void create(Fl_Widget* obj,void*v){
        Intro_Window* ptr=(Intro_Window*)v;
        ptr->create_the_map();
    }
    void create_the_map(){
        mw->get_canvas().edit_level();
    }

    static void prev_map(Fl_Widget*obj,void*v){
        Intro_Window* ptr=(Intro_Window*)v;
        ptr->select_prev_map();
    }
    
    void select_prev_map(){
        if(current_map-1>=0){
            current_map--;
            mw->get_canvas().make_board(maps[current_map]);
        }
    }

    static void next_map(Fl_Widget* obj,void*v){
      Intro_Window* ptr=(Intro_Window*)v;
      ptr->select_next_map();
    }
    
    void select_next_map(){
      if(current_map+1<5){
          current_map++;
          mw->get_canvas().make_board(maps[current_map]);
      }
    }

    static void make_score_board(){
    }

    static void start_game_candy(Fl_Widget* obj,void*v){                                    //Function to toggle on/off the game window
        Intro_Window* ptr=(Intro_Window*)v;
        if(ptr->get_game()){
            ptr->start_the_game=false;
        }else{
            ptr->start_the_game=true;
        }

        if(ptr->get_game()){                    //Esteban:if you manually close the window you have to double click on start the game to open it again
            ptr->mw->can_show();
            // ptr->start_the_game = false;
        }else{
            ptr->mw->can_hide();
        }
    }

    bool get_game(){
        return start_the_game;
    }    
    void set_mc(MainWindow& w){
        mw=&w;
    }

};


int main(int argc, char *argv[]){
    srand(time(NULL)); //Vlad: Seed to make rand() work
    Intro_Window window;
    window.show(argc, argv);
    MainWindow mw;;
    window.set_mc(mw);
    return Fl::run();
}