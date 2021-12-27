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

using namespace std;

#ifndef GameManager_H_
#define GameManager_H_

class GameManager{
   shared_ptr<Item>** candy=new shared_ptr<Item>*[9]();
   Score* candy_score{0};
   Objective* game_obj;
   bool* can_vibrate;
   bool* not_impossible;
   Fl_Color color[5]={FL_RED,FL_BLUE,FL_YELLOW,FL_DARK_CYAN,FL_GREEN};
public:
   GameManager();
   void set_up(shared_ptr<Item>**,Score&,Objective&);
   bool break_candies(int x,int y,int i,int j,bool pc=false);
   void destroy_candies(int x,int y,int counter_left_right,int counter_up_down,bool pc=false);
   int fall_candies(int,int, bool);
   void fall_walls(int,int);  
   void set_the_neighbours();
 

};


#endif


// int GameManager::fall_candies(int x, int y){
//     cout<<"x: "<<x<<" y: "<<y<<endl;
//     if (y==0){
//         if(candy[x][y].is_fall_complete()){
//             candy[x][y].setFillColor(color[rand()%5]);
//             candy[x][y].start_fall_animation({candy[x][y].getCenter().x, candy[x][y].getCenter().y-50});
//             candy[x][y].set_move(false);

//         }else{
//             candy[x][y].set_move(true);
//         }
//         return 1;

//     }
//     if(not candy[x][y].get_wall()){
//         if(candy[x][y].is_fall_complete()){
//             candy[x][y].set_move(false);
//             candy[x][y].setFillColor(candy[x][y-1].getFillColor());
//             candy[x][y].start_fall_animation({candy[x][y].getCenter().x, candy[x][y].getCenter().y-50});
//         }else{
//             return 1;
//         }
//         fall_candies(x,y-1);
//     }

//     return 1;
// }
// int GameManager::fall_candies(int x, int y){
//     cout<<"x: "<<x<<" y: "<<y<<endl;
//     if (y==0){
//         candy[x][y].setFillColor(color[rand()%5]);
//         Point fall = candy[x][y].getCenter();
//         if(candy[x][y].is_fall_complete()){
//             candy[x][y].setCenter({candy[x][y].getCenter().x, candy[x][y].getCenter().y-50});
//             candy[x][y].start_fall_animation(fall, false);
//             candy[x][y].set_move(false);
//         }else{
//             candy[x][y].set_move(true);
//         }
//         return 1;

//     }
//     if(not candy[x][y].get_wall()){
//         Point fall = candy[x][y].getCenter();
//         if(not candy[x][y-1].get_wall()){
//             candy[x][y].setCode(candy[x][y-1].getFillColor());
//         }else{
//             for (int x_fork=x-1;x_fork<x+1;x_fork++){
//                 if (not candy[x_fork][y].get_wall() && x_fork<9 && x_fork>=0){
//                     candy[x][y].setCode(candy[x_fork][y-1].getFillColor());
//                     break;
//                 }  
//             }
//         }
//         if(candy[x][y].is_fall_complete()){
//             candy[x][y].setCenter({candy[x][y].getCenter().x, candy[x][y].getCenter().y-50});
//             candy[x][y].start_fall_animation(fall, false);
//             candy[x][y].set_move(false);
//         }else{
//             candy[x][y].set_move(true);
//             return 1;
//         }
//         fall_candies(x, y-1);
//         return 1;
//     }else{
//         for (int x_fork=x-1;x_fork<x+1;x_fork++){
//             if (not candy[x_fork][y].get_wall() && x_fork<9 && x_fork>=0){
//                 fall_candies(x_fork, y-1);
//                 Point fall = candy[x_fork][y].getCenter();
//                 candy[x_fork][y].setCenter(candy[x_fork][y-1].getCenter());
//                 candy[x_fork][y].setCode(candy[x_fork][y-1].getFillColor());
//                 candy[x_fork][y].start_fall_animation(fall, false);
//                 return 1;
//             }
//             return 1;
//         }
//     }

//     return 1;
// }


// int GameManager::fall_candies(int x, int y){
//     cout<<"x: "<<x<<" y: "<<y<<endl;
//     if (y==0){
//         candy[x][y].setFillColor(color[rand()%5]);
//         Point fall = candy[x][y].getCenter();
//         if(candy[x][y].is_fall_complete()){
//             candy[x][y].setCenter({candy[x][y].getCenter().x, candy[x][y].getCenter().y-50});
//             candy[x][y].start_fall_animation(fall, false);
//             candy[x][y].set_move(false);
//         }else{
//             candy[x][y].set_move(true);
//         }
//         return 1;

//     }
//     if(not candy[x][y].get_wall()){
//         Point fall = candy[x][y].getCenter();
//         int x_fork;
//         if(not candy[x][y-1].get_wall()){
//             candy[x][y].setCode(candy[x][y-1].getFillColor());
//             candy[x][y].setCenter({candy[x][y].getCenter().x, candy[x][y].getCenter().y-50});
//             x_fork=x;
//         }else{
//             for (x_fork=x-1;x_fork<x+1;x_fork++){
//                 if (not candy[x_fork][y].get_wall() && x_fork<9 && x_fork>=0){
//                     candy[x][y].setCode(candy[x_fork][y-1].getFillColor());
//                     candy[x][y].setCenter(candy[x_fork][y-1].getCenter());
//                     break;
//                 }  
//             }
//         }
//         if(candy[x][y].is_fall_complete()){
//             candy[x][y].start_fall_animation(fall, false);
//             candy[x][y].set_move(false);
//         }else{
//             // candy[x][y].set_move(true);
//             return 1;
//         }
//         fall_candies(x_fork, y-1);
//         return 1;
//     }
//     return 1;
// }

