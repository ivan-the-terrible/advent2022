#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Round {
  public:
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
        break;
      case 1: //win
        return 6;
      case 2: //lose
        return 0;
      default:
        return -1;
      }
    }
    Round(char opponentChoice, char userChoice) {
      opponentValue = determineOpponentValue(opponentChoice);
      userValue = determineUserValue(userChoice);
      score = determineOutcome(opponentValue, userValue) + (userValue + 1); //need to add 1 to offset the RPS values I needed to enumerate for use of modulo 3
    }
};

int part1() {
  string line;
  ifstream myfile("input.txt");

  int totalScore = 0;

  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      if (!line.empty()) {
        Round currentRound(line[0], line[2]);
        totalScore += currentRound.score;
      }
    }
  }
  myfile.close();
  return totalScore;
}

int main() {
  int totalScore = part1();

  cout << "Strategy Guide Total Score: " << totalScore;
}

/*
  Strategy Guide Input: first column is opponent's play, second column is user's play
  Enum opponent = { A = rock, B = paper, C = scissors }
  Enum user = { X = rock, Y = paper, Z = scissors }

  Winner of tournament is player with highest score.
  Total Score = ∑(round's score)
  Round Score = outcome + shape
    Enum outcomeScore = { lost = 0, draw = 3, won = 6 }
    Enum shapeScore = { rock = 1, paper = 2, scissors = 3 }

  Example:
    A   Y      - opponent (A = rock) and user (Y = paper), user wins with score of 8 (paper = 2 + won = 6, thus 8)
    B   X      - opponent (B = paper) and user (X = rock), user loses with score of 1 (rock = 1 + lose = 0, thus 1)
    C   Z      - opponent (C = scissors) and user (Z = scissors), draw with score of 6 (scissors = 3 + draw = 3, thus 6)

  Thus, total score of 15 (8 + 1 + 6)
*/

/*
  Multithreading
  ---------------
  What if I assigned two different workers: one to handle even lines and one to handle odd lines?
  What if I split the file in half and read it in two different threads? - I like this one
*/