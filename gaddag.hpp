#pragma once
#include <iostream>
#include "spot.hpp"
#include "GaddagNode.hpp"

using namespace std;

struct Gaddag{
    //default initialization of a scrabble board
    GaddagNode* racine;
    Gaddag() ;
    ~Gaddag();
    
    void clear(GaddagNode* node);
    void insertion(const string &word) ;
    bool recherche(const string &word) const;
    void chargerDictionnaire(const std::string& filename);

};