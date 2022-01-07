#include<iostream>
#include<vector>
#include<array>
#include<memory>
#include"candy.h"
#include"rectangle.h"
#include"objective.h"
#include"Animation.h"
#include"score.h"
#include"const.h"
#include"canvas.h"
#include"GameManager.h"
#include"hint.h"
#include<random>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include<Fl/Fl_Button.H>
#include<Fl/Fl_Text_Display.H>

using namespace std;

Canvas::Canvas(){  
        for(int i=0;i<9;i++){
            candy[i]=new shared_ptr<Item>[9]();
        }                          
        make_board(maps[0]); 
        game_obj=Objective();
        candy_score=Score(0);      
}

Canvas::~Canvas(){
    
}

void Canvas::make_board(string map){ //TODO: Finish it later.
        
        candy_score=Score(0);
        read_file(map);
        current_map=map;
        int fruits=0;
        bool gj=false;
        
        for (int x = 0; x<9; x++){
            for (int y = 0; y<9; y++){
                // should be replace by a switch
                if (lines[y][x] == *c){
                    candy[x][y]=make_shared<Candy>(Point{50*x+25,50*y+25},40,40,COLORS[rand()%6],FL_BLACK);
                }
                else if(lines[y][x]== *w){
                    candy[x][y]=make_shared<Wall>(Point{50*x+25, 50*y+25}, 40, 40);
                }
                else if(lines[y][x]== *i){
                    candy[x][y]=make_shared<Ingredient>(Point{50*x+25, 50*y+25},20);
                    fruits++;
                    gj=true;
                }else if(isdigit(lines[y][x])){
                    candy[x][y]=make_shared<Candy>(Point{50*x+25, 50*y+25},40, 40,COLORS[rand()%6],FL_CYAN);
                    candy[x][y]->set_layers_of_frosting(lines[y][x] - '0');
                }else if(lines[y][x] == 'S'){
                    candy[x][y]=make_shared<Striped_candy>(Point{50*x+25, 50*y+25},40, 40,COLORS[rand()%6],FL_CYAN);
                    candy[x][y]->set_direction(horizontal);
                }else if(lines[y][x] == 'w'){
                    candy[x][y]=make_shared<Wrapped_candy>(Point{50*x+25, 50*y+25}, 40, 40);
                }else if(lines[y][x] == 'B'){
                    candy[x][y]=make_shared<Bomb_candy>(Point{50*x+25, 50*y+25}, 40, 40, FL_DARK_GREEN);
                }
            }
        }    
        if(gj){game_obj.fruits_on(fruits);}else{game_obj=Objective();}
        set_the_neighbours();
        gm.set_up(candy,candy_score,game_obj);
        ht.set_up(candy,can_vibrate,not_impossible);
        
}


void Canvas::read_file(string map){
        lines.clear();  //Clear the map so the next one can be loaded in the vector.
        string line;                        //temporary input var
        ifstream myfile(map);
        current_map=map;
        while (myfile >> line) {
            lines.push_back(line);
        }
        myfile.close(); 
        
}

void Canvas::draw(){
         for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                candy[i][j]->draw();
                if(!is_board_moving() && candy[i][j]->get_wall()!=true && candy[i][j]->is_ingredient()==false){
                    gm.break_candies(i,j,0,0,true); //Checks all the candies all the time and breaks them if it must.
                    set_the_neighbours();
                } 
                ht.check_impossible(i,j,can_vibrate);
            }
        } 

        // for(int i=0;i<9;i++){
        //     if(candy[i][8]->is_ingredient()){
        //         game_obj.dec_fruits();
        //         candy[i][8]->set_fruit(false);
        //         candy[i][8]->setFillColor(FL_BLACK);
        //     }
        // }

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

        if(timer!=200){  //Timer set ; Vlad: pressing on a candy or when candies fall/break will restart the timer.
            timer++;
        }else{          //restart the timer.
            timer=0;
            can_vibrate=true;
        }

        for(int i=8;i>=0;i--){ //Checks if there is a free place that can be filled ; Takes into consideration the walls(phyics) as well.
            for(int j=8;j>=0;j--){
                if(!candy[i][j]->is_special_candy() && !candy[i][j]->is_ingredient() && candy[i][j]->getFillColor() == FL_BLACK){
                    if (not is_board_moving()){
                        gm.fall_candies(i,j,has_moved);
                        timer=0; //reset the timer.
                    }else{
                        break;
                    }
                    can_vibrate=false; //reset the vibrate;
                }
            }
            if (is_board_moving()){
                break;
            }
        }  
        gm.set_the_neighbours();

} 

void Canvas::mouseMove(Point mouseLoc){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(candy[i][j]->contains(mouseLoc)){
                    //cout<<"here "<<i<<" "<<j<<endl;
                    candy[i][j]->setFrameColor(FL_RED);
                }else{
                    if (candy[i][j]->has_frosting()){
                        candy[i][j]->setFrameColor(FL_CYAN);
                    }else{
                        candy[i][j]->setFrameColor(FL_BLACK);
                    }
                }
            }
        }
}

