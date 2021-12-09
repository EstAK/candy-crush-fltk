#include<iostream>

#ifndef CONST_H_
#define CONST_H_

//header file containing all the constants
const int windowWidth=550;
const int windowHeight=550;
const double refreshPerSecond=60;
const std::string maps[3]={"boards/board0.txt","boards/board1.txt","boards/board2.txt"}; //[2] changed -> number of maps.
const std::string score_file = "score_file.txt";
const char* const w = "W";
const char* const c = "C";
const char* const i = "I";
#endif