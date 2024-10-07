#ifndef TYPEOFSERVICE_H // checks if this file has not been defined.
#define TYPEOFSERVICE_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Structure
struct Service {
    string ServiceID;
    string TypeOfService;
    string Description;
    Service* next;
};

Service* ServiceHead = NULL;

// print table
void printService(Service *n) {
  // Print table header
  cout << setw(5) << left << "Service ID"
       << " | " << setw(5) << left << "Type Of Service"
       << " | " << setw(5) << left << "Description" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->ServiceID << " | " 
         << setw(5) << left << n->TypeOfService << " | " 
         << setw(5) << left << n->Description << endl;
    n = n->next;
  }
  cout << endl;
}

// Push back function
void PushBackService(
    string serviceID,
    string typeOfService,
    string description) {
    Service* newService = new Service();
    newService->ServiceID = serviceID;
    newService->TypeOfService = typeOfService;
    newService->Description = description;
    newService->next = NULL;

    if (ServiceHead == NULL) {
        ServiceHead = newService;
    } else {
        Service* temp = ServiceHead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newService;
    }
}

// input to add new data
void addNewService() {
    string ServiceID, TypeOfService, Description;
    cout << "Enter Service ID: ";
    cin >> ServiceID;
    cout << "Enter TypeOfService: ";
    cin >> TypeOfService;
    cout << "Enter Description: ";
    cin >> Description;

    PushBackService(ServiceID, TypeOfService, Description);
    cout << "Patient Allergy added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to generate the next ServiceID
string getNextServiceID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(ServiceID, 2) AS INTEGER)) FROM TypeOfService"; // Update table name to 'Service'
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
  return "S" + to_string(maxID);
}

// Function to read all Services
void adminReadService() {
  string query = "SELECT * FROM TypeOfService;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateService() {
  string addMore;
  do {
    string TypeofService, Description;

    cout << "\nAdd New Service" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Service ID
    string ServiceID = getNextServiceID();

    cout << "Enter Type of Service: ";
    getline(cin >> ws, TypeofService);

    cout << "Enter Description: ";
    getline(cin >> ws, Description);

    // Insert the new Service into the linked list
    PushBackService(ServiceID, TypeofService, Description);

    // Insert the new Service into the database
    string sql =
        "INSERT INTO TypeOfService (ServiceID, TypeofService, Description) VALUES ('" +
        ServiceID + "', '" + TypeofService + "', '" + Description + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Service added successfully!\n" << endl;
    }

    // Optionally read and print all Services to confirm the addition
    adminReadService();

    cout << "Do you want to add another Service? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Service in the database
void adminUpdateService() {
  string ServiceID;

  cout << "Enter Service ID to update (blank to exit update process): ";
  getline(cin, ServiceID);

    // Check if input is empty
    if (ServiceID.empty()) {
        cout << "Update process exited.";
        return;
    }

    // Ensure ServiceID starts with "S"
    if (ServiceID.find("S") != 0) {
        ServiceID = "S" + ServiceID;
    }

  // Check if ServiceID exists
  string checkSql =
      "SELECT COUNT(*) FROM TypeOfService WHERE ServiceID = '" + ServiceID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking ServiceID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"TypeofService", "Description"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip "
          "updating the field."
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

  string sql =
      "UPDATE TypeOfService SET " + updates + " WHERE ServiceID = '" + ServiceID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadService();
}

// Function to delete Service from the database
void adminDeleteService() {
  string ServiceID;

  cout << "Enter Service ID to delete (blank to exit delete process): ";
  getline(cin, ServiceID);

  if (ServiceID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    // Prepend "S" if it's not present
    if (ServiceID.find("S") != 0) {
        ServiceID = "S" + ServiceID;
    }

  // Check if ServiceID exists
  string checkSql =
      "SELECT COUNT(*) FROM TypeOfService WHERE ServiceID = '" + ServiceID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking ServiceID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM TypeOfService WHERE ServiceID = '" + ServiceID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadService();
}

#endif // ends this file conditional inclusion (must be the last line)