#include "board.hpp"
#include "gaddag.hpp"
#include "sac.hpp"
#include "LettresJoueur.hpp"
#include "etat.hpp"
#include <stack>

#include <iostream>
#include <sstream>

using namespace std;

int sideWordCheckerVertical(const Etat &etat, Sac s, const Gaddag &gaddag, int row, int col)
{
  int x_current = row;
  int y_current = col;
  bool up_empty = false;
  bool down_empty = false;

  // boarder check
  if (x_current > 0 && etat.b.spots[(x_current - 1) * 15 + y_current].letter == 0)
  {
    up_empty = true;
  }
  else if (x_current == 0)
  {
    up_empty = true;
  }

  if (x_current < 14 && etat.b.spots[(x_current + 1) * 15 + y_current].letter == 0)
  {
    down_empty = true;
  }
  else if (x_current == 14)
  {
    down_empty = true;
  }

  if (up_empty && down_empty)
    return 0;

  int score = 0;
  int wordMultiplier = 1;
  int sens = -1; // we will start searching by going left, but this

  string word = ""; // the side word start with the letter in the current position, we initiliaze it to empty string
  while (true)
  {
    if (x_current >= 0 && x_current < 15 && etat.b.spots[x_current * 15 + y_current].letter != 0)
    {
      word += etat.b.spots[x_current * 15 + y_current].letter;

      if (x_current == row && y_current == col)
      {
        score += etat.b.spots[x_current * 15 + y_current].bonus.apply_letter(s.lettreValeurs(etat.b.spots[x_current * 15 + y_current].letter));
      }
      else
      {
        score += s.lettreValeurs(etat.b.spots[x_current * 15 + y_current].letter);
      }
      if (int(etat.b.spots[x_current * 15 + y_current].bonus.word_factor != 1) && x_current == row && y_current == col)
      {
        wordMultiplier = etat.b.spots[x_current * 15 + y_current].bonus.apply_word(wordMultiplier);
      }

      x_current += sens;
    }
    else if (x_current >= 0 && x_current < 15 && etat.b.spots[x_current * 15 + y_current].letter == 0)
    {
      if (sens == -1)
      {
        sens = 1;
        word += '+';
        x_current = row + 1;
        y_current = col;
      }
      else
      {
        if (gaddag.recherche(word))
        {
          return score * wordMultiplier;
        }
        else
        {
          return -1;
        }
      }
    }
    else
    {
      if (sens == -1)
      {
        sens = 1;
        word += '+';
        x_current = row + 1;
        y_current = col;
      }
      else
      {
        if (gaddag.recherche(word))
        {
          return score * wordMultiplier;
        }
        else
        {
          return -1;
        }
      }
    }
  }
}

int sideWordCheckerHorizontal(const Etat &etat, Sac s, const Gaddag &gaddag, int row, int col)
{
  int x_current = row;
  int y_current = col;

  bool left_empty = false;
  bool right_empty = false;

  // boarder check
  if (y_current > 0 && etat.b.spots[x_current * 15 + (y_current - 1)].letter == 0)
  {
    left_empty = true;
  }
  else if (y_current == 0)
  {
    left_empty = true;
  }

  if (y_current < 14 && etat.b.spots[x_current * 15 + (y_current + 1)].letter == 0)
  {
    right_empty = true;
  }
  else if (y_current == 14)
  {
    right_empty = true;
  }

  if (right_empty && left_empty)
    return 0;

  int score = 0;
  int wordMultiplier = 1;
  int sens = -1; // we will start searching by going left as usual

  string word = ""; // the side word start with the letter in the current position, we initiliaze it to empty string
  while (true)
  {
    if (y_current >= 0 && y_current < 15)
    {
      if (etat.b.spots[x_current * 15 + y_current].letter != 0)
      {
        word += etat.b.spots[x_current * 15 + y_current].letter;

        if (x_current == row && y_current == col)
        { // this is the letter that we are adding so we can apply the bonus
          score += etat.b.spots[x_current * 15 + y_current].bonus.apply_letter(s.lettreValeurs(etat.b.spots[x_current * 15 + y_current].letter));
        }
        else
        { // this spot was already occupied by a letter, so we can just add the value of the letter. No bonus applied
          score += s.lettreValeurs(etat.b.spots[x_current * 15 + y_current].letter);
        }
        if (int(etat.b.spots[x_current * 15 + y_current].bonus.word_factor) != 1 && x_current == row && y_current == col)
        {
          wordMultiplier = etat.b.spots[x_current * 15 + y_current].bonus.apply_word(wordMultiplier); // word factor applied, because we are in the initial position
        }

        y_current += sens;
      }
      else if (etat.b.spots[x_current * 15 + y_current].letter == 0)
      { // the sequence of letters ended
        if (sens == -1)
        { // sequence ended but we are in negative direction, meaning that we should add '+' and continue in the other direction from the initial position +1
          sens = 1;
          word += '+';
          x_current = row;
          y_current = col + 1;
        }
        else
        {
          if (gaddag.recherche(word))
          {
            return score * wordMultiplier;
          }
          else
          {
            return -1;
          }
        }
      }
    }

    else
    { // now we are out of the board
      if (sens == -1)
      { // first option is we are going up, so the word is not terminated now, we need to change direction and continue from the initial position +1
        sens = 1;
        word += '+';
        x_current = row;
        y_current = col + 1;
      }
      else
      { // we are going down and we are out of the board, so the word is terminated, lets check if it is in the dictionary
        if (gaddag.recherche(word))
        {
          return score * wordMultiplier;
        }
        else
        {
          return -1;
        }
      }
    }
  }
}

