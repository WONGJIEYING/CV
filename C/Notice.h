#ifndef NOTICE_H // checks if this file has not been defined.
#define NOTICE_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct Notice {
  string NoticeID;
  string Description;
  string PostManageBy;
  Notice *next;
};

Notice *Noticehead = NULL;

// print table
void printNotice(Notice *n) {
  // Print table header
  cout << setw(5) << left << "Notice ID"
       << " | " << setw(5) << left << "Description"
       << " | " << setw(5) << left << "Post Manage By" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->NoticeID << " | " 
         << setw(5) << left << n->Description << " | " 
         << setw(5) << left << n->PostManageBy << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackNotice(string noticeID, string description, string postManageBy) {
  Notice *newNotice = new Notice();
  newNotice->NoticeID = noticeID;
  newNotice->Description = description;
  newNotice->PostManageBy = postManageBy;
  newNotice->next = NULL;

  if (Noticehead == NULL) {
    Noticehead = newNotice;
  } else {
    Notice *temp = Noticehead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newNotice;
  }
}

// input to add new data
void addNewNotice() {
    string NoticeID, Description, PostManageBy;
    cout << "Enter Notice ID: ";
    cin >> NoticeID;
    cout << "Enter PDescription: ";
    cin >> Description;
    cout << "Enter Post Manage By (Employee ID): ";
    cin >> PostManageBy;

    PushBackNotice(NoticeID, Description, PostManageBy);
    cout << "Notice added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to generate the next NoticeID
string getNextNoticeID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(NoticeID, 2) AS INTEGER)) FROM Notice"; // Update table name to 'Notice'
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
  return "N" + to_string(maxID);
}

// Function to read all Notices
void adminReadNotice() {
  string query = "SELECT * FROM Notice;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void patientReadNotice() {
  string query = "SELECT NoticeID, Description FROM Notice;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateNotice(const set<string>& validUserIDs) {
  string addMore;
  do {
    string Description, PostManageBy;

    cout << "\nAdd New Notice" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Notice ID
    string NoticeID = getNextNoticeID();

    cout << "Enter Description: ";
    getline(cin >> ws, Description);

    bool validUserID = false;
        do {
          cout << "Enter User ID of the person who manages the post ( valid options are: ";
          for (const auto &id : validUserIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, PostManageBy);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(PostManageBy) && !PostManageBy.empty()) {
            PostManageBy = "E" + PostManageBy;
          }

          PostManageBy = toUpperCase(PostManageBy);

          // Direct validation check
          validUserID = validUserIDs.find(PostManageBy) != validUserIDs.end();

          if (!validUserID) {
            cout << "Invalid User ID. Please enter a valid User ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validUserIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validUserID);

    // Insert the new notice into the database
    string sql =
        "INSERT INTO Notice (NoticeID, Description, PostManageBy) VALUES ('" +
        NoticeID + "', '" + Description + "', '" + PostManageBy + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Notice added successfully!\n" << endl;
    }

    // Optionally read and print all notices to confirm the addition
    adminReadNotice();

    cout << "Do you want to add another notice? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Notice in the database
void adminUpdateNotice(const set<string>& validUserIDs) {
  string NoticeID;

  cout << "Enter Notice ID to update (blank to exit update process): ";
  getline(cin, NoticeID);

    if (NoticeID.empty()) {
        cout << "Update process exited.";
        return;
    }

    if (NoticeID.find("N") != 0) {
        NoticeID = "N" + NoticeID;
    }

  // Check if NoticeID exists
  string checkSql =
      "SELECT COUNT(*) FROM Notice WHERE NoticeID = '" + NoticeID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking NoticeID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"Description", "PostManageBy"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field."
       << endl;

  for (const string &field : fields) {
    string newValue;
     bool validInput = false;

   do {
      cout << field << ": ";
      getline(cin, newValue);

      if (field == "PostManageBy" && !newValue.empty()) {
        if (isdigit(newValue[0])) {
          newValue = "E" + newValue;
        }
        newValue = toUpperCase(newValue);

        if (validUserIDs.find(newValue) != validUserIDs.end()) {
          validInput = true;
        } else {
          cout << "Invalid PostManageBy. Please enter a valid UserID." << endl;
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
      "UPDATE Notice SET " + updates + " WHERE NoticeID = '" + NoticeID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadNotice();
}

// Function to delete Notice from the database
void adminDeleteNotice() {
  string NoticeID;

  cout << "Enter Notice ID to delete (blank to exit delete process): ";
  getline(cin, NoticeID);

  if (NoticeID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

    // Prepend "I" if it's not present
    if (NoticeID.find("N") != 0) {
        NoticeID = "N" + NoticeID;
    }

  // Check if NoticeID exists
  string checkSql =
      "SELECT COUNT(*) FROM Notice WHERE NoticeID = '" + NoticeID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking NoticeID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Notice WHERE NoticeID = '" + NoticeID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!\n" << endl;
  }
  adminReadNotice();
}
#endif // ends this file conditional inclusion (must in the last line)
