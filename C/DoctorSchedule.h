#ifndef DOCTORSCHEDULE_H // checks if this file has not been defined.
#define DOCTORSCHEDULE_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include "function.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct DoctorSchedule {
  string DoctorScheduleID;
  string DoctorID;
  string Monday;
  string Tuesday;
  string Wednesday;
  string Thursday;
  string Friday;
  string Saturday;
  string Sunday;
  DoctorSchedule *next;
};

DoctorSchedule *DoctorSchedulehead = NULL;

// print table
void printDoctorSchedule(DoctorSchedule *n) {
  // Print table header
  cout << setw(5) << left << "Doctor Schedule ID"
       << " | " << setw(5) << left << "Doctor Name"
       << " | " << setw(5) << left << "Monday"
       << " | " << setw(5) << left << "Tuesday"
       << " | " << setw(5) << left << "Wednesday"
       << " | " << setw(5) << left << "Thursday"
       << " | " << setw(5) << left << "Friday"
       << " | " << setw(5) << left << "Saturday"
       << " | " << setw(5) << left << "Sunday" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->DoctorScheduleID << " | " 
         << setw(5) << left << n->DoctorID << " | " 
         << setw(5) << left << n->Monday << " | "
         << setw(5) << left << n->Tuesday << " | " 
         << setw(5) << left  << n->Wednesday << " | " 
         << setw(5) << left << n->Thursday << " | "
         << setw(5) << left << n->Friday << " | " 
         << setw(5) << left << n->Saturday << " | " 
         << setw(5) << left << n->Sunday << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackDoctorSchedule(
  string doctorScheduleID, 
  string doctorID,
  string monday, 
  string tuesday, 
  string wednesday,
  string thursday, 
  string friday, 
  string saturday,
  string sunday) {
  DoctorSchedule *newDoctorSchedule = new DoctorSchedule();
  newDoctorSchedule->DoctorScheduleID = doctorScheduleID;
  newDoctorSchedule->DoctorID = doctorID;
  newDoctorSchedule->Monday = monday;
  newDoctorSchedule->Tuesday = tuesday;
  newDoctorSchedule->Wednesday = wednesday;
  newDoctorSchedule->Thursday = thursday;
  newDoctorSchedule->Friday = friday;
  newDoctorSchedule->Saturday = saturday;
  newDoctorSchedule->Sunday = sunday;
  newDoctorSchedule->next = NULL;

  if (DoctorSchedulehead == NULL) {
    DoctorSchedulehead = newDoctorSchedule;
  } else {
    DoctorSchedule *temp = DoctorSchedulehead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newDoctorSchedule;
  }
}

// input to add new data
void addNewDoctorSchedule() {
  string DoctorScheduleID, DoctorID, Monday, Tuesday, Wednesday, Thursday,
      Friday, Saturday, Sunday;
  cout << "Enter Doctor Schedule ID: ";
  cin >> DoctorScheduleID;
  cout << "Enter Doctor ID: ";
  cin >> DoctorID;
  cout << "Enter Monday status (Working/Off): ";
  cin >> Monday;
  cout << "Enter Tuesday status (Working/Off): ";
  cin >> Tuesday;
  cout << "Enter Wednesday status (Working/Off): ";
  cin >> Wednesday;
  cout << "Enter Thursday status (Working/Off): ";
  cin >> Thursday;
  cout << "Enter Friday status (Working/Off): ";
  cin >> Friday;
  cout << "Enter Saturday status (Working/Off): ";
  cin >> Saturday;
  cout << "Enter Sunday status (Working/Off): ";
  cin >> Sunday;

  PushBackDoctorSchedule(DoctorScheduleID, DoctorID, Monday, Tuesday, Wednesday,
                         Thursday, Friday, Saturday, Sunday);
  cout << "Doctor Schedule added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to get the next DoctorScheduleID

string getNextDoctorScheduleID() {
  sqlite3_stmt *stmt;
  string sql = "SELECT MAX(CAST(SUBSTR(DoctorScheduleID, 2) AS INTEGER)) FROM "
               "DoctorSchedule"; // Update table name to 'DoctorSchedule'
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

  return "D" + to_string(maxID);
}

// Function to read DoctorSchedule
void adminReadDoctorSchedule() {
  string query = "SELECT * FROM DoctorSchedule;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateDoctorSchedule(const set<string>& validDoctorIDs) {
  string addMore;
  do {
  string DoctorID, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday,
      Sunday;

  cout << "\nAdd New Doctor Schedule" << endl;
    cout << "***********************************" << endl;

  // Retrieve the next DoctorScheduleID
  string DoctorScheduleID = getNextDoctorScheduleID();

  bool validDoctorID = false;
        do {
          cout << "Enter Doctor ID ( valid options are: ";
          for (const auto &id : validDoctorIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, DoctorID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(DoctorID) && !DoctorID.empty()) {
            DoctorID = "E" + DoctorID;
          }

          DoctorID = toUpperCase(DoctorID);

          // Direct validation check
          validDoctorID = validDoctorIDs.find(DoctorID) != validDoctorIDs.end();

          if (!validDoctorID) {
            cout << "Invalid Doctor ID. Please enter a valid Doctor ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validDoctorIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validDoctorID);

  cout << "Enter Monday Schedule: ";
  cin >> Monday;
  cout << "Enter Tuesday Schedule: ";
  cin >> Tuesday;
  cout << "Enter Wednesday Schedule: ";
  cin >> Wednesday;
  cout << "Enter Thursday Schedule: ";
  cin >> Thursday;
  cout << "Enter Friday Schedule: ";
  cin >> Friday;
  cout << "Enter Saturday Schedule: ";
  cin >> Saturday;
  cout << "Enter Sunday Schedule: ";
  cin >> Sunday;

  // Insert the new DoctorSchedule into the database
    string sql =
      "INSERT INTO DoctorSchedule (DoctorScheduleID, DoctorID, Monday, "
      "Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday) VALUES('" +
      DoctorScheduleID + "', '" + DoctorID + "', '" + Monday + "', '" +
      Tuesday + "', '" + Wednesday + "', '" + Thursday + "', '" + Friday +
      "', '" + Saturday + "', '" + Sunday + "');";

  char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "DoctorSchedule added successfully!" << endl;
    }

    // Optionally read and print all DoctorSchedules to confirm the addition
    adminReadDoctorSchedule();

    cout << "Do you want to add another DoctorSchedule? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update DoctorSchedule in the database
void adminUpdateDoctorSchedule(const set<string>& validDoctorIDs) {
  string DoctorScheduleID;

  cout << "Enter Doctor Schedule ID to update (blank to exit update process): ";
  getline(cin, DoctorScheduleID);

  if (DoctorScheduleID.empty()) {
        cout << "Update process exited.";
        return;
    }

    if (DoctorScheduleID.find("D") != 0) {
        DoctorScheduleID = "D" + DoctorScheduleID;
    }

  // Check if DoctorScheduleID exists
  string checkSql = "SELECT COUNT(*) FROM DoctorSchedule WHERE DoctorScheduleID = '" + DoctorScheduleID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking DoctorScheduleID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"DoctorID", "Monday", "Tuesday",  "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

   for (const string& field : fields) {
        string newValue;
        bool validInput = false;

        do {
            cout << field << ": ";
            getline(cin, newValue);

            if (field == "DoctorID" && !newValue.empty()) {
                if (isdigit(newValue[0])) {
                    newValue = "E" + newValue;
                }
                newValue = toUpperCase(newValue);

                if (validDoctorIDs.find(newValue) != validDoctorIDs.end()) {
                    validInput = true;
                } else {
                   cout << "Invalid DoctorID. Please enter a valid DoctorID." << endl;
                }
            }else if (!newValue.empty()) {
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

  string sql = "UPDATE DoctorSchedule SET " + updates + " WHERE DoctorScheduleID = '" + DoctorScheduleID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadDoctorSchedule();
}

// Function to delete DoctorSchedule from the database
void adminDeleteDoctorSchedule() {
  string DoctorScheduleID;

  cout << "Enter Doctor Schedule ID to delete (blank to exit delete process): ";
  getline(cin, DoctorScheduleID);

  if (DoctorScheduleID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    // Prepend "INS" if it's not present
    if (DoctorScheduleID.find("D") != 0) {
        DoctorScheduleID = "D" + DoctorScheduleID;
    }

  // Check if DoctorScheduleID exists
  string checkSql =
      "SELECT COUNT(*) FROM DoctorSchedule WHERE DoctorScheduleID = '" + DoctorScheduleID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking DoctorScheduleID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM DoctorSchedule WHERE DoctorScheduleID = '" +
               DoctorScheduleID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadDoctorSchedule();
}

#endif // ends this file conditional inclusion (must in the last line)