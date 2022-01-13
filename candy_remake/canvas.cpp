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
#include<cstring>

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

void Canvas::make_board(string map){ 
        
        has_moved = false;
        gm.set_moved_state(has_moved);
        
        candy_score=Score(0);
        read_file(map);
        current_map=map;
        int fruits=0;
        bool is_fruit_level=false;
        for (int x = 0; x<9; x++){          // makes board array based on the selected file level
            for (int y = 0; y<9; y++){
                if (lines[y][x] == *c){ // If line = C then makes a candy with a random color.
                    candy[x][y]=make_shared<Candy>(Point{itemDistance*x+itemDistance/2,itemDistance*y+itemDistance/2},squareLenght,squareLenght,COLORS[rand()%6],FL_BLACK);
                }
                else if(lines[y][x]== *w){ //If line= W then makes a wall candy
                    candy[x][y]=make_shared<Wall>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2});
                }
                else if(lines[y][x]== *i){ //If line = I then it makes a fruit.
                    candy[x][y]=make_shared<Ingredient>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2},20);
                    fruits++;
                    is_fruit_level=true;
                }else if(isdigit(lines[y][x])){  // If is a number (1..9) then it makes a frozen candy with(1..9) layers of frost.
                    candy[x][y]=make_shared<Candy>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2},squareLenght, squareLenght,COLORS[rand()%6],FL_CYAN);
                    candy[x][y]->set_layers_of_frosting(lines[y][x] - '0');
                }else if(lines[y][x] == 'S'){  //stripped Candy
                    candy[x][y]=make_shared<Striped_candy>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2},squareLenght, squareLenght,COLORS[rand()%6],FL_CYAN);
                    candy[x][y]->set_direction(horizontal);
                }else if(lines[y][x] == 'w'){ //Wrapped candy
                    candy[x][y]=make_shared<Wrapped_candy>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2});
                }else if(lines[y][x] == 'B'){ //Bomb candy
                    candy[x][y]=make_shared<Bomb_candy>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2}, squareLenght, squareLenght, FL_DARK_GREEN);
                }else{
                    candy[x][y]=make_shared<Edit_Candy>(Point{itemDistance*x+itemDistance/2, itemDistance*y+itemDistance/2});
                }
            }
        }    
        if(is_fruit_level){ //If fruits have been detected on the map then the objectif will be to make all the fruits fall of.
            game_obj.fruits_on(fruits);
        }else{
            game_obj=Objective();
        }

        set_the_neighbours();
        gm.set_up(candy,candy_score,game_obj);
        ht.set_up(candy,can_vibrate,not_impossible);
        
}

void Canvas::edit_level(){
    editing=true;
    make_board("edit_board.txt");
   
}

