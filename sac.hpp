#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <ctime>
using namespace std;

struct Sac{
    Sac();
    vector<char> lettres;
    unordered_map<char, int> occurences;
    unordered_map<char, int> valeurs;
    char piocherLettre();
    bool estVide();
    int lettreValeurs(char lettre);
};

