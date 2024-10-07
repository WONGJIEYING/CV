#ifndef _01USER_H // checks if this file has not been defined.
#define _01USER_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <regex> // For regex validatio
#include "PatientMenu.h"
#include "newUser.h"
#include "globalCRUD.h"
#include "sqlite3.h"

using namespace std;

// structure
struct User {
    string UserID;
    string Name;
    string ICNo;
    string Gender;
    string Position;
    string ContactNumber;
    string Email;
    string Address;
    string Username;
    string Password;
    User* next;
};

User* Userhead = NULL;

string getNextUserID(const string &Position);

string formatPatientID(const string &input) {
  if (input.empty()) {
        return input;
    }
    string formattedID = input;
    if (formattedID[0] != 'P') {
        formattedID = 'P' + formattedID;
    }
    for (char &ch : formattedID) {
        ch = toupper(ch);
    }
    return formattedID;
}

//link list print table
void printUser(User* n) {
    // Print table header
    cout << setw(5) << left << "User ID" << " | "
         << setw(5) << left << "Name" << " | "
         << setw(5) << left << "IC No" << " | "
         << setw(5) << left << "Gender" << " | "
         << setw(5) << left << "Position" << " | "
         << setw(5) << left << "Contact No" << " | "
         << setw(5) << left << "Email" << " | "
         << setw(5) << left << "Address" << " | "
         << setw(5) << left << "Username" << " | "
         << setw(5) << left << "Password" << endl;
    //print data
    while (n != NULL) {
        cout << setw(5) << left << n->UserID << " | "
             << setw(5) << left << n->Name << " | "
             << setw(5) << left << n->ICNo << " | "
             << setw(5) << left << n->Gender << " | "
             << setw(5) << left << n->Position << " | "
             << setw(5) << left << n->ContactNumber << " | "
             << setw(5) << left << n->Email << " | "
             << setw(5) << left << n->Address << " | "
             << setw(5) << left << n->Username << " | "
             << setw(5) << left << n->Password << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackUser(
    string userID,
    string name,
    string iCNo,
    string gender,
    string position,
    string contactNumber,
    string email,
    string address,
    string username,
    string password) {
    User* newUser = new User();
    newUser->UserID = userID;
    newUser->Name = name;
    newUser->ICNo = iCNo;
    newUser->Gender = gender;
    newUser->Position = position;
    newUser->ContactNumber = contactNumber;
    newUser->Email = email;
    newUser->Address = address;
    newUser->Username = username;
    newUser->Password = password;
    newUser->next = NULL;

    if (Userhead == NULL) {
        Userhead = newUser;
    } else {
        User* temp = Userhead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newUser;
    }
}
//=====================================================================================
// Function to validate IC No (must be 12 digits)
bool validateICNo(const string& ICNo) {
    return ICNo.length() == 12 && all_of(ICNo.begin(), ICNo.end(), ::isdigit);
}

// Function to validate Gender (must be "female" or "male")
bool validateGender(const string& Gender) {
    return Gender == "female" || Gender == "Female" || Gender == "male" || Gender == "Male";
}

// Function to validate Position (must be "patient", "doctor", or "admin")
bool validatePosition(const string& Position) {
    return Position == "patient" || Position == "Patient" || Position == "doctor" || Position == "Doctor" || Position == "admin" || Position == "Admin";
}

// Function to validate Contact Number (must be at least 10 digits)
bool validateContactNumber(const string& ContactNumber) {
    return ContactNumber.length() >= 10 && all_of(ContactNumber.begin(), ContactNumber.end(), ::isdigit);
}

// Function to validate Email (must include '@')
bool validateAdminEmail(const string& Email) {
    return Email.find('@') != string::npos;
}

// Function to prompt user input until it is valid
void promptUserInput(string& input, const string& prompt, bool(*validateFunc)(const string&)) {
    while (true) {
        cout << prompt;
        cin >> input;
        if (validateFunc(input)) {
            break;
        } else {
            cout << "Input is not valid. Please try again." << endl;
        }
    }
}

// linklist addNewUser function
void addNewUser() {
    string Name, ICNo, Gender, Position, ContactNumber, Email, Address, Username, Password;
    
    cout << "Enter Your Name: ";
    cin >> Name;
    
    promptUserInput(ICNo, "Enter IC Number (12 digits): ", validateICNo);
    promptUserInput(Gender, "Enter Gender (Female/Male): ", validateGender);
    promptUserInput(Position, "Enter Position (Patient/Doctor/Admin): ", validatePosition);
    promptUserInput(ContactNumber, "Enter User Contact Number (at least 10 digits): ", validateContactNumber);
    promptUserInput(Email, "Enter Email: ", validateAdminEmail);

    cout << "Enter Address: ";
    cin >> Address;
    cout << "Enter Username: ";
    cin >> Username;
    cout << "Enter Password: ";
    cin >> Password;

    string UserID = getNextUserID(Position);
    
    PushBackUser(UserID, Name, ICNo, Gender, Position, ContactNumber, Email, Address, Username, Password);
    cout << "User added successfully." << endl;
}

//database CRUD
//=====================================================================================
// Function to get the next UserID based on Position
string getNextUserID(const string &Position) {
    sqlite3_stmt *stmt;
    string sql;
    int exit;
    int maxID = 0;
    
    // SQL query based on position
    if (Position == "Patient" || Position == "patient") {
        sql = "SELECT MAX(CAST(SUBSTR(UserID, 2) AS INTEGER)) FROM User WHERE UserID LIKE 'P%'";
    } else if (Position == "Doctor" || Position == "doctor" || Position == "Admin" || Position == "admin") {
        sql = "SELECT MAX(CAST(SUBSTR(UserID, 2) AS INTEGER)) FROM User WHERE UserID LIKE 'E%'";
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
    if (Position == "Patient" || Position == "patient") {
        return "P" + to_string(maxID);
    } else {
        return "E" + to_string(maxID);
    }
}

//admin CRUD
//=====================================================================================
// Function to read all users
void adminReadUser() {
    string query = "SELECT UserID, Name, ICNO, Gender, Position, ContactNumber, Email, Address FROM User ORDER BY UserID;";
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to filter users
void filterUser() {
    string filterBy, filterValue;
    bool validFilter = false;

    do {
        cout << "Filter by (Name, ICNo, Gender, Position, ContactNumber, Email, Address, Username): ";
        cin >> filterBy;

        // Checks if the input is a valid field
        if (filterBy == "Name" || filterBy == "name" || filterBy == "ICNo" || filterBy == "icno" || 
            filterBy == "Gender" || filterBy == "gender" || filterBy == "Position" || filterBy == "position" || 
            filterBy == "ContactNumber" || filterBy == "contactnumber" || filterBy == "Email" || filterBy == "email" ||
            filterBy == "Address" || filterBy == "address" || filterBy == "Username" || filterBy == "username") {
            validFilter = true;
        } else {
            cout << "Invalid field. Please try again." << endl;
        }
    } while (!validFilter);

    cout << "Enter value for " << filterBy << ": ";
    cin.ignore();
    getline(cin, filterValue);

    string query = "SELECT * FROM User WHERE " + filterBy + " LIKE '%" + filterValue + "%';";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cerr << "Error executing query: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    bool found = false;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;
        // Display results
        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* columnValue = (const char*)sqlite3_column_text(stmt, i);
            cout << columnName << ": " << (columnValue ? columnValue : "NULL") << endl;
        }
        cout << endl;
    }

    if (!found) {
        cout << "No relevant search results found." << endl;
    }

    sqlite3_finalize(stmt);
}

// Function to add new user
void adminCreateUser() {
    string addMore;
    do {
        string Name, ICNo, Gender, Position, ContactNumber, Email, Address, Username, Password;

        cout << "\nAdd New User" << endl;
        cout << "***********************************" << endl;

        // Prompt for user input
        cout << "Enter User Name: ";
        getline(cin >> ws, Name);
        
        promptUserInput(ICNo, "Enter IC Number (12 digits): ", validateICNo);
        promptUserInput(Gender, "Enter Gender (Female/Male): ", validateGender);
        promptUserInput(Position, "Enter Position (Patient/Doctor/Admin): ", validatePosition);
        promptUserInput(ContactNumber, "Enter User Contact Number (at least 10 digits): ", validateContactNumber);
        promptUserInput(Email, "Enter Email: ", validateAdminEmail);

        cout << "Enter Address: ";
        getline(cin >> ws, Address);
        
        cout << "Enter Username: ";
        getline(cin >> ws, Username);
        
        cout << "Enter Password: ";
        getline(cin >> ws, Password);

        // Generate UserID based on Position
        string UserID = getNextUserID(Position);
        
        // Construct the SQL statement
        string sql = "INSERT INTO User (UserID, Name, ICNo, Gender, Position, ContactNumber, Email, Address, Username, Password) VALUES ('" +
                     UserID + "', '" + Name + "', '" + ICNo + "', '" + Gender + "', '" + Position + "', '" + ContactNumber +
                     "', '" + Email + "', '" + Address + "', '" + Username + "', '" + Password + "');";

        char* messageError;
        // Execute the SQL statement
        int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Error Insert: " << messageError << endl;
            sqlite3_free(messageError);
        } else {
            cout << "User created successfully!\n" << endl;
        }

        // Optionally read and print all users to confirm the addition
        adminReadUser();

        cout << "Do you want to add another user? (Y/N): ";
        getline(cin >> ws, addMore);

    } while (addMore == "Y" || addMore == "y");
}

// Function to update user in the database
void adminUpdateUser() {
    string UserID;
    
    cout << "Enter Entire User ID to update (blank to exit update process): ";
    getline(cin, UserID);
    //cin.ignore();

    if (UserID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    // Check if UserID exists
    string checkSql = "SELECT COUNT(*) FROM User WHERE UserID = '" + UserID + "';";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);
    
    if (result != SQLITE_OK) {
        cerr << "Error checking UserID: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count == 0) {
            cout << "No relevant search results found." << endl;
            sqlite3_finalize(stmt);
            return;
        }
    }
    sqlite3_finalize(stmt);
    
    string fields[] = {"Name", "ICNo", "Gender", "Position", "ContactNumber", "Email", "Address", "Username", "Password"};
    string updates = "";

    cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

    for (const string& field : fields) {
        string newValue;
        bool isValid = false;

        do {
            cout << field << ": ";
            getline(cin, newValue);

            if (newValue.empty()) {
                isValid = true; // Allow skipping fields
            } else if (field == "ICNo") {
                if (newValue.length() == 12 && all_of(newValue.begin(), newValue.end(), ::isdigit)) {
                    isValid = true;
                } else {
                    cout << "Invalid ICNo. It must be exactly 12 digits.\n";
                }
            } else if (field == "Gender") {
                if (newValue == "male" || newValue == "female" || newValue == "Male" || newValue == "Female") {
                    isValid = true;
                } else {
                    cout << "Invalid Gender. Please enter 'Male', 'Female', or any lowercase variation.\n";
                }
            } else if (field == "Position") {
                if (newValue == "Patient" || newValue == "Doctor" || newValue == "Admin" ||
                    newValue == "patient" || newValue == "doctor" || newValue == "admin") {
                    isValid = true;
                } else {
                    cout << "Invalid Position. Please enter 'Patient', 'Doctor', 'Admin', or any lowercase variation.\n";
                }
            } else if (field == "ContactNumber") {
                if ((newValue.length() == 11 || newValue.length() == 12) && all_of(newValue.begin(), newValue.end(), ::isdigit)) {
                    isValid = true;
                } else {
                    cout << "Invalid Contact Number. It must be 11 or 12 digits.\n";
                }
            } else if (field == "Email") {
                if (newValue.find('@') != string::npos) {
                    isValid = true;
                } else {
                    cout << "Invalid Email. It must contain an '@' symbol.\n";
                }
            } else if (!newValue.empty()) {
                isValid = true;
            } else {
                isValid = true; // Skip empty fields without validation
            }
        } while (!isValid);

        if (!newValue.empty()) {
            if (!updates.empty()) {
                updates += ", ";
            }
      updates += field + " = '" + newValue + "'";
    }
  }

    if (updates.empty()) {
        cout << "No fields were updated." << endl;
        return;
    }
    
    string sql = "UPDATE User SET " + updates + " WHERE UserID = '" + UserID + "';";
    
    char* messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Update: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Data updated successfully!\n" << endl;
    }
    
    adminReadUser();
}