int main()
{
  srand(time(0));
  Board b;
  LettresJoueurs lj;
  Sac s;
  std::stack<Etat> stack;

  // lj.lettres.push_back('Z');
  // lj.lettres.push_back('S');
  // lj.lettres.push_back('E');
  // lj.lettres.push_back('I');
  // lj.lettres.push_back('E');
  // lj.lettres.push_back('A');
  // lj.lettres.push_back('Q');

  lj.remplirLettres(s);
  
  Gaddag gaddag;
  gaddag.chargerDictionnaire("dico.txt");

  std::cout << b << std::endl;

  std::stringstream ss;

  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;
  ss << "..............." << endl;

  b.load(ss);

  

  Etat current_etat;
  LettresJoueurs new_lj;
  GaddagNode *new_courant;

  int total_score = 0;

  while (!lj.estVide())
  {
    cout << "Lettres of the player: ";
    lj.afficherLettres();
    bool boardIsEmpty = (b.spots[7 * 15 + 7].letter == 0);
    bool isAnchor = false;
    Etat best_move;
    best_move.score = 0;

    for (int i = 0; i < 225; ++i)
    {
      if (b.spots[i].letter == 0)
      {
        int row = i / 15;
        int col = i % 15;

        isAnchor =
            (col < 14 && b.spots[i + 1].letter != 0) ||  // right
            (col > 0 && b.spots[i - 1].letter != 0) ||   // left
            (row < 14 && b.spots[i + 15].letter != 0) || // down
            (row > 0 && b.spots[i - 15].letter != 0);    // up

        if (boardIsEmpty)
        {
          isAnchor = (row == 7 && col == 7); // only allow (7,7) on first move
        }

        if (isAnchor)
        {
          Board trialBoard = b;
          Etat new_etat;
          int x = row; // this will be dinamic but row and col are the initial position, when we reach + sign we will continue from row and col values
          int y = col;
          GaddagNode *courant = gaddag.racine;
          Etat etat = Etat(courant, lj, x, y, trialBoard, -1, 1, 0);
          stack.push(etat);
          while (!stack.empty())
          { // for vertical only
            current_etat = stack.top();
            stack.pop();

            if (current_etat.x < 0 || current_etat.x > 14 || current_etat.y < 0 || current_etat.y > 14)
            {
              if (current_etat.sens == -1)
              {
                if (current_etat.courant->enfants.find('+') != current_etat.courant->enfants.end())
                {
                  new_etat = Etat(current_etat.courant->enfants['+'], current_etat.lj, row + 1, col, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score); // here we will continue from other side of the initial position (initial+1)
                  new_etat.score = current_etat.score;
                  new_etat.sens = 1;
                  stack.push(new_etat);
                }
              }
              else
              {
                if (current_etat.courant && current_etat.courant->terminal && current_etat.sens == 1)
                {
                  new_etat = current_etat;
                  new_etat.score = new_etat.wf * new_etat.score;
                  new_etat.score += new_etat.side_score;

                  if (new_etat.score > best_move.score)
                  {
                    best_move = new_etat;
                  }
                }
              }
              continue; // we are out of the board we need to continue without checking the other if statements
            }
            if (current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter != 0)
            {
              if (current_etat.courant->enfants.find(current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter) != current_etat.courant->enfants.end())
              {
                new_etat = Etat(current_etat.courant->enfants[current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter], current_etat.lj, current_etat.x + current_etat.sens, current_etat.y, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score);
                new_etat.score = current_etat.score;

                // if (int(current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.word_factor) != 1){ //the spot that we are in has a word factor, we need to multiply the score for the new_etat
                //   new_etat.wf = current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_word(new_etat.wf);
                // }

                // again we're using the letter value of the current position to compute the score of the new etat
                // new_etat.score += current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_letter(s.lettreValeurs(current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter));

                new_etat.score += s.lettreValeurs(current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter);
                stack.push(new_etat);
              }
            }
            else
            {
              if (current_etat.courant && current_etat.courant->terminal && current_etat.sens == 1)
              {
                new_etat = current_etat;
                new_etat.score = new_etat.wf * new_etat.score;
                new_etat.score += new_etat.side_score;

                if (new_etat.score > best_move.score)
                {
                  best_move = new_etat;
                }
              }

              for (int i = current_etat.lj.lettres.size() - 1; i >= 0; i--)
              {
                char letter = current_etat.lj.lettres[i];
                if (current_etat.courant != nullptr && current_etat.courant->enfants.find(current_etat.lj.lettres[i]) != current_etat.courant->enfants.end())
                {
                  new_lj.lettres = current_etat.lj.lettres;
                  new_lj.lettres.erase(new_lj.lettres.begin() + i);
                  new_courant = current_etat.courant->enfants[letter];
                  new_etat = Etat(new_courant, new_lj, current_etat.x + current_etat.sens, current_etat.y, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score);
                  new_etat.score = current_etat.score;
                  new_etat.b.spots[current_etat.x * 15 + current_etat.y].letter = letter;
                  if (int(current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.word_factor) != 1)
                  { // the spot that we are in has a word factor, we need to multiply the score for the new_etat
                    new_etat.wf = current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_word(new_etat.wf);
                  }

                  int horizontal_score = sideWordCheckerHorizontal(new_etat, s, gaddag, current_etat.x, current_etat.y);

                  if (horizontal_score != -1)
                  {
                    new_etat.side_score += horizontal_score;
                    new_etat.score += new_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_letter(s.lettreValeurs(new_etat.b.spots[current_etat.x * 15 + current_etat.y].letter));
                    stack.push(new_etat);
                  }
                }
              }
              // we need to change directions
              if (current_etat.courant->enfants.find('+') != current_etat.courant->enfants.end() && current_etat.sens == -1)
              {                                                                                                                                                                  // no need to update the score
                new_etat = Etat(current_etat.courant->enfants['+'], current_etat.lj, row + 1, col, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score); // here we will continue from other side of the initial position (initial+1)
                new_etat.score = current_etat.score;
                new_etat.sens = 1;
                stack.push(new_etat);
              }
            }
          }
          stack.push(etat);
          while (!stack.empty())
          { // for horizontal only
            current_etat = stack.top();
            stack.pop();
            new_courant = current_etat.courant;

            // we are out of bonds so either we need to change direction or we need to terminate the word
            // If these options are not possible we need to continue to the next loop iteration
            if (current_etat.x < 0 || current_etat.x > 14 || current_etat.y < 0 || current_etat.y > 14)
            {
              if (current_etat.sens == -1)
              {
                if (current_etat.courant->enfants.find('+') != current_etat.courant->enfants.end())
                {
                  new_etat = Etat(current_etat.courant->enfants['+'], current_etat.lj, row, col + 1, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score); // here we will continue from other side of the initial position (initial+1)
                  new_etat.score = current_etat.score;
                  new_etat.sens = 1;
                  stack.push(new_etat);
                }
              }
              else
              {
                if (current_etat.courant && current_etat.courant->terminal && current_etat.sens == 1)
                {
                  new_etat = current_etat;
                  new_etat.score = new_etat.wf * new_etat.score;
                  new_etat.score += new_etat.side_score;

                  if (new_etat.score > best_move.score)
                  {
                    best_move = new_etat;
                  }
                }
              }
              continue; // we are out of the board we need to continue without checking the other if statements
            }
            // Threre is a letter in the current position
            if (current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter != 0)
            {
              if (current_etat.courant->enfants.find(current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter) != current_etat.courant->enfants.end())
              {
                new_etat = Etat(current_etat.courant->enfants[current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter], current_etat.lj, current_etat.x, current_etat.y + current_etat.sens, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score);
                new_etat.score = current_etat.score;

                // if (int(current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.word_factor) != 1){ //the spot that we are in has a word factor, we need to multiply the score for the new_etat
                //   new_etat.wf = current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_word(new_etat.wf);
                // }
                //  again we're using the letter value of the current position to compute the score of the new etat
                // new_etat.score += current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_letter(s.lettreValeurs(current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter));

                new_etat.score += s.lettreValeurs(current_etat.b.spots[current_etat.x * 15 + current_etat.y].letter);
                stack.push(new_etat);
              }
            }
            // There is no letter in the current position
            else
            {
              if (current_etat.courant && current_etat.courant->terminal && current_etat.sens == 1)
              {
                new_etat = current_etat;
                new_etat.score = new_etat.wf * new_etat.score;
                new_etat.score += new_etat.side_score;

                if (new_etat.score > best_move.score)
                {
                  best_move = new_etat;
                }
              }

              for (int i = current_etat.lj.lettres.size() - 1; i >= 0; i--)
              {
                char letter = current_etat.lj.lettres[i];
                if (current_etat.courant != nullptr && current_etat.courant->enfants.find(current_etat.lj.lettres[i]) != current_etat.courant->enfants.end())
                {
                  new_lj = current_etat.lj;
                  new_lj.lettres.erase(new_lj.lettres.begin() + i);
                  new_courant = current_etat.courant->enfants[letter];
                  new_etat = Etat(new_courant, new_lj, current_etat.x, current_etat.y + current_etat.sens, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score);
                  new_etat.score = current_etat.score;
                  new_etat.b.spots[current_etat.x * 15 + current_etat.y].letter = letter;
                  if (int(new_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.word_factor) != 1)
                  { // the spot that we are in has a word factor, we need to multiply the score for the new_etat
                    new_etat.wf = current_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_word(new_etat.wf);
                  }
                  int vertical_score = sideWordCheckerVertical(new_etat, s, gaddag, current_etat.x, current_etat.y);
                  if (vertical_score != -1)
                  {
                    new_etat.side_score += vertical_score;
                    new_etat.score += new_etat.b.spots[current_etat.x * 15 + current_etat.y].bonus.apply_letter(s.lettreValeurs(new_etat.b.spots[current_etat.x * 15 + current_etat.y].letter));
                    stack.push(new_etat);
                  }
                }
              }
              // we need to change directions
              if (current_etat.courant->enfants.find('+') != current_etat.courant->enfants.end() && current_etat.sens == -1)
              {
                new_etat = Etat(current_etat.courant->enfants['+'], current_etat.lj, row, col + 1, current_etat.b, current_etat.sens, current_etat.wf, current_etat.side_score); // here we will continue from other side of the initial position (initial+1)
                new_etat.score = current_etat.score;
                new_etat.sens = 1;
                stack.push(new_etat);
              }
            }
          }
        }
      }
    }
    if (best_move.score > 0)
    {
      std::cout << "Best move (score " << best_move.score << "):\n"
                << best_move.b << std::endl;

      // Apply best move
      b = best_move.b;
      lj = best_move.lj;    // This already has updated rack (used letters removed)
      lj.remplirLettres(s); // Refill to 7
      total_score += best_move.score;
      // best_move.b.save(cout); // optional: save intermediate board state
    }
    else
    {
      cout << "⚠️ No valid move found. Game over.\n";
      cout << "Total score: " << total_score << endl;
      return 0;
    }
  }
  cout << "No letters left in the bag. Game over.\n";
  cout << "Total score: " << total_score << endl;

  return 0;
}
