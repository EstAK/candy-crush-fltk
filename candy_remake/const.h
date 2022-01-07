#include<iostream>

#ifndef CONST_H_
#define CONST_H_

//header file containing all the constants
enum Direction{
        horizontal=0, vertical
    };

const Fl_Color COLORS[6]={FL_RED,FL_BLUE,FL_YELLOW,FL_DARK_CYAN,FL_GREEN, FL_CYAN};

const int windowWidth=550;
const int windowHeight=550;
const double refreshPerSecond=60;
const std::string maps[4]={"boards/board0.txt","boards/board1.txt","boards/board2.txt", "boards/board3.txt"}; //[2] changed -> number of maps.
const std::string score_file = "score_file.txt";
const char* const w = "W";
const char* const c = "C";
const char* const i = "I";
#endif