#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <exception>
#include <stdexcept>

using namespace std;

class HighScore {
private:
     vector<int> scores;
     vector <string> names;
     static HighScore* instance_;
     HighScore() {}
     HighScore(const HighScore& that) = delete; //prevent copying
	    
public: 
    static HighScore& instance();

	//loads the top 10 high scores from a file into the vector
	//or creates a new one
    void LoadScore(string filename);

    //inserts a new score into the corresponding vectors
    int NewHighScore(int score);

    //inserts a new name in tho the specified slot
    void NewHighScoreName(string name, int placeInVector);

	//saves the current scores in the vector to a file
	void SaveScores(string filename);

	//returns the score at the specified place in the vector
    int getScore(int i) { return scores.at(i); }

    //returns lowest High score
    int getLowestScore() { return scores.at(9); }
	
	//returns the name at the specified place in the vector
    string getName(int i) {return names.at(i); }
	
	//unit tests for the program
     static void UnitTests();
	
	//delete the singleton class
	static void teardown();
};

#endif // HIGHSCORE_H