// Function to delete user from the database
void adminDeleteUser() {
    string UserID;
    
    cout << "Enter Entire User ID to delete (blank to exit delete process): ";
    getline(cin, UserID);
    //cin.ignore();

    if (UserID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    // Check if UserID exists
    string checkSql = "SELECT COUNT(*) FROM User WHERE UserID = '" + UserID + "';";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);
    
    if (result != SQLITE_OK) {
        cerr << "Error checking UserID: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count == 0) {
            cout << "No relevant search results found." << endl;
            sqlite3_finalize(stmt);
            return;
        }
    }
    sqlite3_finalize(stmt);

    string sql = "DELETE FROM User WHERE UserID = '" + UserID + "';";
    
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Delete: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Data delete successfully!\n" << endl;
    }
    adminReadUser();
}




// patient CRUD
//=====================================================================================
void readDoctorInfo() {
    string query = "SELECT UserID, Name, Gender, Position, ContactNumber, Email FROM User WHERE Position IN ('doctor', 'Doctor');";
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void patientReadProfile(const string& UserID) {
    string query = "SELECT * FROM User WHERE UserID = '" + UserID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read allergies: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Function to update user in the database
void patientUpdateUser() {
    string UserID;
    
    cout << "Enter User ID to update (blank to exit update process): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, UserID);

    if (UserID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (UserID.find('P') != 0) {
        UserID = 'P' + UserID;
    }
    
    // Check if UserID exists
    string checkSql = "SELECT COUNT(*) FROM User WHERE UserID = '" + UserID + "';";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);
    
    if (result != SQLITE_OK) {
        cerr << "Error checking UserID: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count == 0) {
            cout << "No relevant search results found." << endl;
            sqlite3_finalize(stmt);
            return;
        }
    }
    sqlite3_finalize(stmt);
    
    string fields[] = {"Name", "ICNo", "Gender", "ContactNumber", "Email", "Address", "Username", "Password"};
    string updates = "";

    cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;
    for (const string& field : fields) {
        string newValue;
        bool isValid = false;

         do {
            cout << field << ": ";
            getline(cin, newValue);

            if (newValue.empty()) {
                isValid = true; // Allow skipping fields
            } else if (field == "ICNo") {
                if (newValue.length() == 12 && all_of(newValue.begin(), newValue.end(), ::isdigit)) {
                    isValid = true;
                } else {
                    cout << "Invalid ICNo. It must be exactly 12 digits.\n";
                }
            } else if (field == "Gender") {
                if (newValue == "male" || newValue == "female" || newValue == "Male" || newValue == "Female") {
                    isValid = true;
                } else {
                    cout << "Invalid Gender. Please enter 'Male', 'Female', or any lowercase variation'.\n";
                }
            } else if (field == "ContactNumber") {
                if ((newValue.length() == 11 || newValue.length() == 12) && all_of(newValue.begin(), newValue.end(), ::isdigit)) {
                    isValid = true;
                } else {
                    cout << "Invalid Contact Number. It must be 11 or 12 digits.\n";
                }
            } else if (field == "Email") {
                if (newValue.find('@') != string::npos) {
                    isValid = true;
                } else {
                    cout << "Invalid Email. It must contain an '@' symbol.\n";
                }
            } else if (!newValue.empty()) {
                isValid = true;
            } else {
                isValid = true; // Skip empty fields without validation
            }
        } while (!isValid);

        if (!newValue.empty()) {
            if (!updates.empty()) {
                updates += ", ";
            }
      updates += field + " = '" + newValue + "'";
    }
  }

    if (updates.empty()) {
        cout << "No fields were updated." << endl;
        return;
    }
    
    string sql = "UPDATE User SET " + updates + " WHERE UserID = '" + UserID + "';";
    
    char* messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Update: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Data updated successfully!\n" << endl;
    }
    
    patientReadProfile(UserID);
}

