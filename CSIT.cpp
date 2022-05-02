// Authors:            Anton Silivonchyk, 
// Source file:        CSIT.cpp
// Description:        Final Project: streaming service;
// IDE used:           Visual Studio 2019

#include <iostream>
#include <string>
#include "DB.cpp"
using namespace std;

bool MenuSelection(int&);
string UserLogin(bool);
void UserPanel(int, bool);

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
                //batch processing
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
        if (codePanel == 1) cout << "show user panel" << endl;
        if (codePanel == 2 && login.length() != 3) cout << "show admin panel" << endl;
    }
    else 
    {
        cout << "The entered user credentials are not correct! Please check data and try again!" << endl;
    }
}