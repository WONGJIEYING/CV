#ifndef _08_2SUPPLIER_H // checks if this file has not been defined.
#define _08_2SUPPLIER_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"

using namespace std;

// structure
struct Supplier {
    string SupplierID;
    string SupplierName;
    string SupplierContactNumber;
    Supplier* next;
};
Supplier* Supplierhead = NULL;

//print table
void printSupplier(Supplier* n) {
    // Print table header
    cout << setw(5) << left << "Supplier ID" << " | "
         << setw(5) << left << "Supplier Name" << " | "
         << setw(5) << left << "Supplier Contact Number" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->SupplierID << " | "
             << setw(5) << left << n->SupplierName << " | "
             << setw(5) << left << n->SupplierContactNumber << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackSupplier(
    string supplierID,
    string supplierName,
    string supplierContactNumber) {
    Supplier* newSupplier = new Supplier();
    newSupplier->SupplierID = supplierID;
    newSupplier->SupplierName = supplierName;
    newSupplier->SupplierContactNumber = supplierContactNumber;
    newSupplier->next = NULL;

    if (Supplierhead == NULL) {
        Supplierhead = newSupplier;
    } else {
        Supplier* temp = Supplierhead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newSupplier;
    }
}

// input to add new data
void addNewSupplier() {
    string SupplierID, SupplierName, SupplierContactNumber;
    cout << "Enter Supplier ID: ";
    cin >> SupplierID;
    cout << "Enter Supplier Name: ";
    cin >> SupplierName;
    cout << "Enter Supplier Contact Number: ";
    cin >> SupplierContactNumber;

    PushBackSupplier(SupplierID, SupplierName, SupplierContactNumber);
    cout << "Supplier added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to validate Contact Number (must be at least 10 digits)
bool validateSupplierContactNumber(const string& ContactNumber) {
    return ContactNumber.length() >= 10 && all_of(ContactNumber.begin(), ContactNumber.end(), ::isdigit);
}

// Function to generate the next SupplierID
string getNextSupplierID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(SupplierID, 3) AS INTEGER)) FROM Supplier"; // Update table name to 'Supplier'
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
  return "SU" + to_string(maxID);
}

// Function to read all Suppliers
void adminReadSupplier() {
  string query = "SELECT * FROM Supplier;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateSupplier() {
  string addMore;
  do {
    string SupplierName, SupplierContactNumber;

    cout << "\nAdd New Supplier" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Supplier ID
    string SupplierID = getNextSupplierID();

    cout << "Enter Supplier Name: ";
    getline(cin >> ws, SupplierName);

   do {
      cout << "Enter Contact Number: ";
      getline(cin >> ws, SupplierContactNumber);

      // Validate the Contact Number
      if (!validateSupplierContactNumber(SupplierContactNumber)) {
        cout << "Invalid Contact Number. Please enter a number with at least 10 digits." << endl;
      }
    } while (!validateSupplierContactNumber(SupplierContactNumber));

    // Insert the new Supplier into the linked list
    //PushBackSupplier(SupplierID, SupplierName, SupplierContactNumber);

    // Insert the new Supplier into the database
    string sql =
        "INSERT INTO Supplier (SupplierID, SupplierName, ContactNumber) VALUES ('" +
        SupplierID + "', '" + SupplierName + "', '" + SupplierContactNumber + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Supplier added successfully!\n" << endl;
    }

    // Optionally read and print all Suppliers to confirm the addition
    adminReadSupplier();

    cout << "Do you want to add another Supplier? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Supplier in the database
void adminUpdateSupplier() {
  string SupplierID;

  cout << "Enter Supplier ID to update (blank to exit update process): ";
  getline(cin, SupplierID);

  if (SupplierID.empty()) {
        cout << "Update process exited." << endl;
        return;
    }

    if (SupplierID.find("SU") != 0) {
        SupplierID = "SU" + SupplierID;
    }

  // Check if SupplierID exists
  string checkSql =
      "SELECT COUNT(*) FROM Supplier WHERE SupplierID = '" + SupplierID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking SupplierID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"SupplierName", "ContactNumber"};
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

  string sql =
      "UPDATE Supplier SET " + updates + " WHERE SupplierID = '" + SupplierID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadSupplier();
}

// Function to delete Supplier from the database
void adminDeleteSupplier() {
  string SupplierID;

  cout << "Enter Supplier ID to delete (blank to exit delete process): ";
  getline(cin, SupplierID);

  if (SupplierID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    if (SupplierID.find("SU") != 0) {
        SupplierID = "SU" + SupplierID;
    }

  // Check if SupplierID exists
  string checkSql = "SELECT COUNT(*) FROM Supplier WHERE SupplierID = '" + SupplierID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking SupplierID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Supplier WHERE SupplierID = '" + SupplierID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadSupplier();
}
#endif // ends this file conditional inclusion (must in the last line)


