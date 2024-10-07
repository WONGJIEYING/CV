#ifndef FEEDBACK_H // checks if this file has not been defined.
#define FEEDBACK_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include "function.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct Feedback {
  string FeedbackID;
  string PatientID;
  string Comment;
  int Rating;
  string Date;
  Feedback *next;
};

Feedback *Feedbackhead = NULL;

// print table
void printFeedback(Feedback *n) {
  // Print table header
  cout << setw(5) << left << "Feedback ID"
       << " | " << setw(5) << left << "Patient ID"
       << " | " << setw(5) << left << "Comment"
       << " | " << setw(5) << left << "Rating"
       << " | " << setw(5) << left << "Date" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->FeedbackID << " | " 
         << setw(5) << left << n->PatientID << " | " 
         << setw(5) << left << n->Comment << " | "
         << setw(5) << left << n->Rating << " | " 
         << setw(5) << left << n->Date << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackFeedback(string feedbackID, string patientID, string comment,
                      int rating, string date) {
  Feedback *newFeedback = new Feedback();
  newFeedback->FeedbackID = feedbackID;
  newFeedback->PatientID = patientID;
  newFeedback->Comment = comment;
  newFeedback->Rating = rating;
  newFeedback->Date = date;
  newFeedback->next = NULL;

  if (Feedbackhead == NULL) {
    Feedbackhead = newFeedback;
  } else {
    Feedback *temp = Feedbackhead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newFeedback;
  }
}

// Function to check if a string is a valid date in YYYY-MM-DD format
bool isValidDate(const string &date) {
  if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
    return false;
  }
  for (int i = 0; i < date.size(); ++i) {
    if (i == 4 || i == 7)
      continue;
    if (!isdigit(date[i]))
      return false;
  }
  int year = stoi(date.substr(0, 4));
  int month = stoi(date.substr(5, 2));
  int day = stoi(date.substr(8, 2));
  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;
  if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    return false;
  if (month == 2) {
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > 29 || (day == 29 && !isLeapYear))
      return false;
  }
  return true;
}

// input to add new data
void addnewFeedback() {
  string FeedbackID, PatientID, Comment, Date;
  int Rating;
  cout << "Enter Feedback: ";
  cin >> FeedbackID;
  cout << "Enter Patient ID: ";
  cin >> PatientID;
  cout << "Enter Comment: ";
  cin >> Comment;
  cout << "Enter Rating (0-5): ";
  cin >> Rating;
  cout << "Enter Date: ";
  cin >> Date;

  PushBackFeedback(FeedbackID, PatientID, Comment, Rating, Date);
  cout << "Feedback added successfully." << endl;
}

// database CRUD
//admin
//==========================================================================================================
void readFeedback() {
  string query = "SELECT * FROM Feedback;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}
//==========================================================================================================
//patient 
// Function to generate the next FeedbackID
string getNextFeedbackID() {
  sqlite3_stmt *stmt;
  string sql = "SELECT MAX(CAST(SUBSTR(FeedbackID, 2) AS INTEGER)) FROM "
               "Feedback"; // Update table name to 'Feedback'
  int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

  if (exit != SQLITE_OK) {
    cerr << "Failed to prepare statement for ID generation: "
         << sqlite3_errmsg(DB) << endl;
    return "";
  }

  int maxID = 0;
  exit = sqlite3_step(stmt);
  if (exit == SQLITE_ROW) {
    maxID = sqlite3_column_int(
        stmt, 0); // Get the max ID (numeric part) from the table
  }

  sqlite3_finalize(stmt);

  maxID++; // Increment the ID for the new record

  return "F" + to_string(maxID);
}

void patientReadFeedback(const string& PatientID) {
    string query = "SELECT * FROM Feedback WHERE PatientID = '" + PatientID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read Feedback: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Function to add new data
void patientCreateFeedback(const string& UserID) {
  string addMore;
  do {
    string Comment, Date;
    int Rating;

    cout << "\nAdd New Feedback" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Feedback ID
    string FeedbackID = getNextFeedbackID();

    cout << "Enter Comment: ";
    getline(cin >> ws, Comment);

    do {
        cout << "Enter Rating (0-5): ";
        cin >> Rating;

        // Check if the input is a valid integer and within the range 0-5
        if (cin.fail() || Rating < 0 || Rating > 5) {
          handleInvalidInput();
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore any extra input
            break; // Input is valid, exit the loop
        }
    } while (true);

  do {
    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin, Date);
    if (!isValidDatePA(Date)) {
      cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
    }
  } while (!isValidDatePA(Date));

    // Insert the new Feedback into the database
    string sql = "INSERT INTO Feedback (FeedbackID, PatientID, Comment, Rating, Date) "
                 "VALUES ('" +
                 FeedbackID + "', '" + UserID + "', '" + Comment + "', '" +
                 to_string(Rating) + "', '" + Date + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Feedback added successfully!\n" << endl;
    }

    cout << "Do you want to add another Feedback? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Feedback in the database
void patientUpdateFeedback() {
  string FeedbackID, UserID;

  cout << "Enter Feedback ID to update (blank to exit update process): ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, FeedbackID);

  if (FeedbackID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (FeedbackID.find("F") != 0) {
        FeedbackID = "F" + FeedbackID;
    }

  // Check if FeedbackID exists
  string checkSql =
      "SELECT COUNT(*) FROM Feedback WHERE FeedbackID = '" + FeedbackID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking FeedbackID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"Comment", "Rating", "Date"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field."
       << endl;

  for (const string &field : fields) {
    string newValue;
    bool validInput = false;

     do {
    cout << field << ": ";
    getline(cin, newValue);

    validInput = false; // Reset validInput for each field

            if (field == "Rating" && !newValue.empty()) {
    try {
        float rating = stof(newValue);
        
        if (rating >= 0.0 && rating <= 5.0) {
            validInput = true;
        } else {
            cout << "Invalid Rating value. Please enter a value between 0 and 5." << endl;
        }
    } catch (exception& e) {
        cout << "Invalid Rating value. Please enter a valid number between 0 and 5." << endl;
    }
}
else if (field == "Date" && !newValue.empty()) {
                if (isValidDatePA(newValue)) {
                    validInput = true;
                } else {
                    cout << "Invalid Date. Please enter a valid date in the format YYYY-MM-DD." << endl;
                }
            } else if (!newValue.empty()) {
                validInput = true;
            } else {
                validInput = true; // Skip empty fields without validation
            }

        } while (!validInput);

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

  string sql = "UPDATE Feedback SET " + updates + " WHERE FeedbackID = '" + FeedbackID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  patientReadFeedback(UserID);
}

// Function to delete Feedback from the database
void patientDeleteFeedback() {
  string FeedbackID, UserID;

  cout << "Enter Feedback ID to delete (blank to exit delete process): ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, FeedbackID);

  if (FeedbackID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    if (FeedbackID.find("F") != 0) {
        FeedbackID = "F" + FeedbackID;
    }

  // Check if FeedbackID exists
  string checkSql =
      "SELECT COUNT(*) FROM Feedback WHERE FeedbackID = '" + FeedbackID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking FeedbackID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Feedback WHERE FeedbackID = '" + FeedbackID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  patientReadFeedback(UserID);
}

#endif // ends this file conditional inclusion (must in the last line)