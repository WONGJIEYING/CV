#ifndef EMERGENCYTOOLS_H // checks if this file has not been defined.
#define EMERGENCYTOOLS_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Structure
struct EmergencyTools {
  string EmergencyToolsID;
  string Type;
  string Tool;
  string EmergencyNumber;
  EmergencyTools *next;
};

EmergencyTools *EmergencyToolsHead = NULL;

// print table
void printEmergencyTools(EmergencyTools *n) {
  // Print table header
  cout << setw(5) << left << "Emergency Tools ID"
       << " | " << setw(5) << left << "Type"
       << " | " << setw(5) << left << "Tool"
       << " | " << setw(5) << left << "Emergency Number" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->EmergencyToolsID << " | " 
         << setw(5) << left << n->Type << " | " 
         << setw(5) << left << n->Tool << " | " 
         << setw(5) << left << n->EmergencyNumber << endl;
    n = n->next;
  }
  cout << endl;
}

// Push back function
void PushBackEmergencyTools(
  string emergencyToolsID, 
  string type, 
  string tool,
  string emergencyNumber) {
  EmergencyTools *newEmergencyTools = new EmergencyTools();
  newEmergencyTools->EmergencyToolsID = emergencyToolsID;
  newEmergencyTools->Type = type;
  newEmergencyTools->Tool = tool;
  newEmergencyTools->EmergencyNumber = emergencyNumber;
  newEmergencyTools->next = NULL;

  if (EmergencyToolsHead == NULL) {
    EmergencyToolsHead = newEmergencyTools;
  } else {
    EmergencyTools *temp = EmergencyToolsHead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newEmergencyTools;
  }
}

// input to add new data
void addnewEmergencyTools() {
  string EmergencyToolsID, Type, Tool, EmergencyNumbere;
  cout << "Enter Feedback: ";
  cin >> EmergencyToolsID;
  cout << "Enter Patient ID: ";
  cin >> Type;
  cout << "Enter Comment: ";
  cin >> Tool;
  cout << "Enter Emergency Number: ";
  cin >> EmergencyNumbere;

  PushBackEmergencyTools(EmergencyToolsID, Type, Tool, EmergencyNumbere);
  cout << "Feedback added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to validate Contact Number (must be at least 10 digits)
bool validateEmergencyNumber(const string& ContactNumber) {
    return ContactNumber.length() >= 3 && all_of(ContactNumber.begin(), ContactNumber.end(), ::isdigit);
}

// Function to generate the next EmergencyToolsID
string getNextEmergencyToolsID() {
  sqlite3_stmt *stmt;
  string sql = "SELECT MAX(CAST(SUBSTR(EmergencyToolsID, 3) AS INTEGER)) FROM "
               "EmergencyTools"; // Update table name to 'EmergencyTools'
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

  // Return the new ID with the "N" prefix
  return "ET" + to_string(maxID);
}

// Function to read all EmergencyToolss
void adminReadEmergencyTools() {
  string query = "SELECT * FROM EmergencyTools;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateEmergencyTools() {
  string addMore;
  do {
    string Type, Tool, EmergencyNumber;

    cout << "\nAdd New Emergency Tools" << endl;
    cout << "***********************************" << endl;

    // Automatically generate EmergencyTools ID
    string EmergencyToolsID = getNextEmergencyToolsID();

    cout << "Enter Type: ";
    getline(cin >> ws, Type);

    cout << "Enter Tool: ";
    getline(cin >> ws, Tool);

    do {
      cout << "Enter Emergency Number: ";
      getline(cin >> ws, EmergencyNumber);

      // Validate the Contact Number
      if (!validateEmergencyNumber(EmergencyNumber)) {
        cout << "Invalid Contact Number. Please enter a number with at least 3 digits." << endl;
      }
    } while (!validateEmergencyNumber(EmergencyNumber));

    // Insert the new EmergencyTools into the database
    string sql = "INSERT INTO EmergencyTools (EmergencyToolsID, Type,Tool,EmergencyNumber) VALUES ('" +
                 EmergencyToolsID + "', '" + Type + "', '" + Tool + "','" + EmergencyNumber + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "EmergencyTools added successfully!" << endl;
    }

    // Optionally read and print all EmergencyToolss to confirm the addition
    adminReadEmergencyTools();

    cout << "Do you want to add another Emergency Tools? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update EmergencyTools in the database
void adminUpdateEmergencyTools() {
  string EmergencyToolsID;

  cout << "Enter Emergency Tools ID to update (blank to exit update process): ";
  getline(cin, EmergencyToolsID);

    if (EmergencyToolsID.empty()) {
        cout << "Update process exited.";
        return;
    }

    if (EmergencyToolsID.find("ET") != 0) {
        EmergencyToolsID = "ET" + EmergencyToolsID;
    }

  // Check if EmergencyToolsID exists
  string checkSql = "SELECT COUNT(*) FROM EmergencyTools WHERE EmergencyToolsID = '" + EmergencyToolsID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking EmergencyToolsID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"Type", "Tool", "EmergencyNumber"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field."
       << endl;
       
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

  string sql = "UPDATE EmergencyTools SET " + updates + " WHERE EmergencyToolsID = '" + EmergencyToolsID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadEmergencyTools();
}

// Function to delete EmergencyTools from the database
void adminDeleteEmergencyTools() {
  string EmergencyToolsID;

  cout << "Enter Emergency Tools ID to delete (blank to exit delete process): ";
  getline(cin, EmergencyToolsID);

  if (EmergencyToolsID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    // Prepend "I" if it's not present
    if (EmergencyToolsID.find("ET") != 0) {
        EmergencyToolsID = "ET" + EmergencyToolsID;
    }

  // Check if EmergencyToolsID exists
  string checkSql =
      "SELECT COUNT(*) FROM EmergencyTools WHERE EmergencyToolsID = '" +
      EmergencyToolsID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking EmergencyToolsID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM EmergencyTools WHERE EmergencyToolsID = '" +
               EmergencyToolsID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadEmergencyTools();
}
#endif // ends this file conditional inclusion (must in the last line)