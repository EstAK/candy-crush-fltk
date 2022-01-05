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
        candy[i]=new shared_ptr<Item>[9]();
    }
}

void GameManager::set_up(shared_ptr<Item>** arr,Score& score,Objective& obj){
    candy=arr;
    candy_score=&score;
    game_obj=&obj;
    
   
}

bool GameManager::break_candies(int x,int y,int i,int j,bool pc){
    
    if (candy[x][y]->is_striped() && candy[i][j]->is_striped()){
        cout<<"doing something"<<endl;
        candy[x][y]->set_direction(horizontal);
        candy[i][j]->set_direction(vertical);
        break_striped(x,y);
        break_striped(i,j);
        return true;
    }else if(candy[x][y]->is_striped() && candy[i][j]->is_striped()){

    }else if(!pc && candy[x][y]->is_bomb() || candy[i][j]->is_bomb()){
        if (candy[x][y]->is_bomb()){
            candy[x][y]->set_color_to_break(candy[i][j]->getFillColor());
            break_bomb(x, y);
        }else{
            candy[i][j]->set_color_to_break(candy[x][y]->getFillColor());
            break_bomb(i, j);
        }
        return true;
    }

    shared_ptr<Item> temp_candy=candy[x][y];  
    shared_ptr<Item> temp_candy2=candy[i][j];
    int counter_left_right=1; 
    int counter_up_down=1;
    int counter_left_right2=1;
    int counter_up_down2=1;

    if (candy[x][y]->getFillColor() == FL_BLACK || candy[i][j]->getFillColor() == FL_BLACK ){
        return false;
    }

    for(int i=x+1;i<9;i++){  //Counter the same candies on the right line of the same color; counter begins of 1
        if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->is_ingredient()){
            break;
        }else{
            counter_left_right+=1;
        }
    }

    for(int k=i+1;k<9;k++){  //Counter the same candies on the right line of the same color; counter begins of 1 for temp_candy2
        if(candy[k][j]->getFillColor()!=temp_candy2->getFillColor() || candy[k][j]->is_ingredient()){
            break;
        }else{
            counter_left_right2+=1;
        }
    }

    for(int i=y+1;i<9;i++){  //Counts the candies under.
        if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->is_ingredient()){
            break;
        }else{
            counter_up_down+=1;
        }
    }

    for(int k=j+1;k<9;k++){  //Counts the candies under. 2
        if(candy[i][k]->getFillColor()!=temp_candy2->getFillColor() || candy[i][k]->is_ingredient()){
            break;
        }else{
            counter_up_down2+=1;
        }
    }
  
    for(int i=y-1;i>=0;i--){  //Counts the candies upwards
        if(candy[x][i]->getFillColor()!=temp_candy->getFillColor() || candy[x][i]->is_ingredient()){
            break;
        }else{
            counter_up_down+=1;
        }
    }
   
    for(int k=j-1;k>=0;k--){  //Counts the candies upwards 2
        if(candy[i][k]->getFillColor()!=temp_candy2->getFillColor() || candy[i][k]->is_ingredient()){
            break;
        }else{
            counter_up_down2+=1;
        }
    }


    for(int i=x-1;i>=0;i--){  //Left
        if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->is_ingredient()){
            break;
        }else{
            counter_left_right+=1;
        }
    }

    for(int k=i-1;k>=0;k--){  //Left2
        if(candy[k][j]->getFillColor()!=temp_candy2->getFillColor() || candy[k][j]->is_ingredient()){
            break;
        }else{
            counter_left_right2+=1;
        }
    }
    

    if(candy[x][y]->is_special_candy() || candy[i][j]->is_special_candy()){
        if(counter_left_right<3 && counter_up_down<3 && counter_left_right2<3 && counter_up_down2<3 && !pc){
            shared_ptr<Item> save2=candy[x][y];
            Point save=Point{candy[x][y]->getCenter().x,candy[x][y]->getCenter().y};
            candy[x][y]->setCenter(Point{candy[i][j]->getCenter().x,candy[i][j]->getCenter().y});
            candy[i][j]->setCenter(save);
            candy[x][y]=candy[i][j];
            candy[i][j]=save2; 
           return false;
        }else{
           destroy_candies(x,y,counter_left_right,counter_up_down);
           destroy_candies(i,j,counter_left_right2,counter_up_down2);
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
    shared_ptr<Item> temp_candy=candy[x][y];
    vector<Point> coord;
    bool can_change_color = true;
    if(counter_left_right>counter_up_down){ //A way to know which one will have the priority.
        if(counter_left_right>=3){
            int start_x=x;
            int start_y=y;
            for(int i=x+1;i<9;i++){  //Counter the same candies on the right line of the same color; counter begins of 1
                if(candy[i][y]->getFillColor()!=temp_candy->getFillColor() || candy[i][y]->is_ingredient()){
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
                if(candy[i][start_y]->getFillColor()!=temp_candy->getFillColor() || candy[i][start_y]->is_ingredient()){
                    break;
                }else{
                    coord.push_back(Point{i,start_y});
                    candy_score->set_score(counter_left_right); //Temp way to increase the score; TODO: Change it later
                    //cout<<"The score is "<<candy_score.get_score()<<endl;	
                }
            }
            for(auto c:coord){
                if (!candy[c.x][c.y]->has_frosting()){
                    if (candy[c.x][c.y]->is_striped()){
                        cout<<"found a striped candy horizontaly"<<endl;
                        break_striped(c.x, c.y);
                        return;
                    }else if(candy[c.x][c.y]->is_wrapped()){
                        break_wrapped(c.x, c.y);
                    }
                    if(has_moved){
                        if (counter_left_right == 4 && c.x == x && c.y == y){
                            candy[c.x][c.y] = make_shared<Striped_candy>(candy[c.x][c.y]->getCenter(), 40, 40, candy[c.x][c.y]->getFillColor());
                            candy[c.x][c.y]->set_direction(vertical);
                        }else if(counter_left_right == 5 && c.x == x && c.y == y){
                            candy[c.x][c.y] = make_shared<Bomb_candy>(candy[c.x][c.y]->getCenter(), 40, 40);
                            can_change_color = false;
                        }
                        
                    }
                    if (can_change_color){
                        candy[c.x][c.y]->setFillColor(FL_BLACK);
                    }
                }else{
                    if (has_moved){
                        candy[c.x][c.y]->set_layers_of_frosting(candy[c.x][c.y]->get_layers_of_frosting()-1);
                    }
                }
                candy[c.x][c.y]->start_pop_animation();
                can_change_color = true;
            }
            
        }
    }else if(counter_up_down>=3){
        int start_x=x;int start_y=y; 
        Point ingredient_pos;
        for(int i=y-1;i>=0;i--){  //Counts the candies upwards
            if(candy[x][i]->getFillColor()!=temp_candy->getFillColor()){
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
            
            if(candy[start_x][i]->getFillColor()!=temp_candy->getFillColor() || candy[start_x][i]->is_ingredient()){
                break;
            }else{
                coord.push_back(Point{start_x,i});

                candy_score->set_score(counter_up_down);
		        //cout<<"The score is "<<candy_score.get_score()<<endl; //Temp way to increase the score; change it later
            }
        }
        for(auto c:coord){
            if (!candy[c.x][c.y]->has_frosting()){
                if (candy[c.x][c.y]->is_striped()){
                    cout<<"found a striped candy verticaly"<<endl;
                    break_striped(c.x, c.y);
                    return;
                }else if(candy[c.x][c.y]->is_wrapped()){
                    break_wrapped(c.x, c.y);
                }
                if (has_moved){
                    if (counter_up_down == 4 && c.x == x && c.y == y){
                        candy[c.x][c.y] = make_shared<Striped_candy>(candy[c.x][c.y]->getCenter(), 40, 40, candy[c.x][c.y]->getFillColor());
                        candy[c.x][c.y]->set_direction(horizontal);
                        can_change_color = false;
                    }else if(counter_left_right == 5 && c.x == x && c.y == y){
                        candy[c.x][c.y] = make_shared<Bomb_candy>(candy[c.x][c.y]->getCenter(), 40, 40);
                        can_change_color = false;
                    }
                    candy[c.x][c.y]->update_frosted_neighbours();
                }
                if (can_change_color){
                    candy[c.x][c.y]->setFillColor(FL_BLACK);
                }
            }else{
                if (has_moved){
                    candy[c.x][c.y]->set_layers_of_frosting(candy[c.x][c.y]->get_layers_of_frosting()-1);
                }
            }
            candy[c.x][c.y]->start_pop_animation();
            can_change_color = true;
        }
   }
}



int GameManager::fall_candies(int x, int y, bool animate){
    if (y==0){
        candy[x][y]->setFillColor(color[rand()%5]);
        Point fall = candy[x][y]->getCenter();
        if(animate){
            candy[x][y]->setCenter({candy[x][y]->getCenter().x, candy[x][y]->getCenter().y-50});
            candy[x][y]->start_fall_animation(fall, false);
        }
        return 1;

    }else if(!candy[x][y]->get_wall()){
        Point fall = candy[x][y]->getCenter();
        int x_fork = x;

        if (candy[x][y-1]->is_ingredient()){
            //TODO
        }else if(candy[x][y-1]->is_special_candy()){
            candy[x][y] = candy[x][y-1];
            candy[x][y-1] = make_shared<Candy>(candy[x][y-1]->getCenter(), 40, 40);
            if (!animate){
                candy[x][y]->setCenter(fall);
            }
        }else if(!candy[x][y-1]->get_wall()){
            candy[x][y]->setCode(candy[x][y-1]->getFillColor());
            if (animate){
                candy[x][y]->setCenter({candy[x][y]->getCenter().x, candy[x][y]->getCenter().y-50});
            }
        }else{  // has wall on top 
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
            candy[x][y]->start_fall_animation(fall, false);
        }
        fall_candies(x_fork, y-1,animate);
        return 1;
    }
    return 1;
}


void GameManager::set_the_neighbours(){
    for(int i=0;i<9;i++){
         for(int j=0;j<9;j++){
          if(i+1<9) candy[i][j]->set_neigh(candy[i+1][j]);
          if(j+1<9) candy[i][j]->set_neigh(candy[i][j+1]);
          if(i-1>=0) candy[i][j]->set_neigh(candy[i-1][j]);
          if(j-1>=0) candy[i][j]->set_neigh(candy[i][j-1]);
         }
        }
}

void GameManager::set_moved_state(bool b){
    has_moved = b;
}

void GameManager::break_row(int x, int y){
    for(int i=x+1;i<9;i++){ //right
        if(!candy[i][y]->get_wall()){
            if(candy[i][y]->is_striped()){  //copy pasted 4 times might be a good idea to make it a standalone function
                break_striped(x, i);
            }
            candy[i][y]->setFillColor(FL_BLACK);
            candy[i][y]->update_frosted_neighbours();
            candy[i][y]->start_pop_animation();
        }else{
            break;
        }
    }
    for(int i=x-1;i>=0;i--){
        if(!candy[i][y]->get_wall()){   //left
            if(candy[i][y]->is_striped()){  //copy pasted 4 times might be a good idea to make it a standalone function
                break_striped(x, i);
            }
            candy[i][y]->setFillColor(FL_BLACK);
            candy[i][y]->update_frosted_neighbours();
            candy[i][y]->start_pop_animation();
        }else{
            break;
        }
    }
}

void GameManager::break_column(int x, int y){
    for(int i=y+1;i<9;i++){   //down
        if(!candy[x][i]->get_wall() && !candy[x][i]->is_ingredient()){
            if(candy[x][i]->is_striped()){  //copy pasted 4 times might be a good idea to make it a standalone function
                break_striped(x, i);
            }
            candy[x][i]->setFillColor(FL_BLACK);
            candy[x][i]->update_frosted_neighbours();
            candy[x][i]->start_pop_animation();

        }else{
            break;
        }
    }
    for(int i=y-1;i>=0;i--){    //up
        if(!candy[x][i]->get_wall() && !candy[x][i]->is_ingredient()){
            // if(candy[x][i]->is_striped()){
            //     break_striped(x, i);
            // }
            candy[x][i]->setFillColor(FL_BLACK);
            candy[x][i]->update_frosted_neighbours();
            candy[x][i]->start_pop_animation();
        }else{
            break;
        }
    }
}

void GameManager::break_striped(int x, int y){
    if (candy[x][y]->get_direction() == horizontal){
        break_row(x, y);
    }else{
        cout<<candy[x][y]->get_direction()<<endl;
        break_column(x, y);
    }
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
}


void GameManager::break_wrapped(int x, int y){
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
    for(int i=x-1;i<=x+1;i++){
        for(int j=y-1;j<=y+1;j++){
            if (i>=0 && i<9 && j>=0 && j<9 && !candy[i][j]->get_wall() && !candy[i][j]->is_ingredient()){
                candy[i][j]->setFillColor(FL_BLACK);
                candy[i][j]->start_pop_animation();
            }
        }
    }
}

void GameManager::break_bomb(int x, int y){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if (candy[i][j]->getFillColor() == candy[x][y]->get_color_to_break()){
                if (candy[i][j]->is_striped()){
                    break_striped(i, j);
                }else if(candy[i][j]->is_wrapped()){
                    break_wrapped(i, j);
                }else{
                    candy[i][j]->setFillColor(FL_BLACK);
                }
            }
        }
    }
}