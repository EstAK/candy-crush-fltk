#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include "const.h"
#include <array>
#include<unistd.h>
// #include <functional> //not needed for now but might become handy

#if __cplusplus >= 202002L
#include <numbers>
using std::numbers::pi;
#else
const double pi=3.141592653589793238462643383279502884L;
#endif

using namespace std;


struct Point{int x; int y;};

//class declaration Esteban: the code is becoming a total clusterfuck
//we should move it to separate files because I don't even where to put     
//new classes without causing a billion class not found because written 
//before but cannot write before otherwise code goes full kamikaze mode

//Vlad: Roger Roger

class Rectangle {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  
  Fl_Color getFillColor() {
    return fillColor;
  }
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
    void draw() {
      fl_draw_box(FL_FLAT_BOX, center.x-w/2, center.y-h/2, w, h, fillColor);
      fl_draw_box(FL_BORDER_FRAME, center.x-w/2, center.y-h/2, w, h, frameColor);
    }

    void setFillColor(Fl_Color newFillColor) {
      fillColor = newFillColor;
    }

    void setFrameColor(Fl_Color newFrameColor) {
      frameColor = newFrameColor;
    }

    bool contains(Point p) {
      return p.x>=center.x-w/2 &&
             p.x<center.x+w/2 &&
             p.y>=center.y-h/2 &&
             p.y<center.y+h/2;
    }

};

class Circle {
  Point center;
  int r;
  Fl_Color fillColor, frameColor;
 public:
  Circle(Point center, int r,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_WHITE);
  void draw();
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }
  bool contains(Point p);
  Point getCenter() {
    return center;
  }
};

Circle::Circle(Point center, int r,
                     Fl_Color frameColor,
                     Fl_Color fillColor):
  center{center}, r{r}, fillColor{fillColor}, frameColor{frameColor} {}

void Circle::draw() {
  array<Point,37> points;
  for (int i=0;i<36;i++)
    points[i]={static_cast<int>(center.x+r*sin(i*10*pi/180)),
      static_cast<int>(center.y+r*cos(i*10*pi/180))};
  points[36]=points[0];
  fl_color(fillColor);
  fl_begin_polygon();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_polygon();
  fl_color(frameColor);
  fl_begin_line();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_line();
}

void Circle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}

void Circle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}

bool Circle::contains(Point p) {
  return ( (p.x-center.x)*(p.x-center.x)
          +(p.y-center.y)*(p.y-center.y)<=r*r);
}

struct Pop{
    Pop(int x, int y, int w, int h){
        fl_draw_box(FL_BORDER_FRAME, x, y, w, h, FL_BLACK);
        fl_push_matrix();
    }
    ~Pop(){
        fl_pop_matrix();
    }
};

struct Slide{
    Slide(Point p, Rectangle *r){
        fl_push_matrix();
        r->setCenter(p);
    }
    ~Slide(){
        fl_pop_matrix();
    }
};


class Animation{
protected:
    Rectangle* r;
    int time{0};
    int animation_time;
public:
    bool is_complete(){
        return time>animation_time;
    }

    void set_animation_time(int t){
        animation_time=t;
    }

    void set_rectangle(Rectangle* rect){
        r=rect;
    }
};


class Animation_slide: public Animation{
    //esteban: inheriting this class from a base class having thoseg private instances might be a good idea as they are nearly the same as in Animation_pop
    Point initial_pos;
    Point destination;
    int distance_x = 0;
    int distance_y = 0;
    int speed = 1;

public:
    Animation_slide(Rectangle* candy_to_animate, Point d, int animation_time=50): 
    initial_pos{candy_to_animate->getCenter()} , destination{d} {

       
  
        set_animation_time(animation_time*speed);
        set_rectangle(candy_to_animate);

        // initializing should be done the constructor initializer but it's more readable this way
        
        distance_x = (destination.x - r->getCenter().x)/animation_time; // x distance per animation frame
        distance_y = (destination.y - r->getCenter().y)/animation_time; // y distance per animation frame


    }

    ~Animation_slide(){
        r->setCenter(initial_pos);
        
    }

    void draw(){
        time++;

        Slide s ({initial_pos.x+distance_x*time, initial_pos.y+distance_y*time}, r);

        r->draw();
    }

};


class Animation_pop: public Animation{
    int spread_coeff = 10;
public:

    Animation_pop(Rectangle* candy_to_animate,int animation_time = 25){
        set_animation_time(animation_time);
        set_rectangle(candy_to_animate);
    }

