#pragma once
#include <iostream>
#include "GaddagNode.hpp"
#include "LettresJoueur.hpp"
#include "board.hpp"
using namespace std;

struct Etat{
    GaddagNode* courant;
    LettresJoueurs lj;
    int x; // x is the row
    int y; // y is the column
    Board b;
    int wf; // word factor
    int score;
    int side_score;
    int sens;
    Etat(GaddagNode* courant, LettresJoueurs lj, int x, int y, Board b, int sens, int wf, int side_score);
    Etat();
};