// Function to delete user from the database
void patientDeleteUser(const string& UserID) {
    string input;

    cout << "Do you want to delete your account (blank to exit delete process)? (Y/N): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, input);

    if (input.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }
// Convert the input to uppercase to check if it's 'Y'
    char choice = toupper(input[0]);

    if (choice != 'Y') {
        cout << "Account deletion canceled." << endl;
        return;  // Exit the function if the choice is not 'Y'
    }

    // Proceed with deletion if the user confirms
    string sql = "DELETE FROM User WHERE UserID = '" + UserID + "';";
    char* messageError = nullptr;
    
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    
    if (exit != SQLITE_OK) {
        cerr << "Error during deletion: " << messageError << endl;
        sqlite3_free(messageError);
    }
        
        // Optional: Verify if the deletion was successful
        string checkSql = "SELECT COUNT(*) FROM User WHERE UserID = '" + UserID + "';";
        sqlite3_stmt* stmt;
        int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);
        
        if (result == SQLITE_OK) {
            result = sqlite3_step(stmt);
            if (result == SQLITE_ROW) {
                int count = sqlite3_column_int(stmt, 0);
                if (count == 0) {
                    cout << "Data deleted successfully!" << endl;
                } else {
                    cout << "The record still exists in the database." << endl;
                }
            }
            sqlite3_finalize(stmt);
        } else {
            cerr << "Error verifying deletion: " << sqlite3_errmsg(DB) << endl;
        }
    }