    ~ Animation_pop(){
        sleep(0.5);
    }
    void draw() {
        ++time;
        
        int w = log(r->getWidth()*time)*spread_coeff;
        int h = log(r->getHeight()*time)*spread_coeff;

        Pop t{r->getCenter().x-w/2, r->getCenter().y-h/2, w, h};        //WOP i still have to make it pretty
        
        r->draw();
    }

};


class Score{
    int current_score=0;
    int best_score=0; 
public:
    Score(int score=0):current_score(score){}
    ~Score()=default;

    //Setters
    void set_score(int new_score){
        current_score+=new_score;
        if(best_score<=current_score){ //If the current score is better than the best score than we update the best_score
            best_score=current_score;
        }
    }


    //Getters
    int get_score(){return current_score;}
    int get_best_score(){return best_score;}

};

class Objective{
  int number_of_tries=0; //Vlad: there must be a max number of movements
  string poss_objectives[3]={"Break->x->candies","Break->x->candies->color","Break x->cubs of ice"}; // Vlad: More to be added later.
  string selected_obj;  //The obj that will be selected.
  int number_to_break=0;
  string condition; //If there is a condition (like candies need to be of a certain color or ice cubs or etc)
  Fl_Color color[6]={FL_RED,FL_BLUE,FL_YELLOW,FL_BLACK,FL_DARK_CYAN,FL_GREEN};
  bool obj_completed=false;
public:
  Objective(){
      selected_obj=poss_objectives[rand()%3];
      number_of_tries=25+(rand()%(40-25+1)); //Generate a number max between 25-40 mvts
      setUp();
  }
  void setUp(){ //Set up the objective
     if(selected_obj=="Break->x->candies"){
       number_to_break=25+(rand()%(40-25+1));
       condition="nothing";
     }else if(selected_obj=="Break->x->candies->color"){
         number_to_break=25+(rand()%(40-25+1));
         condition=to_string(color[rand()%6]);
     }else if(selected_obj=="Break x->cubs of ice"){
         number_to_break=25+(rand()%(40-25+1));
         cout<<"Not implemented yet"<<endl; //Vlad: To be continued.
     }
    
  }
   
  void fruits_on(int fruits){
      selected_obj="fruits on";
      number_of_tries=25+(rand()%(40-25+1));
      number_to_break=fruits;
  }

  void inc_fruits(){
      number_to_break++;
  }

  void dec_fruits(){
      number_to_break--;
  }

  bool constant_check(){ //Method to check if the player has completed the obj
      if(number_of_tries==0 && !obj_completed){
          cout<<"Game lost"<<endl;
          return false;
      }else if(number_of_tries==0 && obj_completed){
          cout<<"Game"<<endl;
          return true;
          // TODO: Change to the next map;
      }else if(obj_completed){
          cout<<"Game Won"<<endl;
          return true;
          //TODO: Change to the next map;
      }
      return false;
  }

  void mv_done(int nr,int nr_of_candies,Fl_Color candy_color=FL_WHITE){ //1 movement has been done.
     number_of_tries-=nr;
     if(nr!=0){cout<<number_of_tries<<" tries left"<<endl;} //Vlad: little print
     if(condition=="nothing"){
         number_to_break-=nr_of_candies;
         objCompleted();
     }else if(selected_obj=="Break->x->candies->color"){
          if(to_string(candy_color)==condition){
              number_to_break-=nr_of_candies;
              cout<<number_to_break<<" candies left to break of color "<<condition<<" left"<<endl;
              objCompleted();
          }
     }
     else if(selected_obj=="Break x->cubs of ice"){

     }
     else if(selected_obj=="fruits on"){
         objCompleted();
     }
  }
  
   void objCompleted(){ //Method to set the obj_completed to true if the obj has benn completed
       if(number_to_break<=0 && number_of_tries>=0){
           obj_completed=true;
       }
   }
   
   int nr_breaks(){return number_to_break;}
   int nr_tries(){return number_of_tries;}
};

class Map_Editor{
  string new_map[9][9];
public:
  Map_Editor(){
      cout<<"c=candy with random color"<<endl;
      cout<<"w=wall"<<endl;
      cout<<"below is an example of a map"<<endl;
      for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<<"c";
        }
        cout<<endl;
      }
      cout<<endl;
      create_level();
  }

  void create_level(){
      cout<<"Put a space after each letter"<<endl;
      for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin>>new_map[i][j];
        }
      }
      write_the_map();
  }

  void write_the_map(){
       ofstream fw;
       fw.open("custom_map.txt");
       for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            fw<<new_map[i][j];
        }
        fw<<"\n";
       }
       cout<<"Map finished you can load the game"<<endl;
    }

};

