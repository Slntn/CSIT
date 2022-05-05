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
bool AdminMainMenu(int&);
void UserPanel(int, bool);
void UserMenu(std::vector<Movie>);
void AdminMenu();
void AddNewMovie();
void AddNewUser();
void DeleteUser();
void DeleteMovie();
void DisplayMovieList(std::vector<Movie>);
void MovieRaiting(string);
void ShowUserList();
void SetString(string&, string, char);
int MovieMenu();
string UserLogin(bool);
string RequestValue(string);
std::vector<Movie> ShowMovieList(bool isAdmin = false);

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
    return user + "," + pass;
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
            AdminMenu();
        }
    }
    else 
    {
        cout << "The entered user credentials are not correct! Please check data and try again!" << endl;
    }
}

std::vector<Movie> ShowMovieList(bool isAdmin)
{
    DB dbReader;
    vector<string> result = dbReader.FetchMovieList();
    vector<Movie> movieListVector;

    cout << "Available movies: " << endl;
    const int size = (result.size() - 1);
    for (int i = 0; i < size; i++)
    {      
       Movie mv(result.at(i));
       mv.DisplayMovie(i, isAdmin);
       movieListVector.push_back(mv);
    }
    return movieListVector;
}

void DisplayMovieList(std::vector<Movie> movies)
{
    cout << "Available movies: " << endl;
    int index = 0;
    for (auto i : movies)
    {
        i.DisplayMovie(index);
        index++;
    }
}

void UserMenu(std::vector<Movie> dataList) 
{
    int menu = 0, movie = 0;
    cout << "\nSelect Movie:";
    cin >> movie;
    dataList.at(movie).ShowMovieInfo();
    do {    
        menu = MovieMenu();
        if (menu == 1) 
        {
            int selection;
            cout << "A few time later..." << endl;
            cout << "Do you want to rate a movie? 1 - Yes / Any number - No" << endl;
            cin >> selection;
            if (selection == 1) MovieRaiting(dataList.at(menu).GetMovieId());
            DisplayMovieList(dataList);
        } 
        if (menu == 2) {
            DisplayMovieList(dataList);
            UserMenu(dataList);
        }
        if (menu == 3) {
            string id = dataList.at(movie).GetMovieId();
            DB dbCtrl;
            double currentMovieRaring = dbCtrl.GetRating(id);
            if (currentMovieRaring == 0) 
            {
                cout << "The selected show has not been rated yet." << endl;
            }else 
            {
                cout << "Rating: " << dbCtrl.GetRating(id) << endl;
                cout << "Reviews: " << endl;
                dbCtrl.DisplayReviews(id);
            }
            
        }
        if (menu == 4){ menu = -1; }
    } while (menu != -1);

}

void MovieRaiting(string movieId) 
{
    int rating = 0;
    string comment;
    cout << "Enter your rating (0 - 10) : " << endl;
    cin >> rating;
    cout << "Enter your review : " << endl;
    std::getline(std::cin >> std::ws, comment);
    std::string s = std::to_string(rating);
    DB dbCtrl;
    dbCtrl.UpdateRating(movieId, s, comment);
}

void ShowUserList() 
{
    DB dbReader;
    std::vector<string> dbData = dbReader.GetAllUsers();
    int i = 0;
    cout << "List of the users: " << endl;
    for (auto user : dbData) 
    {
        if(user.length()) cout << i << " - " << user << endl;
        i++;
    }
    cout << endl;
}

int MovieMenu() {
    int menu = 0;
    cout << "\n\n1. Play Movie" << endl;
    cout << "2. Movie list" << endl;
    cout << "3. Reviews" << endl;
    cout << "4. Close Menu and LogOut" << endl;
    cin >> menu;
    return menu;
}

void AdminMenu() 
{
    int select = 0;
    do {
        if (select == 1) ShowUserList();
        if (select == 2) AddNewUser();
        if (select == 3) DeleteUser();
        if (select == 4) 
        {
            ShowMovieList(true);
            cout << endl;
        }
        if (select == 5) AddNewMovie();
        if (select == 6) DeleteMovie();
    }while (!AdminMainMenu(select));

}

bool AdminMainMenu(int& selection) 
{
    cout << "Admin Control:" << endl;
    cout << "1. The list of the registered users" << endl;
    cout << "2. Add a new user" << endl;
    cout << "3. Delete user" << endl;
    cout << "4. The list of available movies" << endl;
    cout << "5. Add a new movie" << endl;
    cout << "6. Delete a movie" << endl;
    cout << "0. Close Menu and LogOut" << endl;
    cin >> selection;
    return (selection == 0);
}

void AddNewMovie()
{
    DB dbControl;
    string newMovie = "";
    SetString(newMovie, "Enter title : ", ',');
    SetString(newMovie, "Enter show length : ", ',');
    SetString(newMovie, "Enter MPAA rating : ", ',');
    SetString(newMovie, "Enter genres : ", ',');
    SetString(newMovie, "Enter release year : ", ',');
    SetString(newMovie, "Enter expiration date or 'NA'(Example: 05 / 02 / 2024): ", ';');
    dbControl.AddNewMovie(newMovie);
    cout << "Add new movie process - completed!" << endl;
}

void SetString(string& localVar, string msg, char sign) 
{
    string value = "";
    cout << msg << endl;
    std::getline(std::cin >> std::ws, value);
    localVar = localVar + value + sign;
}

void AddNewUser()
{
    DB dbControl;
    string user = "";
    string cred = "";
    SetString(user, "Enter user id : ", ',');
    cred = user;
    SetString(user, "Enter user first name : ", ',');
    SetString(user, "Enter user last name : ", ',');
    SetString(user, "Enter current date (Example: 05 / 02 / 2024): ", ';');
    SetString(cred, "Enter user password : ", ';');
    dbControl.AddNewUser(user, cred);
    cout << "Add new user process - completed!" << endl;
}

void DeleteUser()
{
    ShowUserList();
    string userId = RequestValue("Enter user ID:");
    if (userId == "admin") 
    {
        cout << "The selected user cannot be deleted!" << endl;
        return;
    }
    DB dbControl;
    dbControl.DeleteUser(userId);
}

void DeleteMovie() 
{
    ShowMovieList(true);
    string id = RequestValue("Enter movie ID:");
    DB dbControl;
    dbControl.DeleteMovie(id);
}

string RequestValue(string msg) 
{
    string value = "";
    cout << msg;
    std::getline(std::cin >> std::ws, value);
    cout << endl;
    return value;
}