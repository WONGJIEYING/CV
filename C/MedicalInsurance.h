#ifndef MEDICALINSURANCE_H
#define MEDICALINSURANCE_H
#include <iostream>
#include <string>
#include <iomanip>
#include <set>
#include <cctype>
#include "globalCRUD.h"
#include "sqlite3.h"
#include "function.h"

using namespace std;

// structure
struct MedicalInsurance {
    string MedicalInsuranceID;
    string PatientID;
    string InsuranceProvider;
    string PolicyNumber;
    double Deductible;
    MedicalInsurance* next;
};

MedicalInsurance* MedicalInsurancehead = NULL;

//print table
void printMedicalInsurance(MedicalInsurance* n) {
    // Print table header
    cout << setw(5) << left << "Medical Insurance ID" << " | "
         << setw(5) << left << "Patient ID" << " | "
         << setw(5) << left << "Insurance Provider" << " | "
         << setw(5) << left << "Policy Number" << " | "
         << setw(5) << left << "Deductible" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->MedicalInsuranceID << " | "
             << setw(5) << left << n->PatientID << " | "
             << setw(5) << left << n->InsuranceProvider << " | "
             << setw(5) << left << n->PolicyNumber << " | "
             << setw(5) << left << n->Deductible << endl;
        n = n->next;
    }
    cout << endl;
}

// Push back function
void PushBackMedicalInsurance(
    string medicalInsuranceID,
    string patientID,
    string insuranceProvider,
    string policyNumber,
    double deductible) {
    MedicalInsurance* newMedicalInsurance = new MedicalInsurance();
    newMedicalInsurance->MedicalInsuranceID = medicalInsuranceID;
    newMedicalInsurance->PatientID = patientID;
    newMedicalInsurance->InsuranceProvider = insuranceProvider;
    newMedicalInsurance->PolicyNumber = policyNumber;
    newMedicalInsurance->Deductible = deductible;
    newMedicalInsurance->next = NULL;

    if (MedicalInsurancehead == NULL) {
        MedicalInsurancehead = newMedicalInsurance;
    } else {
        MedicalInsurance* temp = MedicalInsurancehead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newMedicalInsurance;
    }
}


// input to add new data
  void addNewMMedicalInsurance() {
  string MedicalInsuranceID, PatientID, InsuranceProvider, PolicyNumber;
  double Deductible;
  

  cout << "Enter Medical Insurance ID: ";
  cin >> MedicalInsuranceID;

  cout << "Enter Patient ID: ";
  cin >> PatientID;

  cout << "Enter Insurance Provider: ";
  cin >> InsuranceProvider;

  cout << "Enter Policy Number: ";
  cin >> PolicyNumber;

  cout << "Enter Deductible: ";
  cin >> Deductible;

  PushBackMedicalInsurance(MedicalInsuranceID, PatientID, InsuranceProvider, PolicyNumber, Deductible);
  cout << "Medical Diagnosis added successfully." << endl;
}

//database CRUD
//==========================================================================================================
//admin
// Function to generate the next MedicalInsuranceID


#include <cctype>
#include <string>

extern sqlite3* DB;  // Assuming the database connection is handled elsewhere

string getNextMedicalInsuranceID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(MedicalInsuranceID, 4) AS INTEGER)) FROM MedicalInsurance"; // Update table name to 'MedicalInsurance'
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

  return "INS" + to_string(maxID);
}

