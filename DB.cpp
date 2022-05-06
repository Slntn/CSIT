#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

class DB {
	public:
        bool VerifyCredentials(string userData)
        {
            string filePath = GetTablePath(PASSWORD_TABLE);
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
        string GetTableData(int tableCode) 
        {
            string filePath = GetTablePath(tableCode);
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
        double GetRating(string movieId)
        {
            string path = GetRatingTablePath(movieId);
            string data = GetTableData(path);
            const double RESULT = ParseAndCalculateRating(data);
            return RESULT;
        }
        void UpdateRating(string movieId, string rating, string comment)
        {
            string RatingPath = GetRatingTablePath(movieId);
            string CommentPath = GetCommentsTablePath(movieId);
            string RatingData = GetTableData(RatingPath);
            string CommentData = GetTableData(CommentPath);
            RatingData = RatingData + ";" + rating;
            CommentData = CommentData +";" + comment;
            OverWriteFile(RatingData, RatingPath);
            OverWriteFile(CommentData, CommentPath);
        }   
        void DisplayReviews(string movieId)
        {
            string path = GetCommentsTablePath(movieId);
            string data = GetTableData(path);
            std::vector<string> dbData = Parser(data);
            for (auto line : dbData) cout << line << endl;
        }
        void AddNewMovie(string newMovie) 
        {
            string id = "M0VIE#" + Counter();
            string newRecord = id + ',' + newMovie;
            string tableData = GetTableData(MOVIE_TABLE);
            string newTableData = tableData + newRecord;
            OverWriteFile(newTableData, GetTablePath(MOVIE_TABLE));
            //create new tables
            OverWriteFile("", GetRatingTablePath(id));
            OverWriteFile("", GetCommentsTablePath(id));
        }
        void AddNewUser(string user, string cred)
        {
            string passTableData = GetTableData(PASSWORD_TABLE);
            string usersTableData = GetTableData(USERS_TABLE);
            string newCredData = passTableData + cred;
            string newUsersData = usersTableData + user;
            OverWriteFile(newCredData, GetTablePath(PASSWORD_TABLE));
            OverWriteFile(newUsersData, GetTablePath(USERS_TABLE));
        }    
        void DeleteUser(string userId) 
        {
            string users = GetTableData(USERS_TABLE);
            string pass = GetTableData(PASSWORD_TABLE);
            vector<string> data = Parser(users);
            vector<string> dataPass = Parser(pass);
            string newUserData = "";
            string newCredData = "";
            for (std::string user : data) 
                if (user.rfind(userId, 0) != 0 && user.length() > 1) newUserData += (user + ";");
                
            for (std::string cred : dataPass) 
                if (cred.rfind(userId, 0) != 0 && cred.length() > 1) newCredData += (cred + ";");

            OverWriteFile(newUserData, GetTablePath(USERS_TABLE));
            OverWriteFile(newCredData, GetTablePath(PASSWORD_TABLE));
        }
        void DeleteMovie(string movieId)
        {
            string dbData = GetTableData(MOVIE_TABLE);
            vector<string> data = Parser(dbData);
            string newData = "";
            for (std::string movie : data) {
                if (movie.rfind(movieId, 0) != 0 && movie.length() > 1) newData += (movie + ";");
            }      
            OverWriteFile(newData, GetTablePath(MOVIE_TABLE));
            string pathR = GetRatingTablePath(movieId);
            string pathC = GetCommentsTablePath(movieId);
            std::remove(pathR.c_str());
            std::remove(pathC.c_str());
        }
        std::vector<string> FetchMovieList()
        {
            const string movieList = GetTableData(MOVIE_TABLE);
            vector<string> result = Parser(movieList);
            return result;
        }
        std::vector<string> GetAllUsers()
        {
            string dbData = GetTableData(USERS_TABLE);
            return Parser(dbData);
        }
        void BatchProcess(int i)
        {
            string logs = "";
            string data = GetTableData(INBOUND_JOB_PATH);
            if (data.length() > 3) 
            {
                SetLogs(logs, std::to_string(i), "Data received!\n");
                std::vector<string> parsedData = Parser(data);
                SetLogs(logs, std::to_string(i), "Data parsed!\n");
                for (auto i : parsedData)
                {
                    if (i.length() != 0) AddNewMovie((i+";"));
                }

                string jobFilePath = GetTablePath(INBOUND_JOB_PATH);
                std::remove(jobFilePath.c_str());
            }
            else 
            {
                SetLogs(logs, std::to_string(i), "No data received!\n");
            }

            SetLogs(logs, std::to_string(i), "Job process completed!\n");
            string tableLogsData = GetTableData(BATCH_LOGS_TABLE);
            tableLogsData += logs;
            OverWriteFile(tableLogsData, GetTablePath(BATCH_LOGS_TABLE));
        }
    private:
        const int PASSWORD_TABLE = 1;
        const int USERS_TABLE = 2;
        const int MOVIE_TABLE = 3;
        const int COUNTER_TABLE = 4;
        const int BATCH_LOGS_TABLE = 5;
        const int INBOUND_JOB_PATH = 6;
        string GetTablePath(int tableCode) 
        {
            if (tableCode == 1) return "c:\\db\\pass.txt";
            if (tableCode == 2) return "c:\\db\\users.txt";
            if (tableCode == 3) return "c:\\db\\movies.txt";
            if (tableCode == 4) return "c:\\db\\counter.txt";
            if (tableCode == 5) return "c:\\db\\batch\\logs.txt";
            if (tableCode == 6) return "c:\\inbound\\job.txt";
            return "NA";
        }
        string GetRatingTablePath(string movieId) { return "c:\\db\\rating\\" + movieId + ".txt"; }
        string GetCommentsTablePath(string movieId) { return "c:\\db\\comments\\" + movieId + ".txt"; }
        void OverWriteFile(string data, string path) 
        {
            std::ofstream ofs(path, std::ofstream::trunc);
            ofs << data;
            ofs.close();
        }
        void SetLogs(string& logs, string job, string msg)
        {
            string log = ("JOB#" + job + " - " + std::to_string(TimeSinceEpochMillisec()) + msg);
            logs += log;
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
        string Counter() 
        {
            string value = GetTableData(COUNTER_TABLE);
            string newValue = std::to_string((std::stoi(value) + 1));
            OverWriteFile(newValue, GetTablePath(COUNTER_TABLE));
            return value;
        }
        uint64_t TimeSinceEpochMillisec() 
        {
            using namespace std::chrono;
            return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        }
};