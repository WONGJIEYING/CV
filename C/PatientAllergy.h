#ifndef PATIENTALLERGY_H // checks if this file has not been defined.
#define PATIENTALLERGY_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"

using namespace std;

// structure
struct PatientAllergy {
    string PatientAllergyID;
    string PatientID;
    string AllergyName;
    string Severity;
    string Rection;
    PatientAllergy* next;
};

PatientAllergy* PatientAllergyhead = NULL;

//print table
void printPatientAllergy(PatientAllergy* n) {
    // Print table header
    cout << setw(5) << left << "Patient Allergy ID" << " | "
         << setw(5) << left << "Patient ID" << " | "
         << setw(5) << left << "Allergy Name" << " | "
         << setw(5) << left << "Severity" << " | "
         << setw(5) << left << "Rection" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->PatientAllergyID << " | "
             << setw(5) << left << n->PatientID<< " | "
             << setw(5) << left << n->AllergyName << " | "
             << setw(5) << left << n->Severity << " | "
             << setw(5) << left << n->Rection << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackPatientAllergy(
    string patientAllergyID,
    string patientID,
    string allergyName,
    string severity,
    string rection) {
    PatientAllergy* newPatientAllergy = new PatientAllergy();
    newPatientAllergy->PatientAllergyID = patientAllergyID;
    newPatientAllergy->PatientID = patientID;
    newPatientAllergy->AllergyName = allergyName;
    newPatientAllergy->Severity = severity;
    newPatientAllergy->Rection = rection;
    newPatientAllergy->next = NULL;

    if (PatientAllergyhead== NULL) {
        PatientAllergyhead = newPatientAllergy;
    } else {
        PatientAllergy* temp = PatientAllergyhead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newPatientAllergy;
    }
}

// input to add new data
void addNewPatientAllergy() {
    string PatientAllergyID, PatientID, AllergyName, Severity, Rection;
    cout << "Enter Patient Allergy ID: ";
    cin >> PatientAllergyID;
    cout << "Enter Patient ID: ";
    cin >> PatientID;
    cout << "Enter Allergy Nam: ";
    cin >> AllergyName;
    cout << "Enter Severity: ";
    cin >> Severity;
    cout << "Enter Rection: ";
    cin >> Rection;

    PushBackPatientAllergy(PatientAllergyID, PatientID, AllergyName, Severity, Rection);
    cout << "Patient Allergy added successfully." << endl;
}

// database CRUD
//==========================================================================================================

string authenticatedUserID;

string getNextPatientAllergyID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(PatientAllergyID, 3) AS INTEGER)) FROM PatientAllergy"; // Update table name to 'MedicalInsurance'
  int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

  if (exit != SQLITE_OK) {
    cerr << "Failed to prepare statement for ID generation: "
         << sqlite3_errmsg(DB) << endl;
    return "";
  }

  int maxID = 0;
  exit = sqlite3_step(stmt);
  if (exit == SQLITE_ROW) {
    maxID = sqlite3_column_int(stmt, 0); // Get the max ID (numeric part) from the table
  }

  sqlite3_finalize(stmt);

  maxID++; // Increment the ID for the new record

  // Return the new ID with the "N" prefix
  return "AI" + to_string(maxID);
}

