#ifndef CRUD_H
#define CRUD_H

#include <iostream>
#include <string>
#include <iomanip>
#include <set>
#include "sqlite3.h"
#include "global.h"

using namespace std;

struct user {
  string userID;
  string name;
  string ICNo;
  string gender;
  string position;
  string contactNumber;
  string email;
  string address;
  string username;
  string password;
  bool firstLogin;
  user *next;
};

user *userHead = NULL;
user *currentUser = userHead;

struct patientAppointment {
  string appointmentID;
  string patientID;
  string serviceID;
  string date;
  string time;
  string status;
  string doctorID;
  patientAppointment *next ;
};

struct cancelAppointment {
  string cancelAppointmentID;
  string appointmentID;
  string doctorID;
  string reason;
  cancelAppointment *next;
};

struct medicalDiagnosis {
  string patientDiagnosisID;
  string patientID;
  string doctorID;
  string medicalHistory;
  string medicationsTaken;
  string diagnosis;
  string lastUpdate;
  medicalDiagnosis *next;
};

struct followUp {
  string followUpRecordID;
  string patientDiagnosisID;
  string followUpDate;
  string description;
  string doctorOrders;
  followUp *next;
};

struct admissionTransfer {
  string patientStatusID;
  string patientID;
  string checkInTime;
  string checkOutTime;
  string currentStatus;
  admissionTransfer *next;
};

struct vaccination {
  string vaccinationID;
  string patientID;
  string vaccineType;
  string doseNumber;
  string administrationDate;
  vaccination *next;
};

struct doctorSchedule {
  string doctorScheduleID;
  string doctorID;
  string monday;
  string tuesday;
  string wednesday;
  string thursday;
  string friday;
  string saturday;
  string sunday;
  doctorSchedule *next;
};

struct patientAllergy {
  string patientAllergyID;
  string patientID;
  string allergyName;
  string severity;
  string reaction;
  patientAllergy *next;
};

struct emergencyTools {
  string emergencyToolsID;
  string type;
  string tool;
  string emergencyNumber;
  emergencyTools *next;
};

struct project {
  string projectID;
  string title;
  string description;
  string dateOfPublication;
  project *next;
};

// Function declarations
void openDatabase();
void executeSQL(const string& sql);

int DBExit;

void openDatabase() {
    DBExit = sqlite3_open("testing.DB", &DB);
    if (DBExit) {
        cerr << "Can't open database: " << sqlite3_errmsg(DB) << endl;
        return;
    } else {
        cout << "Opened database successfully" << endl;
    }
}

void populateValidPatientIDs(set<string>& validPatientIDs, sqlite3 *DB) {
    string sql = "SELECT UserID FROM User WHERE Position = ('patient', 'Patient')";
    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string patietnID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        validPatientIDs.insert(patietnID);
    }

    sqlite3_finalize(stmt);
}

void populateValidDoctorIDs(set<string>& validDoctorIDs, sqlite3 *DB) {
    string sql = "SELECT UserID FROM User WHERE Position IN ('doctor', 'Doctor');";
    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string doctorID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        validDoctorIDs.insert(doctorID);
    }

    sqlite3_finalize(stmt);
}



void createUser(const string &userID, const string &name, const string &icNo, const string &gender, const string &position, const string &contactNumber, const string &email, const string &address, const string &username, const string &password) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO User (UserID, Name, ICNO, Gender, Position, ContactNumber, Email, Address, Username, Password) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, icNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, gender.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, position.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, contactNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, password.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "User inserted successfully" << endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
}


// UPDATE Medical Diagnosis
void userUpdateData(const string& userID, const string& name, const string& icno, const string& gender, const string& contactNumber, const string& email, const string& address, const string& username, const string& password) {
    sqlite3_stmt* stmt;
    string sql = "UPDATE User SET Name = ?, ICNO = ?, Gender = ?, ContactNumber = ?, Email = ?, Address = ?, Username = ?, Password = ? WHERE UserID = ?";

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind parameters to the SQL statement
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, icno.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, gender.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, contactNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, userID.c_str(), -1, SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Updated user with ID '" << userID << "' successfully." << endl;
    }
    sqlite3_finalize(stmt);
}

void deleteUser(const string& userID, sqlite3 *DB) {
    sqlite3_stmt* stmt;
    int DBExit;

    // Prepare SQL query to delete the user
    string sql = "DELETE FROM User WHERE UserID = ?";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the user ID
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the delete query
    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Deleted user with ID '" << userID << "' successfully." << endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
}

