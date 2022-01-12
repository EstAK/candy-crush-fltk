#include<iostream>

#ifndef CONST_H_
#define CONST_H_

//header file containing all the constants
enum Direction{
        horizontal=0, vertical
    };

enum INDEX_COLORS{
        red = 0, blue, yellow, orange, green, purple
    };

#define PURPLE fl_rgb_color(102,0,204)
#define ORANGE fl_rgb_color(255,153,51)
#define GREEN fl_rgb_color(0,153,0)
#define GRAY fl_rgb_color(160,160,160)


const Fl_Color COLORS[6]={FL_RED,FL_BLUE,FL_YELLOW,ORANGE,GREEN, PURPLE};

const int windowWidth=550;
const int windowHeight=550;
const int squareLenght=40;
const int circleRadius=20;

const double refreshPerSecond=60;
const std::string maps[5]={"boards/board0.txt","boards/board1.txt","boards/board2.txt", "boards/board3.txt","boards/edited_level.txt"}; //[2] changed -> number of maps.
const std::string score_file = "score_file.txt";

const char* const w = "W";
const char* const c = "C";
const char* const i = "I";
#endif