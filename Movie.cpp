#include<string>
#include<iostream>
#include<vector>
#include<sstream>
using namespace std;

class Movie {
	public:
		string id;
		string title;
		string time;
		string mpaa;
		string genre;
		string exparation;
		int year;

		Movie(string data) 
		{
			vector<string> result;
			stringstream strStream(data);
			while (strStream.good()) {
				string str;
				getline(strStream, str, ',');
				result.push_back(str);
			}

			id = result[0];
			title = result[1];
			time = result[2];
			mpaa = result[3];
			genre = result[4];
			year = std::stoi(result[5]);
			exparation = result[6];
		}

		void DisplayMovie(int i) 
		{
			cout << i << "\t" << title << " - " << genre << endl;
		}

		void ShowMovieInfo() 
		{
			cout << "Movie Information" << endl;
			cout << "Title: " << title << endl;
			cout << "Info: " << time << "\t" << mpaa << endl;
			cout << "Genere: " << genre << endl;
			cout << "Released year: " << year << endl;
			cout << "Available till: " << exparation << endl;
		}
};