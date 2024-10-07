#ifndef PROJECT_H // checks if this file has not been defined.
#define PROJECT_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Structure
struct Project {
    string ProjectID;
    string Title;
    string Description;
    string DateOfPublication;
    Project* next;
};

Project* ProjectHead = NULL;

// Print table
void printProject(Project *n) {
    // Print table header
    cout << setw(15) << left << "Project ID"
         << " | " << setw(25) << left << "Title"
         << " | " << setw(30) << left << "Description"
         << " | " << setw(20) << left << "Date Of Publication" << endl;
    cout << string(90, '-') << endl;  // Add a line of dashes for separation
    
    // Print data
    while (n != NULL) {
        cout << setw(15) << left << n->ProjectID << " | " 
             << setw(25) << left << n->Title << " | " 
             << setw(30) << left << n->Description << " | " 
             << setw(20) << left << n->DateOfPublication << endl;
        n = n->next;
    }
    cout << endl;
}

// Push back function
void PushBackProject(
    string projectID,
    string title,
    string description,
    string dateOfPublication) {
    Project* newProject = new Project();
    newProject->ProjectID = projectID;
    newProject->Title = title;
    newProject->Description = description;
    newProject->DateOfPublication = dateOfPublication;
    newProject->next = NULL;

    if (ProjectHead == NULL) {
        ProjectHead = newProject;
    } else {
        Project* temp = ProjectHead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newProject;
    }
}

// Input to add new data
void addNewProject() {
    string ProjectID, Title, Description, DateOfPublication;
    cout << "Enter Project ID: ";
    cin >> ProjectID;
    cout << "Enter Title: ";
    cin.ignore();  // To ignore leftover newline character
    getline(cin, Title);  // Use getline to handle spaces
    cout << "Enter Description: ";
    getline(cin, Description);  // Use getline to handle spaces
    cout << "Enter Date Of Publication: ";
    cin >> DateOfPublication;

    PushBackProject(ProjectID, Title, Description, DateOfPublication);
    cout << "Project added successfully.\n" << endl;
}

// database CRUD
//==========================================================================================================
// Function to check if a string is a valid date in YYYY-MM-DD format
bool isValidDateProject(const string &date) {
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

// Function to generate the next ProjectID
string getNextProjectID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(ProjectID, 4) AS INTEGER)) FROM Project"; // Update table name to 'Project'
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

  return "PRO" + to_string(maxID);
}

// Function to read all Projects
void adminReadProject() {
  string query = "SELECT * FROM Project;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreateProject() {
  string addMore;
  do {
    string Title, Description, DateOfPublication;

    cout << "\nAdd New Project" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Project ID
    string ProjectID = getNextProjectID();

    cout << "Enter Title: ";
    getline(cin >> ws, Title);

    cout << "Enter Description: ";
    getline(cin >> ws, Description);

    do {
        cout << "Enter Date Of Publication (YYYY-MM-DD): ";
        getline(cin, DateOfPublication);
        if (!isValidDateBilling(DateOfPublication)) {
        cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
    }
    }     while (!isValidDateBilling(DateOfPublication));


    // Insert the new Project into the database
    string sql =
        "INSERT INTO Project (ProjectID, Title, Description, DateOfPublication) VALUES ('" +
        ProjectID + "', '" + Title + "', '" + Description + "', '" + DateOfPublication + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Project added successfully!\n" << endl;
    }

    // Optionally read and print all Projects to confirm the addition
    adminReadProject();

    cout << "Do you want to add another Project? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Project in the database
void adminUpdateProject() {
  string ProjectID;

  cout << "Enter Project ID to update (blank to exit update process): ";
  getline(cin, ProjectID);

    if (ProjectID.empty()) {
        cout << "Update process exited.";
        return;
    }

    if (ProjectID.find("PRO") != 0) {
        ProjectID = "PRO" + ProjectID;
    }

  // Check if ProjectID exists
  string checkSql =
      "SELECT COUNT(*) FROM Project WHERE ProjectID = '" + ProjectID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking ProjectID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"Title", "Description", "DateOfPublication"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip "
          "updating the field."
       << endl;
       
  for (const string &field : fields) {
    string newValue;
    bool validInput = false;

    do {
        cout << field << ": ";
        getline(cin, newValue);

        if (field == "DateOfPublication" && !newValue.empty()) {
            // Validate DateOfPublication as a date in YYYY-MM-DD format
            regex datePattern(R"(\d{4}-\d{2}-\d{2})");

            if (regex_match(newValue, datePattern)) {
                validInput = true;
            } else {
                cout << "Invalid DateOfPublication value. Please enter the date in YYYY-MM-DD format." << endl;
            }
        } else {
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
      "UPDATE Project SET " + updates + " WHERE ProjectID = '" + ProjectID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!" << endl;
  }

  adminReadProject();
}

// Function to delete Project from the database
void adminDeleteProject() {
  string ProjectID;

  cout << "Enter Project ID to delete (blank to exit delete process): ";
  getline(cin, ProjectID);

  if (ProjectID.empty()) {
        cout << "Deletion process exited." << endl;
        return;
    }

  if (ProjectID.find("PRO") != 0) {
    // Prepend "INS" if it's not present
    ProjectID = "PRO" + ProjectID;
  }

  // Check if ProjectID exists
  string checkSql =
      "SELECT COUNT(*) FROM Project WHERE ProjectID = '" + ProjectID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking ProjectID: " << sqlite3_errmsg(DB) << endl;
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

  string sql = "DELETE FROM Project WHERE ProjectID = '" + ProjectID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data deleted successfully!" << endl;
  }
  adminReadProject();
}
#endif // ends this file conditional inclusion (must be the last line)