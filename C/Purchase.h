#ifndef PURCHASE_H // checks if this file has not been defined.
#define PURCHASE_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct Purchase {
    string POID;
    string SupplierID;
    int ItemID;
    double PricePerBox;
    int QuantityInBox;
    int PurchaseQuantityBox;
    double TotalAmount;
    Purchase* next;
};

Purchase *Purchasehead = NULL;

//print table
void printPurchase(Purchase* n) {
    // Print table header
    cout << setw(5) << left << "PO ID" << " | "
         << setw(5) << left << "Supplier ID" << " | "
         << setw(5) << left << "Item ID" << " | "
         << setw(5) << left << "Price Per Box" << " | "
         << setw(5) << left << "Quantity In A Box" << " | "
         << setw(5) << left << "Purchase Quantity In Box" << " | "
         << setw(5) << left << "Total Amount" << endl;
    //print data
    while (n != NULL) {
        cout << setw(5) << left << n->POID << " | "
             << setw(5) << left << n->SupplierID << " | "
             << setw(5) << left << n->ItemID << " | "
             << setw(5) << left << "RM" << fixed << setprecision(2) << n->PricePerBox << " | "//fixed in 2 digit
             << setw(5) << left << n->QuantityInBox << " | "
             << setw(5) << left << n->PurchaseQuantityBox << " | "
             << setw(5) << left << "RM" << fixed << setprecision(2) << n->TotalAmount << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackPurchase(
    string pOID,
    string supplierID,
    int itemID,
    double pricePerBox,
    int quantityInBox,
    int PurchaseQuantityBox,
    double totalAmount) {
    Purchase* newPurchase = new Purchase();
    newPurchase->POID = pOID;
    newPurchase->SupplierID = supplierID;
    newPurchase->ItemID = itemID;
    newPurchase->PricePerBox = pricePerBox;
    newPurchase->QuantityInBox = quantityInBox;
    newPurchase->PurchaseQuantityBox = PurchaseQuantityBox;
    newPurchase->TotalAmount = totalAmount;
    newPurchase->next = NULL;

  if (Purchasehead == NULL) {
    Purchasehead = newPurchase;
  } else {
    Purchase *temp = Purchasehead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newPurchase;
  }
}

// input to add new data
void addNewPurchase() {
    string POID, SupplierID;
    double PricePerBox, TotalAmount;
    int ItemID, QuantityInBox, PurchaseQuantityBox;
    cout << "Enter PO ID: ";
    cin >> POID;
    cout << "Enter Supplier ID: ";
    cin >> SupplierID;
    cout << "Enter Item ID: ";
    cin >> ItemID;
    cout << "Enter Price Per Box (RM): ";
    cin >> PricePerBox;
    cout << "Enter Quantity in Box: ";
    cin >> QuantityInBox;
    cout << "Enter Purchase Quantity in Box: ";
    cin >> PurchaseQuantityBox;

    // calculation for total amount
    TotalAmount = PricePerBox * PurchaseQuantityBox;
    
    PushBackPurchase(POID, SupplierID, ItemID, PricePerBox, QuantityInBox, PurchaseQuantityBox, TotalAmount);
    cout << "Purchase order added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to generate the next POID
string getNextPOID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(POID, 3) AS INTEGER)) FROM Purchase"; // Update
                                                                    // table
                                                                    // name to
                                                                    // 'Purchase'
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
  return "PO" + to_string(maxID);
}