void viewUser(const string &userID) {
    sqlite3_stmt* stmt;
    string sql = "SELECT * FROM User WHERE UserID = ?";

     // Prepare the SQL statement
    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the userID parameter
    DBExit = sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_STATIC);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << "-------------------------------------------------------------------------------------------------------" << endl;
        cout << " User ID        : " << (const char*)sqlite3_column_text(stmt, 0) << endl;
        cout << " Name           : " << (const char*)sqlite3_column_text(stmt, 1) << endl;
        cout << " IC No          : " << (const char*)sqlite3_column_text(stmt, 2) << endl;
        cout << " Gender         : " << (const char*)sqlite3_column_text(stmt, 3) << endl;
        cout << " Position       : " << (const char*)sqlite3_column_text(stmt, 4) << endl;
        cout << " Contact Number : " << (const char*)sqlite3_column_text(stmt, 5) << endl;
        cout << " Email          : " << (const char*)sqlite3_column_text(stmt, 6) << endl;
        cout << " Address        : " << (const char*)sqlite3_column_text(stmt, 7) << endl;
        cout << " Username       : " << (const char*)sqlite3_column_text(stmt, 8) << endl;
        cout << " Password       : " << (const char*)sqlite3_column_text(stmt, 9) << endl;
        cout << "-------------------------------------------------------------------------------------------------------" << endl;
    }

    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_finalize(stmt);
}

void loadUsersFromDatabase(sqlite3 *DB, user *&userHead) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT username, password, position, userID FROM User";
    
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    userHead = nullptr;
    user *lastUser = nullptr;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string userID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        user *newUser = new user();
        newUser->username = username;
        newUser->password = password;
        newUser->position = position;
        newUser->userID = userID;
        newUser->firstLogin = false;
        newUser->next = nullptr;

        if (userHead == nullptr) {
            userHead = newUser;
        } else {
            lastUser->next = newUser;
        }
        lastUser = newUser;
    }

    sqlite3_finalize(stmt);
}

bool userExists(const string &username) {
    sqlite3_stmt* stmt;
    string sql = "SELECT COUNT(*) FROM User WHERE Username = ?";

    // Prepare the SQL statement
    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return false;
    }

    // Bind the username parameter
    DBExit = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Execute the query
    DBExit = sqlite3_step(stmt);
    if (DBExit == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count > 0;
    } else {
        cerr << "Failed to execute query: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
}

void updatePassword(sqlite3* DB, const string& username, const string& newPassword) {
    string updateSql = "UPDATE User SET Password = ? WHERE Username = ?";
    sqlite3_stmt* updateStmt;
    int DBExit;

    // Prepare the SQL statement
    DBExit = sqlite3_prepare_v2(DB, updateSql.c_str(), -1, &updateStmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare update statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the new password
    DBExit = sqlite3_bind_text(updateStmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind new password: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(updateStmt);
        return;
    }

    // Bind the username
    DBExit = sqlite3_bind_text(updateStmt, 2, username.c_str(), -1, SQLITE_STATIC);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind username: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(updateStmt);
        return;
    }

    // Execute the SQL statement
    int result = sqlite3_step(updateStmt);
    if (result != SQLITE_DONE) {
        cerr << "Failed to execute update statement: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Password changed successfully!" << endl;
    }

    sqlite3_finalize(updateStmt);
}


bool isValidICNo2(const string &icNo) {
  return icNo.length() == 12 && all_of(icNo.begin(), icNo.end(), ::isdigit);
}

bool isValidGender2(const string &gender) {
  return gender == "male" || gender == "female" || gender == "Male" || gender == "Female";
}

bool isValidPosition2(const string &position) {
  return position == "patient" || position == "doctor" || position == "admin" || position == "Patient" || position == "Doctor" || position == "Admin";
}

bool isValidContactNumber2(const string &contactNumber) {
  return (contactNumber.length() >= 11 && contactNumber.length() <= 12) && all_of(contactNumber.begin(), contactNumber.end(), ::isdigit);
}

bool validateEmail2(const string &email) {
  return email.find('@') != string::npos;
}

string formatUserID(const string &input) {
  string formattedID = input;
  if (formattedID.empty())
    return formattedID;

  // Capitalize and add 'e' if not present
  if (formattedID[0] != 'E') {
    formattedID = 'E' + formattedID;
  }

  // Convert to uppercase
  for (char &ch : formattedID) {
    ch = toupper(ch);
  }
  return formattedID;
}



// CREATE Patients Appointment
void createPatientsAppointment(const string &appointmentID, const string &patientID, const string &serviceID, const string &date, const string &time, const string &status, const string &doctorID) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO patientsAppointment (AppointmentID, PatientID, ServiceID, Date, Time, Status, DoctorID) VALUES (?, ?, ?, ?, ?, ?, ?);";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, appointmentID.c_str(), appointmentID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, patientID.c_str(), patientID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, serviceID.c_str(), serviceID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, date.c_str(), date.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, time.c_str(), time.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, status.c_str(), status.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, doctorID.c_str(), doctorID.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Patients Appointment inserted successfully" << endl;
    }
    sqlite3_finalize(stmt);
}