void doctorUpdateUser() {
    string UserID;
    
    cout << "Enter User ID to update (blank to return to menu): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, UserID);

    if (UserID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (UserID.find('P') != 0) {
        UserID = 'P' + UserID;
    }
    
    // Check if UserID exists
    string checkSql = "SELECT COUNT(*) FROM User WHERE UserID = '" + UserID + "';";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);
    
    if (result != SQLITE_OK) {
        cerr << "Error checking UserID: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count == 0) {
            cout << "No relevant search results found." << endl;
            sqlite3_finalize(stmt);
            return;
        }
    }
    sqlite3_finalize(stmt);
    
    string fields[] = {"Name", "ICNo", "Gender", "Position", "ContactNumber", "Email", "Address", "Username", "Password"};
    string updates = "";

    cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;
    for (const string& field : fields) {
        string newValue;
        bool isValid = false;

         do {
            cout << field << ": ";
            getline(cin, newValue);

            if (newValue.empty()) {
                isValid = true; // Allow skipping fields
            } else if (field == "ICNo") {
                if (newValue.length() == 12 && all_of(newValue.begin(), newValue.end(), ::isdigit)) {
                    isValid = true;
                } else {
                    cout << "Invalid ICNo. It must be exactly 12 digits.\n";
                }
            } else if (field == "Gender") {
                if (newValue == "male" || newValue == "female" || newValue == "Male" || newValue == "Female") {
                    isValid = true;
                } else {
                    cout << "Invalid Gender. Please enter 'Male', 'Female', or any lowercase variation'.\n";
                }
            } else if (field == "Position") {
                if (newValue == "Patient" || newValue == "Doctor" || newValue == "Admin" ||
                    newValue == "patient" || newValue == "doctor" || newValue == "admin") {
                    isValid = true;
                } else {
                    cout << "Invalid Position. Please enter 'Patient', 'Doctor', 'Admin', or any lowercase variation.\n";
                }
            } else if (field == "ContactNumber") {
                if ((newValue.length() == 11 || newValue.length() == 12) && all_of(newValue.begin(), newValue.end(), ::isdigit)) {
                    isValid = true;
                } else {
                    cout << "Invalid Contact Number. It must be 11 or 12 digits.\n";
                }
            } else if (field == "Email") {
                if (newValue.find('@') != string::npos) {
                    isValid = true;
                } else {
                    cout << "Invalid Email. It must contain an '@' symbol.\n";
                }
            } else if (!newValue.empty()) {
                isValid = true;
            } else {
                isValid = true; // Skip empty fields without validation
            }
        } while (!isValid);

        if (!newValue.empty()) {
            if (!updates.empty()) {
                updates += ", ";
            }
      updates += field + " = '" + newValue + "'";
    }
  }

    if (updates.empty()) {
        cout << "No fields were updated." << endl;
        return;
    }
    
    string sql = "UPDATE User SET " + updates + " WHERE UserID = '" + UserID + "';";
    
    char* messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Update: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Data updated successfully!\n" << endl;
    }
}

