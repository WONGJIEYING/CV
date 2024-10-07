#ifndef _14VACCINATIONHISTORY_H // checks if this file has not been defined.
#define _14VACCINATIONHISTORY_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"

using namespace std;

// structure
struct VaccinationRecord {
    string VaccinationID;
    string PatientID;
    string VaccineType;
    string DoseNumber;
    string AdministrationDate;
    VaccinationRecord* next;
};

VaccinationRecord* VaccinationRecordhead = NULL;

//print table
void printVaccinationRecord(VaccinationRecord* n) {
    // Print table header
    cout << setw(5) << left << "Vaccination ID" << " | "
         << setw(5) << left << "Patient ID" << " | "
         << setw(5) << left << "Vaccine Type" << " | "
         << setw(5) << left << "Dose Number" << " | "
         << setw(5) << left << "Administration Date" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->VaccinationID << " | "
             << setw(5) << left << n->PatientID << " | "
             << setw(5) << left << n->VaccineType << " | "
             << setw(5) << left << n->DoseNumber << " | "
             << setw(5) << left << n->AdministrationDate << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackVaccinationRecord(
    string vaccinationID,
    string patientID,
    string vaccineType,
    string doseNumber,
    string administrationDate) {
    VaccinationRecord* newVaccinationRecord = new VaccinationRecord();
    newVaccinationRecord->VaccinationID = vaccinationID;
    newVaccinationRecord->PatientID = patientID;
    newVaccinationRecord->VaccineType = vaccineType;
    newVaccinationRecord->DoseNumber = doseNumber;
    newVaccinationRecord->AdministrationDate = administrationDate;
    newVaccinationRecord->next = NULL;

    if (VaccinationRecordhead == NULL) {
        VaccinationRecordhead = newVaccinationRecord;
    } else {
        VaccinationRecord* temp = VaccinationRecordhead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newVaccinationRecord;
    }
}

// input to add new data
void addNewVaccinationRecord() {
    string VaccinationID, PatientID, VaccineType, DoseNumber, AdministrationDate;
    cout << "Enter Vaccination ID: ";
    cin >> VaccinationID;
    cout << "Enter Patient ID: ";
    cin >> PatientID;
    cout << "Enter Vaccine Type: ";
    cin >> VaccineType;
    cout << "Enter Dose Number: ";
    cin >> DoseNumber;
    cout << "Enter Administration Date: ";
    cin >> AdministrationDate;
    
    PushBackVaccinationRecord(VaccinationID, PatientID, VaccineType, DoseNumber, AdministrationDate);
    cout << "Vaccination Record added successfully." << endl;
}

// database CRUD
//==========================================================================================================
string getNextVaccinationRecordsID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(VaccinationID, 2) AS INTEGER)) FROM VaccinationRecords"; // Update table name to 'MedicalInsurance'
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
  return "V" + to_string(maxID);
}

void adminReadVaccinationRecords() {
  string query = "SELECT * FROM VaccinationRecords;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}
//==========================================================================================================

// input to add new data
void patientCreateVaccinationRecord(const string& UserID) {
    string VaccineType, DoseNumber, AdministrationDate;
    
    cout << "Enter Vaccine Type: ";
    getline(cin >> ws, VaccineType);

    cout << "Enter Dose Number: ";
    getline(cin >> ws, DoseNumber);

   do {
        cout << "Enter Administration Date (YYYY-MM-DD): ";
        getline(cin >> ws, AdministrationDate);
        if (!isValidDatePA(AdministrationDate)) {
                cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
        }
    } while (!isValidDatePA(AdministrationDate));


    // Generate a new VaccinationID
    string VaccinationID = getNextVaccinationRecordsID();

    // Create the SQL insert query
    string query = "INSERT INTO VaccinationRecords (VaccinationID, PatientID, VaccineType, DoseNumber, AdministrationDate) VALUES ('" +
                   VaccinationID + "', '" + UserID + "', '" + VaccineType + "', '" + DoseNumber + "', '" + AdministrationDate + "');";

    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        cerr << "Failed to add allergy: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Patient Vaccination Records added successfully!" << endl;
    }
}