// Function to read all Purchases
void adminReadPurchase() {
  string query = "SELECT * FROM Purchase;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreatePurchase(const set<string>& validSupplierIDs, const set<string>& validItemIDs) {
    string addMore;
    do {
        string SupplierID, ItemID;
        int QuantityInBox, PurchaseQuantityBox;
        double PricePerBox, TotalAmount;

      // Check if SupplierID exists
      string checkSql = "SELECT COUNT(*) FROM Supplier WHERE SupplierID = '" +
                        SupplierID + "';";
      sqlite3_stmt *stmt;
      int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

        // Automatically generate Purchase ID
        string POID = getNextPOID();

        bool validSupplierID = false;
    do {
      cout << "Enter Supplier ID (valid options are: ";
      for (const auto &id : validSupplierIDs) {
        cout << id << " ";
      }
      cout << "): ";

      getline(cin >> ws, SupplierID);

      if (isdigit(SupplierID[0])) {
        SupplierID = "SU" + SupplierID;
      }

      SupplierID = toUpperCase(SupplierID);
          
      // Direct validation check
      validSupplierID = validSupplierIDs.find(SupplierID) != validSupplierIDs.end();

      if (!validSupplierID) {
        cout << "Invalid Supplier ID. Please enter a valid Supplier ID from the list." << endl;
        cout << "Valid options are: ";
        for (const auto &id : validSupplierIDs) {
          cout << id << " ";
        }
        cout << endl;
      }
    } while (!validSupplierID);

    bool validItemID = false;
    do {
      cout << "Enter Item ID (valid options are: ";
      for (const auto &id : validItemIDs) {
        cout << id << " ";
      }
      cout << "): ";

      getline(cin >> ws, ItemID);

      if (isdigit(ItemID[0])) {
        ItemID = "I" + ItemID;
      }

      ItemID = toUpperCase(ItemID);
          
      // Direct validation check
      validItemID = validItemIDs.find(ItemID) != validItemIDs.end();

      if (!validItemID) {
        cout << "Invalid Item ID. Please enter a valid Item ID from the list." << endl;
        cout << "Valid options are: ";
        for (const auto &id : validItemIDs) {
          cout << id << " ";
        }
        cout << endl;
      }
    } while (!validItemID);

        cout << "Enter Price per Box: ";
        cin >> PricePerBox;

        while (cin.fail()) {
        handleInvalidInput();
        cin >> PricePerBox;
    }

        cout << "Enter Quantity in Box: ";
        cin >> QuantityInBox;

        while (cin.fail()) {
        handleInvalidInput();
        cin >> QuantityInBox;
        }

        cout << "Enter Purchase Quantity in Box: ";
        cin >> PurchaseQuantityBox;

        while (cin.fail()) {
        handleInvalidInput();
        cin >> PurchaseQuantityBox;
    }

        // Calculate Total Amount
        TotalAmount = PricePerBox * PurchaseQuantityBox;

        // Insert the new Purchase into the database
        string sql =  "INSERT INTO Purchase (POID, SupplierID, ItemID, PricePerBox, QuantityInBox, PurchaseQuantityBox, TotalAmount) VALUES ('" +
            POID + "', '" + SupplierID + "', '" + ItemID + "', " + to_string(PricePerBox) + ", " + to_string(QuantityInBox) + ", " + to_string(PurchaseQuantityBox) + ", " + to_string(TotalAmount) + ");";

        char *messageError;
        int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Error inserting data: " << messageError << endl;
            sqlite3_free(messageError);
        } else {
            cout << "Purchase added successfully!\n" << endl;
        }

        // Optionally read and print all Purchases to confirm the addition
        adminReadPurchase();

        cout << "Do you want to add another Purchase? (Y/N): ";
        getline(cin >> ws, addMore);

    } while (addMore == "Y" || addMore == "y");
}

