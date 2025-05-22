#include "sac.hpp"
#include <istream>
#include <fstream>
#include <random>

using namespace std;

Sac::Sac(){
    occurences = {
        {'A', 9}, {'B', 2}, {'C', 2}, {'D', 3}, {'E', 15}, {'F', 2}, {'G', 2}, 
        {'H', 2}, {'I', 8}, {'J', 1}, {'K', 1}, {'L', 5}, {'M', 3}, {'N', 6}, 
        {'O', 6}, {'P', 2}, {'Q', 1}, {'R', 6}, {'S', 6}, {'T', 6}, {'U', 6}, 
        {'V', 2}, {'W', 1}, {'X', 1}, {'Y', 1}, {'Z', 1}
    };

    valeurs = {
        {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1}, {'F', 4}, {'G', 2}, 
        {'H', 4}, {'I', 1}, {'J', 8}, {'K', 10}, {'L', 1}, {'M', 2}, {'N', 1}, 
        {'O', 1}, {'P', 3}, {'Q', 8}, {'R', 1}, {'S', 1}, {'T', 1}, {'U', 1}, 
        {'V', 4}, {'W', 10}, {'X', 10}, {'Y', 10}, {'Z', 10}
    };

    for (auto& pair : occurences) {
        char lettre = pair.first;
        int occurence = pair.second;
        occurences[lettre] = occurence;
        for (int i = 0; i < occurence; ++i) {
            lettres.push_back(lettre);
        }
    }

}

char Sac::piocherLettre(){
    if (lettres.empty()) return '\0'; // No letters left, empty char

    int index = std::rand() % lettres.size(); // Select random index
    char lettre = lettres[index]; // Get letter at that index

    // Swap with last element and remove last element (O(1)), if we use erase it becomes O(n)
    lettres[index] = lettres.back();
    lettres.pop_back();

    // Decrease count in the hashmap
    occurences[lettre]--;

    return lettre;
}

bool Sac::estVide(){
    return lettres.empty();
}

int Sac::lettreValeurs(char lettre){
    return valeurs[lettre];
}
