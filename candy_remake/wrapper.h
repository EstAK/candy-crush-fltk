#include<iostream>

using namespace std;

#ifndef WRAPPER_H_
#define WRAPPER_H_

template<typename Type>
class Widget_wrapper : public Type {
    /* 
        wrapper class abstracting text_window implementation from the intro_window
    */
public:
    Widget_wrapper(int posx, int posy, int height, int width, const char* txt): Type(posx, posy, height, width, txt) {}


};

#endif