class Candy:public Rectangle{
    bool wall=false; //Vlad:Use this to know which rectangle(candy) is a wall, no need for a whole class with a constructor for this in my opinion.
    Animation_pop* animation_pop;           //not templating because every Candy has to have those 2 animations
    Animation_slide* animation_slide;
    Circle* c;
    bool is_fruit=false;
public:
    vector<Candy*> neighbours;
    Candy(){} //Dummy Constructor
    Candy(Point center, int w, int h,Fl_Color fillColor = FL_WHITE, Fl_Color frameColor = FL_BLACK, 
            Animation_pop* pop=nullptr, Animation_slide* slide=nullptr): animation_pop{pop}, animation_slide{slide} {
        setCenter(center);
        setWidth(w);
        setHeight(h);
        setFrameColor(frameColor);
        setFillColor(fillColor);
    }
    bool verify_neighbours(Candy current){
        if(this->get_wall()==true){return false;}
        if(this->get_fruit() && current.get_fruit()){return false;}
        
        for(auto i:neighbours){
            if(i->getCenter().x==current.getCenter().x && i->getCenter().y==current.getCenter().y){
                if(!this->get_fruit() && !current.get_fruit()){
                Fl_Color save=this->getFillColor();    //TODO : Add the animation code here.
                this->setCode(i->getFillColor());
                i->setCode(save);   
                return true;
                }else{
                    if(this->get_fruit()){
                    Fl_Color save=this->getFillColor();
                    this->set_fruit(false);
                    i->set_fruit(true);
                    this->setCode(i->getFillColor());
                    i->setCode(save);
                    }else{
                        
                         
                    }
                }
            }
        }
        return false;
    }

    void set_wall(bool wa){wall=wa;}
    bool get_wall(){return wall;}
    void set_fruit(bool fr){is_fruit=fr;}
    bool get_fruit(){return is_fruit;}

    void start_pop_animation(){
        animation_pop = new Animation_pop(this);
    }

    void start_slide_animation(Point dest){
        animation_slide = new Animation_slide(this, dest);
    }

    bool is_slide_complete(){
        // checks wheter or not the slide animaiton is complete or non existant
        if (animation_slide){
            return animation_slide->is_complete();
        }else{
            return true;
        }
    }

    void draw(){    //redifining rectangle animation in candy
        if(is_fruit){
            c=new Circle({getCenter().x,getCenter().y},20,getFrameColor(),getFillColor());
            c->draw();
            return;
        }

        if (animation_pop && animation_pop->is_complete()){
            delete animation_pop;
            animation_pop = nullptr;
        }
        if (animation_slide && animation_slide->is_complete()){
            delete animation_slide;
            animation_slide = nullptr;
        }

        if (animation_slide){
            animation_slide->draw();
        }else if (animation_pop) {
            animation_pop->draw();
        }else{
            Rectangle::draw();
        }
    }
}; 


class Canvas{
    bool has_moved=false;
    array<array<Candy,9>,9> candy;            //2d Array
    Fl_Color color[5]={FL_RED,FL_BLUE,FL_YELLOW,FL_DARK_CYAN,FL_GREEN};
    vector<string> lines;
    Candy current{{0,0},0,0}; //stocks the current cell clicked on. esteban: might cause an issue somewhere as base cas is now FL_BLACK
    int x=0;int y=0; //currents coord in the array
    Candy* point_current=&current;
    Score candy_score; //By default the score begins at 0
    bool not_impossible=false;
    
    int time=0;  //Var used for timer
    bool can_vibrate=false;
    string current_map=maps[0];

    Objective game_obj;
public:
    Canvas(Point center={100,100},int wi=850,int hi=850){                           //wi and hi are not used x think we should remove them ; Vlad:Okey;
        make_board(maps[0]); //Vlad: Made the board in a method so when impossible is true than the board can be remaked later.
        
    }

    
    void make_board(string map){ //TODO: Finish it later.
        read_file(map);
        current_map=map;
        candy_score=Score();
        int fruits=0;
        bool gj=false;
        
        for (int x = 0; x<9; x++){
            for (int y = 0; y<9; y++){
              
                if (lines[y][x] == *c){
                    candy[x][y]=(Candy{{50*x+25, 50*y+25}, 40, 40, color[rand()%5]});
                }
                else if(lines[y][x]== *w){
                    candy[x][y]=(Candy{{50*x+25, 50*y+25}, 40, 40,FL_MAGENTA,FL_BLACK});
                }
                else if(lines[y][x]== *i){
                    candy[x][y]=(Candy{{50*x+25, 50*y+25}, 40, 40,FL_RED});
                    candy[x][y].set_fruit(true);
                    fruits++;
                    gj=true;
                }
            }
        } 
            if(gj){game_obj.fruits_on(fruits);}else{game_obj=Objective();}
            set_the_neighbours();
            set_the_wall();
        
    }

