#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class DB {
	public:
        bool VerifyCredentials(string userData)
        {
            string filePath = getTablePath(1);
            string line;
            bool found = false;          
            ifstream file(filePath);
            if (!file.is_open()) {
                cout << "The file doesn't exist! Please check the path and try again." << endl;
            }

            while (std::getline(file, line) && !found)
            {
                if (line.find(userData) != string::npos) {found = true;}
            }
            return found;
        }
        string GetUserTablePath(string userName) { return "c:\\db\\users\\" + userName + ".txt"; }
        string GetTableData(int tableCode) 
        {
            string filePath = getTablePath(tableCode);
            string line, fileData;
            ifstream file(filePath);
            if (!file.is_open()) {
                cout << "The file doesn't exist! Please check the path and try again." << endl;
            }
            while (getline(file, line)) { fileData = line; }
            file.close();
            return fileData;
        }


    private:
        string getTablePath(int tableCode) 
        {
            if (tableCode == 1) return "c:\\db\\pass.txt";
            if (tableCode == 2) return "c:\\db\\users.txt";
            if (tableCode == 3) return "c:\\db\\movies.txt";
            if (tableCode == 4) return "c:\\db\\counter.txt";
            if (tableCode == 5) return "c:\\db\\rating.txt";
            if (tableCode == 6) return "c:\\db\\comments.txt";
            return "NA";
        }
};