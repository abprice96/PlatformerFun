#include "highscore.h"

HighScore* HighScore::instance_ =  nullptr;

HighScore& HighScore::instance() {
	if (instance_== nullptr) {
		instance_ = new HighScore();
	}
	return *instance_;

}
//read in the high scores numbers and users from a file into two vectors
//or exits if there is no file with the specified name
void HighScore::LoadScore (string filename) {
	string input;
	int score;
	bool wasString;
	string name;
	fstream file;
	file.open(filename);

	if (file.is_open()) {
		while (file >> input) {
			try  {
				score = stoi(input);

			} catch (invalid_argument){
				wasString = true;
			}
			if (wasString == true) {
				name += " ";
				name += input;
				wasString = false;
			} else {
				score = stoi(input);
				scores.push_back(score);
				names.push_back(name);
				wasString = false;
				name = "";
			}
		}
		file.close();
		return;
	} else {
		fstream fs(filename,ios::out);
		for (int i = 9; i >= 0; i--) {
			fs << "none " << to_string(10 * i) << endl;
		}
		fs.close();
		HighScore::instance().LoadScore(filename);
		return;
	}


}


//saves the high score of the current user to the vectors using the
//specified <string> and <score>
int HighScore::NewHighScore(int score) {

	for (int i = 9; i >= 0; i--) {
		if (scores.at(i) > score) {
			scores.insert(scores.begin() + (i + 1), score);
			scores.pop_back();
			return (i + 1);
		}

	}
	if (scores.at(0) <= score ) {
		scores.insert(scores.begin(), score);
		scores.pop_back();
		return 0;
	}
	return 0;
}

void HighScore::NewHighScoreName(string name, int placeInVector){
    names.insert(names.begin() + placeInVector, name);
	names.pop_back();
}

//writes the scores and names in the vector to the file
void HighScore::SaveScores(string filename) {
    ofstream file;
    names.resize(10);
    scores.resize(10);
    filename = "data/" + filename;
	file.open (filename);
	if (file.is_open()) {
		for (int i = 0; i < 10; i++) {
			file << names.at(i)  <<" " << scores.at(i) << endl;
		}
	} else {
		cout << "The file couldn't be created!" << endl;
	}
	file.close();
}


 void HighScore::UnitTests() {

			//unit tests
            HighScore::instance().LoadScore("impossiblescores.txt");
            assert(HighScore::instance().getScore(0) == 90);
			assert(HighScore::instance().getName(0) == "none");

            int i = HighScore::instance().NewHighScore(100);
            assert(i == 0);


            assert(HighScore::instance().getScore(0) == 100);
            HighScore::instance().NewHighScoreName("Joe", i);
			assert(HighScore::instance().getName(0) == "Joe");
			
            assert(HighScore::instance().getScore(9) == 10);
			assert(HighScore::instance().getName(9) == "none");
			
			HighScore::instance().SaveScores("createnewfile.txt");


	}

void HighScore::teardown() {
	if (instance_ != NULL)
	{
		delete instance_;
		instance_ = NULL;
	}
}