// READ Patients Appointment
void readPatientsAppointment(const string &doctorID){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM patientsAppointment WHERE DoctorID = ?;";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    DBExit = sqlite3_bind_text(stmt, 1, doctorID.c_str(), -1, SQLITE_TRANSIENT);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout << "Appointment ID" << setw(2) << "|" << setw(11) << "Patient ID" << setw(2) << "|" << setw(11) << "Service ID" << setw(2) << "|"
        << setw(9) << "Date" << setw(6) << "|" << setw(8) << "Time" << setw(6) << "|" << setw(9) << "Status" << setw(3) << "|" << setw(9) << "Doctor ID" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;

    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(9) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(8) << " | " << setw(6) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(7) << " | " << setw(7) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(6) << " | " << setw(12) << (const char*)sqlite3_column_text(stmt, 3) 
             << setw(3) << " | " << setw(9) << (const char*)sqlite3_column_text(stmt, 4)
             << setw(5) << " | " << setw(9) << (const char*)sqlite3_column_text(stmt, 5) 
             << setw(2) << " | " << setw(5) << (const char*)sqlite3_column_text(stmt, 6) << endl;
    }

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}

// CREATE Patients Appointment
void createCancelAppointment(const string &cancelAppointmentID, const string &appointmentID, const string &doctorID, const string &reason) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO cancelAppointment (CancelAppointmentID, AppointmentID, DoctorID, Reason) VALUES (?, ?, ?, ?);";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, cancelAppointmentID.c_str(), cancelAppointmentID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, appointmentID.c_str(), appointmentID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, doctorID.c_str(), doctorID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, reason.c_str(), reason.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Patients Cancel Appointment inserted successfully" << endl;
    }
    sqlite3_finalize(stmt);
}

// READ Patients Appointment
void readCancelAppointment(const string &doctorID){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM cancelAppointment WHERE DoctorID = ?;";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    DBExit = sqlite3_bind_text(stmt, 1, doctorID.c_str(), -1, SQLITE_TRANSIENT);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

  cout << "---------------------------------------------------------------------------"  << endl;
  cout << "Cancel Appointment ID" << setw(2) << "|" << setw(15)  << "Appointment ID" << setw(2) << "|" << setw(7) << "Doctor ID" << "|" << setw(10) << "Reason" << endl;
  cout << "---------------------------------------------------------------------------" << endl;

  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(13) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(11) << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(7) << " | " << setw(6) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(4) << " | " << setw(20) << (const char*)sqlite3_column_text(stmt, 3) << endl;
    }

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}


// CREATE Medical Diagnosis
void createMedicalDiagnosis(const string &patientDiagnosisID, const string &patientID, const string &doctorID, const string &medicalHistory, const string &medicationsTaken,
                            const string &diagnosis, const string &lastUpdate) {
    sqlite3_stmt* stmt;

    // SQL query to insert a new diagnosis, but only if the doctorID matches in PatientsAppointment
    string sql = "INSERT INTO MedicalDiagnosis (PatientDiagnosisID, PatientID, DoctorID, MedicalHistory, MedicationsTaken, Diagnosis, LastUpdate) "
                 "SELECT ?, ?, ?, ?, ?, ?, ? "
                 "WHERE EXISTS (SELECT 1 FROM PatientsAppointment WHERE PatientID = ? AND DoctorID = ?);";

    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, patientID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, doctorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, medicalHistory.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, medicationsTaken.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, diagnosis.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, lastUpdate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, patientID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, doctorID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query
    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Medical diagnosis '" << patientDiagnosisID << "' added successfully." << endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
}

// READ Medical Diagnosis
void readDiagnosis(const string &patientDiagnosisID){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM medicalDiagnosis WHERE DoctorID = ?";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
      cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
      return;
    }

    sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);

    if (DBExit != SQLITE_OK) {
      cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
      sqlite3_finalize(stmt);
      return;
    }

  cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  cout << setw(18) << "Patient Diagnosis ID" << " | " << setw(10) << "Patient ID" << " | " << setw(10) << "Doctor ID" << " | " << setw(15) << "Medical History"
       << " | " << setw(32) << "Medications Taken" << setw(16) << " | " << setw(16) << "Diagnosis" << setw(10) << " | " << setw(10) << "Last Update" << endl;
  cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
    cout << setw(11) << (const char*)sqlite3_column_text(stmt, 0) 
         << setw(12) << " | " << setw(6) << (const char*)sqlite3_column_text(stmt, 1) 
         << setw(7) << " | " << setw(6) << (const char*)sqlite3_column_text(stmt, 2) 
         << setw(7) << " | " << setw(15) << (const char*)sqlite3_column_text(stmt, 3) 
         << " | "  << setw(45) << (const char*)sqlite3_column_text(stmt, 4) 
         << " | "  << setw(23) << (const char*)sqlite3_column_text(stmt, 5) 
         << " | "  << setw(10)  << (const char*)sqlite3_column_text(stmt, 6) << endl;
  }    

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}

