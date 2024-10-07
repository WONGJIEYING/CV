#ifndef INVENTORY_H // checks if this file has not been defined.
#define INVENTORY_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include "function.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct Inventory {
  int ItemID;
  string PurchaseItem;
  int Quantity;
  string PersonChecking;
  Inventory *next;
};
Inventory *Inventoryhead = NULL;

// print table
void printInventory(Inventory *n) {
  // Print table header
  cout << setw(5) << left << "Item ID"
       << " | " << setw(5) << left << "PurchaseItem"
       << " | " << setw(5) << left << "Quantity"
       << " | " << setw(5) << left << "Person Checking" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->ItemID << " | " 
         << setw(5) << left << n->PurchaseItem << " | " 
         << setw(5) << left << n->Quantity << " | "
         << setw(5) << left << n->PersonChecking << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackInventory(
  int itemID, 
  string purchaseItem, 
  int quantity,
  string personChecking) {
  Inventory *newInventory = new Inventory();
  newInventory->ItemID = itemID;
  newInventory->PurchaseItem = purchaseItem;
  newInventory->Quantity = quantity;
  newInventory->PersonChecking = personChecking;
  newInventory->next = NULL;

  if (Inventoryhead == NULL) {
    Inventoryhead = newInventory;
  } else {
    Inventory *temp = Inventoryhead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newInventory;
  }
}

// input to add new data
void addNewInventory() {
  string PurchaseItem, PersonChecking;
  int ItemID, Quantity;
  cout << "Enter Item ID: ";
  cin >> ItemID;
  cout << "Enter Purchase Item: ";
  cin >> PurchaseItem;
  cout << "Enter Quantity: ";
  cin >> Quantity;
  cout << "Enter Person Checking: ";
  cin >> PersonChecking;

  PushBackInventory(ItemID, PurchaseItem, Quantity, PersonChecking);
  cout << "Inventory added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to read all Inventorys

string getNextInventoryID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(ItemID, 2) AS INTEGER)) FROM Inventory"; // Update table name to 'Inventory'
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

  return "I" + to_string(maxID);
}

void adminReadInventory() {
  string query = "SELECT * FROM Inventory;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateInventory(const set<string>& validUserIDs) {
  string addMore;
  do {
    string PurchaseItem, PersonChecking;
    int Quantity;

    cout << "\nAdd New Inventory" << endl;
    cout << "***********************************" << endl;

    string ItemID = getNextInventoryID();

    cout << "Enter Purchase Item: ";
    getline(cin >> ws, PurchaseItem);

    cout << "Enter Quantity: ";
    cin >> Quantity;

    while (cin.fail()) {
        handleInvalidInput();
        cin >> Quantity;
    }

    bool validUserID = false;
        do {
          cout << "Enter User ID ( valid options are: ";
          for (const auto &id : validUserIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, PersonChecking);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(PersonChecking) && !PersonChecking.empty()) {
            PersonChecking = "E" + PersonChecking;
          }

          PersonChecking = toUpperCase(PersonChecking);

          // Direct validation check
          validUserID = validUserIDs.find(PersonChecking) != validUserIDs.end();

          if (!validUserID) {
            cout << "Invalid User ID. Please enter a valid User ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validUserIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validUserID);

    // Insert the new Inventory into the database
    string sql = "INSERT INTO Inventory (ItemID, PurchaseItem, Quantity, PersonChecking) VALUES ('" +
                     ItemID + "', '" + PurchaseItem + "', '" + to_string(Quantity) + "', '" +
                     PersonChecking + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Inventory added successfully!\n" << endl;
    }

    // Optionally read and print all Inventorys to confirm the addition
    adminReadInventory();

    cout << "Do you want to add another Inventory? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Inventory in the database
void adminUpdateInventory(const set<string>& validUserIDs) {
  string ItemID;

  cout << "Enter Inventory ID to update (blank to exit update process): ";
  getline(cin, ItemID);

    if (ItemID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (ItemID.find("I") != 0) {
        ItemID = "I" + ItemID;
    }

  // Check if ItemID exists
  string checkSql =
      "SELECT COUNT(*) FROM Inventory WHERE ItemID = '" + ItemID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking ItemID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"PurchaseItem", "Quantity", "PersonChecking"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field."
       << endl;

  for (const string &field : fields) {
    string newValue;
    bool validInput = false;

   do {
      cout << field << ": ";
      getline(cin, newValue);

      if (field == "Quantity" && !newValue.empty()) {
                try {
                    stof(newValue);
                    validInput = true;
                } catch (exception& e) {
                    cout << "Invalid Quantity value. Please enter a valid integer." << endl;
                }
            } else if (field == "UserID" && !newValue.empty()) {
        if (isdigit(newValue[0])) {
          newValue = "E" + newValue;
        }
        newValue = toUpperCase(newValue);

        if (validUserIDs.find(newValue) != validUserIDs.end()) {
          validInput = true;
        } else {
          cout << "Invalid UserID. Please enter a valid UserID." << endl;
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

  string sql = "UPDATE Inventory SET " + updates + " WHERE ItemID = '" + ItemID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadInventory();
}

// Function to delete Inventory from the database
void adminDeleteInventory() {
  string ItemID;

  cout << "Enter Inventory ID to delete (blank to exit delete process): ";
  getline(cin, ItemID);

  if (ItemID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    // Prepend "I" if it's not present
    if (ItemID.find("I") != 0) {
        ItemID = "I" + ItemID;
    }

  // Check if ItemID exists
  string checkSql = "SELECT COUNT(*) FROM Inventory WHERE ItemID = '" + ItemID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking ItemID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Inventory WHERE ItemID = '" + ItemID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadInventory();
}
#endif // ends this file conditional inclusion (must in the last line)