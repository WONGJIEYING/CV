#ifndef NEWUSER_H
#define NEWUSER_H

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include "sqlite3.h"
#include "crud.h"
#include "PatientMenu.h"
#include "AdminMenu.h"
//#include "doctor.h"

using namespace std;
using std::setw;

// Forward declarations



void login(string &username, string &password, user *userHead, string &role, string &userID, patientAppointment *patientAppointmentHead, string &doctorID, cancelAppointment *cancelAppointmentHead, doctorSchedule *doctorScheduleHead, patientAllergy *patientAllergyHead, sqlite3* DB);
void doctorMenu(user *userHead, string loggedInUser, patientAppointment *patientAppointmentHead, string doctorID, cancelAppointment *cancelAppointmentHead, doctorSchedule *doctorScheduleHead, patientAllergy *patientAllergyHead);


user *createUserNode(string userID, string name, string ICNo, string gender, string position, string contactNumber, string email, string address, string username, string password) {
  user *newNode = new user();
  newNode->userID = userID;
  newNode->name = name;
  newNode->ICNo = ICNo;
  newNode->gender = gender;
  newNode->position = position;
  newNode->contactNumber = contactNumber;
  newNode->email = email;
  newNode->address = address;
  newNode->username = username;
  newNode->password = password;
  newNode->firstLogin = false;
  newNode->next = NULL;
  return newNode;
}

user *loginNode(string username, string password) { // change name
  user *newNode = new user();
  newNode->username = username;
  newNode->password = password;
  newNode->firstLogin = true;
  newNode->next = NULL;
  return newNode;
}

string toLower(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// print current loged in user
bool searchUserProfile(user *userHead, const string &userID) {
  viewUser(userID);
  return true;
}

bool isValidICNo(const string &icNo) {
  return icNo.length() == 12 && all_of(icNo.begin(), icNo.end(), ::isdigit);
}

bool isValidGender(const string &gender) {
  return gender == "male" || gender == "female" || gender == "Male" || gender == "Female";
}

bool isValidPosition(const string &position) {
  return position == "patient" || position == "doctor" || position == "admin" || position == "Patient" || position == "Doctor" || position == "Admin";
}

bool isValidContactNumber(const string &contactNumber) {
  return (contactNumber.length() >= 11 && contactNumber.length() <= 12) && all_of(contactNumber.begin(), contactNumber.end(), ::isdigit);
}

bool validateEmail(const string &email) {
  return email.find('@') != string::npos;
}

string generateUserID(const string &position) {
    sqlite3_stmt *stmt;
    string sql;
    int exit;
    int maxID = 0;
    
    // SQL query based on position
    if (position == "Patient" || position == "patient") {
        sql = "SELECT MAX(CAST(SUBSTR(userID, 2) AS INTEGER)) FROM User WHERE UserID LIKE 'P%'";
    } else if (position == "Doctor" || position == "doctor" || position == "Admin" || position == "admin") {
        sql = "SELECT MAX(CAST(SUBSTR(userID, 2) AS INTEGER)) FROM User WHERE UserID LIKE 'E%'";
    } else {
        cerr << "Invalid position" << endl;
        return "";
    }

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    } else if (exit == SQLITE_DONE) {
        maxID = 0; // No IDs found, start from 0
    }

    sqlite3_finalize(stmt);

    maxID++;
    if (position == "Patient" || position == "patient") {
        return "P" + to_string(maxID);
    } else {
        return "E" + to_string(maxID);
    }
}

