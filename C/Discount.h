#ifndef _11DISCOUNT_H // checks if this file has not been defined.
#define _11DISCOUNT_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"
#include "function.h"

using namespace std;

// structure
struct Discount {
    string DiscountID;
    string DiscountAmount;
    string IssuedDate;
    Discount* next;
};

Discount* Discounthead = NULL;

//print table
void printDiscount(Discount* n) {
    // Print table header
    cout << setw(5) << left << "Discount ID" << " | "
         << setw(5) << left << "Discount Amount" << " | "
         << setw(5) << left << "Issued Date" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->DiscountID << " | "
             << setw(5) << left << n->DiscountAmount << " | "
             << setw(5) << left << n->IssuedDate << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackDiscount(
   string DiscountID,
    string discountAmount,
    string issuedDate) {
    Discount* newDiscount = new Discount();
    newDiscount->DiscountID = DiscountID;
    newDiscount->DiscountAmount = discountAmount;
    newDiscount->IssuedDate = issuedDate;
    newDiscount->next = NULL;

    if (Discounthead == NULL) {
        Discounthead = newDiscount;
    } else {
        Discount* temp = Discounthead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newDiscount;
    }
}

// input to add new data
void addNewDiscount() {
    string DiscountID, DiscountAmount, IssuedDate;
    cout << "Enter Discount ID: ";
    cin >> DiscountID;
    cout << "Enter Discount Amount: ";
    cin >> DiscountAmount;
    cout << "Enter Issued Date: ";
    cin >> IssuedDate;

    PushBackDiscount(DiscountID, DiscountAmount, IssuedDate);
    cout << "Discount added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to check if a string is a valid date in YYYY-MM-DD format
bool isValidDateDiscount(const string &date) {
  if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
    return false;
  }
  for (int i = 0; i < date.size(); ++i) {
    if (i == 4 || i == 7)
      continue;
    if (!isdigit(date[i]))
      return false;
  }
  int year = stoi(date.substr(0, 4));
  int month = stoi(date.substr(5, 2));
  int day = stoi(date.substr(8, 2));
  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;
  if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    return false;
  if (month == 2) {
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > 29 || (day == 29 && !isLeapYear))
      return false;
  }
  return true;
}

// Function to generate the next DiscountID
string getNextDiscountID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(DiscountID, 2) AS INTEGER)) FROM Discount"; // Update table name to 'Discount'
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
  return "V" + to_string(maxID);
}

// Function to read all Discounts
void adminReadDiscount() {
  string query = "SELECT * FROM Discount;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateDiscount() {
  string addMore;
  do {
    string IssuedDate;
    double DiscountAmount;

    cout << "\nAdd New Discount" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Discount ID
    string DiscountID = getNextDiscountID();

    do {
        cout << "Enter Discount Amount (1-100 %): ";
        cin >> DiscountAmount;

        // Check if the input is a valid integer and within the range 0-5
        if (cin.fail() || DiscountAmount < 1.0 || DiscountAmount > 100.0) {
            handleInvalidInput();
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore any extra input
            break; // Input is valid, exit the loop
        }
    } while (true);

    do {
        cout << "Enter Issued Date (YYYY-MM-DD): ";
        getline(cin, IssuedDate);
        if (!isValidDateDiscount(IssuedDate)) {
        cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
    }
    }     while (!isValidDateDiscount(IssuedDate));

    // Insert the new Discount into the database
    string sql =
        "INSERT INTO Discount (DiscountID, DiscountAmount, IssuedDate) VALUES ('" +
        DiscountID + "', '" + to_string(DiscountAmount) + "%', '" + IssuedDate + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Discount added successfully!" << endl;
      cout << endl;
    }

    // Optionally read and print all Discounts to confirm the addition
    adminReadDiscount();

    cout << "Do you want to add another Discount? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Discount in the database
void adminUpdateDiscount() {
  string DiscountID;

    cout << "Enter Discount ID to update (blank to exit update process): ";
    getline(cin, DiscountID);

    if (DiscountID.empty()) {
        cout << "Update process exited.";
        return;
    }

    if (DiscountID.find("V") != 0) {
        DiscountID = "V" + DiscountID;
    }

    // Check if DiscountID exists
    string checkSql =
        "SELECT COUNT(*) FROM Discount WHERE DiscountID = '" + DiscountID + "';";
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cerr << "Error checking DiscountID: " << sqlite3_errmsg(DB) << endl;
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

    string fields[] = {"DiscountAmount", "IssuedDate"};
    string updates = "";

    cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

    cin.ignore(); // To clear the newline character from the input buffer
    for (const string &field : fields) {
        string newValue;
        bool validInput = false;

        do {
        cout << field << ": ";
        getline(cin, newValue);

        if (field == "DiscountAmount" && !newValue.empty()) {
            // Ensure that the percentage sign is included
            if (newValue.find('%') == string::npos) {
                newValue += '%';
            }
            try {
              // Extract the numeric part of the DiscountAmount (before the '%')
              string numericPart = newValue.substr(0, newValue.find('%'));
              int discountValue = stoi(numericPart);

              // Validate that the DiscountAmount is between 1 and 100
              if (discountValue >= 1 && discountValue <= 100) {
                validInput = true;
              } else {
                cout << "Invalid DiscountAmount. Please enter a value between 1 and 100." << endl;
              }
            }catch (exception &e) {
                    cout << "Invalid DiscountAmount. Please enter a valid number between 1 and 100." << endl;
                }
            } else if (field == "IssuedDate" && !newValue.empty()) {
                // Validate IssuedDate as a date in YYYY-MM-DD format
                regex datePattern(R"(\d{4}-\d{2}-\d{2})");

                if (regex_match(newValue, datePattern)) {
                    validInput = true;
                } else {
                    cout << "Invalid IssuedDate value. Please enter the date in YYYY-MM-DD format." << endl;
                }
            } else {
                validInput = true; // No specific validation for other fields
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
        "UPDATE Discount SET " + updates + " WHERE DiscountID = '" + DiscountID + "';";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Update: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Data updated successfully!\n" << endl;
    }
   adminReadDiscount();
}

// Function to delete Discount from the database
void adminDeleteDiscount() {
  string DiscountID;

  cout << "Enter Discount ID to delete (blank to exit delete process): ";
  getline(cin, DiscountID);

  if (DiscountID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

  if (DiscountID.find("V") != 0) {
    DiscountID = "V" + DiscountID;
  }

  // Check if DiscountID exists
  string checkSql =
      "SELECT COUNT(*) FROM Discount WHERE DiscountID = '" + DiscountID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking DiscountID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Discount WHERE DiscountID = '" + DiscountID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadDiscount();
}
#endif // ends this file conditional inclusion (must in the last line)