void Canvas::read_file(string map){
    // reads a board file
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
    if(editing){    // draw for the level editor
        done=true;
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                candy[i][j]->draw();
                if(candy[i][j]->getFillColor()==255){
                    done=false;     // a certain candy is placed
                }
            }
        }

        if(done){   //If all the white candies have been modified then the level will be saved.
            save_the_map();     
            editing=false;
            make_board(maps[0]);    // loads the map that the user made
        }

    return;

    }

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            candy[i][j]->draw();
            if(!is_board_moving() && candy[i][j]->is_wall()!=true && candy[i][j]->is_ingredient()==false){
                    gm.break_candies(i,j,0,0,true); //Checks all the candies all the time and breaks them if it must.
                    set_the_neighbours();
                } 
                ht.check_impossible(i,j,can_vibrate);
            }
        } 

        if(not_impossible){
            not_impossible=false;

        }else{      // no available movement
            make_board(current_map);
        }

        if (has_moved){     // checks if the player has already made a move to prevent the score from going up by making the board as all the colors are random
            if(game_obj.constant_check()){ //Always checks if the objective has been completed
                // user has won
                game_obj=Objective(); //New objective
                
                if (candy_score.get_score()>candy_score.get_best_score()){      //updates best_score (needs restart to apply)
                    candy_score.set_best_score(candy_score.get_score());
                }
                
                if(next_level<4){   // there is a map after
                    next_level++;
                    make_board(maps[next_level]);
                }else{              // no more map after
                    make_board(current_map);
                }

            }else if(!game_obj.constant_check() && game_obj.nr_tries()==0){
                // user lost the objective 
                make_board(current_map);
                game_obj=Objective();
            }
        }

        string obj=to_string(game_obj.nr_breaks());
        string tries=to_string(game_obj.nr_tries());
        string score=to_string(candy_score.get_score());

        fl_draw(obj.c_str(),450,100);
        fl_draw("to break",450,120); 
        fl_draw(tries.c_str(),450,170);
        fl_draw("tries left",450,190);
        fl_draw(score.c_str(),450,210);
        fl_draw("score",450,230);

        if(timer!=200){  //Timer set ; Vlad: pressing on a candy or when candies fall/break will restart the timer.
            timer++;
        }else{          //restart the timer.
            timer=0;
            can_vibrate=true;

            for(int i=0;i<9;i++){       //hints the player a possible move 
                if(candy[i][8]->is_ingredient() && !is_board_moving()){
                    game_obj.dec_fruits();
                    candy[i][8]->set_fruit(false);
                    candy[i][8]=make_shared<Candy>(candy[i][8]->getCenter(),40,40,FL_BLACK);
                    set_the_neighbours();
                }
            }
        }

        for(int i=8;i>=0;i--){ //Checks if there is a free place that can be filled ; Takes into consideration the walls(phyics) as well.
            for(int j=8;j>=0;j--){
                if(!candy[i][j]->is_special_candy() && !candy[i][j]->is_ingredient() && candy[i][j]->getFillColor() == FL_BLACK){
                    if (not is_board_moving()){
                        gm.fall_candies(i,j,has_moved);     // removes everything that is black
                        timer=0;        //reset the timer.
                    }else{
                        break;
                    }
                    can_vibrate=false;  //reset the vibrate;
                }
            }
            if (is_board_moving()){     // candies do not fall when the board is moving 
                break;
            }
        }  
        gm.set_the_neighbours();

    } 

void Canvas::save_the_map(){
    // saves the current map to edited_file
    // only used for level editor 

    fstream file(edited_level, ios::out);

    for(int i=0;i<9;i++){
        char l[9];
        for(int j=0;j<9;j++){
            if(candy[j][i]->is_wall()){
               l[j]='W';
            }else if(candy[j][i]->get_box_type()==FL_FLAT_BOX){         // normal candy     
              l[j]='C';       
            }else if(candy[j][i]->get_box_type()==FL_DIAMOND_BOX){      // striped candy
               l[j]='S';
            }else if(candy[j][i]->get_box_type()==FL_ROUNDED_BOX){      // wrapped candy
               l[j]='B';
            }else if(candy[j][i]->get_box_type()==FL_PLASTIC_UP_BOX){   // bomb
               l[j]='w';
            }else if(candy[j][i]->is_ingredient()){                     // ingredient
               l[j]='I';
            }
        }
        for(size_t k=0;k<strlen(l);k++){
            file.put(l[k]);
            cout<<l[k]<<" ";
        }cout<<endl;
        file.put('\n');
    }
    file.close();
}

