#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

void LoginProcess() {
  int Select;
  cout << "\nSelect Process" << endl;
  cout << "--------------------------" << endl;
  cout << "1. Log In" << endl;
  cout << "2. Register" << endl;
  cout << "3. Forget Password" << endl;
  cout << "Enter your choice: ";
  cin >> Select;

  switch (Select) {
  case 1: {
    string LoginUsername, LoginPassword;
    cout << "Insert Your Username: ";
    cin >> LoginUsername;
    if (LoginUsername == "u" || LoginUsername == "U") {
      cout << "Insert Your Password: ";
      cin >> LoginPassword;
      if (LoginPassword == "p" || LoginPassword == "P") {
        cout << "Logged in successfully" << endl;
      } else {
        cout << "Incorrect password" << endl;
        LoginProcess();
      }
    } else {
      cout << "Incorrect username" << endl;
      LoginProcess();
    }
    break;
  }
  case 2: {
    string RegisterUsername, RegisterPassword;
    cout << "Insert Your Username: ";
    cin >> RegisterUsername;
    cout << "Insert Your Password: ";
    cin >> RegisterPassword;
    cout << "Registered successfully" << endl;
    LoginProcess();
    break;
  }
  case 3: {
    string ForgetUsername, ForgetPassword;
    cout << "Insert Your Username: ";
    cin >> ForgetUsername;
    cout << "Insert Your New Password: ";
    cin >> ForgetPassword;
    cout << "Password reset successfully" << endl;
    LoginProcess();
    break;
  }
  default:
    cout << "Invalid choice, please try again." << endl;
    LoginProcess();
    break;
  }
}



