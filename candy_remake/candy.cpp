#include"rectangle.h"
#include"circle.h"
#include"candy.h"

using namespace std;

Candy::Candy(Point center,int w,int h,Fl_Color fillColor,Fl_Color frameColor, Fl_Boxtype box_type):Item(),Rectangle(center,w,h,fillColor,frameColor, box_type){}

bool Candy::verify_neighbours(shared_ptr<Item> current){
    // method that verifies if the Item given as an argument is a Candy type neighbour 

    if(this->is_wall()==true){
        return false;
    }
    if(this->is_ingredient() && current->is_ingredient()){
        return false;
    }

    if(this->is_special_candy() || current->is_special_candy() ){  // ingredient or candy type Items
        for(auto i:neighbours){
            if(i->getCenter()==current->getCenter()){   // overloading Point struct == operator
                if(!this->is_ingredient() && !current->is_ingredient()){    // verifying each neighbour
                    return true;
                }

            }
        }
    }

    for(auto i:neighbours){     // verifying neighbours for normal Candy
        if(i->getCenter()==current->getCenter()){   // overloading Point struct == operator
            if(!this->is_ingredient() && !current->is_ingredient()){
                Fl_Color save=this->getFillColor(); //swapping colors
                this->setCode(i->getFillColor());
                i->setCode(save);
                return true;
            }else{
                if(this->is_ingredient()){
                    this->set_fruit(false); ////swapping ingredients
                    i->set_fruit(true);
                    return true;
                }else{

                }
            }
        }
    }
    return false;
}

void Candy::update_frosted_neighbours(){
    // removes a layer of frosting if a neighbour is frosted

    for(auto n: this->neighbours){
        if (n->has_frosting() && n->getFillColor() != this->getFillColor() ){
            n->set_layers_of_frosting(n->get_layers_of_frosting()-1);
        }
    }
}

void Candy::draw(){
    if (animation_pop && animation_pop->is_complete()){     // when completed assign animation to a nullptr
        delete animation_pop;
        animation_pop = nullptr;
    }
    if (animation_fall && animation_fall->is_complete()){   // when completed assign animation to a nullptr
        delete animation_fall;
        animation_fall = nullptr;
    }

    if (animation_fall){                // prioritize the fall animation over the pop animation 
        animation_fall->draw();
    }else if (animation_pop) {
        animation_pop->draw();
    }else{
        Rectangle::draw();          // if there is no animation draw the rectangle by itself
    }
}

void Candy::start_pop_animation(){
        animation_pop = new Animation_pop<Rectangle>(this);
    }

void Candy::start_fall_animation(Point dest, bool gb){
    animation_fall = new Animation_fall<Rectangle>(this, dest, gb);
}

bool Candy::is_fall_complete(){
        // checks if the fall animaiton is complete or non existant
        if (animation_fall){
            return false;
        }else{
            return true;
        }
    }

void Candy::set_layers_of_frosting(int new_layers){
    layers = new_layers;
}

int Candy::get_layers_of_frosting(){
    return layers;
}

bool Candy::has_frosting(){
    return get_layers_of_frosting() > 0;
}

void Candy::set_neigh(shared_ptr<Item> neigh){
    neighbours.push_back(neigh);    // add a new Item neighbour to neighbours vector
}

//Special_Candy
Special_candy::Special_candy(Point center, int w, int h, Fl_Color fillColor, Fl_Color frameColor, Fl_Boxtype box_type): 
        Candy(center, w, h, fillColor, frameColor, box_type) {}

//Striped_candy

void Striped_candy::set_direction(int d){
    stripes_direction = d;
}

int Striped_candy::get_direction(){
    return stripes_direction;
}


//Wrapped_candy

Wrapped_candy::Wrapped_candy(Point center, int w, int h, Fl_Color fillColor, Fl_Color frameColor, Fl_Boxtype box):Special_candy(center, w, h, fillColor, frameColor, box){}

//Bomb_candy

Bomb_candy::Bomb_candy(Point center, int w, int h, Fl_Color fillColor, Fl_Color frameColor, Fl_Boxtype box):Special_candy(center, w, h, fillColor, frameColor, box){}

void Bomb_candy::set_color_to_break(Fl_Color color){
    color_to_break = color;
}

Fl_Color Bomb_candy::get_color_to_break(){
    return color_to_break;
}

//Wall class
Wall::Wall(Point center, int w, int h, Fl_Color fillColor, Fl_Color frameColor):Item(),Rectangle(center,w,h,fillColor,frameColor, FL_FLAT_BOX){}

bool Wall::is_wall(){
    return true;
}


void Wall::draw(){
    Rectangle::draw();
}

Ingredient::Ingredient(Point center, int radius, Fl_Color fillColor, Fl_Color frameColor):Item(),Circle(center,radius,frameColor,fillColor){}

void Ingredient::draw(){
    // the same logic as in Candy::draw applies here but without a pop animation because an ingredient can't break

    if (animation_fall && animation_fall->is_complete()){
        delete animation_fall;
        animation_fall = nullptr;
    }

    if (animation_fall){
        animation_fall->draw();
    }else{
        Circle::draw();
    }
}

void Ingredient::start_fall_animation(Point dest, bool gb){
    animation_fall = new Animation_fall<Circle>(this, dest, gb);
}

bool Ingredient::verify_neighbours(shared_ptr<Item> current){
    for(auto i: neighbours){
        if (current->getCenter() == i->getCenter()){
            return true;
        }
    }
    return false;
}

void Ingredient::set_neigh(shared_ptr<Item> neigh){
    neighbours.push_back(neigh);
}
