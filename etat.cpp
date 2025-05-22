#include "etat.hpp"
#include "board.hpp"

// Constructor
Etat::Etat(GaddagNode* courant, LettresJoueurs lj, int x, int y, Board b, int sens, int wf, int side_score) {
    this->side_score = side_score;
    this->courant = courant;   
    this->lj = lj;      
    this->x = x; 
    this->y = y;  
    this->b = b;      
    this->sens = sens;
    this->wf = wf;
    this->score = 0;
}

Etat::Etat() {
    this->courant = nullptr;   
    this->lj = LettresJoueurs();      
    this->x = 0; 
    this->y = 0;  
    this->b = Board();      
    this->sens = -1;
    this->wf = 1;
    this->score = 0;
    this->side_score = 0;
}

