#ifndef PATIENTAPPOINTMENT_H // checks if this file has not been defined.
#define PATIENTAPPOINTMENT_H //defines this file
#include "DoctorSchedule.h"
#include "TypeOfService.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"
#include "typeOfService.h"
#include "function.h"
#include <regex>
using namespace std;

// structure
struct PatientAppointment {
    string AppointmentID;
    string PatientID;
    string ServiceID;
    string Date;
    string Time;
    string Status;
    string DoctorID;
    PatientAppointment* next;
};

PatientAppointment* PatientAppointmenthead = NULL;

//print table
void printPatientAppointment(PatientAppointment* n) {
    // Print table header
    cout << setw(5) << left << "Appointment ID" << " | "
         << setw(5) << left << "Patient ID" << " | "
         << setw(5) << left << "Service ID" << " | "
         << setw(5) << left << "Date" << " | "
         << setw(5) << left << "Time" << " | "
         << setw(5) << left << "Status" << " | "
         << setw(5) << left << "DoctorID" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->AppointmentID << " | "
             << setw(5) << left << n->PatientID << " | "
             << setw(5) << left << n->ServiceID << " | "
             << setw(5) << left << n->Date << " | "
             << setw(5) << left << n->Time << " | "
             << setw(5) << left << n->Status << " | "
             << setw(5) << left << n->DoctorID << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackPatientAppointment(
    string appointmentID,
    string patientID,
    string serviceID,
    string date,
    string time,
    string status,
    string doctorID) {
    PatientAppointment* newPatientAppointment = new PatientAppointment();
    newPatientAppointment->AppointmentID = appointmentID;
    newPatientAppointment->PatientID = patientID;
    newPatientAppointment->ServiceID = serviceID;
    newPatientAppointment->Date = date;
    newPatientAppointment->Time = time;
    newPatientAppointment->Status = status;
    newPatientAppointment->DoctorID = doctorID;
    newPatientAppointment->next = NULL;

    if (PatientAppointmenthead == NULL) {
        PatientAppointmenthead = newPatientAppointment;
    } else {
        PatientAppointment* temp = PatientAppointmenthead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newPatientAppointment;
    }
}

// Function to check if a string is a valid date in YYYY-MM-DD format
bool isValidDatePA(const string &date) {
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

bool isValidTime(const string &time) {
    // Regular expression to match the format HH:MM where HH is 00-23 and MM is 00-59
    regex timeRegex(R"((([01][0-9]|2[0-3]):([0-5][0-9])))");

    // Check if the format matches
    if (!regex_match(time, timeRegex)) {
        return false;
    }
    return true;
}

// input to add new data
void addNewPatientAppointment() {
    string AppointmentID, PatientID, ServiceID, Date, Time, Status, DoctorID;
    cout << "Enter AppointmentID: ";
    cin >> AppointmentID;
    cout << "Enter Patient ID: ";
    cin >> PatientID;
    cout << "Enter Service ID: ";
    cin >> ServiceID;
    cout << "Enter Date: ";
    cin >> Date;
    cout << "Enter Time: ";
    cin >> Time;
    cout << "Enter Status (Accessed / Cancelled): ";
    cin >> Status;
    cout << "Enter Doctor ID: ";
    cin >> DoctorID;

    PushBackPatientAppointment(AppointmentID, PatientID, ServiceID, Date, Time, Status, DoctorID);
    cout << "Patient Appointment added successfully." << endl;
}

//database CRUD
//=====================================================================================
// Function to generate the next AppointmentID


#include <cctype>
#include <string>





void typeOfService() {
  string typeOfService;
  cout << "\nType of Service" << endl;
  cout << "--------------------------" << endl;
  cout << "Serivce ID 1: Laboratory Services" << endl;
  cout << "Serivce ID 2: Vaccination" << endl;
  cout << "Serivce ID 3: Family Medicine" << endl;
}

string getNextAppointmentID() {
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(AppointmentID, 2) AS INTEGER)) FROM PatientsAppointment"; // Update table name to 'PatientsAppointment'
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
  return "A" + to_string(maxID);
}