// add new user and complete profile
void addUser(user *&userHead, const string &username, const string &password) {
  string name, ICNo, gender, position, contactNumber, email, address;

  cout << "\nComplete User Profile" << endl;
  cout << "-----------------------" << endl;

  do {
    cout << "Enter Name: ";
    getline(cin, name);
    if (name.empty()) {
      cout << "Name cannot be empty. Please try again." << endl;
    }
  } while (name.empty());

  do {
    cout << "Enter IC No (12 digits): ";
    getline(cin, ICNo);
    if (!isValidICNo(ICNo)) {
      cout << "IC No must be exactly 12 digits. Please try again." << endl;
    }
  } while (!isValidICNo(ICNo));

  do {
    cout << "Enter Gender (Male/Female): ";
    getline(cin, gender);
    if (!isValidGender(gender)) {
      cout << "Gender must be either 'male' or 'female'. Please try again."
           << endl;
    }
  } while (!isValidGender(gender));

  do {
    cout << "Enter Position (Patient/Doctor/Admin): ";
    getline(cin, position);
    if (!isValidPosition(position)) {
      cout << "Position must be either 'patient', 'doctor', or 'admin'. Please try again."
           << endl;
    }
  } while (!isValidPosition(position));

  do {
    cout << "Enter Contact Number (11-12 digits): ";
    getline(cin, contactNumber);
    if (!isValidContactNumber(contactNumber)) {
      cout << "Contact number must be between 11 to 12 digits. Please try again."
           << endl;
    }
  } while (!isValidContactNumber(contactNumber));

  do {
    cout << "Enter Email: ";
    getline(cin, email);
    if (!validateEmail(email)) {
      cout << "Incorrect email format. Please try again." << endl;
    }
  } while (!validateEmail(email));

  do {
    cout << "Enter Address: ";
    getline(cin, address);
    if (address.empty()) {
      cout << "Address cannot be empty. Please try again." << endl;
    }
  } while (address.empty());

  string userID = generateUserID(position);

  // Create a new user node
  user *newUser = createUserNode(userID, name, ICNo, gender, position, contactNumber, email, address, username, password);

  // Add the new user to the linked list
  if (userHead == NULL) {
    userHead = newUser;
  } else {
    user *temp = userHead;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newUser;
  }
  
  createUser(userID, name, ICNo, gender, position, contactNumber, email, address, username, password);

  // Redirect to the appropriate menu based on position
  if (newUser->position == "Patient" || newUser->position == "patient") {
    patientMenu(userID); // Assuming null pointers for simplicity
  } else if (newUser->position == "Doctor" || newUser->position == "doctor") {
    doctorMenu(userHead, newUser->userID, nullptr, newUser->userID, nullptr, nullptr, nullptr); // Assuming null pointers for simplicity
  } else if (newUser->position == "Admin" || newUser->position == "admin") {
    adminMenu(userID);
  } else {
    cout << "Invalid position" << endl;
  }
}

// Function to check if a username already exists
bool userExists(string username, string password, user *userHead) {
  user *currentUser = userHead;
  while (currentUser != NULL) {
    if (currentUser->username == username &&
        currentUser->password == password) {
      return true;
    }
    currentUser = currentUser->next;
  }
  return false;
}

user* findUser(const string &username, const string &password, user *userHead) {
    user *current = userHead;
    while (current != nullptr) {
        if (current->username == username && current->password == password) {
            return current;
        }
        current = current->next;
    }
    cout << "User not found." << endl;
    return nullptr;
}

bool usernameExists(string username, user *userHead) {
  user *currentUser = userHead;
  while (currentUser != nullptr) {
    if (currentUser->username == username) {
      return true;
    }
    currentUser = currentUser->next;
  }
  return false;
}

void registerAccount(user *&userHead) {
  string username, password;

  cout << "\nRegister Account" << endl;
  cout << "-----------------------" << endl;

  while (true) {
    cout << "Enter username: ";
    getline(cin, username);

    if (username.empty()) {
      cout << "Username cannot be empty. Please enter again." << endl;
    } else if (usernameExists(username, userHead)) {
      cout << "Username already exists. Please try another username." << endl;
    } else {
      break;
    }
  }

  while (true) {
    cout << "Enter password: ";
    getline(cin, password);

    if (password.empty()) {
      cout << "Password cannot be empty. Please enter again." << endl;
    } else {
      break;
    }
  }

  user *newUser = loginNode(username, password);

  if (userHead == NULL) {
    userHead = newUser;
  } else {
    user *current = userHead;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newUser;
  }
  cout << "Account registered successfully!\n" << endl;

  // Call addNewUser with registered username and password
  addUser(userHead, username, password);
}