// UPDATE Medical Diagnosis
void updateDiagnosis(const string& patientDiagnosisID, const string& patientID, const string& doctorID, const string& medicalHistory, const string& medicationsTaken,
                     const string& diagnosis, const string& lastUpdate, sqlite3* DB){
    sqlite3_stmt* stmt;

    string sql = "UPDATE medicalDiagnosis SET PatientID = ?, DoctorID = ?, MedicalHistory = ?, MedicationsTaken = ?, Diagnosis = ?, LastUpdate = ? WHERE PatientDiagnosisID = ?";

    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    //medicalDiagnosis *current = NULL;
    sqlite3_bind_text(stmt, 1, patientID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, doctorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, medicalHistory.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, medicationsTaken.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, diagnosis.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, lastUpdate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
         cout << "Updated diagnosis with ID '" << patientDiagnosisID << "' successfully." << endl;
    }
    sqlite3_finalize(stmt);
}

// DELETE Medical Diagnosis
void deleteDiagnosis(const string& loggedInDoctorID, const string& patientDiagnosisID, sqlite3* DB) {
    sqlite3_stmt* stmt;
    bool authorized = false;

    string selectSQL = "SELECT PatientID FROM medicalDiagnosis WHERE PatientDiagnosisID = ?";
    int DBExit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);
    
     // Execute the query
    DBExit = sqlite3_step(stmt);
    if (DBExit == SQLITE_ROW) {
        string patientID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);  // Finalize the statement before preparing a new one

        // Check if the patient belongs to the logged-in doctor
        string patientCheckSQL = "SELECT 1 FROM PatientsAppointment WHERE PatientID = ? AND DoctorID = ?";
        DBExit = sqlite3_prepare_v2(DB, patientCheckSQL.c_str(), -1, &stmt, NULL);

        if (DBExit != SQLITE_OK) {
            cerr << "Failed to prepare patient authorization statement: " << sqlite3_errmsg(DB) << endl;
            return;
        }

        // Bind patient ID and doctor ID
        sqlite3_bind_text(stmt, 1, patientID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the query
        DBExit = sqlite3_step(stmt);
        if (DBExit == SQLITE_ROW) {
            authorized = true; // Authorization successful
        } else {
            cout << "You are not authorized to delete this diagnosis." << endl;
        }
        sqlite3_finalize(stmt);  // Finalize the statement before preparing a new one

        if (authorized) {
            // Proceed to delete the diagnosis
            string deleteSQL = "DELETE FROM medicalDiagnosis WHERE PatientDiagnosisID = ?";
            DBExit = sqlite3_prepare_v2(DB, deleteSQL.c_str(), -1, &stmt, NULL);

            if (DBExit != SQLITE_OK) {
                cerr << "Failed to prepare DELETE statement: " << sqlite3_errmsg(DB) << endl;
                return;
            }

            // Bind the diagnosis ID
            sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);

            // Execute the delete query
            DBExit = sqlite3_step(stmt);
            if (DBExit != SQLITE_DONE) {
                cerr << "Failed to delete diagnosis: " << sqlite3_errmsg(DB) << endl;
            } else {
                cout << "Deleted diagnosis with ID '" << patientDiagnosisID << "' successfully." << endl;
            }
        }
    } else {
        cout << "Diagnosis with ID '" << patientDiagnosisID << "' not found. No deletion performed." << endl;
    }

    sqlite3_finalize(stmt);  // Ensure statement is finalized at the end
}

