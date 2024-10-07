#ifndef CANCELAPPOINTMENT_H
#define CANCELAPPOINTMENT_H
#include "PatientAppointment.h"
#include "globalCRUD.h"
#include "sqlite3.h"
#include "function.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct CancelAppointment {
  string CancelAppointmentID;
  string AppointmentID;
  string DoctorID;
  string Reason;
  CancelAppointment *next;
};

CancelAppointment *CancelAppointmenthead = NULL;

// print table
void printCancelAppointment(CancelAppointment *n) {
  // Print table header
  cout << setw(5) << left << "Cancel Appointment ID"
       << " | " << setw(5) << left << "Appointment ID"
       << " | " << setw(5) << left << "Doctor ID"
       << " | " << setw(5) << left << "Reason" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->CancelAppointmentID << " | " 
         << setw(5) << left << n->AppointmentID << " | " 
         << setw(5) << left << n->DoctorID << " | " 
         << setw(5) << left << n->Reason << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackCancelAppointment(
  string cancelAppointmentID, 
  string appointmentID,
  string doctorID, 
  string reason) {
  CancelAppointment *newCancelAppointment = new CancelAppointment();
  newCancelAppointment->CancelAppointmentID = cancelAppointmentID;
  newCancelAppointment->AppointmentID = appointmentID;
  newCancelAppointment->DoctorID = doctorID;
  newCancelAppointment->Reason = reason;
  newCancelAppointment->next = NULL;

  if (CancelAppointmenthead == NULL) {
    CancelAppointmenthead = newCancelAppointment;
  } else {
    CancelAppointment *temp = CancelAppointmenthead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newCancelAppointment;
  }
}

// input to add new data
void addNewCancelAppointment() {
  string CancelAppointmentID, AppointmentID, DoctorID, Reason;
  cout << "Enter Cancel Appointment ID: ";
  cin >> CancelAppointmentID;
  cout << "Enter Appointment ID: ";
  cin >> AppointmentID;
  cout << "Enter Doctor ID: ";
  cin >> DoctorID;
  cout << "Enter Reason: ";
  cin >> Reason;

  PushBackCancelAppointment(CancelAppointmentID, AppointmentID, DoctorID,
                            Reason);
  cout << "Cancel Appointment added successfully." << endl;
}

// database CRUD
//=====================================================================================
//admin
//  Function to generate the next CancelAppointmentID
string getNextCancelAppointmentID() {
    sqlite3_stmt *stmt;
    string sql = "SELECT MAX(CAST(SUBSTR(CancelAppointmentID, 3) AS INTEGER)) FROM CancelAppointment;";
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    int maxID = 0;
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0); // Get the max ID (numeric part) from the table
    }

    sqlite3_finalize(stmt);

    maxID++; // Increment the ID for the new record

    return "CA" + to_string(maxID);
}


