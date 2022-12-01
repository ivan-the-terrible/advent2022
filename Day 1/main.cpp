#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int part1()
{
string line;
  ifstream myfile("input.txt");
  if (myfile.is_open())
  {
    int highestCalorie, totalCalories;
    highestCalorie = totalCalories = 0;
    while (getline(myfile, line))
    {
      if (line.empty())
      {
        if (totalCalories > highestCalorie) highestCalorie = totalCalories;
        totalCalories = 0;
      }
      else
      {
        totalCalories += stoi(line);
      }
    }
    myfile.close();
    return highestCalorie;
  }
}
int part2()
{
  //Get top 3 totals and sum them.
  string line;
  ifstream myfile("input.txt");
  if (myfile.is_open())
  {
    int first, second, third, totalCalories;
    first = second = third = totalCalories = 0;
    while (getline(myfile, line))
    {
      if (line.empty())
      {
        if (totalCalories > first)
        {
          third = second;
          second = first;
          first = totalCalories;
          }
        else if (totalCalories > second) {
          third = second;
          second = totalCalories;
        }
        else if (totalCalories > third) {
          third = totalCalories;
        }

        totalCalories = 0;
      }
      else
      {
        totalCalories += stoi(line);
      }
    }
    myfile.close();
    int sum = first + second + third;
    return sum;
  }
}

int main()
{
  int highestCalorie = part1();
  int top3CaloriesSum = part2();

  cout << "Highest Calorie for Part 1: " << highestCalorie;
  cout << "\nTop 3 calories sum for Part 2: " << top3CaloriesSum;
}