// CREATE Follow Up
bool createPatientFollowUp(const string &followUpRecordID, const string &patientDiagnosisID, const string &followUpDate, const string &description, const string &doctorOrders, const string &doctorID) {
    sqlite3_stmt *stmt;
    
    string sql = "INSERT INTO PatientFollowUp (FollowUpRecordID, PatientDiagnosisID, FollowUpDate, Description, DoctorOrders, DoctorID) "
             "SELECT ?, ?, ?, ?, ?, ? "
             "WHERE EXISTS (SELECT 1 FROM MedicalDiagnosis WHERE PatientDiagnosisID = ? AND DoctorID = ?);";

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return false;
    }


    // Bind values to the SQL statement
    sqlite3_bind_text(stmt, 1, followUpRecordID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, patientDiagnosisID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, followUpDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, doctorOrders.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, doctorID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, patientDiagnosisID.c_str(), -1, SQLITE_STATIC);  // Bind for EXISTS subquery
    sqlite3_bind_text(stmt, 8, doctorID.c_str(), -1, SQLITE_STATIC);  // Bind for EXISTS subquery

    // Execute the SQL statement
     DBExit = sqlite3_step(stmt);
    if (DBExit == SQLITE_DONE) {
        cout << "Patient follow up '" << followUpRecordID << "' added successfully." << endl;
        sqlite3_finalize(stmt);
        return true;
    } else {
        cerr << "Error adding patient follow up: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
    /*if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, followUpRecordID.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, patientDiagnosisID.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, followUpDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, doctorOrders.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, doctorID.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << "Patient follow up '" << followUpRecordID << "' added successfully." << endl;
            sqlite3_finalize(stmt);
            return true;
        } else {
            cerr << "Error adding patient follow up: " << sqlite3_errmsg(DB) << endl;
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
    }*/

    //return false;
}

// READ Follow Up
void readPatientFollowUp(const string &doctorID){
    sqlite3_stmt* stmt;
    int DBExit;

    string sql = "SELECT FollowUpRecordID, PatientDiagnosisID, FollowUpDate, Description, DoctorOrders, DoctorID "
                 "FROM PatientFollowUp WHERE DoctorID = ?";

    // Prepare SQL statement
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the doctor ID parameter
    DBExit = sqlite3_bind_text(stmt, 1, doctorID.c_str(), -1, SQLITE_TRANSIENT);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

  cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  cout << "Follow Up Record ID" << setw(2) << "|" << setw(11) << "Patient Diagnosis ID" << setw(2) << "|" << setw(11) << "Follow-up Date"
       << setw(1) << "|" << setw(23) << "Description" << setw(12) << "|" << setw(33) << "Doctor Orders" << setw(20) << "|" << setw(3) << "Doctor ID" << endl;
  cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
    cout << setw(12) << (const char*)sqlite3_column_text(stmt, 0) 
         << setw(10) << " | " << setw(11) << (const char*)sqlite3_column_text(stmt, 1) 
         << setw(11) << " | " << setw(11) << (const char*)sqlite3_column_text(stmt, 2) 
         << setw(4) << " | " << setw(32) << (const char*)sqlite3_column_text(stmt, 3) << setw(2)
         << " | "  << setw(50) << (const char*)sqlite3_column_text(stmt, 4) << setw(2)
         << " | "  << setw(3) << (const char*)sqlite3_column_text(stmt, 5) << endl;
  }    

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}

// UPDATE Follow Up
void updatePatientFollowUp(const string &followUpRecordID, const string &patientDiagnosisID, const string &followUpDate, const string &description, const string &doctorOrders, const string &doctorID){
    sqlite3_stmt* stmt;

    string sql = "UPDATE PatientFollowUp SET PatientDiagnosisID = ?, FollowUpDate = ?, Description = ?, DoctorOrders = ?, DoctorID = ? WHERE FollowUpRecordID = ?";

    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    //medicalDiagnosis *current = NULL;
    sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, followUpDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, doctorOrders.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, doctorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, followUpRecordID.c_str(), -1, SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
         cout << "Updated patient follow up with ID '" << followUpRecordID << "' successfully." << endl;
    }
    sqlite3_finalize(stmt);
}

// DELETE Follow Up
void deletePatientFollowUp(const string& followUpRecordID) {
    sqlite3_stmt* stmt;

    string sql = "DELETE FROM PatientFollowUp WHERE FollowUpRecordID = ?";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, followUpRecordID.c_str(), -1, SQLITE_TRANSIENT);
    
    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
      cout << "Deleted patient follow up with ID '" << followUpRecordID << "' successfully." << endl;
    }
    sqlite3_finalize(stmt);
}

