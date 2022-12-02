#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Base Class
class StrategyGuideRound {
  protected:
    int score;
    int opponentValue;
    int userValue;
    int determineOpponentValue(char choice) {
      switch (choice)
      {
      case 'A': //rock
        return 0;
      case 'B': //paper
        return 1;
      case 'C': //scissor
        return 2;
      default:
        return -1;
      }
    }
  public:
    int getScore() {
      return score;
    }
    StrategyGuideRound(char opponentChoice) {
      opponentValue = determineOpponentValue(opponentChoice);
    }
};

//Derived Classes
class EncryptedRound : public StrategyGuideRound {
  private:
    int determineUserValue(char choice) {
      switch (choice)
      {
      case 'X': //rock
        return 0;
      case 'Y': //paper
        return 1;
      case 'Z': //scissor
        return 2;
      default:
        return -1;
      }
    }
    int determineOutcome(int opponentValue, int userValue) {
      int difference = userValue - opponentValue;
      int modularArithmetic = (difference % 3 + 3) % 3;
      //https://en.wikipedia.org/wiki/Modulo_operation#In_programming_languages
      //HOLY MOTHER OF GOD I HAD NO IDEA THAT MODULO WAS THIS COMPLICATED TO IMPLEMENT
      switch (modularArithmetic)
      {
      case 0: //draw
        return 3;
      case 1: //win
        return 6;
      case 2: //lose
        return 0;
      default:
        return -1;
      }
    }
  public:
    EncryptedRound(char opponentChoice, char userChoice) : StrategyGuideRound(opponentChoice) {
      userValue = determineUserValue(userChoice);
      score = determineOutcome(opponentValue, userValue) + (userValue + 1); //need to add 1 to offset the RPS values I needed to enumerate for use of modulo 3
    }
};

class DecryptedRound : public StrategyGuideRound {
  private:
    int determineUserValue(int opponentValue, char outcome) { //unused, just a proof
      switch (outcome)
      {
      case 'X': //lose
        return (opponentValue + 2) % 3; //0, 1, 2 input produces 2, 0, 1
      case 'Y': //draw
        return opponentValue;
      case 'Z': //win
        return (opponentValue + 1) % 3; //0, 1, 2 input produces 1, 2, 0
      default:
        return -1;
      }
    }
    int determineScore(int opponentValue, char outcome) {
      //need to add 1 to offset the RPS values I needed to enumerate for use of modulo 3
      switch (outcome)
      {
      case 'X': //lose
        return ((opponentValue + 2) % 3) + 1;
      case 'Y': //draw
        return (opponentValue + 1) + 3; // + 3 for draw
      case 'Z': //win
        return (opponentValue + 1) % 3 + 7; // + 7 comes from 6 for winning and 1 for choice offset
      default:
        return -1;
      }
    }

  public:
    DecryptedRound(char opponentChoice, char outcome) : StrategyGuideRound(opponentChoice) {
      //userValue = determineUserValue(opponentValue, outcome);
      score = determineScore(opponentValue, outcome);
    }
};

int calculateTotalScore(bool useEncryptedGuide) {
  string line;
  ifstream myfile("input.txt");

  int totalScore = 0;

  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      if (!line.empty()) {
        if (useEncryptedGuide) {
          EncryptedRound currentRound(line[0], line[2]);
          totalScore += currentRound.getScore();
        } else {
          DecryptedRound currentRound(line[0], line[2]);
          totalScore += currentRound.getScore();
        }
      }
    }
  }
  myfile.close();
  return totalScore;
}

int main() {
  int encryptedScore = calculateTotalScore(true);
  int decryptedScore = calculateTotalScore(false);

  cout << "Encrypted Strategy Guide Total Score: " << encryptedScore << endl;
  cout << "Decrypted Strategy Guide Total Score: " << decryptedScore;
}

/*
  PART 1
  -------------------------
  Strategy Guide Input: first column is opponent's play, second column is user's play
  opponent = { A = rock, B = paper, C = scissors }
  user = { X = rock, Y = paper, Z = scissors }

  Winner of tournament is player with highest score.
  Total Score = ∑(round's score)
  EncryptedRound Score = outcome + shape
    Enum outcomeScore = { lost = 0, draw = 3, won = 6 }
    Enum shapeScore = { rock = 1, paper = 2, scissors = 3 }

  Example:
    A   Y      - opponent (A = rock) and user (Y = paper), user wins with score of 8 (paper = 2 + won = 6, thus 8)
    B   X      - opponent (B = paper) and user (X = rock), user loses with score of 1 (rock = 1 + lose = 0, thus 1)
    C   Z      - opponent (C = scissors) and user (Z = scissors), draw with score of 6 (scissors = 3 + draw = 3, thus 6)

  Thus, total score of 15 (8 + 1 + 6)
*/
/*
  PART 2
  -------------------------
  Strategy Guide Input: first column is opponent's play, second column is outcome
  opponent = { A = rock, B = paper, C = scissors }
  outcome = { X = lose, Y = draw, Z = win}

  Need to determine user's shape. Formulated like this: outcome + opponentChoice = userChoice

  Example:
    A   Y     - opponent (A = rock) and outcome (Y = draw) thus user chooses rock with score of 4 (rock = 1 + draw = 3, thus 4)
    B   X     - opponent (B = paper) and outcome (X = lose) thus user chooses rock with score of 1 (rock = 1 + lose = 0, thus 1)
    C   Z     - opponent (C = scissors) and outcome (Z = win) thus user chooses rock with score of 7 (rock = 1 + win = 6, thus 7)

    Thus, total score of 12 (4 + 1 + 7)
*/

/*
  Multithreading
  ---------------
  What if I assigned two different workers: one to handle even lines and one to handle odd lines?
  What if I split the file in half and read it in two different threads? - I like this one
*/