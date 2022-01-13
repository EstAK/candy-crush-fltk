#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>
#include<memory>
#include"candy.h"
#include"rectangle.h"
#include"circle.h"
#include"score.h"
#include"hint.h"

using namespace std;

Hint::Hint(){
    for(int i=0;i<9;i++){
        candy[i]=new shared_ptr<Item>[9]();
    }
}

void Hint::set_up(shared_ptr<Item>** arr,bool& can_vib,bool& not_imp){
    candy=arr;
    can_vibrate=&can_vib;
    not_impossible=&not_imp;
}

void Hint::check_impossible(int i,int j,bool vibrate){
        Fl_Color save=candy[i][j]->getFillColor();
        if(candy[i][j]->is_wall()){return;}
        if(candy[i][j]->is_ingredient()){return;}
        if(candy[i][j]->has_frosting()){return;}
        candy[i][j]->setFillColor(FL_WHITE); //Set it temp white(colorless) so the algo doesn't include this one when it's forshadowing this candy.
        if(i+1<9 && !candy[i+1][j]->is_wall() && !candy[i+1][j]->is_ingredient() && !candy[i+1][j]->has_frosting()){
            if(forshadowing_over_9000(i+1,j,save,vibrate)){
               *not_impossible=true;
               if(vibrate){
                   candy[i][j]->start_pop_animation();
                   *can_vibrate=false;
               } //Vibrate then turn it off.
            }
        }

        if(j+1<9 && !candy[i][j+1]->is_wall() && !candy[i][j+1]->is_ingredient() && !candy[i][j+1]->has_frosting()){
             if(forshadowing_over_9000(i,j+1,save,vibrate)){
                *not_impossible=true;
                 if(vibrate){
                   candy[i][j]->start_pop_animation();
                   *can_vibrate=false;
               }
             }
        }

        if(i-1>=0 && !candy[i-1][j]->is_wall() && !candy[i-1][j]->is_ingredient() && !candy[i-1][j]->has_frosting()){
            if(forshadowing_over_9000(i-1,j,save,vibrate)){
               *not_impossible=true;
                if(vibrate){
                   candy[i][j]->start_pop_animation();
                   *can_vibrate=false;
               }
            }
        }

        if(j-1>=0 && !candy[i][j-1]->is_wall() && !candy[i][j-1]->is_ingredient() && !candy[i][j-1]->has_frosting()){
            if(forshadowing_over_9000(i,j-1,save,vibrate)){
               *not_impossible=true;
                if(vibrate){
                   candy[i][j]->start_pop_animation();
                   *can_vibrate=false;
               }
            }
        }
        
        candy[i][j]->setCode(save); //Sets the color back.
}

bool Hint::forshadowing_over_9000(int x,int y,Fl_Color color,bool vibrate){     // unlimited cpu usage because it's over 9000
       shared_ptr<Item> temp_candy=candy[x][y];
       Fl_Color save=candy[x][y]->getFillColor(); 
       temp_candy->setCode(color);
       int counter_left_right=1; 
       int counter_up_down=1;
        for(int i=x+1;i<9;i++){  //Counter the same candies on the right line of the same color; counter begins of 1
            if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->is_ingredient() || candy[i][y]->has_frosting()){
                break;
            }else{
                counter_left_right+=1;
            }
        }
        

        for(int i=y+1;i<9;i++){  //Counts the candies under.
            if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->is_ingredient() || candy[x][i]->has_frosting()){
                break;
            }else{
                counter_up_down+=1;
            }
        }
  
        for(int i=y-1;i>=0;i--){  //Counts the candies upwards
            if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->is_ingredient() || candy[x][i]->has_frosting()){
                break;
            }else{
                counter_up_down+=1;
            }
        }
       
        for(int i=x-1;i>=0;i--){  //Left
            if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->is_ingredient() || candy[i][y]->has_frosting()){
                break;
            }else{
                counter_left_right+=1;
            }
        }

        
        if(counter_left_right<3 && counter_up_down<3 ){ 
            temp_candy->setCode(save);
            return false; 
        }else{
            if(vibrate){
                if(counter_up_down>=3){
                  for(int i=y-1;i>=0;i--){
                     if(candy[x][i]->getFillColor()==temp_candy->getFillColor()){
                         candy[x][i]->start_pop_animation();
                     }else{break;}
                  }
                  for(int i=y+1;i<9;i++){
                      if(candy[x][i]->getFillColor()==temp_candy->getFillColor()){
                          candy[x][i]->start_pop_animation();
                      }else{break;}
                  }
                }else{
                    for(int i=x-1;i>=0;i--){
                        if(candy[i][y]->getFillColor()==temp_candy->getFillColor()){
                            candy[i][y]->start_pop_animation();
                        }else{break;}
                    }
                    for(int i=x+1;i<9;i++){
                        if(candy[i][y]->getFillColor()==temp_candy->getFillColor()){
                            candy[i][y]->start_pop_animation();
                        }else{break;}
                    }
                } 
                temp_candy->setCode(save);        
                return true;

            }else{
                temp_candy->setCode(save);
                return true;
            }
        }
           
    } 