void adminReadCancelAppointment() {
  string query = "SELECT * FROM CancelAppointment;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateCancelAppointment(const set<string> &validAppointmentIDs,
                                  const set<string> &validDoctorIDs) {
  string addMore;
  do {
    string AppointmentID, DoctorID, Reason;

    cout << "\nAdd New Cancel Appointment" << endl;
    cout << "***********************************" << endl;

    // Automatically generate CancelAppointment ID
    string CancelAppointmentID = getNextCancelAppointmentID();

    if (CancelAppointmentID.empty()) {
      cerr << "Failed to generate CancelAppointmentID. Please try again."
           << endl;
      return;
    }

    bool validAppointmentID = false; // Fixed variable name spelling
    do {
      cout << "Enter Appointment ID (valid options are: ";
      for (const auto &id : validAppointmentIDs) {
        cout << id << " ";
      }
      cout << "): ";

      getline(cin >> ws,
              AppointmentID); // Read input with leading whitespace stripped

      if (isNumeric(AppointmentID) && !AppointmentID.empty()) {
        AppointmentID = "A" + AppointmentID;
      }

      AppointmentID = toUpperCase(AppointmentID);

      // Direct validation check
      validAppointmentID =
          validAppointmentIDs.find(AppointmentID) != validAppointmentIDs.end();

      if (!validAppointmentID) {
        cout << "Invalid Appointment ID. Please enter a valid Appointment ID "
                "from the list."
             << endl;
        cout << "Valid options are: ";
        for (const auto &id : validAppointmentIDs) {
          cout << id << " ";
        }
        cout << ": ";
      }
    } while (!validAppointmentID);

    string sqlCheck =
        "SELECT COUNT(*) FROM CancelAppointment WHERE AppointmentID = '" +
        AppointmentID + "';";
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(DB, sqlCheck.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
      cerr << "Failed to prepare statement: \n" << sqlite3_errmsg(DB) << endl;
      return;
    }

    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
      cout << "This Appointment ID has already been cancelled. Update process "
              "exited.\n"
           << endl;
      sqlite3_finalize(stmt);
      return;
    }

    sqlite3_finalize(stmt);

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
        cout << "Invalid Doctor ID. Please enter a valid Doctor ID from the "
                "list."
             << endl;
        cout << "Valid options are: ";
        for (const auto &id : validDoctorIDs) {
          cout << id << " ";
        }
        cout << ": ";
      }
    } while (!validDoctorID);

    cout << "Enter Cancel Reason: ";
    getline(cin >> ws, Reason);

    // Insert the new CancelAppointment into the database
    string sqlInsert = "INSERT INTO CancelAppointment (CancelAppointmentID, "
                       "AppointmentID, DoctorID, Reason) VALUES ('" +
                       CancelAppointmentID + "', '" + AppointmentID + "', '" +
                       DoctorID + "', '" + Reason + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sqlInsert.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "CancelAppointment added successfully!\n" << endl;

      // Update Status in PatientsAppointment table
      string sqlUpdate = "UPDATE PatientsAppointment SET Status = 'Cancelled' "
                         "WHERE AppointmentID = '" +
                         AppointmentID + "';";
      exit = sqlite3_exec(DB, sqlUpdate.c_str(), NULL, 0, &messageError);
      if (exit != SQLITE_OK) {
        cerr << "Error updating PatientsAppointment status: " << messageError
             << endl;
        sqlite3_free(messageError);
      } else {
        cout << "PatientsAppointment status updated to 'Cancelled'!\n" << endl;
      }
    }

    // Optionally read and print all CancelAppointments to confirm the addition
    adminReadCancelAppointment();

    cout << "Do you want to add another Cancel Appointment? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update CancelAppointment in the database