    void read_file(string map){
        lines.clear();  //Clear the map so the next one can be loaded in the vector.
        string line;                        //temporary input var
        ifstream myfile(map);
        current_map=map;
        while (myfile >> line) {
            lines.push_back(line);
        }
        myfile.close(); 
        
    }

    void draw(){
        for(int i=0;i<candy.size();i++){
            for(int j=0;j<candy[0].size();j++){
                candy[i][j].draw();
                if(candy[i][j].get_wall()!=true && candy[i][j].get_fruit()==false){
                    break_candy(i,j,0,0,true);} //Checks all the candies all the time and breaks them if it must.
                    check_impossible(i,j,can_vibrate);
            }
        } 

        for(int i=0;i<candy.size();i++){
            if(candy[i][8].get_fruit()){
                game_obj.dec_fruits();
                candy[i][8].set_fruit(false);
                candy[i][8].setFillColor(FL_BLACK);
            }
        }

        if(not_impossible){
            not_impossible=false;

        }else{
            make_board(current_map);
        }

        if(game_obj.constant_check()){ //Always checks if the obj has been completed
             game_obj=Objective(); //New obj
             //TODO makeboard(next_map)
 
        }else if(!game_obj.constant_check() && game_obj.nr_tries()==0){
            make_board(current_map);
            game_obj=Objective();
        } 

        string obj=to_string(game_obj.nr_breaks());
        string tries=to_string(game_obj.nr_tries());
        string score=to_string(candy_score.get_score());

        fl_draw(obj.c_str(),450,100); //Vlad: Text print of how many blocks it has to break;
        fl_draw("to break",450,120); //Vlad: Make it prettier yourself :(
        fl_draw(tries.c_str(),450,170);
        fl_draw("tries left",450,190);
        fl_draw(score.c_str(),450,210);
        fl_draw("score",450,230);

        if(time!=200){  //Timer set ; Vlad: pressing on a candy or when candies fall/break will restart the timer.
            time++;
        }else{          //restart the timer.
            time=0;
            can_vibrate=true;
        }

        for(int i=0;i<candy.size();i++){ //Checks if there is a free place that can be filled ; Takes into consideration the walls(phyics) as well.
            for(int j=0;j<candy[0].size();j++){
                if(candy[i][j].getFillColor() == FL_BLACK){
                    if (not has_moved || candy[i][j].is_slide_complete()){
                        fall_candies(i,j);
                        time=0; //reset the timer.
                    }
                    can_vibrate=false; //reset the vibrate;
                }
            }
        }  
    }

    void edit_level(){
        Map_Editor mp;
        make_board("custom_map.txt");
        game_obj=Objective();
    }

    void check_impossible(int i,int j,bool vibrate){
        Fl_Color save=candy[i][j].getFillColor();
        if(candy[i][j].get_wall()){return;}
        if(candy[i][j].get_fruit()){return;}
        candy[i][j].setFillColor(FL_WHITE); //Set it temp white(colorless) so the algo doesn't include this one when it's forshadowing this candy.
        if(i+1<candy.size() && !candy[i+1][j].get_wall() && !candy[i+1][j].get_fruit()){
            if(forshadowing_over_9000(i+1,j,save,vibrate)){
               not_impossible=true;
               if(vibrate){
                   candy[i][j].start_pop_animation();
                   can_vibrate=false;
               } //Vibrate then turn it off.
            }
        }

        if(j+1<candy[0].size() && !candy[i][j+1].get_wall() && !candy[i][j+1].get_fruit()){
             if(forshadowing_over_9000(i,j+1,save,vibrate)){
                not_impossible=true;
                 if(vibrate){
                   candy[i][j].start_pop_animation();
                   can_vibrate=false;
               }
             }
        }

        if(i-1>=0 && !candy[i-1][j].get_wall() && !candy[i-1][j].get_fruit()){
            if(forshadowing_over_9000(i-1,j,save,vibrate)){
               not_impossible=true;
                if(vibrate){
                   candy[i][j].start_pop_animation();
                   can_vibrate=false;
               }
            }
        }

        if(j-1>=0 && !candy[i][j-1].get_wall() && !candy[i][j-1].get_fruit()){
            if(forshadowing_over_9000(i,j-1,save,vibrate)){
               not_impossible=true;
                if(vibrate){
                   candy[i][j].start_pop_animation();
                   can_vibrate=false;
               }
            }
        }
        
        candy[i][j].setCode(save); //Sets the color back.
    }