void adminReadPatientsAppointment() {
  string query = "SELECT * FROM PatientsAppointment;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

// Function to add new data
void adminCreatePatientsAppointment(const set<string>& validPatientIDs, const set<string>& validServiceIDs, const set<string>& validDoctorIDs) {
    string addMore;
    do {
      string PatientID, ServiceID, Date, Time, DoctorID, Status;

      cout << "\nAdd New PatientsAppointment" << endl;
      cout << "***********************************" << endl;

      // Automatically generate PatientsAppointment ID
      string AppointmentID = getNextAppointmentID();

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
          cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;
          cout << "Valid options are: ";
          for (const auto &id : validPatientIDs) {
            cout << id << " ";
          }
          cout << endl;
        }
      } while (!validPatientID);

        //adminReadService();
        cout << endl;
        adminReadService();
        bool validServiceID = false;
        do {
          cout << "\nEnter Service ID ( valid options are: ";
          for (const auto &id : validServiceIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, ServiceID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(ServiceID) && !ServiceID.empty()) {
            ServiceID = "S" + ServiceID;
          }

          ServiceID = toUpperCase(ServiceID);

          // Direct validation check
          validServiceID = validServiceIDs.find(ServiceID) != validServiceIDs.end();

          if (!validServiceID) {
            cout << "Invalid Service ID. Please enter a valid Service ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validServiceIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validServiceID);

  do {
            cout << "Enter Last Update(YYYY-MM-DD): ";
            getline(cin, Date);
            if (!isValidDatePA(Date)) {
                cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
            }
        } while (!isValidDatePA(Date));

        do {
            cout << "Enter Last Update(HH:MM): ";
            getline(cin, Time);
            if (!isValidTime(Time)) {
                cout << "Invalid date format. Please enter a valid date (HH:MM)." << endl;
            }
        } while (!isValidTime(Time));

        cout << endl;
        adminReadDoctorSchedule();

        bool validDoctorID = false;
        do {
          cout << "Enter Doctor ID ( valid options are: ";
          for (const auto &id : validDoctorIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, DoctorID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(DoctorID) && !DoctorID.empty()) {
            DoctorID = "E" + DoctorID;
          }

          DoctorID = toUpperCase(DoctorID);

          // Direct validation check
          validDoctorID = validDoctorIDs.find(DoctorID) != validDoctorIDs.end();

          if (!validDoctorID) {
            cout << "Invalid Doctor ID. Please enter a valid Doctor ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validDoctorIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validDoctorID);

        // Set the Status as "Accessed" for new appointments
        Status = "Accessed";

        // Insert the new PatientsAppointment into the database
        string sql = "INSERT INTO PatientsAppointment (AppointmentID, PatientID, "
                     "ServiceID, Date, Time, Status, DoctorID) VALUES ('" +
                     AppointmentID + "', '" + PatientID + "', '" + ServiceID + "', '" +
                     Date + "', '" + Time + "', '" + Status + "', '" + DoctorID + "');";

        char *messageError;
        int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Error inserting data: " << messageError << endl;
            sqlite3_free(messageError);
        } else {
            cout << "PatientsAppointment added successfully!\n" << endl;
        }
        
        adminReadPatientsAppointment();

        cout << "Do you want to add more appointments? (Y/N): ";
        getline(cin >> ws, addMore);

    } while (addMore == "Y" || addMore == "y");
}

// Function to update PatientsAppointment in the database
void adminUpdatePatientsAppointment(const set<string>& validPatientIDs, const set<string>& validServiceIDs, const set<string>& validDoctorIDs) {
  string AppointmentID;

  cout << "Enter PatientsAppointment ID to update (blank to exit update process): ";
  getline(cin, AppointmentID);
  //cin.ignore();

    // Check if input is empty
    if (AppointmentID.empty()) {
        cout << "Update process exited.";
        return;
    }

    if (AppointmentID.find("A") != 0) {
        AppointmentID = "A" + AppointmentID;
    }

  // Check if AppointmentID exists
  string checkSql = "SELECT COUNT(*) FROM PatientsAppointment WHERE AppointmentID = '" + AppointmentID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK) {
    cerr << "Error checking AppointmentID: " << sqlite3_errmsg(DB) << endl;
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

  string fields[] = {"PatientID", "ServiceID", "Date", "Time", "DoctorID"};
  string updates = "";

  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

  for (const string &field : fields) {
    string newValue;
     bool validInput = false;

     do {
            cout << field << ": ";
            getline(cin, newValue);

            validInput = false; // Reset validInput for each field

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
            } else if (field == "ServiceID" && !newValue.empty()) {
                if (isdigit(newValue[0])) {
                    newValue = "S" + newValue;
                }
                newValue = toUpperCase(newValue);

                if (validServiceIDs.find(newValue) != validServiceIDs.end()) {
                    validInput = true;
                } else {
                    cout << "Invalid ServiceID. Please enter a valid ServiceID." << endl;
                }
            } else if (field == "Date" && !newValue.empty()) {
                if (isValidDatePA(newValue)) {
                    validInput = true;
                } else {
                    cout << "Invalid Date. Please enter a valid date in the format YYYY-MM-DD." << endl;
                }
            } else if (field == "Time" && !newValue.empty()) {
                if (isValidTime(newValue)) {
                    validInput = true;
                } else {
                    cout << "Invalid Time. Please enter a valid time in the format HH:MM." << endl;
                }
            }else if (field == "DoctorID" && !newValue.empty()) {
                if (isdigit(newValue[0])) {
                    newValue = "E" + newValue;
                }
                newValue = toUpperCase(newValue);

                if (validDoctorIDs.find(newValue) != validDoctorIDs.end()) {
                    validInput = true;
                } else {
                    cout << "Invalid DoctorID. Please enter a valid DoctorID." << endl;
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

  string sql =
      "UPDATE PatientsAppointment SET " + updates + " WHERE AppointmentID = '" + AppointmentID + "';";

  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error Update: " << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Data updated successfully!\n" << endl;
  }

  adminReadPatientsAppointment();
}


//=====================================================================================
// Function to add new data
void patientCreatePatientsAppointment(const string& UserID, const set<string>& validServiceIDs, const set<string>& validDoctorIDs) {
    string addMore;
    do {
      string PatientID, ServiceID, Date, Time, DoctorID, Status;

      cout << "\nAdd New PatientsAppointment" << endl;
      cout << "***********************************" << endl;

      // Automatically generate PatientsAppointment ID
      string AppointmentID = getNextAppointmentID();

        //cout << "Enter Patient ID: ";
        //(cin >> ws, PatientID);
        //adminReadService();
        adminReadService();
        bool validServiceID = false;
        do {
          cout << "Enter Service ID ( valid options are: ";
          for (const auto &id : validServiceIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, ServiceID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(ServiceID) && !ServiceID.empty()) {
            ServiceID = "S" + ServiceID;
          }

          ServiceID = toUpperCase(ServiceID);

          // Direct validation check
          validServiceID = validServiceIDs.find(ServiceID) != validServiceIDs.end();

          if (!validServiceID) {
            cout << "Invalid Service ID. Please enter a valid Service ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validServiceIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validServiceID);

        if (ServiceID.find('S') != 0) {
        ServiceID = 'S' + ServiceID;
    }
        //cin.ignore();
  do {
            cout << "Enter Date(YYYY-MM-DD): ";
            getline(cin, Date);
            if (!isValidDatePA(Date)) {
                cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
            }
        } while (!isValidDatePA(Date));

        do {
            cout << "Enter Time(HH:MM): ";
            getline(cin, Time);
            if (!isValidTime(Time)) {
                cout << "Invalid date format. Please enter a valid date (HH:MM)." << endl;
            }
        } while (!isValidTime(Time));
        cout << endl;

        adminReadDoctorSchedule();

        bool validDoctorID = false;
        do {
          cout << "Enter Doctor ID ( valid options are: ";
          for (const auto &id : validDoctorIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, DoctorID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(DoctorID) && !DoctorID.empty()) {
            DoctorID = "E" + DoctorID;
          }

          DoctorID = toUpperCase(DoctorID);

          // Direct validation check
          validDoctorID = validDoctorIDs.find(DoctorID) != validDoctorIDs.end();

          if (!validDoctorID) {
            cout << "Invalid Doctor ID. Please enter a valid Doctor ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validDoctorIDs) {
              cout << id << " ";
            }
            cout << ": ";
          }
        } while (!validDoctorID);

        // Set the Status as "Accessed" for new appointments
        Status = "Accessed";

        // Insert the new PatientsAppointment into the database
        string sql = "INSERT INTO PatientsAppointment (AppointmentID, PatientID, "
                     "ServiceID, Date, Time, Status, DoctorID) VALUES ('" +
                     AppointmentID + "', '" + UserID + "', '" + ServiceID + "', '" +
                     Date + "', '" + Time + "', '" + Status + "', '" + DoctorID + "');";

        char *messageError;
        int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Error inserting data: " << messageError << endl;
            sqlite3_free(messageError);
        } else {
            cout << "PatientsAppointment added successfully!" << endl;
        }

        cout << "Do you want to add more appointments? (Y/N): ";
        getline(cin >> ws, addMore);

    } while (addMore == "Y" || addMore == "y");
}

void patientReadAppointment(const string& PatientID) {
    string query = "SELECT * FROM PatientsAppointment WHERE PatientID = '" + PatientID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read PatientsAppointment: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

#endif // ends this file conditional inclusion (must in the last line)