// CREATE Patient Admission Transfer
bool createAdmissionTransfer(const string &patientStatusID, const string &patientID, const string &checkInTime, const string &checkOutTime, const string &currentStatus, const string& doctorID) {
    string authorizationCheckSQL = "SELECT 1 FROM PatientsAppointment WHERE PatientID = ? AND DoctorID = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(DB, authorizationCheckSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, patientID.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, doctorID.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
            cerr << "Doctor is not authorized to add admission transfer for this patient." << endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare authorization check statement: " << sqlite3_errmsg(DB) << endl;
        return false;
    }
    
    /*string sql = "INSERT INTO PatientAdmissionTransfer (PatientStatusID, PatientID, CheckInTime, CheckOutTime, CurrentStatus, DoctorID) "
                 "SELECT ?, ?, ?, ?, ?, ? "
                 "WHERE EXISTS (SELECT 1 FROM PatientsAppointment WHERE PatientID = ? AND DoctorID = ?);";*/

    string sql = "INSERT INTO PatientAdmissionTransfer (PatientStatusID, PatientID, CheckInTime, CheckOutTime, CurrentStatus) "
                       "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, patientStatusID.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, patientID.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, checkInTime.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, checkOutTime.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, currentStatus.c_str(), -1, SQLITE_STATIC);
        //sqlite3_bind_text(stmt, 6, doctorID.c_str(), -1, SQLITE_STATIC);
        //sqlite3_bind_text(stmt, 8, patientID.c_str(), -1, SQLITE_TRANSIENT);           // Ensure the patientID matches in PatientsAppointment
        //sqlite3_bind_text(stmt, 9, doctorID.c_str(), -1, SQLITE_TRANSIENT); 

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << "Patient Status '" << patientStatusID << "' added successfully." << endl;
            sqlite3_finalize(stmt);
            return true;
        } else {
            cerr << "Error adding patient status: " << sqlite3_errmsg(DB) << endl;
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
    }

    return false;
}

// READ Patient Admission Transfer
/*void readAdmissionTransfer(const string &doctorID){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM PatientAdmissionTransfer WHERE DoctorID = ?";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
      cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
      return;
    }

    sqlite3_bind_text(stmt, 1, doctorID.c_str(), -1, SQLITE_TRANSIENT);

    if (DBExit != SQLITE_OK) {
      cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
      sqlite3_finalize(stmt);
      return;
    }

  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "Patient Status ID" << setw(2) << "|" << setw(11) << "Patient ID" << setw(2) << "|" << setw(16) << "Check-in Time" << setw(3) << "|"
       << setw(16) << "Check-out Time" << setw(3) << "|" << setw(18) << "Current Status" << setw(2) << "|" << setw(6) << "Doctor ID" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;

  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
    cout << setw(10) << (const char*)sqlite3_column_text(stmt, 0) 
         << setw(10) << " | " << setw(6) << (const char*)sqlite3_column_text(stmt, 1) 
         << setw(7) << " | " << setw(12) << (const char*)sqlite3_column_text(stmt, 2) 
         << setw(2) << " | " << setw(12) << (const char*)sqlite3_column_text(stmt, 3) << setw(2)
         << " | "  << setw(17) << (const char*)sqlite3_column_text(stmt, 4) << setw(2)
         << " | "  << setw(5) << (const char*)sqlite3_column_text(stmt, 5) << setw(5) << endl;
  }    

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}*/

void readAdmissionTransfer(const string &doctorID) {
    sqlite3_stmt *stmt;
    
    // Step 1: Check if the doctor is authorized to view patient admission transfers
    string authorizationCheckSQL = "SELECT 1 FROM PatientsAppointment WHERE DoctorID = ?;";
    
    if (sqlite3_prepare_v2(DB, authorizationCheckSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, doctorID.c_str(), -1, SQLITE_STATIC);

        // Execute the query
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            cerr << "Doctor is not authorized to view any patient admission transfers." << endl;
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare authorization check statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Step 2: Retrieve and display patient admission transfers
    string selectSQL = "SELECT p.PatientStatusID, p.PatientID, p.CheckInTime, p.CheckOutTime, p.CurrentStatus "
                       "FROM PatientAdmissionTransfer p "
                       "JOIN PatientsAppointment a ON p.PatientID = a.PatientID "
                       "WHERE a.DoctorID = ?;";

    if (sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, doctorID.c_str(), -1, SQLITE_STATIC);

        cout << "-------------------------------------------------------------------------------------------" << endl;
        cout << "Patient Status ID" << setw(2) << "|" << setw(11) << "Patient ID" << setw(2) << "|" << setw(16) << "Check-in Time" << setw(3) << "|"
             << setw(16) << "Check-out Time" << setw(3) << "|" << setw(18) << "Current Status" << endl;
        cout << "-------------------------------------------------------------------------------------------" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            cout << setw(10) << (const char*)sqlite3_column_text(stmt, 0) 
                 << setw(10) << " | " << setw(6) << (const char*)sqlite3_column_text(stmt, 1) 
                 << setw(7) << " | " << setw(12) << (const char*)sqlite3_column_text(stmt, 2) 
                 << setw(2) << " | " << setw(12) << (const char*)sqlite3_column_text(stmt, 3) << setw(2)
                 << " | "  << setw(17) << (const char*)sqlite3_column_text(stmt, 4) << endl;
        }    

        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare select statement: " << sqlite3_errmsg(DB) << endl;
    }
}

