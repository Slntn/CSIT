// Authors:            Anton Silivonchyk, 
// Source file:        CSIT.cpp
// Description:        Final Project: streaming service;
// IDE used:           Visual Studio 2019

#include <iostream>
#include <string>
#include "DB.cpp"
#include "Movie.cpp"
using namespace std;

bool MenuSelection(int&);
void UserPanel(int, bool);
void UserMenu(std::vector<Movie>);
void AdminMenu(std::vector<Movie>);
void DisplayMovieList(std::vector<Movie>);
string UserLogin(bool);
std::vector<Movie> ShowMovieList();

int main()
{   
    int menuSelection = 0;
    do {
        switch (menuSelection) 
        {
			case 1:
                UserPanel(1, false);
				break;
            case 2:
                UserPanel(2, true);
				break;
            case 3:
                cout << "Batch processing!" << endl;
                break;
        }
    } while (!MenuSelection(menuSelection));

    return 0;
}

bool MenuSelection(int& menuSelection)
{
    cout << "Main Menu" << endl;
    cout << "1. User login" << endl;
    cout << "2. Admin login" << endl;
    cout << "3. Job Batch" << endl;
    cout << "0. Close app" << endl;
    cin >> menuSelection;
    return (menuSelection == 0);
}

string UserLogin(bool adminLogin)
{
    string user = "", pass = "";
    cout << "Please, enter user name:";
    cin >> user;
    cout << endl;
    if (adminLogin && user != "admin") return "N?A";
    cout << "Please, enter password:";
    cin >> pass;
    cout << endl;
    return user + ":" + pass;
}

void UserPanel(int codePanel, bool isAdminLogin)
{   
    DB dbReader;
    string login = UserLogin(isAdminLogin);
    bool loginResult = dbReader.VerifyCredentials(login);
    if (loginResult) 
    {
        if (codePanel == 1) 
        {
            UserMenu(ShowMovieList());
        }
        if (codePanel == 2 && login.length() != 3) 
        {
            AdminMenu(ShowMovieList());
        }
    }
    else 
    {
        cout << "The entered user credentials are not correct! Please check data and try again!" << endl;
    }
}

std::vector<Movie> ShowMovieList()
{
    DB dbReader;
    vector<string> result;
    vector<Movie> movieListVector;
    const string movieList = dbReader.GetTableData(3);   
    stringstream strStream(movieList);
    while (strStream.good()) {
        string str;
        getline(strStream, str, ';');
        result.push_back(str);
    }

    cout << "Available movies: " << endl;
    const int size = (result.size() - 1);
    for (int i = 0; i < size; i++)
    {      
       Movie mv(result.at(i));
       mv.DisplayMovie(i);
       movieListVector.push_back(mv);
    }
    return movieListVector;
}

void DisplayMovieList(std::vector<Movie> movies)
{
    int index = 0;
    for (auto i : movies)
    {
        i.DisplayMovie(index);
        index++;
    }
}

void UserMenu(std::vector<Movie> dataList) 
{
    int menu = 0;
    do {
        cout << "\nSelect Movie:";
        cin >> menu;
        dataList.at(menu).ShowMovieInfo();
        cout << "\n\n1. Play Movie" << endl;
        cout << "2. Show movie list" << endl;
        cout << "3. Close Menu and Exit" << endl;
        cin >> menu;
        if (menu == 1) 
        {
            int selection;
            cout << "A few time later..." << endl;
            cout << "Do you want to rate a movie? 1 - Yes / Any number - No" << endl;
            cin >> selection;
            if (selection == 1) {// add rating!  
            }
            DisplayMovieList(dataList);
        } 
        if (menu == 2) DisplayMovieList(dataList);
        if (menu == 3) menu = -1;
    } while (menu != -1);

}
void AdminMenu(std::vector<Movie> dataList) 
{
    
}