    bool forshadowing_over_9000(int x,int y,Fl_Color color,bool vibrate){ //Vlad: just change it's name later.... IT'S OVER 9000!!!
       //Vlad: Prototype method; I will change break_candies in order to work with this as well when i will have the mood -_-
       Candy temp_candy=candy[x][y]; 
       temp_candy.setCode(color);
       int counter_left_right=1; 
       int counter_up_down=1;
        for(int i=x+1;i<candy.size();i++){  //Counter the same candies on the right line of the same color; counter begins of 1
            if(candy[i][y].getFillColor()!=temp_candy.getFillColor() || candy[i][y].get_fruit()){
                break;
            }else{
                counter_left_right+=1;
            }
        }
        

        for(int i=y+1;i<candy[0].size();i++){  //Counts the candies under.
            if(candy[x][i].getFillColor()!=temp_candy.getFillColor() || candy[x][i].get_fruit()){
                break;
            }else{
                counter_up_down+=1;
            }
        }
  
        for(int i=y-1;i>=0;i--){  //Counts the candies upwards
            if(candy[x][i].getFillColor()!=temp_candy.getFillColor() || candy[x][i].get_fruit()){
                break;
            }else{
                counter_up_down+=1;
            }
        }
       
        for(int i=x-1;i>=0;i--){  //Left
            if(candy[i][y].getFillColor()!=temp_candy.getFillColor() || candy[i][y].get_fruit()){
                break;
            }else{
                counter_left_right+=1;
            }
        }

          
        if(counter_left_right<3 && counter_up_down<3 ){ 
            return false; 
        }else{
            if(vibrate){
                if(counter_up_down>=3){
                  for(int i=y-1;i>=0;i--){
                     if(candy[x][i].getFillColor()==temp_candy.getFillColor()){
                         candy[x][i].start_pop_animation();
                     }else{break;}
                  }
                  for(int i=y+1;i<candy[0].size();i++){
                      if(candy[x][i].getFillColor()==temp_candy.getFillColor()){
                          candy[x][i].start_pop_animation();
                      }else{break;}
                  }
                }else{
                    for(int i=x-1;i>=0;i--){
                        if(candy[i][y].getFillColor()==temp_candy.getFillColor()){
                            candy[i][y].start_pop_animation();
                        }else{break;}
                    }
                    for(int i=x+1;i<candy.size();i++){
                        if(candy[i][y].getFillColor()==temp_candy.getFillColor()){
                            candy[i][y].start_pop_animation();
                        }else{break;}
                    }
                }         
                return true;

            }else{
                return true;
            }
        }
           
    } 

    int fall_candies(int start_x,int start_y){ //Function by Recursion
        if(start_y<0){
            return 0;
        }
        if(candy[start_x][start_y].get_wall()){
            fall_walls(start_x,start_y);
            return 0;
        } //If it's a wall then make candies fall from the diag with fall_walls()
        if(start_y-1>=0){

            int s_y=start_y-1;
            if(candy[start_x][s_y].getFillColor() != FL_BLACK && candy[start_x][s_y].get_wall()!=true){ //If there is a candy above and is not a wall.
                Point fall{candy[start_x][s_y].getCenter().x,candy[start_x][s_y].getCenter().y+50}; //that Candy will fall down and it's place will be liberated.
                candy[start_x][start_y].setCenter(fall); //Candy falls down
                candy[start_x][start_y].setCode(candy[start_x][s_y].getFillColor());
                candy[start_x][start_y].set_fruit(candy[start_x][s_y].get_fruit());
                candy[start_x][s_y].setFillColor(FL_BLACK); //Place liberated of the candy that fell Down.
                candy[start_x][s_y].set_fruit(false);
                set_the_neighbours(); //Reset the neig because new candies have been created.
                fall_candies(start_x,s_y); //Continue the Rec.
            }else{
                fall_candies(start_x,s_y); //Else we will search more above.
            }

        }else{  //We know that we reached the top we want to generate a random candy in the top but we need the position of the column first.
            if(candy[start_x][start_y+1].getFillColor() != FL_BLACK && candy[start_x][start_y+1].get_wall()!=true){
                Point fall{candy[start_x][start_y+1].getCenter().x,candy[start_x][start_y+1].getCenter().y-50};
                candy[start_x][start_y].setCenter(fall);
                candy[start_x][start_y].setFillColor(color[rand()%5]);
                set_the_neighbours();
            }else if(start_x+1<candy.size() && candy[start_x+1][start_y].get_wall()!=true){
                Point fall{candy[start_x+1][start_y].getCenter().x-50,candy[start_x+1][start_y].getCenter().y}; //Get the pos in using -50 the right-neigh position.
                candy[start_x][start_y].setCenter(fall);
                candy[start_x][start_y].setFillColor(color[rand()%5]);
                set_the_neighbours();
            }else if(start_x-1>=0 && candy[start_x-1][start_y].get_wall()!=true){
                Point fall{candy[start_x-1][start_y].getCenter().x+50,candy[start_x-1][start_y].getCenter().y}; //Get the pos in adding +50 the left-neigh position.
                candy[start_x][start_y].setCenter(fall);
                candy[start_x][start_y].setFillColor(color[rand()%5]);
                set_the_neighbours();
            }
        }
           
        return 0;
    }
    