void Canvas::mouseMove(Point mouseLoc){
    // event handler for the mouse movement
    // highlights the Item under the mouse

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(candy[i][j]->contains(mouseLoc)){
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
    // only used for the level editor

    if(editing){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(candy[i][j]->contains(mouseLoc)){    
                    if(candy[i][j]->getFillColor()==255 || candy[i][j]->is_ingredient()){                   // changes the Item type each time it clicks a cell
                        candy[i][j]=make_shared<Candy>(candy[i][j]->getCenter(),40,40,COLORS[rand()%5]);    // cycling through normal, walls, ingredients and special candies
                        candy[i][j]->inc_box();
                    }else{
                       if(candy[i][j]->current_box()==4){
                            candy[i][j]=make_shared<Wall>(candy[i][j]->getCenter());
                        }else if(candy[i][j]->is_wall()){
                            candy[i][j]=make_shared<Ingredient>(candy[i][j]->getCenter());
                        }
                    candy[i][j]->set_box_type(boxes[candy[i][j]->current_box()]);       // only changes the aspect of a normal candy to make it a special candy as 
                    candy[i][j]->inc_box();                                             // it's functionalities are not needed in the level editor

                    }
                }
            }
        }
    }
}



void Canvas::mouseDrag(Point mouseLoc){
    // drags the candy that can be dragged

    if(editing){                    // doesn't work when using level editor
        return;
    }

    if(is_board_moving()){          // same as above but when the board is moving
        return;
    }

    if (has_released){              // when first "refresh" of the mouseDrag setting the dragged candy to current
        for(int i=0;i<9;i++){       
            for(int j=0;j<9;j++){
                if(candy[i][j]->contains(mouseLoc)){
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
    }else if(current->is_wall() || current->has_frosting()){        // not dragging wall or frosting
        return;
    }else{
        if (abs(mouseLoc.x - curr_pos.x) > abs(mouseLoc.y - curr_pos.y)){       // mouse is further on left or right so the current item snaps to 
                                                                                // the horizontal line following the mouse
            if (abs(mouseLoc.x - curr_pos.x) <= 50){
                current->setCenter({mouseLoc.x, curr_pos.y});   // follows the mouse if not further than the neigbhour

            }else{                                              // doesn't go further than neighbour                                                       
                current->setCenter({curr_pos.x+50*((mouseLoc.x - curr_pos.x)/abs(mouseLoc.x - curr_pos.x)), curr_pos.y});
            }
        }else{                                                  // same logice as above applies
            if (abs(mouseLoc.y - curr_pos.y) <= 50){
                current->setCenter({curr_pos.x, mouseLoc.y});
            }else{
                current->setCenter({curr_pos.x, curr_pos.y+50*((mouseLoc.y - curr_pos.y)/abs(mouseLoc.y - curr_pos.y))});
            }
        }
    }
}

void Canvas::mouseRelease(Point mouseLoc){

    if(editing){
        return;
    }

    timer=0;
    can_vibrate = false;
    has_released = true;
    current->setCenter(curr_pos);
    
    if(is_board_moving()){
        return;
    }

    if (current->has_frosting() || current->is_wall()){
        return;
    }

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){       // checks for item under the mouse at the time of release

            if (candy[i][j]->contains(mouseLoc) && ! candy[i][j]->is_wall() && ! candy[i][j]->has_frosting() && current->verify_neighbours(candy[i][j])){
                    has_moved = true;
                if(current->is_special_candy() || candy[i][j]->is_special_candy() || current->is_ingredient() || candy[i][j]->is_ingredient()){
                    special_neigh(i,j);
                    return;
                }else{
                    if (gm.break_candies(x,y,i,j)){     // swaps normal candies
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
    //Method that will make a changement between 2 special candies or 1 special candy with 1 normal candy.
    //First the 2 candies will be changed and the it will be checked if the changement is possible by the method break_candies.
    //If movement not possible break_candies will change them back like how they were.
    //If movement is possible then the animations will start
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
    //Set up the neigh of each candy in a vector.
    //Used to make the changement between candies.
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
    //Method is used to check if an animation of fall_candies is happening.
    //If true then the player can't make a movement untill the animation is over.
    for (int x = 0;x<9;x++){
        for (int y = 0;y<9;y++){
            if (not candy[x][y]->is_fall_complete()){
                return true;
            }
        }
    }
    return false;
}
