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
#include"objective.h"
#include"GameManager.h"
using namespace std;

GameManager::GameManager(){
    for(int i=0;i<9;i++){
        candy[i]=new shared_ptr<Candy>[9]();
    }
}

void GameManager::set_up(shared_ptr<Candy>** arr,Score& score,Objective& obj){
    candy=arr;
    candy_score=&score;
    game_obj=&obj;
    
   
}

bool GameManager::break_candies(int x,int y,int i,int j,bool pc){
   shared_ptr<Candy> temp_candy=candy[x][y];  
   shared_ptr<Candy> temp_candy2=candy[i][j];
   int counter_left_right=1; 
   int counter_up_down=1;
   int counter_left_right2=1;
   int counter_up_down2=1;

        for(int i=x+1;i<9;i++){  //Counter the same candies on the right line of the same color; counter begins of 1
            if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->get_fruit()){
                break;
            }else{
                counter_left_right+=1;
            }
        }

        for(int k=i+1;k<9;k++){  //Counter the same candies on the right line of the same color; counter begins of 1 for temp_candy2
            if(candy[k][j]->getFillColor()!=temp_candy2->getFillColor() || candy[k][j]->get_fruit()){
                break;
            }else{
                counter_left_right2+=1;
            }
        }

        for(int i=y+1;i<9;i++){  //Counts the candies under.
            if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->get_fruit()){
                break;
            }else{
                counter_up_down+=1;
            }
        }

        for(int k=j+1;k<9;k++){  //Counts the candies under. 2
            if(candy[i][k]->getFillColor()!=temp_candy2->getFillColor() || candy[i][k]->get_fruit()){
                break;
            }else{
                counter_up_down2+=1;
            }
        }
      
        for(int i=y-1;i>=0;i--){  //Counts the candies upwards
            if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->get_fruit()){
                break;
            }else{
                counter_up_down+=1;
            }
        }
       
        for(int k=j-1;k>=0;k--){  //Counts the candies upwards 2
            if(candy[i][k]->getFillColor()!=temp_candy2->getFillColor() || candy[i][k]->get_fruit()){
                break;
            }else{
                counter_up_down2+=1;
            }
        }


        for(int i=x-1;i>=0;i--){  //Left
            if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->get_fruit()){
                break;
            }else{
                counter_left_right+=1;
            }
        }

        for(int k=i-1;k>=0;k--){  //Left2
            if(candy[k][j]->getFillColor()!=temp_candy2->getFillColor() || candy[k][j]->get_fruit()){
                break;
            }else{
                counter_left_right2+=1;
            }
        }
       
       //Vlad: Can you try to add a delay when you do the animations?
        if(counter_left_right<3 && counter_up_down<3 && counter_left_right2<3 && counter_up_down2<3 && !pc){ //Changes back if the movement will not result in a break but i don't know how to add a delay.
            Fl_Color save=candy[i][j]->getFillColor();  // TODO : Add The animation code here

            candy[i][j]->setCode(candy[x][y]->getFillColor());
            candy[x][y]->setCode(save);

            return false;
        }

        destroy_candies(x,y,counter_left_right,counter_up_down,pc);
        //There will break all the candies in a row; First it will decide which one is better from left<->right or up<-->down in calculation the nomber of candies that will be broken.
        
    
        //Second candy
        destroy_candies(i,j,counter_left_right2,counter_up_down2,pc);

        return true;
        
}


