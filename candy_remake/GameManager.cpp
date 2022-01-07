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
    if(candy[x][y]->getFillColor()==255){
        return false;
    }
    
    if (candy[x][y]->getCenter() == candy[i][j]->getCenter()){
        // this situation shouldn't be able to happen but better be safe
        return false;
    }
    
    if (!pc){
        if (candy[x][y]->is_striped() && candy[i][j]->is_striped()){
            candy[x][y]->set_direction(horizontal);
            candy[i][j]->set_direction(vertical);
            break_striped(x,y);
            break_striped(i,j);
            return true;
        }else if(candy[x][y]->is_wrapped() && candy[i][j]->is_wrapped()){ 
            break_wrapped(x, y, 2);
            break_wrapped(i, j, 2);
            return true;
        }else if((candy[x][y]->is_wrapped() && candy[i][j]->is_striped()) || (candy[x][y]->is_striped() && candy[i][j]->is_wrapped())){
            candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
            candy[i][j] = make_shared<Candy>(candy[i][j]->getCenter(), 40, 40);
            break_striped_wrapped(x, y);
        }else if(candy[x][y]->is_bomb() && candy[i][j]->is_bomb()){    
            candy[i][j] = make_shared<Candy>(candy[i][j]->getCenter(), 40, 40);
            break_board(x, y);
            return true;
        }else if((candy[x][y]->is_bomb() && candy[i][j]->is_wrapped()) || (candy[x][y]->is_wrapped() && candy[i][j]->is_bomb())){  
            if (candy[x][y]->is_bomb()){
                candy[x][y]->set_color_to_break(candy[i][j]->getFillColor());
                break_bomb_wrapped(x, y);
            }else{
                candy[i][j]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb_wrapped(i, j);
            }
            return true;
        }else if((candy[x][y]->is_bomb() && candy[i][j]->is_striped()) || (candy[x][y]->is_striped() && candy[i][j]->is_bomb())){
            if (candy[x][y]->is_bomb()){
                candy[x][y]->set_color_to_break(candy[i][j]->getFillColor());
                break_bomb_striped(x, y);
            }else{
                candy[i][j]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb_striped(i, j);
            }
            return true;
        }else if(candy[x][y]->is_bomb() || candy[i][j]->is_bomb()){
            if(candy[x][y]->is_bomb()){
                candy[x][y]->set_color_to_break(candy[i][j]->getFillColor());
                break_bomb(x, y);
            }else{
                candy[i][j]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb(i, j);
            }
            return true;
        }
    }else{
        if(candy[x][y]->is_bomb() && i == 0 && j==0){
            return false;
        }
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
           return true;
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

                if(candy[c.x][c.y]->is_special_candy()){

                    cout<<"inside the special candy case"<<endl;
                    if (candy[c.x][c.y]->is_striped()){
                        cout<<"found a striped candy horizontaly"<<endl;
                        break_striped(c.x, c.y);
                    }else if(candy[c.x][c.y]->is_wrapped()){
                        break_wrapped(c.x, c.y);
                    }

                }else if (!candy[c.x][c.y]->has_frosting()){

                    if(has_moved){
                        if (counter_left_right == 4 && c.x == x && c.y == y){
                            candy[c.x][c.y] = make_shared<Striped_candy>(candy[c.x][c.y]->getCenter(), 40, 40, candy[c.x][c.y]->getFillColor());
                            candy[c.x][c.y]->set_direction(vertical);
                        }else if(counter_left_right >= 5 && c.x == x && c.y == y){
                            cout<<"making a bomb"<<endl;
                            candy[c.x][c.y] = make_shared<Bomb_candy>(candy[c.x][c.y]->getCenter(), 40, 40, FL_DARK_GREEN);
                        }
                        }else if ((counter_left_right + counter_up_down - 1) == 5 && c.x == x && c.y == y){     // -1 because the candy is included in each counter
                            candy[c.x][c.y] = make_shared<Wrapped_candy>(candy[c.x][c.y]->getCenter(), 40, 40, candy[c.x][c.y]->getFillColor());
                    }

                    if (!candy[c.x][c.y]->is_special_candy()){
                        candy[c.x][c.y]->setFillColor(FL_BLACK);
                    }


                }else{
                    if (has_moved){
                        candy[c.x][c.y]->set_layers_of_frosting(candy[c.x][c.y]->get_layers_of_frosting()-1);
                    }
                }

                candy[c.x][c.y]->start_pop_animation();
            }
            
        }
    }else if(counter_up_down>=3){
        int start_x=x;
        int start_y=y; 
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

            if(candy[c.x][c.y]->is_special_candy()){
                cout<<"inside the special candy case"<<endl;
                if (candy[c.x][c.y]->is_striped()){
                    cout<<"found a striped candy verticaly"<<endl;
                    break_striped(c.x, c.y);
                }else if(candy[c.x][c.y]->is_wrapped()){
                    break_wrapped(c.x, c.y);
                }

            }else if (!candy[c.x][c.y]->has_frosting()){

                if (has_moved){
                    if (counter_up_down == 4 && c.x == x && c.y == y){
                        candy[c.x][c.y] = make_shared<Striped_candy>(candy[c.x][c.y]->getCenter(), 40, 40, candy[c.x][c.y]->getFillColor());
                        candy[c.x][c.y]->set_direction(horizontal);
                    }else if(counter_up_down >= 5 && c.x == x && c.y == y){
                        cout<<"making a bomb"<<endl;
                        candy[c.x][c.y] = make_shared<Bomb_candy>(candy[c.x][c.y]->getCenter(), 40, 40, FL_DARK_GREEN);
                    }else if ((counter_left_right + counter_up_down - 1) == 5 && c.x == x && c.y == y){     // -1 because the candy is included in each counter
                        candy[c.x][c.y] = make_shared<Wrapped_candy>(candy[c.x][c.y]->getCenter(), 40, 40, candy[c.x][c.y]->getFillColor());
                    }
                }

                if(!candy[c.x][c.y]->is_special_candy()){
                    candy[c.x][c.y]->setFillColor(FL_BLACK);
                }

            }else{
                if (has_moved){
                    candy[c.x][c.y]->set_layers_of_frosting(candy[c.x][c.y]->get_layers_of_frosting()-1);
                }
            }
            candy[c.x][c.y]->start_pop_animation();
        }
   }
}



