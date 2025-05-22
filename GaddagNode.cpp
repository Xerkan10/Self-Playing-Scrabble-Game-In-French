#include "GaddagNode.hpp"
#include <unordered_map>

using namespace std;

GaddagNode::GaddagNode(){
    terminal = false;
    enfants = {};
}

GaddagNode::~GaddagNode() {
    for (auto& pair : enfants) {
        delete pair.second;
    }
}

GaddagNode* GaddagNode::trouverOuCreerEnfant(char c){
    if (enfants.find(c) == enfants.end()) {
        enfants[c] = new GaddagNode();
    }
    return enfants[c];
}