//successful
void forgotPassword(sqlite3* DB, user *userHead, patientAppointment *patientAppointmentHead, string &doctorID, cancelAppointment *cancelAppointmentHead, doctorSchedule *doctorScheduleHead, patientAllergy *patientAllergyHead) {
    string username, newPassword, confirmPassword, password, role, userID;

     while (true) {
        cout << "Enter username to reset password: ";
        getline(cin, username);

        if (username.empty()) {
            cout << "Username cannot be empty. Please enter again." << endl;
        } else {
            break;
        }
    }

    string selectSql = "SELECT * FROM User WHERE Username = ?";
    sqlite3_stmt* stmt;
    int DBExit = sqlite3_prepare_v2(DB, selectSql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare select statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the username
    DBExit = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind username: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Execute the SQL statement
    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        while (true) {
            cout << "Enter new password: ";
            getline(cin, newPassword);
            cout << "Confirm new password: ";
            getline(cin, confirmPassword);

            if (newPassword.empty() || confirmPassword.empty()) {
                cout << "Passwords cannot be empty. Please try again." << endl;
            } else if (newPassword == confirmPassword) {
                updatePassword(DB, username, newPassword);
                break;
            } else {
                cout << "Passwords do not match. Please try again." << endl;
            }
        }
        login(username, newPassword, userHead, role, userID, patientAppointmentHead, doctorID, cancelAppointmentHead, doctorScheduleHead, patientAllergyHead, DB);
    } else {
        cout << "User not found. Please try again." << endl;
        char registerUser;
        cout << "Do you want to register a new account? (Y/N): ";
        cin >> registerUser;
        cin.ignore();

        if (registerUser == 'Y' || registerUser == 'y') {
          registerAccount(userHead);
        } else {
            cout << "Returning to login menu." << endl;
            login(username, newPassword, userHead, role, userID, patientAppointmentHead, doctorID, cancelAppointmentHead, doctorScheduleHead, patientAllergyHead, DB);
        }
    }
    sqlite3_finalize(stmt);
}

void login(string &username, string &password, user *userHead, string &role, string &userID, patientAppointment *patientAppointmentHead, string &doctorID, cancelAppointment *cancelAppointmentHead, doctorSchedule *doctorScheduleHead, patientAllergy *patientAllergyHead, sqlite3* DB) {
  loadUsersFromDatabase(DB, userHead);
  
  int maxAttempts = 3;
  char existingUser;

  while (true) {
    cout << "Do you have an account? (Y/N): ";
    cin >> existingUser;
    cin.ignore();

    if (existingUser == 'Y' || existingUser == 'y') {
      bool userFound = false;
      int attempts = 0;

      while (attempts < maxAttempts) {
        cout << "\n\nLogin Account" << endl;
        cout << "-----------------------" << endl;

        do {
          cout << "Enter your Username: ";
          getline(cin, username);
          if (username.empty()) {
            cout << "Username cannot be empty. Please enter again." << endl;
          }
        } while (username.empty());

        do {
          cout << "Enter your password: ";
          getline(cin, password);
          if (password.empty()) {
            cout << "Password cannot be empty. Please enter again." << endl;
          }
        } while (password.empty());

        user *currentUser = findUser(username, password, userHead);

        if (currentUser) {
          role = currentUser->position;
          userID = currentUser->userID;

        cout << "Login Successful" << endl << "Welcome " << currentUser->username << endl;

        if (currentUser->firstLogin) {
          addUser(userHead, username, password);
          delete currentUser; // Free allocated memory
          return;
        }

        if (role == "Patient" || role == "patient") {
            patientMenu(userID);
            } else if (role == "Doctor" || role == "doctor") {
              doctorID = currentUser->userID;
            doctorMenu(userHead, currentUser->userID, patientAppointmentHead, doctorID, cancelAppointmentHead, doctorScheduleHead, patientAllergyHead);
            } else if (role == "Admin" || role == "admin") {
              adminMenu(userID);
        }
            delete currentUser; // Free allocated memory
            return;
        } else {
          cout << "Username or password incorrect. Please try again.";
          attempts++;
        }
      }

      char registerUser;
      do {
        cout << "\nAccount not found? Register Account? (Y/N): ";
        cin >> registerUser;
        cin.ignore();

        if (registerUser == 'Y' || registerUser == 'y') {
          registerAccount(userHead);
          return;
        } else if (registerUser == 'N' || registerUser == 'n') {
          char forgotPass;

          do {
            cout << "Forgot password? (Y/N): ";
            cin >> forgotPass;
            cin.ignore();
            if (forgotPass == 'Y' || forgotPass == 'y') {
              forgotPassword(DB, userHead, patientAppointmentHead, doctorID, cancelAppointmentHead, doctorScheduleHead, patientAllergyHead);
              return;
            } else if (forgotPass == 'N' || forgotPass == 'n') {
              cout << "Exiting program..." << endl;
              exit(0);
            } else {
              cout << "Invalid input. Please enter Y or N." << endl;
            }
          } while (forgotPass != 'Y' && forgotPass != 'y' && forgotPass != 'N' && forgotPass != 'n');
        } else {
          cout << "Invalid input. Please enter Y or N." << endl;
        }
      }

      while (registerUser != 'Y' && registerUser != 'y' && registerUser != 'N' && registerUser != 'n');
    } else if (existingUser == 'N' || existingUser == 'n') {
      registerAccount(userHead);
      return;
    } else {
      cout << "Invalid input. Please try again." << endl;
    }
  }
}

#endif 