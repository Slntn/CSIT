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
        string GetRatingTablePath(string movieId) { return "c:\\db\\rating\\" + movieId + ".txt"; }
        string GetCommentsTablePath(string movieId) { return "c:\\db\\comments\\" + movieId + ".txt"; }
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
        string GetTableData(string path)
        {
            string line, fileData;
            ifstream file(path);
            if (!file.is_open()) {
                cout << "The file doesn't exist! Please check the path and try again." << endl;
            }
            while (getline(file, line)) { fileData = line; }
            file.close();
            return fileData;
        }
        void UpdateRating(string movieId, string rating, string comment)
        {
            string RatingPath = GetRatingTablePath(movieId);
            string CommentPath = GetCommentsTablePath(movieId);
            string RatingData = GetTableData(RatingPath);
            string CommentData = GetTableData(CommentPath);
            RatingData = RatingData + ";" + rating;
            CommentData = CommentData +";" + comment;
            overWriteFile(RatingData, RatingPath);
            overWriteFile(CommentData, CommentPath);
        }

        double GetRating(string movieId)
        {
            string path = GetRatingTablePath(movieId);
            string data = GetTableData(path);
            const double RESULT = ParseAndCalculateRating(data);
            return RESULT;
        }
        void DisplayReviews(string movieId)
        {
            string path = GetCommentsTablePath(movieId);
            string data = GetTableData(path);
            std::vector<string> dbData = Parser(data);
            for (auto line : dbData) cout << line << endl;
        }
        std::vector<string> GetAllUsers()
        {
            string dbData = GetTableData(2);
            return Parser(dbData);
        }

    private:
        string getTablePath(int tableCode) 
        {
            if (tableCode == 1) return "c:\\db\\pass.txt";
            if (tableCode == 2) return "c:\\db\\users.txt";
            if (tableCode == 3) return "c:\\db\\movies.txt";
            if (tableCode == 4) return "c:\\db\\counter.txt";
            return "NA";
        }
        void overWriteFile(string data, string path) 
        {
            std::ofstream ofs(path, std::ofstream::trunc);
            ofs << data;
            ofs.close();
        }
        double ParseAndCalculateRating(string data)
        {
            vector<string> parserData = Parser(data);
            int index = 0, total = 0;
            for (auto i : parserData) 
            {   
                index++;
                if(i.length() != 0) total += std::stoi(i);
            }
            if (index == 0) return 0;
            double result = (double)total / (double)index;
            return result;
        }
        std::vector<string> Parser(string data) 
        {
            vector<string> result;
            stringstream s_stream(data);
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, ';');
                result.push_back(substr);
            }
            return result;
        }
};