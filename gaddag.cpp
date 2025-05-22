#include "gaddag.hpp"
#include "GaddagNode.hpp"
#include <istream>
#include <fstream>
#include <iostream>
#include <string>   

using namespace std;

Gaddag::Gaddag(){
    racine = new GaddagNode();
}

Gaddag::~Gaddag() {
    clear(racine);
    racine = nullptr;  // Prevent dangling pointer access
}


void Gaddag::clear(GaddagNode* node) {
    if (!node) return;
    
    for (auto& pair : node->enfants) {
        clear(pair.second);
        pair.second = nullptr; // Prevent double deletion
    }
    
    delete node;
}


void Gaddag::insertion(const string &word){
    int n = word.length();
    for (int i = 0; i < n; ++i) {
        string prefix = word.substr(0, i + 1);
        reverse(prefix.begin(), prefix.end()); // Reverse prefix
        string suffix = word.substr(i + 1);

        string gaddagWord = prefix + "+" + suffix; // Use '+' as a separator
        GaddagNode* node = racine;
        for (char c : gaddagWord) {
            node = node->trouverOuCreerEnfant(c);
        }
        node->terminal = true;
    }
}


bool Gaddag::recherche(const string &word) const{
    GaddagNode* node = racine;
    for (char c : word) {
        if (node->enfants.find(c) == node->enfants.end()) {
            return false;
        }
        node = node->enfants[c];
    }
    return node->terminal;
}

void Gaddag::chargerDictionnaire(const string& filename) {
    string path = "../data/"+filename;
    ifstream file(path);
    if (!file) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl;
        return;
    }

    string word;
    while (getline(file, word)) {
        insertion(word); // Insert each word into the GADDAG structure
    }

    file.close();
    cout << "Dictionnaire chargé avec succès depuis " << filename << endl;
}