void adminReadPatientAllergy() {
  string query = "SELECT * FROM PatientAllergy;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}


void adminCreatePatientAllergy(const set<string>& validPatientIDs) {
  string addMore;
  do {
    string PatientID, AllergyName, Severity, Reaction;

    cout << "\nAdd New Patient Allergy" << endl;
    cout << "***********************************" << endl;

    // Automatically generate PatientAllergy ID
    string PatientAllergyID = getNextPatientAllergyID();

    bool validPatientID = false;
    do {
            cout << "Enter new Patient ID (valid options are: ";
            for (const auto &id : validPatientIDs) {
                cout << id << " ";
            }
            cout << "): ";

            getline(cin >> ws, PatientID);

            // Check if the input is numeric and prefix with 'P' if true
           if (PatientID.empty()) {
                cout << "Exiting." << endl;
                return; // Exit if no input is provided
            }

            // Check if the input is numeric and prefix with 'P' if true
           if (isdigit(PatientID[0])) {
                PatientID = "P" + PatientID;
            }

            PatientID = toUpperCase(PatientID);

          
// Direct validation check
            validPatientID = validPatientIDs.find(PatientID) != validPatientIDs.end();

            if (!validPatientID) {
                cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;
                cout << "Valid options are: ";
                for (const auto &id : validPatientIDs) {
                    cout << id << " ";
                }
                cout << endl;
            }
        } while (!validPatientID);

    cout << "Enter Allergy Name: ";
    getline(cin, AllergyName);

    cout << "Enter Severity: ";
    getline(cin, Severity);

    cout << "Enter Reaction: ";
    getline(cin, Reaction);
    //cin.ignore();

    // Insert the new MedicalInsurance into the database
string sql = "INSERT INTO PatientAllergy (PatientAllergyID, PatientID, "
             "AllergyName, Severity, Reaction) VALUES ('" +
             PatientAllergyID + "', '" + PatientID + "', '" +
             AllergyName + "', '" + Severity + "', '" +
             Reaction + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Patien Allergy added successfully!\n" << endl;
    }

    // Optionally read and print all MedicalInsurances to confirm the addition
    adminReadPatientAllergy();

    cout << "Do you want to add another Patient Allergy? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}


void adminUpdatePatientAllergy(const set<string>& validPatientIDs) {
  string PatientAllergyID;

  cout << "Enter Patient Allergy ID to update: ";
  cin >> PatientAllergyID;

   if (PatientAllergyID.find("AI") != 0) {
        // Prepend "AI" if it's not present
        PatientAllergyID = "AI" + PatientAllergyID;
    }

  // Check if PatientAllergyID exists
  string checkSql =
      "SELECT COUNT(*) FROM PatientAllergy WHERE PatientAllergyID = '" + PatientAllergyID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking PatientAllergyID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"PatientID", "AllergyName", "Severity", "Reaction"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

  cin.ignore(); // To clear the newline character from the input buffer
  for (const string &field : fields) {
    string newValue;
    bool validInput = false;

   do {
      cout << field << ": ";
      getline(cin, newValue);


    if (field == "PatientID" && !newValue.empty()) {
        if (isdigit(newValue[0])) {
          newValue = "P" + newValue;
        }
        newValue = toUpperCase(newValue);

        if (validPatientIDs.find(newValue) != validPatientIDs.end()) {
          validInput = true;
        } else {
          cout << "Invalid PatientID. Please enter a valid PatientID." << endl;
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

  string sql =
      "UPDATE PatientAllergy SET " + updates + " WHERE PatientAllergyID = '" + PatientAllergyID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!" << endl;
  }

  adminReadPatientAllergy();
}

void adminDeleteAllergy() {
  string PatientAllergyID;

  cout << "Enter Patient Allergy ID to delete: ";
  cin >> PatientAllergyID;

  // Check if PatientAllergyID exists
  string checkSql =
      "SELECT COUNT(*) FROM PatientAllergy WHERE PatientAllergyID = '" +
      PatientAllergyID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking PatientAllergyID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM PatientAllergy WHERE PatientAllergyID = '" +
               PatientAllergyID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!" << endl;
  }
  adminReadPatientAllergy();
}

void addPatientAllergy(const string& UserID) {
    string AllergyName, Severity, Reaction;
    
    cout << "Enter Allergy Name: ";
    getline(cin >> ws, AllergyName);

    cout << "Enter Severity: ";
    getline(cin >> ws, Severity);

    cout << "Enter Reaction: ";
    getline(cin >> ws, Reaction);

    // Generate a new PatientAllergyID
    string PatientAllergyID = getNextPatientAllergyID();

    // Create the SQL insert query
    string query = "INSERT INTO PatientAllergy (PatientAllergyID, PatientID, AllergyName, Severity, Reaction) VALUES ('" +
                   PatientAllergyID + "', '" + UserID + "', '" + AllergyName + "', '" + Severity + "', '" + Reaction + "');";

    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        cerr << "Failed to add allergy: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Patient Allergy added successfully!" << endl;
    }
}

void patientReadPatientAllergy(const string& PatientID) {
    string query = "SELECT * FROM PatientAllergy WHERE PatientID = '" + PatientID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read allergies: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void patientUpdatePatientAllergy() {
  string PatientAllergyID, UserID;

  cout << "Enter Patient Allergy ID to update (blank to exit update process): ";
  getline(cin, PatientAllergyID);

  if (PatientAllergyID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (PatientAllergyID.find("AI") != 0) {
        PatientAllergyID = "AI" + PatientAllergyID;
    }

  // Check if PatientAllergyID exists
  string checkSql = "SELECT COUNT(*) FROM PatientAllergy WHERE PatientAllergyID = '" + PatientAllergyID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking PatientAllergyID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"AllergyName", "Severity", "Reaction"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

  for (const string &field : fields) {
    string newValue;
    cout << field << ": ";
    getline(cin, newValue);
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

  string sql = "UPDATE PatientAllergy SET " + updates + " WHERE PatientAllergyID = '" + PatientAllergyID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  patientReadPatientAllergy(UserID);
}

void patientDeleteAllergy() {
  string PatientAllergyID, UserID;

  cout << "Enter Patient Allergy ID to delete (blank to exit delete process): ";
  getline(cin, PatientAllergyID);

  if (PatientAllergyID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    if (PatientAllergyID.find("AI") != 0) {
        PatientAllergyID = "AI" + PatientAllergyID;
    }

  // Check if PatientAllergyID exists
  string checkSql =
      "SELECT COUNT(*) FROM PatientAllergy WHERE PatientAllergyID = '" + PatientAllergyID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking PatientAllergyID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM PatientAllergy WHERE PatientAllergyID = '" +
               PatientAllergyID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  patientReadPatientAllergy(UserID);
}

#endif // ends this file conditional inclusion (must in the last line)