// Function to update Purchase in the database
void adminUpdatePurchase(const set<string>& validSupplierIDs, const set<string>& validItemIDs) {
    string POID;

    cout << "Enter Purchase ID to update (blank to exit update process): ";
    getline(cin, POID);

    if (POID.empty()) {
        cout << "Update process exited.\n";
        return;
    }

    if (POID.find("PO") != 0) {
        POID = "PO" + POID;
    }

    // Check if POID exists
    string checkSql = "SELECT COUNT(*) FROM Purchase WHERE POID = '" + POID + "';";
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cerr << "Error checking POID: " << sqlite3_errmsg(DB) << endl;
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

    string fields[] = {"SupplierID", "ItemID", "PricePerBox", "QuantityInBox", "PurchaseQuantityBox"};
    string updates = "";

    cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;
    
    bool fieldsUpdated = false;
    double pricePerBox = 0.0;
    int purchaseQuantityBox = 0;

    for (const string &field : fields) {
        string newValue;
        bool validInput = false;

        do {
            cout << field << ": ";
            getline(cin, newValue);

            if (field == "SupplierID" && !newValue.empty()) {
                if (isdigit(newValue[0])) {
                    newValue = "SU" + newValue;
                }
                newValue = toUpperCase(newValue);

                if (validSupplierIDs.find(newValue) != validSupplierIDs.end()) {
                    validInput = true;
                } else {
                    cout << "Invalid SupplierID. Please enter a valid SupplierID." << endl;
                }
            } else if (field == "ItemID" && !newValue.empty()) {
                if (isdigit(newValue[0])) {
                    newValue = "I" + newValue;
                }
                newValue = toUpperCase(newValue);

                if (validItemIDs.find(newValue) != validItemIDs.end()) {
                    validInput = true;
                } else {
                    cout << "Invalid ItemID. Please enter a valid ItemID." << endl;
                }
            } else if (field == "PricePerBox" && !newValue.empty()) {
                try {
                    pricePerBox = stod(newValue);  // Convert to double
                    validInput = true;
                } catch (exception& e) {
                    cout << "Invalid Price per Box value. Please enter a valid number." << endl;
                }
            } else if (field == "QuantityInBox" && !newValue.empty()) {
                try {
                    int quantityInBox = stoi(newValue);  // Convert to int
                    updates += "QuantityInBox = " + to_string(quantityInBox) + ", ";
                    validInput = true;
                } catch (exception& e) {
                    cout << "Invalid Quantity in Box value. Please enter a valid integer." << endl;
                }
            } else if (field == "PurchaseQuantityBox" && !newValue.empty()) {
                try {
                    purchaseQuantityBox = stoi(newValue);  // Convert to int
                    validInput = true;
                } catch (exception& e) {
                    cout << "Invalid Purchase Quantity in Box value. Please enter a valid integer." << endl;
                }
            } else {
                validInput = true; // Skip empty fields without validation
            }
        } while (!validInput);

        if (!newValue.empty() && field != "PricePerBox" && field != "PurchaseQuantityBox") {
            if (!updates.empty()) {
                updates += ", ";
            }

            updates += field + " = '" + newValue + "'";
            fieldsUpdated = true;
        }
    }

    // Calculate TotalAmount manually in C++
    double totalAmount = pricePerBox * purchaseQuantityBox;
    cout << "Calculated TotalAmount: " << totalAmount << endl;

    if (fieldsUpdated || pricePerBox != 0.0 || purchaseQuantityBox != 0) {
        if (!updates.empty()) {
            updates += ", ";
        }
        updates += "PricePerBox = " + to_string(pricePerBox) + ", PurchaseQuantityBox = " + to_string(purchaseQuantityBox) + ", TotalAmount = " + to_string(totalAmount);
    }

    if (updates.empty()) {
        cout << "No fields were updated." << endl;
        return;
    }

    string sql = "UPDATE Purchase SET " + updates + " WHERE POID = '" + POID + "';";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Update: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Data updated successfully!\n" << endl;
    }

    // Call the function to read and display the updated purchase (assuming you have this function)
    adminReadPurchase();
}

// Function to delete Purchase from the database
void adminDeletePurchase() {
  string POID;

  cout << "Enter Purchase ID to delete (blank to exit delete process): ";
  getline(cin, POID);

  if (POID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    if (POID.find("PO") != 0) {
        POID = "PO" + POID;
    }

  // Check if POID exists
  string checkSql =
      "SELECT COUNT(*) FROM Purchase WHERE POID = '" + POID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking POID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Purchase WHERE POID = '" + POID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadPurchase();
}
#endif // ends this file conditional inclusion (must in the last line)