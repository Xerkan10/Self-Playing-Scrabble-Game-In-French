#pragma once
#include <unordered_map>

using namespace std;

struct GaddagNode {
    bool terminal;
    unordered_map<char, GaddagNode*> enfants; 
    GaddagNode();
    ~GaddagNode();
    GaddagNode* trouverOuCreerEnfant(char c);
};
