#include "LettresJoueur.hpp"
#include <istream>
#include <fstream>

using namespace std;

LettresJoueurs::LettresJoueurs(){
    lettres = {};
}

void LettresJoueurs::remplirLettres(Sac &sac){
    while(lettres.size() < 7 && !sac.estVide()){
        lettres.push_back(sac.piocherLettre());
    }
}

void LettresJoueurs::afficherLettres(){
    for(int i = 0; i < lettres.size(); i++){
        cout << lettres[i] << "";
    }
    cout << endl;
}

bool LettresJoueurs::estVide() const {
    return lettres.empty();
}