void patientReadVaccinationRecord(const string& PatientID) {
    string query = "SELECT * FROM VaccinationRecords WHERE PatientID = '" + PatientID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read VaccinationRecords: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void adminUpdateVaccinationRecords(const set<string>& validPatientIDs) {
  string VaccinationID;

  cout << "Enter Vaccination ID to update: ";
  cin >> VaccinationID;

   if (VaccinationID.find("V") != 0) {
        // Prepend "AI" if it's not present
        VaccinationID = "V" + VaccinationID;
    }

  // Check if VaccinationID exists
  string checkSql =
      "SELECT COUNT(*) FROM VaccinationRecords WHERE VaccinationID = '" + VaccinationID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking VaccinationID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"PatientID", "VaccineType", "DoseNumber", "AdministrationDate"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

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
          cout << "Invalid UserID. Please enter a valid UserID." << endl;
        }
      }else if (field == "AdministrationDate" && !newValue.empty()) {
            // Validate DateOfPublication as a date in YYYY-MM-DD format
            regex datePattern(R"(\d{4}-\d{2}-\d{2})");

            if (regex_match(newValue, datePattern)) {
                validInput = true;
            } else {
                cout << "Invalid AdministrationDate value. Please enter the date in YYYY-MM-DD format." << endl;
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
      "UPDATE VaccinationRecords SET " + updates + " WHERE VaccinationID = '" + VaccinationID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!" << endl;
  }

  adminReadVaccinationRecords();
}

void adminDeleteVaccinationRecords() {
  string VaccinationID;

  cout << "Enter Patient Allergy ID to delete: ";
  cin >> VaccinationID;

  // Check if VaccinationID exists
  string checkSql =
      "SELECT COUNT(*) FROM VaccinationRecords WHERE VaccinationID = '" +
      VaccinationID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking VaccinationID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM VaccinationRecords WHERE VaccinationID = '" +
               VaccinationID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!" << endl;
  }
  adminReadVaccinationRecords();
}

void patientUpdateVaccinationRecords() {
  string VaccinationID, UserID;

  cout << "Enter Vaccination ID to update (blank to exit update process): ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, VaccinationID);

  if (VaccinationID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (VaccinationID.find("V") != 0) {
        VaccinationID = "V" + VaccinationID;
    }

  // Check if VaccinationID exists
  string checkSql =
      "SELECT COUNT(*) FROM VaccinationRecords WHERE VaccinationID = '" + VaccinationID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking VaccinationID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"VaccineType", "DoseNumber", "AdministrationDate"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

  for (const string &field : fields) {
    string newValue;
    bool validInput = false;

   do {
      cout << field << ": ";
      getline(cin, newValue);
    if (field == "AdministrationDate" && !newValue.empty()) {
            // Validate DateOfPublication as a date in YYYY-MM-DD format
            regex datePattern(R"(\d{4}-\d{2}-\d{2})");

            if (regex_match(newValue, datePattern)) {
                validInput = true;
            } else {
                cout << "Invalid AdministrationDate value. Please enter the date in YYYY-MM-DD format." << endl;
            }
        }else {
            validInput = true; // No specific validation for Title and Description
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
      "UPDATE VaccinationRecords SET " + updates + " WHERE VaccinationID = '" + VaccinationID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  patientReadVaccinationRecord(UserID);
}


void patientDeleteVaccinationRecords() {
  string VaccinationID, UserID;

  cout << "Enter Patient Allergy ID to delete (blank to exit delete process): ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, VaccinationID);

  if (VaccinationID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    if (VaccinationID.find("V") != 0) {
        VaccinationID = "V" + VaccinationID;
    }

  // Check if VaccinationID exists
  string checkSql =
      "SELECT COUNT(*) FROM VaccinationRecords WHERE VaccinationID = '" +
      VaccinationID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking VaccinationID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM VaccinationRecords WHERE VaccinationID = '" +
               VaccinationID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  patientReadVaccinationRecord(UserID);
}


#endif // ends this file conditional inclusion (must in the last line)