void adminUpdateCancelAppointment(const set<string> &validAppointmentIDs,
                                  const set<string> &validDoctorIDs) {
  string CancelAppointmentID;

  cout << "Enter CancelAppointment ID to update (blank to exit update process): ";
  getline(cin, CancelAppointmentID);

  if (CancelAppointmentID.empty()) {
    cout << "Update process exited.";
    return;
  }

  if (CancelAppointmentID.find("CA") != 0) {
    CancelAppointmentID = "CA" + CancelAppointmentID;
  }

  // Check if CancelAppointmentID exists
  string checkSql =
      "SELECT COUNT(*) FROM CancelAppointment WHERE CancelAppointmentID = '" +
      CancelAppointmentID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking CancelAppointmentID: " << sqlite3_errmsg(DB)
         << endl;
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

  string fields[] = {"AppointmentID", "DoctorID", "Reason"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip "
          "updating the field."
       << endl;

  for (const string &field : fields) {
    string newValue;
    bool validInput = false;

    do {
      cout << field << ": ";
      getline(cin, newValue);

      if (field == "AppointmentID" && !newValue.empty()) {
        if (isdigit(newValue[0])) {
          newValue = "A" + newValue;
        }
        newValue = toUpperCase(newValue);

        if (validAppointmentIDs.find(newValue) != validAppointmentIDs.end()) {
          validInput = true;
        } else {
          cout << "Invalid AppointmentID. Please enter a valid AppointmentID."
               << endl;
        }
      } else if (field == "DoctorID" && !newValue.empty()) {
        if (isdigit(newValue[0])) {
          newValue = "E" + newValue;
        }
        newValue = toUpperCase(newValue);

        if (validDoctorIDs.find(newValue) != validDoctorIDs.end()) {
          validInput = true;
        } else {
          cout << "Invalid DoctorID. Please enter a valid DoctorID." << endl;
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

  string sql = "UPDATE CancelAppointment SET " + updates +
               " WHERE CancelAppointmentID = '" + CancelAppointmentID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!" << endl;
  }

  adminReadCancelAppointment();
}
//=====================================================================================
//patient
// Function to add new data
string getDoctorIDByAppointmentID(const string& AppointmentID) {
    string DoctorID;
    string sql = "SELECT DoctorID FROM PatientsAppointment WHERE AppointmentID = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    // Bind AppointmentID parameter
    sqlite3_bind_text(stmt, 1, AppointmentID.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        DoctorID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        cerr << "No matching DoctorID found for AppointmentID: " << AppointmentID << endl;
    }

    sqlite3_finalize(stmt);
    return DoctorID;
}

void patientReadCancelAppointment(const string& PatientID) {
    // Define the SQL query with a placeholder for PatientID
    string query = R"(
        SELECT CA.CancelAppointmentID, CA.AppointmentID, CA.DoctorID, CA.Reason
        FROM CancelAppointment CA
        JOIN PatientsAppointment PA ON CA.AppointmentID = PA.AppointmentID
        WHERE PA.PatientID = ?
    )";

    // Prepare the SQL statement
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the PatientID parameter to the SQL query
    result = sqlite3_bind_text(stmt, 1, PatientID.c_str(), -1, SQLITE_STATIC);

    if (result != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Execute the query and handle results
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Extract data from the current row
        string cancelAppointmentID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string appointmentID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string doctorID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string reason = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        // Display or process the data
        cout << "Cancel Appointment ID: " << cancelAppointmentID << endl;
        cout << "Appointment ID: " << appointmentID << endl;
        cout << "Doctor ID: " << doctorID << endl;
        cout << "Reason: " << reason << endl << endl;
    }

    if (result != SQLITE_DONE) {
        cerr << "Failed to fetch results: " << sqlite3_errmsg(DB) << endl;
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);
}

void patientCreateCancelAppointment(const set<string>& validAppointmentIDs, const string& PatientID) {
     string addMore;
    do {
        string AppointmentID, DoctorID, Reason;

        cout << "\nAdd New Cancel Appointment" << endl;
        cout << "***********************************" << endl;

        string CancelAppointmentID = getNextCancelAppointmentID();

        // DEBUG: Print CancelAppointmentID to confirm it's generated
        cout << "Generated CancelAppointmentID: " << CancelAppointmentID << endl;

        if (CancelAppointmentID.empty()) {
            cerr << "Failed to generate CancelAppointmentID. Please try again." << endl;
            return;
        }

       bool validAppointmentID = false;  // Fixed variable name spelling
      do {
    cout << "Enter Appointment ID (valid options are: ";
    for (const auto& id : validAppointmentIDs) {
        cout << id << " ";
    }
    cout << "): ";

    getline(cin >> ws, AppointmentID);  // Read input with leading whitespace stripped

    // Check if the input is numeric and prefix with 'A' if true
    if (isNumeric(AppointmentID) && !AppointmentID.empty()) {
        AppointmentID = "A" + AppointmentID;
    }

    AppointmentID = toUpperCase(AppointmentID);

    // Direct validation check
    validAppointmentID = validAppointmentIDs.find(AppointmentID) != validAppointmentIDs.end();

    if (!validAppointmentID) {
        cout << "Invalid Appointment ID. Please enter a valid Appointment ID from the list." << endl;
        cout << "Valid options are: ";
        for (const auto& id : validAppointmentIDs) {
            cout << id << " ";
        }
        cout << ": ";
    }
} while (!validAppointmentID);

// Check if the AppointmentID already exists in the CancelAppointment table
string sqlCheck =
    "SELECT COUNT(*) FROM CancelAppointment WHERE AppointmentID = '" +
    AppointmentID + "';";
sqlite3_stmt *stmt;
int result = sqlite3_prepare_v2(DB, sqlCheck.c_str(), -1, &stmt, NULL);

if (result != SQLITE_OK) {
  cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
  return;
}

result = sqlite3_step(stmt);

if (result == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
  cout << "This Appointment ID has already been cancelled. Update process "
          "exited."
       << endl;
  sqlite3_finalize(stmt);
  return;
}

sqlite3_finalize(stmt);


        DoctorID = getDoctorIDByAppointmentID(AppointmentID);

        if (DoctorID.empty()) {
            cerr << "No matching DoctorID found for AppointmentID: " << AppointmentID << endl;
            continue; // Skip to next iteration if DoctorID is not found
        }

        cout << "Enter Cancel Reason: ";
        getline(cin >> ws, Reason);

    string sqlInsert = "INSERT INTO CancelAppointment (CancelAppointmentID, AppointmentID, DoctorID, Reason) VALUES ('" +
                     CancelAppointmentID + "', '" + AppointmentID + "', '" + DoctorID + "', '" + Reason + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sqlInsert.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "CancelAppointment added successfully!" << endl;

      // Update Status in PatientsAppointment table
      string sqlUpdate = "UPDATE PatientsAppointment SET Status = 'Cancelled' WHERE AppointmentID = '" +
                         AppointmentID + "';";
      exit = sqlite3_exec(DB, sqlUpdate.c_str(), NULL, 0, &messageError);
      if (exit != SQLITE_OK) {
        cerr << "Error updating PatientsAppointment status: " << messageError
             << endl;
        sqlite3_free(messageError);
      } else {
        cout << "PatientsAppointment status updated to 'Cancelled'!" << endl;
      }
    }

        // Optionally read and print all CancelAppointments to confirm the addition
        patientReadCancelAppointment(PatientID);

        cout << "Do you want to add another Cancel Appointment? (Y/N): ";
        getline(cin >> ws, addMore);

    } while (addMore == "Y" || addMore == "y");
}


void patientUpdateCancelAppointment(const set<string>& validAppointmentIDs, const set<string>& validDoctorIDs) {
  string CancelAppointmentID, PatientID, AppointmentID, DoctorID;

  cout << "Enter CancelAppointment ID to update: ";
  getline(cin, CancelAppointmentID);
  //cin.ignore();
  
  if (CancelAppointmentID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

  if (CancelAppointmentID.find("CA") != 0) {
        CancelAppointmentID = "CA" + CancelAppointmentID;
    }

  // Check if CancelAppointmentID exists
  string checkSql = "SELECT COUNT(*) FROM CancelAppointment WHERE CancelAppointmentID = '" + CancelAppointmentID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking CancelAppointmentID: " << sqlite3_errmsg(DB)
         << endl;
    return;
  }

  sqlite3_bind_text(stmt, 1, CancelAppointmentID.c_str(), -1, SQLITE_TRANSIENT);
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

  string fields[] = {"AppointmentID", "Reason"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field."
       << endl;

  for (const string &field : fields) {
        string newValue;
        bool validInput = false;

        do {
            cout << field << ": ";
            getline(cin, newValue);

            if (field == "AppointmentID" && !newValue.empty()) {
                if (isdigit(newValue[0])) {
                    newValue = "A" + newValue;
                }
                newValue = toUpperCase(newValue);

                if (validAppointmentIDs.find(newValue) != validAppointmentIDs.end()) {
                    AppointmentID = newValue;
                    validInput = true;
                } else {
                    cout << "Invalid AppointmentID. Please enter a valid AppointmentID." << endl;
                }
            }else if (field == "Reason" && !newValue.empty()) {
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

  string sql = "UPDATE CancelAppointment SET " + updates + " WHERE CancelAppointmentID = '" + CancelAppointmentID + "';";
  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  patientReadCancelAppointment(PatientID);
}



#endif // ends this file conditional inclusion (must in the last line)