void GameManager::destroy_candies(int x,int y,int counter_left_right,int counter_up_down,bool pc){
    shared_ptr<Candy> temp_candy=candy[x][y];
    vector<Point> coord;
    if(counter_left_right>counter_up_down){ //A way to know which one will have the priority.
            if(counter_left_right>=3){
                int start_x=x;
                int start_y=y;
                for(int i=x+1;i<9;i++){  //Counter the same candies on the right line of the same color; counter begins of 1
                    if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->get_fruit()){
                        break;
                    }else{
                        start_x=i;
                        start_y=y;
                    }
                }
                if(pc){
                    game_obj->mv_done(0,counter_left_right,temp_candy->getFillColor());} //Checks the obj because 1 mvt has been done
                else{
                    game_obj->mv_done(1,counter_left_right,temp_candy->getFillColor());
                }
                for(int i=start_x;i>=0;i--){  //Left
                    if(candy[i][start_y]->getFillColor()!=temp_candy->getFillColor() || candy[i][start_y]->get_fruit()){
                        break;
                    }else{
                        coord.push_back(Point{i,start_y});
                        candy_score->set_score(counter_left_right); //Temp way to increase the score; TODO: Change it later
                        //cout<<"The score is "<<candy_score.get_score()<<endl;	
                    }
                }
                for(auto c:coord){
                    candy[c.x][c.y]->setFillColor(FL_BLACK);
                    candy[c.x][c.y]->start_pop_animation();
                }
                
            }
        }else if(counter_up_down>=3){
            int start_x=x;int start_y=y; 
            for(int i=y-1;i>=0;i--){  //Counts the candies upwards
                if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->get_fruit()){
                    break;
                }else{
                    start_x=x;
                    start_y=i;
                }
            }
            if(pc){
                game_obj->mv_done(0,counter_up_down,temp_candy->getFillColor());} //Checks the obj
            else{
                game_obj->mv_done(1,counter_up_down,temp_candy->getFillColor());
            }
            for(int i=start_y;i<9;i++){  //Counts the candies under.
                if(candy[start_x][i]->getFillColor()!=temp_candy->getFillColor() || candy[start_x][i]->get_fruit()){
                    break;
                }else{
                    coord.push_back(Point{start_x,i});

                    candy_score->set_score(counter_up_down);
    		        //cout<<"The score is "<<candy_score.get_score()<<endl; //Temp way to increase the score; change it later
                }
            }
            for(auto c:coord){
                candy[c.x][c.y]->setFillColor(FL_BLACK);
                candy[c.x][c.y]->start_pop_animation();
            }
       }
}



int GameManager::fall_candies(int x, int y, bool animate){
    if (y==0){
        candy[x][y]->setFillColor(color[rand()%5]);
        Point fall = candy[x][y]->getCenter();
        if(animate){
            candy[x][y]->setCenter({candy[x][y]->getCenter().x, candy[x][y]->getCenter().y-50});
            candy[x][y]->start_slide_animation(fall, false);
        }
        return 1;
    }
    if(not candy[x][y]->get_wall()){
        Point fall = candy[x][y]->getCenter();
        int x_fork;
        if(not candy[x][y-1]->get_wall()){
            candy[x][y]->setCode(candy[x][y-1]->getFillColor());
            if (animate){
                candy[x][y]->setCenter({candy[x][y]->getCenter().x, candy[x][y]->getCenter().y-50});
            }
            x_fork=x;
        }else{
            for (x_fork=x-1;x_fork<x+1;x_fork+=2){                                  //O(2)
                if (not candy[x_fork][y]->get_wall() && x_fork<9 && x_fork>=0){
                    candy[x][y]->setCode(candy[x_fork][y-1]->getFillColor());
                    if (animate){
                        candy[x][y]->setCenter({candy[x_fork][y-1]->getCenter().x,candy[x_fork][y-1]->getCenter().y-50});
                    }
                    break;
                }  
            }
        }
        if (animate){
            candy[x][y]->start_slide_animation(fall, false);
        }
        fall_candies(x_fork, y-1,animate);
        return 1;
    }
    return 1;
}


void GameManager::set_the_neighbours(){
    for(int i=0;i<9;i++){
         for(int j=0;j<9;j++){
          if(i+1<9) candy[i][j]->neighbours.push_back(candy[i+1][j]);
          if(j+1<9) candy[i][j]->neighbours.push_back(candy[i][j+1]);
          if(i-1>=0) candy[i][j]->neighbours.push_back(candy[i-1][j]);
          if(j-1>=0) candy[i][j]->neighbours.push_back(candy[i][j-1]);
         }
        }
}