void Canvas::mouseClick(Point mouseLoc){
        // has_moved=true;
        // for(int i=0;i<9;i++){
        //     for(int j=0;j<9;j++){
        //         if(candy[i][j]->contains(mouseLoc) && current->getFillColor() == FL_BLACK){
        //             if(candy[i][j]->get_wall()==false ){  //make sure that is not a wall. ; Vlad:time n vibrate
        //                 current=candy[i][j];
        //                 x=i;
        //                 y=j;
        //                 timer=0;
        //                 can_vibrate=false;
        //             }
        //         }else if(candy[i][j]->contains(mouseLoc) && candy[i][j]->is_fall_complete() && candy[x][y]->is_fall_complete()){
        //             if(candy[i][j]->verify_neighbours(current)){
        //                 if (gm.break_candies(i,j,x,y)){
        //                     candy[i][j]->start_fall_animation(candy[x][y]->getCenter());
        //                     candy[x][y]->start_fall_animation(candy[i][j]->getCenter());
        //                 }
        //                 if(candy[x][y]->get_fruit()){
        //                     if(ht.forshadowing_over_9000(i,j,candy[i][j]->getFillColor(),false)==false){
        //                         Fl_Color save=candy[x][y]->getFillColor();
        //                         candy[x][y]->set_fruit(false);
        //                         candy[x][y]->setCode(candy[i][j]->getFillColor());
        //                         candy[i][j]->set_fruit(true);
        //                         candy[i][j]->setCode(save);
        //                     }
        //                 }
        //                 current=make_shared<Candy>();
        //                 x=0;
        //                 y=0;
        //                 cout<<"Neigh"<<endl;
        //                 timer=0;
        //                 can_vibrate=false;
        //             }else{
        //                 cout<<"Selected new one"<<endl;
        //                 if(candy[i][j]->get_wall()==false){  //timer n vibrate
        //                     current=candy[i][j];
        //                     x=i;
        //                     y=j;
        //                     timer=0;
        //                     can_vibrate=false;
        //                 }    
        //             }
        //         }
        //     }
        // }
}

void Canvas::mouseDrag(Point mouseLoc){
    if(is_board_moving()){
        return;
    }

    if (has_released){
        for(int i=0;i<9;i++){       //not optimised better do do double while to make it stop cleanly when match found in so it doesn't go through every cell
            for(int j=0;j<9;j++){
                if(candy[i][j]->contains(mouseLoc)){
                    has_moved = true;
                    gm.set_moved_state(true);
                    current = candy[i][j];
                    curr_pos = current->getCenter();

                    x = i;
                    y = j;
                    
                    has_released = false;
                    return;
                }
            }
        }
    }else if(current->get_wall() || current->has_frosting()){
        return;
    }else{
        if (abs(mouseLoc.x - curr_pos.x) > abs(mouseLoc.y - curr_pos.y)){
            if (abs(mouseLoc.x - curr_pos.x) <= 50){
                current->setCenter({mouseLoc.x, curr_pos.y});
            }else{
                current->setCenter({curr_pos.x+50*((mouseLoc.x - curr_pos.x)/abs(mouseLoc.x - curr_pos.x)), curr_pos.y});
                current->setCenter({curr_pos.x+50*((mouseLoc.x - curr_pos.x)/abs(mouseLoc.x - curr_pos.x)), curr_pos.y});
            }
        }else{
            if (abs(mouseLoc.y - curr_pos.y) <= 50){
                current->setCenter({curr_pos.x, mouseLoc.y});
            }else{
                current->setCenter({curr_pos.x, curr_pos.y+50*((mouseLoc.y - curr_pos.y)/abs(mouseLoc.y - curr_pos.y))});
            }
        }
    }
}

void Canvas::mouseRelease(Point mouseLoc){
    // not proper OOP but will work for now
    timer=0;
    can_vibrate = false;
    has_released = true;
    current->setCenter(curr_pos);
    
    if(is_board_moving()){
        return;
    }

    if (current->has_frosting() || current->get_wall()){
        return;
    }

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){

            if (candy[i][j]->contains(mouseLoc) && ! candy[i][j]->get_wall() && ! candy[i][j]->has_frosting() && current->verify_neighbours(candy[i][j])){
                if(current->is_special_candy() || candy[i][j]->is_special_candy() || current->is_ingredient() || candy[i][j]->is_ingredient()){
                    cout<<"inside dat thing"<<endl;
                    special_neigh(i,j);
                    return;
                }else{
                    if (gm.break_candies(x,y,i,j)){
                        set_the_neighbours();
                        current->start_fall_animation(candy[i][j]->getCenter());
                        candy[i][j]->start_fall_animation(current->getCenter());
                        current=make_shared<Candy>();
                        x=0;
                        y=0;
                        return;
                    }
                }
            }
            
        }
    }
     current=make_shared<Candy>();
     x=0;
     y=0;
}


bool Canvas::special_neigh(int i,int j){
    
    shared_ptr<Item> save2=candy[x][y];
    Point save=Point{current->getCenter().x,current->getCenter().y};
    current->setCenter(Point{candy[i][j]->getCenter().x,candy[i][j]->getCenter().y});
    candy[i][j]->setCenter(save);
    candy[x][y]=candy[i][j];
    candy[i][j]=save2;
    if (gm.break_candies(x,y,i,j)){
        set_the_neighbours();
        current->start_fall_animation(candy[i][j]->getCenter());
        candy[i][j]->start_fall_animation(current->getCenter());
        current=make_shared<Candy>();
        x=0;y=0;
        return true;
    }
    

    return false;
        
    
    
}

void Canvas::set_the_neighbours(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(i+1<9) candy[i][j]->set_neigh(candy[i+1][j]);
            if(j+1<9) candy[i][j]->set_neigh(candy[i][j+1]);
            if(i-1>=0) candy[i][j]->set_neigh(candy[i-1][j]);
            if(j-1>=0) candy[i][j]->set_neigh(candy[i][j-1]);
        }
    }
}

bool Canvas::is_board_moving(){
    for (int x = 0;x<9;x++){
        for (int y = 0;y<9;y++){
            if (not candy[x][y]->is_fall_complete()){
                return true;
            }
        }
    }
    return false;
}