    void fall_walls(int start_x, int start_y){ //Function to make the candies fall in diag if there is a wall above.
        if(candy[start_x-1][start_y].getFillColor() != FL_BLACK ){ //make the candy in the right diag fall.
            Point fall{candy[start_x][start_y].getCenter().x,candy[start_x][start_y].getCenter().y+50};
            candy[start_x][start_y+1].setCenter(fall);
            candy[start_x][start_y+1].setCode(candy[start_x-1][start_y].getFillColor());
            candy[start_x-1][start_y].setFillColor(FL_BLACK);
            set_the_neighbours();
            fall_candies(start_x-1,start_y);
        }else{ //If no candy in the right diag then we take the one from the left diag
            Point fall{candy[start_x][start_y].getCenter().x,candy[start_x][start_y].getCenter().y+50};
            candy[start_x][start_y+1].setCenter(fall);
            candy[start_x][start_y+1].setCode(candy[start_x+1][start_y].getFillColor());
            candy[start_x+1][start_y].setFillColor(FL_BLACK);
            set_the_neighbours();
            fall_candies(start_x+1,start_y);
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
        has_moved=true;
        for(int i=0;i<candy.size();i++){
            for(int j=0;j<candy[0].size();j++){
                if(candy[i][j].contains(mouseLoc) && current.getFillColor() == FL_BLACK){
                    if(candy[i][j].get_wall()==false ){  //make sure that is not a wall. ; Vlad:time n vibrate
                        current=candy[i][j];
                        x=i;
                        y=j;
                        time=0;
                        can_vibrate=false;
                    }
                }else if(candy[i][j].contains(mouseLoc)){
                    if(candy[i][j].verify_neighbours(current)){
                        candy[x][y].start_slide_animation(candy[i][j].getCenter());
                        candy[i][j].start_slide_animation(candy[x][y].getCenter());
                        break_candy(i,j,x,y);
                        if(candy[x][y].get_fruit()){
                            if(!forshadowing_over_9000(i,j,candy[i][j].getFillColor(),false)){
                                Fl_Color save=candy[x][y].getFillColor();
                                candy[x][y].set_fruit(false);
                                candy[x][y].setCode(candy[i][j].getFillColor());
                                candy[i][j].set_fruit(true);
                                candy[i][j].setCode(save);
                            }
                        }
                        current.setFillColor(FL_BLACK);
                        x=0;
                        y=0;
                        cout<<"Neigh"<<endl;
                        time=0;
                        can_vibrate=false;
                    }else{
                        cout<<"Selected new one"<<endl;
                        if(candy[i][j].get_wall()==false){  //timer n vibrate
                            current=candy[i][j];
                            x=i;
                            y=j;
                            time=0;
                            can_vibrate=false;
                        }    
                    }
                }
            }
        }
   }
    
    void break_candy(int x,int y,int i,int j,bool pc=false){  //Function that will break the candies if possible if not it will revert the movement back (but no delay added so it is instant)
        
        Candy temp_candy=candy[x][y];  
        Candy temp_candy2=candy[i][j];
        int counter_left_right=1; 
        int counter_up_down=1;
        int counter_left_right2=1;
        int counter_up_down2=1;

        for(int i=x+1;i<candy.size();i++){  //Counter the same candies on the right line of the same color; counter begins of 1
            if(candy[i][y].getFillColor()!=temp_candy.getFillColor() || candy[i][y].get_fruit()){
                break;
            }else{
                counter_left_right+=1;
            }
        }

        for(int k=i+1;k<candy.size();k++){  //Counter the same candies on the right line of the same color; counter begins of 1 for temp_candy2
            if(candy[k][j].getFillColor()!=temp_candy2.getFillColor() || candy[k][j].get_fruit()){
                break;
            }else{
                counter_left_right2+=1;
            }
        }

        for(int i=y+1;i<candy[0].size();i++){  //Counts the candies under.
            if(candy[x][i].getFillColor()!=temp_candy.getFillColor() || candy[x][i].get_fruit()){
                break;
            }else{
                counter_up_down+=1;
            }
        }

        for(int k=j+1;k<candy[0].size();k++){  //Counts the candies under. 2
            if(candy[i][k].getFillColor()!=temp_candy2.getFillColor() || candy[i][k].get_fruit()){
                break;
            }else{
                counter_up_down2+=1;
            }
        }
      
        for(int i=y-1;i>=0;i--){  //Counts the candies upwards
            if(candy[x][i].getFillColor()!=temp_candy.getFillColor() || candy[x][i].get_fruit()){
                break;
            }else{
                counter_up_down+=1;
            }
        }
       
        for(int k=j-1;k>=0;k--){  //Counts the candies upwards 2
            if(candy[i][k].getFillColor()!=temp_candy2.getFillColor() || candy[i][k].get_fruit()){
                break;
            }else{
                counter_up_down2+=1;
            }
        }


        for(int i=x-1;i>=0;i--){  //Left
            if(candy[i][y].getFillColor()!=temp_candy.getFillColor() || candy[i][y].get_fruit()){
                break;
            }else{
                counter_left_right+=1;
            }
        }

        for(int k=i-1;k>=0;k--){  //Left2
            if(candy[k][j].getFillColor()!=temp_candy2.getFillColor() || candy[k][j].get_fruit()){
                break;
            }else{
                counter_left_right2+=1;
            }
        }
       
       //Vlad: Can you try to add a delay when you do the animations?
        if(counter_left_right<3 && counter_up_down<3 && counter_left_right2<3 && counter_up_down2<3 && !pc){ //Changes back if the movement will not result in a break but i don't know how to add a delay.
            Fl_Color save=candy[i][j].getFillColor();  // TODO : Add The animation code here
            candy[i][j].setCode(candy[x][y].getFillColor());
            candy[x][y].setCode(save);

            
        }
       
        //There will break all the candies in a row; First it will decide which one is better from left<->right or up<-->down in calculation the nomber of candies that will be broken.
        if(counter_left_right>counter_up_down){ //A way to know which one will have the priority.
            if(counter_left_right>=3){
                int start_x=x;
                int start_y=y;
                for(int i=x+1;i<candy.size();i++){  //Counter the same candies on the right line of the same color; counter begins of 1
                    if(candy[i][y].getFillColor()!=temp_candy.getFillColor() || candy[i][y].get_fruit()){
                        break;
                    }else{
                        start_x=i;
                        start_y=y;
                    }
                }
                if(pc){
                    game_obj.mv_done(0,counter_left_right,temp_candy.getFillColor());} //Checks the obj because 1 mvt has been done
                else{
                    game_obj.mv_done(1,counter_left_right,temp_candy.getFillColor());
                }
                for(int i=start_x;i>=0;i--){  //Left
                    if(candy[i][start_y].getFillColor()!=temp_candy.getFillColor() || candy[i][start_y].get_fruit()){
                        break;
                    }else{
                        candy[i][start_y].setFillColor(FL_BLACK);
                        candy[i][start_y].start_pop_animation();

                        candy_score.set_score(counter_left_right); //Temp way to increase the score; TODO: Change it later
                        //cout<<"The score is "<<candy_score.get_score()<<endl;	
                    }
                }
            }
        }else if(counter_up_down>=3){
            int start_x=x;int start_y=y; 
            for(int i=y-1;i>=0;i--){  //Counts the candies upwards
                if(candy[x][i].getFillColor()!=temp_candy.getFillColor() || candy[x][i].get_fruit()){
                    break;
                }else{
                    start_x=x;
                    start_y=i;
                }
            }
            if(pc){
                game_obj.mv_done(0,counter_up_down,temp_candy.getFillColor());} //Checks the obj
            else{
                game_obj.mv_done(1,counter_up_down,temp_candy.getFillColor());
            }
            for(int i=start_y;i<candy[0].size();i++){  //Counts the candies under.
                if(candy[start_x][i].getFillColor()!=temp_candy.getFillColor() || candy[start_x][i].get_fruit()){
                    break;
                }else{
                    candy[start_x][i].setFillColor(FL_BLACK);
                    candy[start_x][i].start_pop_animation();

                    candy_score.set_score(counter_up_down);
    		        //cout<<"The score is "<<candy_score.get_score()<<endl; //Temp way to increase the score; change it later
                }
            }

       }
       
        //Second candy
        if(counter_left_right2>counter_up_down2){ //A way to know which one will have the priority.
            if(counter_left_right2>=3){
                int start_x=i;int start_y=j;
                for(int k=i+1;k<candy.size();k++){  //Counter the same candies on the right line of the same color; counter begins of 1
                    if(candy[k][j].getFillColor()!=temp_candy2.getFillColor() || candy[k][j].get_fruit()){
                        break;
                    }else{
                        start_x=k;
                        start_y=j;
                    }
                }          
                if(pc){
                            game_obj.mv_done(0,counter_left_right2,temp_candy.getFillColor());} //Checks the obj
                        else{
                             game_obj.mv_done(1,counter_left_right2,temp_candy.getFillColor());
                        }
                for(int k=start_x;k>=0;k--){  //Left
                    if(candy[k][start_y].getFillColor()!=temp_candy2.getFillColor() || candy[k][start_y].get_fruit()){
                        break;
                    }else{
                        candy[k][start_y].setFillColor(FL_BLACK);
                        candy[k][start_y].start_pop_animation();

                        
                    }
                }
            }
        }else if(counter_up_down2>=3){
            
            int start_x=i;
            int start_y=j; 
            
            for(int k=j-1;k>=0;k--){  //Counts the candies upwards
                if(candy[i][k].getFillColor()!=temp_candy2.getFillColor() || candy[i][k].get_fruit()){
                    break;
                }else{
                    start_x=i;
                    start_y=k;
                }
            }
            if(pc){
                        game_obj.mv_done(0,counter_up_down2,temp_candy.getFillColor());}//Checks the obj
                    else{
                        game_obj.mv_done(1,counter_up_down2,temp_candy.getFillColor());
                    }

            for(int k=start_y;k<candy[0].size();k++){  //Counts the candies under.
                if(candy[start_x][k].getFillColor()!=temp_candy2.getFillColor() || candy[start_x][k].get_fruit()){
                    break;
                }else{
                    candy[start_x][k].setFillColor(FL_BLACK);
                    candy[start_x][k].start_pop_animation();

                    
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



template<typename Type>
class Widget_wrapper : public Type {
    /* 
        wrapper class abstracting text_window implementation from the intro_window
        TODO : make it pretty
    */
public:
    Widget_wrapper(int posx, int posy, int height, int width, const char* txt): Type(posx, posy, height, width, txt) {}


};

class Score_board {
    // Esteban:Using a global score board might not be relevant anymore when introducing a campaign like level selector as each level should have it's own score

    const char* pt;
    string letsgo;
    void read_score_file(){
        
        string temp_res="TOP SCORE\n", line;                                  //Esteban: I believe that I have found a bug with fltk after spending about 3 hours not understanding
        ifstream myfile(score_file);                            //if not i am just totally dumb 
        int i = 1;
        while(myfile>>line){                                          
            temp_res+=to_string(i)+".\t"+line+"\n";
            i++;
        }
        myfile.close();
        letsgo=temp_res;                                                                                     
                                                 //Vlad:Thats because the return if pretty fcked up if leaved as const char* you can check it later with a print.
    } //Vlad: Try to not make any return const char* or string either (I don't know why but it bugs as fck with the returns and ofstream here).

public:
    Score_board(int posx, int posy, int height, int width){
        read_score_file();
        pt=letsgo.c_str(); //Now it works.
        Fl_Box* box_window = new Widget_wrapper<Fl_Box>(posx, posy, height, width,pt); //Try to make it a pointer rather than auto bcs bad use i think.
    }
    ~Score_board(){
        cout<<"refreshing scores"<<endl;
    }
};


class Intro_Window : public Fl_Window{                                                      //Temporary class for introduction_window; To be remodeled  and completed later;
    bool start_the_game=false;
    MainWindow* mw;
    int current_map=0; //int to know the pos in the maps

public:
    Intro_Window() : Fl_Window(500, 500, 500, 500, "Candy Try") {
        resizable(this);
        Fl_Box* txt_display = new Widget_wrapper<Fl_Box>(70,70,300,100,"Esteban Matricule: later\nVlad Matricule: later\nCandy Try");
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
      if(current_map+1<3){
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