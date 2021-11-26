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
        make_board(maps[0]);     
}

Canvas::~Canvas(){
    
}

void Canvas::make_board(string map){ //TODO: Finish it later.
        read_file(map);
        current_map=map;
        candy_score=Score(0);
        int fruits=0;
        bool gj=false;
        
        for (int x = 0; x<9; x++){
            for (int y = 0; y<9; y++){
              
                if (lines[y][x] == *c){
                    candy[x][y]=Candy(Point{50*x+25,50*y+25},40,40,color[rand()%5],FL_BLACK);
                }
                else if(lines[y][x]== *w){
                    candy[x][y]=Candy(Point{50*x+25, 50*y+25}, 40, 40,FL_MAGENTA,FL_BLACK);
                }
                else if(lines[y][x]== *i){
                    candy[x][y]=Candy(Point{50*x+25, 50*y+25},40, 40,FL_RED,FL_BLACK);
                    fruits++;
                    candy[x][y].set_fruit(true);
                    gj=true;
                }
            }
        }    
        if(gj){game_obj.fruits_on(fruits);}else{game_obj=Objective();}
        set_the_neighbours();
        set_the_wall();
        
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
         for(int i=0;i<candy.size();i++){
            for(int j=0;j<candy[0].size();j++){
                candy[i][j].draw();
                if(candy[i][j].get_wall()!=true && candy[i][j].get_fruit()==false){
                    break_candies(i,j,0,0,true);} //Checks all the candies all the time and breaks them if it must.
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

        if(timer!=200){  //Timer set ; Vlad: pressing on a candy or when candies fall/break will restart the timer.
            timer++;
        }else{          //restart the timer.
            timer=0;
            can_vibrate=true;
        }

        for(int i=0;i<candy.size();i++){ //Checks if there is a free place that can be filled ; Takes into consideration the walls(phyics) as well.
            for(int j=0;j<candy[0].size();j++){
                if(candy[i][j].getFillColor() == FL_BLACK){
                    if (not has_moved|| candy[i][j].is_slide_complete()){
                        fall_candies(i,j);
                        timer=0; //reset the timer.
                    }
                    can_vibrate=false; //reset the vibrate;
                }
            }
        }  

} 

void Canvas::mouseMove(Point mouseLoc){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(candy[i][j].contains(mouseLoc)){
                    candy[i][j].setFrameColor(FL_RED);
                }else{
                    candy[i][j].setFrameColor(FL_BLACK);
                }
            }
        }
}

void Canvas::mouseClick(Point mouseLoc){
        has_moved=true;
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(candy[i][j].contains(mouseLoc) && current.getFillColor() == FL_BLACK){
                    if(candy[i][j].get_wall()==false ){  //make sure that is not a wall. ; Vlad:time n vibrate
                        current=candy[i][j];
                        x=i;
                        y=j;
                        timer=0;
                        can_vibrate=false;
                    }
                }else if(candy[i][j].contains(mouseLoc)){
                    if(candy[i][j].verify_neighbours(current)){
                        break_candies(i,j,x,y);
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
                        timer=0;
                        can_vibrate=false;
                    }else{
                        cout<<"Selected new one"<<endl;
                        if(candy[i][j].get_wall()==false){  //timer n vibrate
                            current=candy[i][j];
                            x=i;
                            y=j;
                            timer=0;
                            can_vibrate=false;
                        }    
                    }
                }
            }
        }
}

void Canvas::set_the_neighbours(){
        for(int i=0;i<9;i++){
         for(int j=0;j<9;j++){
          if(i+1<9) candy[i][j].neighbours.push_back(&candy[i+1][j]);
          if(j+1<9) candy[i][j].neighbours.push_back(&candy[i][j+1]);
          if(i-1>=0) candy[i][j].neighbours.push_back(&candy[i-1][j]);
          if(j-1>=0) candy[i][j].neighbours.push_back(&candy[i][j-1]);
         }
        }
}

void Canvas::set_the_wall(){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(candy[i][j].getFillColor()==248){candy[i][j].set_wall(true);}
            }
        }
}

void Canvas::break_candies(int x,int y,int i,int j,bool pc){
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

void Canvas::check_impossible(int i,int j,bool vibrate){
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

bool Canvas::forshadowing_over_9000(int x,int y,Fl_Color color,bool vibrate){ //Vlad: just change it's name later.... IT'S OVER 9000!!!
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

int Canvas::fall_candies(int start_x,int start_y){ //Function by Recursion
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

void Canvas::fall_walls(int start_x, int start_y){ //Function to make the candies fall in diag if there is a wall above.
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



