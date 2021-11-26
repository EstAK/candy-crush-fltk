#include<iostream>
#include"objective.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>

using namespace std;

Objective::Objective(){
      selected_obj=poss_objectives[rand()%3];
      number_of_tries=25+(rand()%(40-25+1)); //Generate a number max between 25-40 mvts
      setUp();
  }

void Objective::setUp(){
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

void Objective::fruits_on(int fruits){
      selected_obj="fruits on";
      number_of_tries=25+(rand()%(40-25+1));
      number_to_break=fruits;
  }

void Objective::inc_fruits(){
      number_to_break++;
  }

void Objective::dec_fruits(){
      number_to_break--;
  }

bool Objective::constant_check(){ 
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

void Objective::mv_done(int nr,int nr_of_candies,Fl_Color candy_color=FL_WHITE){ //1 movement has been done.
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

void Objective::objCompleted(){ //Method to set the obj_completed to true if the obj has benn completed
       if(number_to_break<=0 && number_of_tries>=0){
           obj_completed=true;
       }
   }
int Objective::nr_breaks(){return number_to_break;}
int Objective::nr_tries(){return number_of_tries;}


