#pragma once
#include "sac.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

struct LettresJoueurs{
    LettresJoueurs();
    vector<char> lettres;
    void remplirLettres(Sac &sac);
    void afficherLettres();
    bool estVide() const;
};