// Function to read all MedicalInsurances
void adminReadMedicalInsurance() {
  string query = "SELECT * FROM MedicalInsurance;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateMedicalInsurance(const set<string>& validPatientIDs) {
  string addMore;
  do {
    string PatientID, InsuranceProvider, PolicyNumber;
    int Deductible;

    cout << "\nAdd New Medical Insurance" << endl;
    cout << "***********************************" << endl;

    // Automatically generate MedicalInsurance ID
    string MedicalInsuranceID = getNextMedicalInsuranceID();

    bool validPatientID = false;
    do {
      cout << "Enter Patient ID (valid options are: ";
      for (const auto &id : validPatientIDs) {
        cout << id << " ";
      }
      cout << "): ";

      getline(cin >> ws, PatientID);

      // Check if the input is numeric and prefix with 'P' if true
      if (isdigit(PatientID[0])) {
        PatientID = "P" + PatientID;
      }

      PatientID = toUpperCase(PatientID);
          
      // Direct validation check
      validPatientID = validPatientIDs.find(PatientID) != validPatientIDs.end();

      if (!validPatientID) {
        cout << "Invalid Patient ID. Please enter a valid Patient ID from the list." << endl;
        cout << "Valid options are: ";
        for (const auto &id : validPatientIDs) {
          cout << id << " ";
        }
        cout << endl;
      }
    } while (!validPatientID);

    cout << "Enter Insurance Provider: ";
    getline(cin, InsuranceProvider);

    cout << "Enter Policy Number: ";
    getline(cin, PolicyNumber);

    cout << "Enter Deductible: ";
    cin >> Deductible;

    while (cin.fail()) {
        handleInvalidInput();
        cin >> Deductible;
    }

    // Insert the new MedicalInsurance into the database
string sql = "INSERT INTO MedicalInsurance (MedicalInsuranceID, PatientID, "
             "InsuranceProvider, PolicyNumber, Deductible) VALUES ('" +
             MedicalInsuranceID + "', '" + PatientID + "', '" +
             InsuranceProvider + "', '" + PolicyNumber + "', '" +
             to_string(Deductible) + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Medical Insurance added successfully!\n" << endl;
    }

    // Optionally read and print all MedicalInsurances to confirm the addition
    adminReadMedicalInsurance();

    cout << "Do you want to add another Medical Insurance? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update MedicalInsurance in the database
void adminUpdateMedicalInsurance(const set<string>& validPatientIDs) {
   string MedicalInsuranceID;

    cout << "Enter Medical Insurance ID to update (blank to exit update process): ";
    getline(cin, MedicalInsuranceID);
    //cin.ignore();

  if (MedicalInsuranceID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (MedicalInsuranceID.find("INS") != 0) {
        MedicalInsuranceID = "INS" + MedicalInsuranceID;
    }

  // Check if MedicalInsuranceID exists
  string checkSql =
      "SELECT COUNT(*) FROM MedicalInsurance WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking MedicalInsuranceID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"PatientID", "InsuranceProvider", "PolicyNumber", "Deductible"};
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
                    cout << "Invalid PatientID. Please enter a valid PatientID." << endl;
                }
            } else if (field == "Deductible" && !newValue.empty()) {
                // Validate Deductible as an integer
                try {
                    stoi(newValue);
                    validInput = true;
                } catch (exception& e) {
                    cout << "Invalid Deductible value. Please enter a valid integer." << endl;
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

  string sql = "UPDATE MedicalInsurance SET " + updates + " WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!" << endl;
  }

  adminReadMedicalInsurance();
}

// Function to delete MedicalInsurance from the database
void adminDeleteMedicalInsurance() {
  string MedicalInsuranceID;

  cout << "Enter MedicalInsurance ID to delete (blank to exit delete process): ";
  getline(cin, MedicalInsuranceID);
  //cin.ignore();

  if (MedicalInsuranceID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    if (MedicalInsuranceID.find("INS") != 0) {
        MedicalInsuranceID = "INS" + MedicalInsuranceID;
    }

  // Check if MedicalInsuranceID exists
  string checkSql =
      "SELECT COUNT(*) FROM MedicalInsurance WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking MedicalInsuranceID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM MedicalInsurance WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadMedicalInsurance();
}
//==========================================================================================================
//patient
void patientReadMedicalInsurance(const string& PatientID) {
    string query = "SELECT * FROM MedicalInsurance WHERE PatientID = '" + PatientID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read MedicalInsurance: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void patientCreateMedicalInsurance(const string& UserID) {
    string InsuranceProvider, PolicyNumber;
    int Deductible;
    
    cout << "Enter Insurance Provider: ";
    getline(cin >> ws, InsuranceProvider);

    cout << "Enter Policy Number: ";
    getline(cin >> ws, PolicyNumber);

    cout << "Enter Deductible: ";
    cin >> Deductible;
    
    while (cin.fail()) {
        handleInvalidInput();
        cin >> Deductible;
    }

    // Generate a new MedicalInsuranceID
    string MedicalInsuranceID = getNextMedicalInsuranceID();

    // Create the SQL insert query
    string query = "INSERT INTO MedicalInsurance (MedicalInsuranceID, PatientID, InsuranceProvider, PolicyNumber, Deductible) VALUES ('" +
                   MedicalInsuranceID + "', '" + UserID + "', '" + InsuranceProvider + "', '" + PolicyNumber + "', '" + to_string(Deductible) + "');";

    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        cerr << "Failed to add Medical Insurance: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Patient Medical Insurance added successfully!" << endl;
    }
}

// Function to update MedicalInsurance in the database
void patientUpdateMedicalInsurance() {
  string MedicalInsuranceID, UserID;

  cout << "Enter MedicalInsurance ID to update (blank to exit update process): ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, MedicalInsuranceID);

  if (MedicalInsuranceID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (MedicalInsuranceID.find("INS") != 0) {
        MedicalInsuranceID = "INS" + MedicalInsuranceID;
    }

  // Check if MedicalInsuranceID exists
  string checkSql = "SELECT COUNT(*) FROM MedicalInsurance WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking MedicalInsuranceID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"InsuranceProvider", "PolicyNumber", "Deductible"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

   for (const string &field : fields) {
        string newValue;
        bool validInput = false;

   do {
      cout << field << ": ";
      getline(cin, newValue);

      if (field == "Deductible" && !newValue.empty()) {
                try {
                    stof(newValue);
                    validInput = true;
                } catch (exception& e) {
                    cout << "Invalid Deductible value. Please enter a valid integer." << endl;
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

  string sql = "UPDATE MedicalInsurance SET " + updates + " WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  patientReadMedicalInsurance(UserID);
}

// Function to delete MedicalInsurance from the database
void patientDeleteMedicalInsurance() {
  string MedicalInsuranceID, UserID;

  cout << "Enter MedicalInsurance ID to delete (blank to exit delete process): ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, MedicalInsuranceID);

  if (MedicalInsuranceID.empty()) {
        cout << "Deletion process exited.\n";
        return;
    }

    if (MedicalInsuranceID.find("INS") != 0) {
        MedicalInsuranceID = "INS" + MedicalInsuranceID;
    }

  // Check if MedicalInsuranceID exists
  string checkSql =
      "SELECT COUNT(*) FROM MedicalInsurance WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking MedicalInsuranceID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM MedicalInsurance WHERE MedicalInsuranceID = '" + MedicalInsuranceID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  patientReadMedicalInsurance(UserID);
}

#endif // ends this file conditional inclusion (must in the last line)

