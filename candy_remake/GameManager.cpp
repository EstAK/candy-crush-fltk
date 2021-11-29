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

void GameManager::break_candies(int x,int y,int i,int j,bool pc){
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

            
        }
        
        destroy_candies(x,y,counter_left_right,counter_up_down,pc);
        //There will break all the candies in a row; First it will decide which one is better from left<->right or up<-->down in calculation the nomber of candies that will be broken.
        
    
        //Second candy
        destroy_candies(i,j,counter_left_right2,counter_up_down2,pc);
        
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

int GameManager::fall_candies(int start_x,int start_y){ //Function by Recursion
        if(start_y<0){
            return 0;
        }
        if(candy[start_x][start_y]->get_wall()){
            fall_walls(start_x,start_y);
            return 0;
        } //If it's a wall then make candies fall from the diag with fall_walls()
        if(start_y-1>=0){

            int s_y=start_y-1;
            if(candy[start_x][s_y]->getFillColor() != FL_BLACK && candy[start_x][s_y]->get_wall()!=true){ //If there is a candy above and is not a wall.
                Point fall{candy[start_x][s_y]->getCenter().x,candy[start_x][s_y]->getCenter().y+50}; //that Candy will fall down and it's place will be liberated.
                candy[start_x][start_y]->setCenter(fall); //Candy falls down
                candy[start_x][start_y]->setCode(candy[start_x][s_y]->getFillColor());
                candy[start_x][start_y]->set_fruit(candy[start_x][s_y]->get_fruit());
                candy[start_x][s_y]->setFillColor(FL_BLACK); //Place liberated of the candy that fell Down.
                candy[start_x][s_y]->set_fruit(false);
                set_the_neighbours(); //Reset the neig because new candies have been created.
                fall_candies(start_x,s_y); //Continue the Rec.
            }else{
                fall_candies(start_x,s_y); //Else we will search more above.
            }

        }else{  //We know that we reached the top we want to generate a random candy in the top but we need the position of the column first.
            if(candy[start_x][start_y+1]->getFillColor() != FL_BLACK && candy[start_x][start_y+1]->get_wall()!=true){
                Point fall{candy[start_x][start_y+1]->getCenter().x,candy[start_x][start_y+1]->getCenter().y-50};
                candy[start_x][start_y]->setCenter(fall);
                candy[start_x][start_y]->setFillColor(color[rand()%5]);
                set_the_neighbours();
            }else if(start_x+1<9 && candy[start_x+1][start_y]->get_wall()!=true){
                Point fall{candy[start_x+1][start_y]->getCenter().x-50,candy[start_x+1][start_y]->getCenter().y}; //Get the pos in using -50 the right-neigh position.
                candy[start_x][start_y]->setCenter(fall);
                candy[start_x][start_y]->setFillColor(color[rand()%5]);
                set_the_neighbours();
            }else if(start_x-1>=0 && candy[start_x-1][start_y]->get_wall()!=true){
                Point fall{candy[start_x-1][start_y]->getCenter().x+50,candy[start_x-1][start_y]->getCenter().y}; //Get the pos in adding +50 the left-neigh position.
                candy[start_x][start_y]->setCenter(fall);
                candy[start_x][start_y]->setFillColor(color[rand()%5]);
                set_the_neighbours();
            }
        }
           
        return 0;
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

void GameManager::fall_walls(int start_x, int start_y){ //Function to make the candies fall in diag if there is a wall above.
        if(candy[start_x-1][start_y]->getFillColor() != FL_BLACK ){ //make the candy in the right diag fall.
            Point fall{candy[start_x][start_y]->getCenter().x,candy[start_x][start_y]->getCenter().y+50};
            candy[start_x][start_y+1]->setCenter(fall);
            candy[start_x][start_y+1]->setCode(candy[start_x-1][start_y]->getFillColor());
            candy[start_x-1][start_y]->setFillColor(FL_BLACK);
            set_the_neighbours();
            fall_candies(start_x-1,start_y);
        }else{ //If no candy in the right diag then we take the one from the left diag
            Point fall{candy[start_x][start_y]->getCenter().x,candy[start_x][start_y]->getCenter().y+50};
            candy[start_x][start_y+1]->setCenter(fall);
            candy[start_x][start_y+1]->setCode(candy[start_x+1][start_y]->getFillColor());
            candy[start_x+1][start_y]->setFillColor(FL_BLACK);
            set_the_neighbours();
            fall_candies(start_x+1,start_y);
        }
    }