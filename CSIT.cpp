// Author:             Anton Silivonchyk
// Source file:        CSIT.cpp
// Description:        Final Project: streaming service;
// IDE used:           Visual Studio 2019

#include <iostream>
#include <string>
using namespace std;

bool MenuSelection(int&);
string UserLogin();

int main()
{   
    int menuSelection = 0;
    do {
        switch (menuSelection) 
        {
			case 1:
                UserLogin();
				break;
            case 2: 
                //admin panel
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

string UserLogin()
{
    string userLogin = "";
    cout << "Enter user login:";
    cin >> userLogin;
    cout << endl;
    return userLogin;
}