// Function to delete user from the database
void doctorDeleteUser(const string& UserID) {
    string input;

    cout << "Do you want to delete your account (blank to return to menu)? (Y/N): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, input);

    if (input.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }
// Convert the input to uppercase to check if it's 'Y'
    char choice = toupper(input[0]);

    if (choice != 'Y') {
        cout << "Account deletion canceled." << endl;
        return;  // Exit the function if the choice is not 'Y'
    }

    // Proceed with deletion if the user confirms
    string sql = "DELETE FROM User WHERE UserID = '" + UserID + "';";
    char* messageError = nullptr;
    
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    
    if (exit != SQLITE_OK) {
        cerr << "Error during deletion: " << messageError << endl;
        sqlite3_free(messageError);
    }
        
        // Optional: Verify if the deletion was successful
        string checkSql = "SELECT COUNT(*) FROM User WHERE UserID = '" + UserID + "';";
        sqlite3_stmt* stmt;
        int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);
        
        if (result == SQLITE_OK) {
            result = sqlite3_step(stmt);
            if (result == SQLITE_ROW) {
                int count = sqlite3_column_int(stmt, 0);
                if (count == 0) {
                    cout << "Data deleted successfully!" << endl;
                } else {
                    cout << "The record still exists in the database." << endl;
                }
            }
            sqlite3_finalize(stmt);
        } else {
            cerr << "Error verifying deletion: " << sqlite3_errmsg(DB) << endl;
        }
    }

#endif // ends this file conditional inclusion (must in the last line)