int GameManager::fall_candies(int x, int y, bool animate){
    if (y==0){
        candy[x][y]->setFillColor(COLORS[rand()%6]);
        Point fall = candy[x][y]->getCenter();
        if(animate){
            candy[x][y]->setCenter({candy[x][y]->getCenter().x, candy[x][y]->getCenter().y-50});
            candy[x][y]->start_fall_animation(fall, false);
        }
        return 1;

    }else if(!candy[x][y]->get_wall()){
        Point fall = candy[x][y]->getCenter();
        int x_fork = x;

        if(candy[x][y-1]->is_special_candy() || candy[x][y-1]->is_ingredient()){
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
            for (x_fork=x-1;x_fork<=x+1;x_fork+=2){                                  //O(2)
                if (not candy[x_fork][y-1]->get_wall() && x_fork<9 && x_fork>=0){

                    if (candy[x_fork][y-1]->is_special_candy() || candy[x_fork][y-1]->is_ingredient()){
                        candy[x][y] = candy[x_fork][y-1];
                        candy[x_fork][y-1] = make_shared<Candy>(candy[x_fork][y-1]->getCenter(), 40, 40);

                    }else{
                        candy[x][y]->setCode(candy[x_fork][y-1]->getFillColor());
                    }

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
            if(candy[i][y]->is_striped()){
                break_striped(i, y);
            }else if(candy[i][y]->is_wrapped()){
                break_wrapped(i, y);
            }else if(candy[i][y]->is_bomb()){
                candy[x][i]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb(i, y);
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
            if(candy[x][i]->is_striped()){
                break_striped(x, i);
            }else if(candy[x][i]->is_wrapped()){
                break_wrapped(x, i);
            }else if(candy[x][i]->is_bomb()){
                candy[x][i]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb(x, i);
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
            if(candy[x][i]->is_striped()){
                break_striped(x, i);
            }else if(candy[x][i]->is_wrapped()){
                break_wrapped(x, i);
            }else if(candy[x][i]->is_bomb()){
                candy[x][i]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb(x, i);
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
            if(candy[x][i]->is_striped()){
                break_striped(x, i);
            }else if(candy[x][i]->is_wrapped()){
                break_wrapped(x, i);
            }else if(candy[x][i]->is_bomb()){
                candy[x][i]->set_color_to_break(candy[x][y]->getFillColor());
                break_bomb(x, i);
            }
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
        cout<<candy[x][y]->get_direction()<<" here "<<endl;
        break_column(x, y);
    }
    cout<<"done"<<endl;
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
}


void GameManager::break_wrapped(int x, int y, int blast_range){
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
    for(int i=x-blast_range;i<=x+blast_range;i++){
        for(int j=y-blast_range;j<=y+blast_range;j++){
            if (i>=0 && i<9 && j>=0 && j<9 && !candy[i][j]->get_wall() && !candy[i][j]->is_ingredient()){
                if(candy[i][j]->is_striped()){
                    break_striped(i, j);
                }else if(candy[i][j]->is_wrapped()){
                    break_wrapped(i, j);
                }else if(candy[i][j]->is_bomb()){
                    candy[i][j]->set_color_to_break(candy[x][y]->getFillColor());
                    break_bomb(i, j);
                }
                candy[i][j]->setFillColor(FL_BLACK);
                candy[i][j]->start_pop_animation();
            }
        }
    }
}

void GameManager::break_bomb(int x, int y){
    cout<<"boom"<<endl;
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
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
}

Fl_Color GameManager::get_most_present_color(){
    enum Colors{
        red = 0, blue, yellow, dark_cyan, green
    };
    int color_count[5] = {0};
    int index_of_res = 0;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            if (candy[i][j]->getFillColor()==FL_RED){
                color_count[red]++;
            }else if (candy[i][j]->getFillColor()==FL_BLUE){
                color_count[blue]++;
            }else if (candy[i][j]->getFillColor()==FL_YELLOW){
                color_count[yellow]++;
            }else if (candy[i][j]->getFillColor()==FL_DARK_CYAN){
                color_count[dark_cyan]++;
            }else if (candy[i][j]->getFillColor()==FL_GREEN){
                color_count[green]++;
            }
        }
    }

    for(int x=0;x<5;x++){
        if (color_count[index_of_res]<color_count[x]){
            index_of_res = x;
        }
    }

    return COLORS[index_of_res];
}

void GameManager::break_board(int x, int y){
    for (Fl_Color c: COLORS){
        candy[x][y] = make_shared<Bomb_candy>(candy[x][y]->getCenter(), 40, 40);
        candy[x][y]->set_color_to_break(c);
        break_bomb(x, y);
    }
}

void GameManager::break_bomb_wrapped(int x, int y){
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            if (candy[i][j]->getFillColor()==candy[x][y]->get_color_to_break()){
                break_wrapped(i, j);
            }
        }
    }
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
}

void GameManager::break_bomb_striped(int x, int y){
    int direction = 0;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            if (candy[i][j]->getFillColor()==candy[x][y]->get_color_to_break()){
                candy[i][j]->set_direction(direction%2);    // not using the enum horizontal and vertical because it's easier in this case
                break_striped(i, j);
                direction++;
            }
        }
    }
    candy[x][y] = make_shared<Candy>(candy[x][y]->getCenter(), 40, 40);
}

void GameManager::break_striped_wrapped(int x, int y){
    if(x-1>=0){
        break_column(x-1, y);       //breaks the column on the left of the center x,y
    }

    break_column(x, y);             //breaks the column of the center x,y
    break_row(x, y);                //breaks the row of the center x,y

    if(x+1<9){
        break_column(x+1, y);       //breaks the column on the left of the center x,y
    }

    if (y-1>=0){                    
        break_row(x, y-1);          //breaks the row on top of the center x,y
    }

    if(y+1<9){
        break_row(x,y+1);           //breaks the row below the center x,y
    }
}