// UPDATE Patient Admission Transfer
void updatePatientAdmissionTransfer(sqlite3* DB, const string &patientStatusID, const string &patientID, const string &checkInTime, const string &checkOutTime, const string &currentStatus){
    sqlite3_stmt* stmt;

    string sql = "UPDATE PatientAdmissionTransfer SET PatientID = ?, CheckInTime = ?, CheckOutTime = ?, CurrentStatus = ? WHERE PatientStatusID = ?";

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }
    
    sqlite3_bind_text(stmt, 1, patientID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, checkInTime.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, checkOutTime.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, currentStatus.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, patientStatusID.c_str(), -1, SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Updated patient status with ID '" << patientStatusID << "' successfully." << endl;
    }
    sqlite3_finalize(stmt);
}

// DELETE Patient Admission Transfer
void deletePatientAdmissionTransfer(const string& patientStatusID) {
    sqlite3_stmt* stmt;

    string sql = "DELETE FROM PatientAdmissionTransfer WHERE PatientStatusID = ?";
    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, patientStatusID.c_str(), -1, SQLITE_TRANSIENT);
    
    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Deleted patient status with ID '" << patientStatusID << "' successfully." << endl;
    }

    sqlite3_finalize(stmt);
}

// CREATE Patients Appointment
void createVaccination(const string &vaccinationID, const string &patientID, const string &vaccineType, const string &doseNumber, const string &administrationDate) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO vaccinationRecords (vaccinationID, PatientID, VaccineType, DoseNumber, AdministrationDate) VALUES (?, ?, ?, ?, ?);";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, vaccinationID.c_str(), vaccinationID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, patientID.c_str(), patientID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, vaccineType.c_str(), vaccineType.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, doseNumber.c_str(), doseNumber.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, administrationDate.c_str(), administrationDate.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Vaccination Records inserted successfully" << endl;
    }
    sqlite3_finalize(stmt);
}

string globalDoctorID;
string globalPatientID;

// READ Patients Appointment
void readVaccination(){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM vaccinationRecords";

    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, globalDoctorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, globalPatientID.c_str(), -1, SQLITE_TRANSIENT);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

  cout << "------------------------------------------------------------------------------------" << endl;
  cout << "Vaccination ID" << setw(2) << "|" << setw(11) << "Patient ID" << setw(2) << "|" << setw(11) << "Vaccine Type" << setw(2) << "|"
       << setw(11) << "Dose Number" << setw(2) << "|" << setw(25) << "Administration Date" << endl;
  cout << "------------------------------------------------------------------------------------" << endl;
  
  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(8) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(9) << " | " << setw(7) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(6) << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(4) << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
             << setw(3) << " | " << setw(20) << (const char*)sqlite3_column_text(stmt, 4) << endl;
    }

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}

// CREATE Doctor Schedule
void createDoctorSchedule(const string &doctorScheduleID, const string &doctorID, const string &monday, const string &tuesday, const string &wednesday, const string &thursday, const string &friday, const string &saturday, const string &sunday) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO doctorSchedule (DoctorScheduleID, DoctorID, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, doctorScheduleID.c_str(), doctorScheduleID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, doctorID.c_str(), doctorID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, monday.c_str(), monday.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, tuesday.c_str(), tuesday.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, wednesday.c_str(), wednesday.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, thursday.c_str(), thursday.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, friday.c_str(), friday.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, saturday.c_str(), saturday.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, sunday.c_str(), sunday.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Doctor Schedule with ID '" << doctorScheduleID << "' added successfully." << endl;
    }
    sqlite3_finalize(stmt);
}

// READ Doctor Schedule
void readDoctorSchedule(){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM doctorSchedule;";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    cout << "--------------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(17) << "Doctor Schedule ID" << setw(2) << "|" << setw(10) << "Doctor ID" << setw(2) << "|" << setw(9) << "Monday" << setw(2) << "|" << setw(9) << "Tuesday"  << setw(2) << "|" << setw(11) 
        << "Wednesday" << setw(2) << "|" << setw(10) << "Thursday" << setw(2) << "|" << setw(9) << "Friday" << setw(2) << "|" << setw(10) << "Saturday" << setw(2) << "|" << setw(7) << "Sunday" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------" << endl;

    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(11) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(10) << " | " << setw(7) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(5) << " | " << setw(8) << (const char*)sqlite3_column_text(stmt, 2) 
             << setw(2) << " | " << setw(8) << (const char*)sqlite3_column_text(stmt, 3) << setw(2)
             << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 4) << setw(2)
             << " | " << setw(9) << (const char*)sqlite3_column_text(stmt, 5) << setw(2)
             << " | " << setw(8) << (const char*)sqlite3_column_text(stmt, 6) << setw(2)
             << " | " << setw(9) << (const char*)sqlite3_column_text(stmt, 7) << setw(2)
              << " | " << setw(5) << (const char*)sqlite3_column_text(stmt, 8) << endl;
    }    

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}

// CREATE Patient Allergy
void createPatientAllergy(const string &patientAllergyID, const string &patientID, const string &allergyName, const string &severity, const string &reaction) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO patientAllergy (PatientAllergyID, PatientID, AllergyName, Severity, Reaction) VALUES (?, ?, ?, ?, ?);";
    
    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, patientAllergyID.c_str(), patientAllergyID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, patientID.c_str(), patientID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, allergyName.c_str(), allergyName.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, severity.c_str(), severity.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, reaction.c_str(), reaction.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Patient Allergy inserted successfully" << endl;
    }
    sqlite3_finalize(stmt);
}

// READ Patient Allergy
void readPatientAllergy(){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM patientAllergy";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, globalDoctorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, globalPatientID.c_str(), -1, SQLITE_TRANSIENT);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

  cout << "--------------------------------------------------------------------------------------------------------------" << endl;
  cout << "Patient Allergy ID" << setw(2) << "|" << setw(11) << "Patient ID" << setw(2) << "|" << setw(11) << "Allergy Name" << setw(2) << "|"
       << setw(11) << "Severity" << setw(2) << "|" << setw(20) << "Rection" << endl;
  cout << "--------------------------------------------------------------------------------------------------------------" << endl;
  
  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(10) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(11) << " | " << setw(7) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(6) << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(4) << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 3)
             << setw(3) << " | " << setw(40) << (const char*)sqlite3_column_text(stmt, 4) << endl;
    }

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }
  sqlite3_finalize(stmt);
}

// CREATE Emergency Tools
void createEmergencyTools(const string &emergencyToolsID, const string &type, const string &tool, const string &emergencyNumber) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO EmergencyTools (EmergencyToolsID, Type, Tool, EmergencyNumber) VALUES (?, ?, ?, ?);";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, emergencyToolsID.c_str(), emergencyToolsID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, type.c_str(), type.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, tool.c_str(), tool.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, emergencyNumber.c_str(), emergencyNumber.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Emergency tools with ID '" << emergencyToolsID << "' inserted successfully" << endl;
    }
    sqlite3_finalize(stmt);
}

// READ Emergency Tools
void readEmergencyTools(){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM EmergencyTools;";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

  cout << "----------------------------------------------------------------------------------------------------" << endl;
  cout << "Emergency Tools ID" << setw(2) << "|" << setw(16) << "Type" << setw(15) << "|" << setw(18) << "Tool" << setw(13) << "|" << setw(15) << "Emergency Number" << endl;
  cout << "----------------------------------------------------------------------------------------------------" << endl;

  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(11) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(10) << " | " << setw(28) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(2) << " | " << setw(28) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(2) << " | " << setw(10) << (const char*)sqlite3_column_text(stmt, 3) << endl;
    }

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}


// CREATE Project
void createProject(const string &projectID, const string &title, const string &description, const string &dateOfPublication) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO Project (ProjectID, Title, Description, DateOfPublication) VALUES (?, ?, ?, ?);";
    
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, projectID.c_str(), projectID.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), title.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), description.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dateOfPublication.c_str(), dateOfPublication.length(), SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
    } else {
        cout << "Project with ID '" << projectID << "' inserted successfully" << endl;
    }
    sqlite3_finalize(stmt);
}

// READ Project
void readProject(){
    sqlite3_stmt* stmt;

    string sql = "SELECT * FROM Project;";
    DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

  cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
  cout << "Project ID" << setw(2) << "|" << setw(16) << "Title" << setw(15) << "|" << setw(35) << "Description" << setw(26) << "|" << setw(17) << "DateOfPublication" << endl;
  cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

  while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        cout << setw(7) << (const char*)sqlite3_column_text(stmt, 0) 
             << setw(6) << " | " << setw(28) << (const char*)sqlite3_column_text(stmt, 1) 
             << setw(2) << " | " << setw(58) << (const char*)sqlite3_column_text(stmt, 2)
             << setw(2) << " | " << setw(8) << (const char*)sqlite3_column_text(stmt, 3) << endl;
    }

  if (DBExit != SQLITE_DONE) {
      cerr << "Execution failed: " << sqlite3_errmsg(DB) << endl;
  }

  sqlite3_finalize(stmt);
}

#endif