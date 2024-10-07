#ifndef DOCTOR_H
#define DOCTOR_H

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include "sqlite3.h"
#include "crud.h"
#include "global.h"
#include "function.h"
#include "User.h"

using namespace std;
using std::setw;

extern sqlite3* DB;

patientAppointment *appointmentHead = NULL;
patientAppointment *currentPatientAppointment = NULL;

cancelAppointment *cancelAppointmentHead = NULL;
cancelAppointment *currentCancelAppointment = NULL;

medicalDiagnosis *medicalDiagnosisHead = NULL;
medicalDiagnosis *currentMedicalDiagnosis = NULL;

followUp *followHead = NULL;
followUp *currentFollow = NULL;

admissionTransfer *admissionTransferHead = NULL;
admissionTransfer *currentAdmissionTransfer = NULL;

vaccination *vaccinationHead = NULL;
vaccination *currentVaccination = NULL;

doctorSchedule *doctorScheduleHead = NULL;
doctorSchedule *currentDoctorSchedule = NULL;

emergencyTools *emergencyToolsHead = NULL;
emergencyTools *currentEmergencyTools = NULL;

project *projectHead = NULL;
project *currentProject = NULL;

patientAllergy *patientAllergyHead = NULL;
patientAllergy *currentPatientAllergy = NULL;

void doctorMenu(user *userHead, string loggedInUser, patientAppointment *patientAppointmentHead, string doctorID, cancelAppointment *cancelAppointmentHead, doctorSchedule *doctorScheduleHead, patientAllergy *patientAllergyHead);

patientAppointment *createAppointmentNode(string appointmentID, string patientID, string serviceID, string date, string time, string status, string doctorID) {
  patientAppointment *newNode = new patientAppointment;
  newNode->appointmentID = appointmentID;
  newNode->patientID = patientID;
  newNode->serviceID = serviceID;
  newNode->date = date;
  newNode->time = time;
  newNode->status = status;
  newNode->doctorID = doctorID;
  newNode->next = nullptr; // Initialize next pointer to null

  return newNode;
}

cancelAppointment *createCancelAppointmentNode(string cancelAppointmentID, string appointmentID, string doctorID, string reason) {
  cancelAppointment *newNode = new cancelAppointment;
  newNode->cancelAppointmentID = cancelAppointmentID;
  newNode->appointmentID = appointmentID;
  newNode->doctorID = doctorID;
  newNode->reason = reason;

  return newNode;
}

medicalDiagnosis *createMedicalDiagnosisNode(string patientDiagnosisID, string patientID, string doctorID, string medicalHistory, string medicationsTaken, string diagnosis, string lastUpdate) {
  medicalDiagnosis *newNode = new medicalDiagnosis;
  newNode->patientDiagnosisID = patientDiagnosisID;
  newNode->patientID = patientID;
  newNode->doctorID = doctorID;
  newNode->medicalHistory = medicalHistory;
  newNode->medicationsTaken = medicationsTaken;
  newNode->diagnosis = diagnosis;
  newNode->lastUpdate = lastUpdate;

  return newNode;
}

followUp *createFollowUpNode(string followUpRecordID, string patientDiagnosisID, string followUpDate, string description, string doctorOrders) {
followUp *newNode = new followUp;
  newNode->followUpRecordID = followUpRecordID;
  newNode->patientDiagnosisID = patientDiagnosisID;
  newNode->followUpDate = followUpDate;
  newNode->description = description;
  newNode->doctorOrders = doctorOrders;

  return newNode;
}

admissionTransfer *createAdmissionTransferNode(string patientStatusID, string patientID, string checkInTime, string checkOutTime, string currentStatus) {
  admissionTransfer *newNode = new admissionTransfer;
  newNode->patientStatusID = patientStatusID;
  newNode->patientID = patientID;
  newNode->checkInTime = checkInTime;
  newNode->checkOutTime = checkOutTime;
  newNode->currentStatus = currentStatus;

  return newNode;
}

vaccination *createVaccinationNode(string vaccinationID, string patientID, string vaccineType, string doseNumber, string administrationDate) {
  vaccination *newNode = new vaccination;
  newNode->vaccinationID = vaccinationID;
  newNode->patientID = patientID;
  newNode->vaccineType = vaccineType;
  newNode->doseNumber = doseNumber;
  newNode->administrationDate = administrationDate;

  return newNode;
}

doctorSchedule *createDoctorScheduleNode(string doctorScheduleID, string doctorID, string monday, string tuesday, string wednesday, string thursday, string friday, string saturday, string sunday) {
  doctorSchedule *newNode = new doctorSchedule;
  newNode->doctorScheduleID = doctorScheduleID;
  newNode->doctorID = doctorID;
  newNode->monday = monday;
  newNode->tuesday = tuesday;
  newNode->wednesday = wednesday;
  newNode->thursday = thursday;
  newNode->friday = friday;
  newNode->saturday = saturday;
  newNode->sunday = sunday;

  return newNode;
}

patientAllergy *createPatientAllergyNode(string patientAllergyID, string patientID, string allergyName, string severity, string reaction) {
  patientAllergy *newNode = new patientAllergy;
  newNode->patientAllergyID = patientAllergyID;
  newNode->patientID = patientID;
  newNode->allergyName = allergyName;
  newNode->severity = severity;
  newNode->reaction = reaction;

  return newNode;
}

emergencyTools *createEmergencyToolsNode(string emergencyToolsID, string type, string tool, string emergencyNumber) {
  emergencyTools *newNode = new emergencyTools;
  newNode->emergencyToolsID = emergencyToolsID;
  newNode->type = type;
  newNode->tool = tool;
  newNode->emergencyNumber = emergencyNumber;

  return newNode;
}

project *createProjectNode(string projectID, string title, string description, string dateOfPublication) {
  project *newNode = new project;
  newNode->projectID = projectID;
  newNode->title = title;
  newNode->description = description;
  newNode->dateOfPublication = dateOfPublication;
  
  return newNode;
}

string formatDiagnosisID(const string &input) {
  string formattedID = input;
  if (formattedID.empty())
    return formattedID;

  for (char &ch : formattedID) {
    ch = toupper(ch);
  }

  if (formattedID.substr(0, 2) != "PH") {
    formattedID = "PH" + formattedID;
  }

  return formattedID;
}

string formatFollowUpID(const string &input) {
  string formattedID = input;
  if (formattedID.empty())
    return formattedID;

  if (formattedID.substr(0, 2) != "FU") {
    formattedID = "FU" + formattedID;
  } else {
    formattedID = "FU" + formattedID.substr(2);
  }

  for (char &ch : formattedID) {
    ch = toupper(ch);
  }
  return formattedID;
}

string formatAdmissionTransferID(const string &input) {
  string formattedID = input;
  if (formattedID.empty())
    return formattedID;

  if (formattedID.substr(0, 2) != "PS") {
    formattedID = "PS" + formattedID;
  } else {
    formattedID = "PS" + formattedID.substr(2);
  }

  for (char &ch : formattedID) {
    ch = toupper(ch);
  }
  return formattedID;
}

string toLowerCase(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
              [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

bool isValidDateTime(const string &dateTime) {
  // format YYYY-MM-DD HH:MM:SS
  regex dateTimeRegex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2})");

  // Check if the format matches
  if (!regex_match(dateTime, dateTimeRegex)) {
    return false;
  }
  return true;
}

bool isNumericWithLength(const string &str, size_t length) {
    return str.length() == length && all_of(str.begin(), str.end(), ::isdigit);
}

bool isValidICNO(const string &icno) {
    return isNumericWithLength(icno, 12);
}

bool isValidEmail(const string &email) {
    return email.find('@') != string::npos;
}

void UserUpdate(const string &loggedInUserID, sqlite3 *DB) {
    string userID = loggedInUserID;
    bool found = true;

        // Prepare the SQL query to check if the ID exists
        string selectSQL = "SELECT * FROM User WHERE UserID = ?";
        sqlite3_stmt *stmt;
        int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

        if (exit != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
            return;
        }

        // Bind the user ID
        sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the query
        exit = sqlite3_step(stmt);
        if (exit == SQLITE_ROW) {
            found = true;

            string newName, newICNO, newGender, newContactNumber, newEmail, newAddress, newUsername, newPassword;

            do {
              cout << "Enter new/current Name: ";
              getline(cin, newName);

              if (newName.empty()) {
                  cout << "Name cannot be empty. Please enter a valid name." << endl;
              }
          } while (newName.empty());

           bool validICNO = false;
        do {
            cout << "Enter new/current ICNO (12 digits): ";
            getline(cin >> ws, newICNO);
            if (!isValidICNO(newICNO)) {
                cout << "Invalid ICNO. It must be exactly 12 digits." << endl;
            } else {
                validICNO = true;
            }
        } while (!validICNO);

            bool validGender = false;
        do {
            cout << "Enter new Gender (valid options are: Male, Female): ";
            getline(cin >> ws, newGender);
            newGender = toLowerCase(newGender); // Convert to lowercase for consistency
            if (newGender != "male" && newGender != "female") {
                cout << "Invalid gender. Please enter 'Male' or 'Female'." << endl;
            } else {
                // Convert to the case expected by the database
                newGender = (newGender == "male") ? "Male" : "Female";
                validGender = true;
            }
        } while (!validGender);


            bool validContactNumber = false;
        do {
            cout << "Enter new/current Contact Number (11-12 digits): ";
            getline(cin >> ws, newContactNumber);
            if (!isValidContactNumber(newContactNumber)) {
                cout << "Invalid contact number. It must be between 11 and 12 digits." << endl;
            } else {
                validContactNumber = true;
            }
        } while (!validContactNumber);

        bool validEmail = false;
        do {
            cout << "Enter new/current Email (must include '@'): ";
            getline(cin >> ws, newEmail);
            if (!isValidEmail(newEmail)) {
                cout << "Invalid email. It must include '@'." << endl;
            } else {
                validEmail = true;
            }
        } while (!validEmail);

        do {
            cout << "Enter new/current Address: ";
            getline(cin, newAddress);

            if (newAddress.empty()) {
                cout << "Address cannot be empty. Please enter a valid address." << endl;
            }
        } while (newAddress.empty());

        do {
            cout << "Enter new/current Username: ";
            getline(cin, newUsername);

            if (newUsername.empty()) {
                cout << "Username cannot be empty. Please enter a valid username." << endl;
            }
        } while (newUsername.empty());

        do {
            cout << "Enter new/current Password: ";
            getline(cin, newPassword);

            if (newPassword.empty()) {
                cout << "Password cannot be empty. Please enter a valid password." << endl;
            }
        } while (newPassword.empty());

            userUpdateData(userID, newName, newICNO, newGender, newContactNumber, newEmail, newAddress, newUsername, newPassword);
        } else {
            cout << "User with ID '" << userID << "' not found. No update performed." << endl;
        }

        sqlite3_finalize(stmt);
}

void handleDeleteUser(sqlite3 *DB) {
    string userID;
    bool found = false;

    cout << "Enter User ID to delete (or blank to exit): ";
    cin.ignore();
    getline(cin, userID);

    if (userID.empty()) {
        return; // Exit if the user input is blank
    }

    // Format the user ID if needed
    userID = formatUserID(userID);

    // Prepare SQL query to check if the user ID exists
    string selectSQL = "SELECT 1 FROM User WHERE UserID = ?";
    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the user ID
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        found = true; // User ID exists
    } else {
        cout << "User with ID '" << userID << "' not found. No deletion performed." << endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    // Call the deleteUser function to delete the user
    deleteUser(userID, DB);
}

void push_back(patientAppointment *&head, patientAppointment *&tail, const string &appointmentID, const string &patientID, const string &serviceID, const string &date, const string &time, const string &status, const string &doctorID) {
  patientAppointment *appointmentNode = new patientAppointment();
  appointmentNode->appointmentID = appointmentID;
  appointmentNode->patientID = patientID;
  appointmentNode->serviceID = serviceID;
  appointmentNode->date = date;
  appointmentNode->time = time;
  appointmentNode->status = status;
  appointmentNode->doctorID = doctorID;
  appointmentNode->next = nullptr;

  if (tail == nullptr) {
    head = tail = appointmentNode;
  } else {
    tail->next = appointmentNode;
    tail = appointmentNode;
  }
}

// Create a unique ID + count
string generateAppointmentID() {
    sqlite3_stmt* stmt;
    int exit;
    string sql = "SELECT MAX(CAST(SUBSTR(AppointmentID, 2) AS INTEGER)) FROM patientsAppointment";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "A" + to_string(maxID);
}
// doctor view own patient appointment (doctor use)
void printPatientAppointment(patientAppointment *patientAppointmentHead, string patientID, string doctorID) {
  readPatientsAppointment(doctorID);
}

void push_back(cancelAppointment *&head, cancelAppointment *&tail, const string &cancelAppointmentID, const string &appointmentID, const string &doctorID, const string &reason) {
  cancelAppointment *cancelAppointmentNode = new cancelAppointment();
  cancelAppointmentNode->cancelAppointmentID = cancelAppointmentID;
  cancelAppointmentNode->appointmentID = appointmentID;
  cancelAppointmentNode->doctorID = doctorID;
  cancelAppointmentNode->reason = reason;
  cancelAppointmentNode->next = nullptr;

  if (tail == nullptr) {
    head = tail = cancelAppointmentNode;
  } else {
    tail->next = cancelAppointmentNode;
    tail = cancelAppointmentNode;
  }
}

string generateCancelAppointmentID() {
    sqlite3_stmt* stmt;
    int exit;
    string sql = "SELECT MAX(CAST(SUBSTR(CancelAppointmentID, 3) AS INTEGER)) FROM cancelAppointment";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "CA" + to_string(maxID);
}

// doctor view own patient appointment (doctor use)
void printCancelAppointment(cancelAppointment *cancelAppointmentHead, string patientID, string doctorID) {
  readCancelAppointment(doctorID);
}

void push_back(medicalDiagnosis *&head, medicalDiagnosis *&tail, const string &patientDiagnosisID, const string &patientID, const string &doctorID, const string &medicalHistory, const string &medicationsTaken, const string &diagnosisText, const string &lastUpdate) {
  medicalDiagnosis *medicalDiagnosisNode = new medicalDiagnosis();
  medicalDiagnosisNode->patientDiagnosisID = patientDiagnosisID;
  medicalDiagnosisNode->patientID = patientID;
  medicalDiagnosisNode->doctorID = doctorID;
  medicalDiagnosisNode->medicalHistory = medicalHistory;
  medicalDiagnosisNode->medicationsTaken = medicationsTaken;
  medicalDiagnosisNode->diagnosis = diagnosisText;
  medicalDiagnosisNode->lastUpdate = lastUpdate;
  medicalDiagnosisNode->next = nullptr;

  if (tail == nullptr) {
    head = tail = medicalDiagnosisNode;
  } else {
    tail->next = medicalDiagnosisNode;
    tail = medicalDiagnosisNode;
  }
}

string generateDiagnosisID(sqlite3 *DB) {
    sqlite3_stmt* stmt;
    string sql = "SELECT MAX(CAST(SUBSTR(PatientDiagnosisID, 3) AS INTEGER)) FROM MedicalDiagnosis";
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    int maxID = 0;
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0); 
    }

    sqlite3_finalize(stmt);

    maxID++;

    return "PH" + to_string(maxID);
}
// add diagnosis
void addDiagnosis(medicalDiagnosis *&head, medicalDiagnosis *&tail, user *currentUser, sqlite3* DB) {
    if (currentUser == nullptr) {
        cerr << "Error: currentUser is null" << endl;
        return;
    }

    // Set to store valid patient IDs for the current doctor
    set<string> validPatientIDs;

    // SQL query to get the Patient IDs for the current doctor
    string sql = "SELECT PatientID FROM PatientsAppointment WHERE DoctorID = ?;";
    sqlite3_stmt* stmt;

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, currentUser->userID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query and populate the set with patient IDs
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Ensure that the doctor has valid patients
    if (validPatientIDs.empty()) {
        cout << "No patients assigned to this doctor." << endl;
        return;
    }

    string addMore;
    do {
        string patientID, medicalHistory, medicationsTaken, diagnosisText, lastUpdate;

        cout << "\nAdd Patient Medical Diagnosis" << endl;
        cout << "***********************************" << endl;

        // Automatically generate Patient Diagnosis ID
        string patientDiagnosisID = generateDiagnosisID(DB);

        bool validPatientID = false;
        do {
          cout << "Enter new Patient ID ( valid options are: ";
          for (const auto &id : validPatientIDs) {
            cout << id << " ";
          }
          cout << "): ";

           getline(cin >> ws, patientID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(patientID)) {
                patientID = "P" + patientID;
            }

          patientID = toUpperCase(patientID);

          // Direct validation check
          validPatientID = validPatientIDs.find(patientID) != validPatientIDs.end();

          if (!validPatientID) {
            cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validPatientIDs) {
              cout << id << " ";
            }
            cout << endl;
          }
        } while (!validPatientID);

        string doctorID = currentUser->userID; // Set doctorID from currentUser

        cout << "Enter Medical History: ";
        getline(cin >> ws, medicalHistory);

        cout << "Enter Medications Taken: ";
        getline(cin >> ws, medicationsTaken);

        cout << "Enter Diagnosis: ";
        getline(cin >> ws, diagnosisText);

        do {
            cout << "Enter Last Update (YYYY-MM-DD): ";
            getline(cin >> ws, lastUpdate);
            if (!isValidDate(lastUpdate)) {
                cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
            }
        } while (!isValidDate(lastUpdate));

        push_back(head, tail, patientDiagnosisID, patientID, doctorID, medicalHistory, medicationsTaken, diagnosisText, lastUpdate);

        createMedicalDiagnosis(patientDiagnosisID, patientID, doctorID, medicalHistory, medicationsTaken, diagnosisText, lastUpdate);

        cout << "Do you want to add another medical diagnosis? (Y/N): ";
        getline(cin >> ws, addMore);

    } while (addMore == "Y" || addMore == "y");
}
  
// doctor view own patient appointment (doctor use)
void printMedicalDiagnoses(medicalDiagnosis *medicalDiagnosisHead, user *currentUser) {
  readDiagnosis(currentUser->userID);
}

void updateMedicalDiagnosis(const string &loggedInDoctorID, const set<string> &validDoctorIDs, sqlite3 *DB) {
    // Set to store valid patient IDs for the current doctor
    set<string> validPatientIDs;

    // SQL query to get the Patient IDs for the current doctor
    string sql = "SELECT PatientID FROM PatientsAppointment WHERE DoctorID = ?;";
    sqlite3_stmt* stmt;

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query and populate the set with patient IDs
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Ensure that the doctor has valid patients
    if (validPatientIDs.empty()) {
        cout << "No patients assigned to this doctor." << endl;
        return;
    }

    string patientDiagnosisID;
    bool found = true;

    do {
    cout << "Enter Patient Diagnosis ID to update (Blank for exit): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, patientDiagnosisID);
    

    if (patientDiagnosisID.empty()) {
      return; // Exit the function to return to the menu
    } 

    patientDiagnosisID = formatDiagnosisID(patientDiagnosisID);

    // Prepare the SQL query to check if the ID exists
    string selectSQL = "SELECT PatientID FROM medicalDiagnosis WHERE PatientDiagnosisID = ?";
    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the diagnosis ID
    sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);

   // Execute the query
        exit = sqlite3_step(stmt);
        if (exit == SQLITE_ROW) {
            string patientID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            // Check if the patient belongs to the logged-in doctor
            if (validPatientIDs.find(patientID) != validPatientIDs.end()) {
                found = true;

        string newPatientID, newDoctorID, newMedicalHistory, newMedicationsTaken, newDiagnosis, newLastUpdate;
        
        bool validPatientID = false;
        do {
          cout << "Enter new Patient ID ( valid options are: ";
          for (const auto &id : validPatientIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, newPatientID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(newPatientID) && !newPatientID.empty()) {
            newPatientID = "P" + newPatientID;
          }

          newPatientID = toUpperCase(newPatientID);

          // Direct validation check
          validPatientID = validPatientIDs.find(newPatientID) != validPatientIDs.end();

          if (!validPatientID) {
            cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;cout << "Valid options are: ";
                    for (const auto &id : validPatientIDs) {
                        cout << id << " ";
                    }
                    cout << endl;
                }
        } while (!validPatientID);

        bool validDoctorID = false;
        do {
          cout << "Enter new Doctor ID ( valid options are: ";
          for (const auto &id : validDoctorIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, newDoctorID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(newDoctorID) && !newDoctorID.empty()) {
            newDoctorID = "E" + newDoctorID;
          }

          newDoctorID = toUpperCase(newDoctorID);

          // Direct validation check
          validDoctorID = validDoctorIDs.find(newDoctorID) != validDoctorIDs.end();

          if (!validDoctorID) {
            cout << "Invalid doctor ID. Please enter a valid patient ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validDoctorIDs) {
                        cout << id << " ";
                    }
                    cout << endl;
                }
        } while (!validDoctorID);

        cout << "Enter new Medical History: ";
        getline(cin, newMedicalHistory);

        cout << "Enter new Medications Taken: ";
        getline(cin, newMedicationsTaken);

        cout << "Enter new Disease: ";
        getline(cin, newDiagnosis);

        do {
          cout << "Enter Last Update (YYYY-MM-DD): ";
          getline(cin >> ws, newLastUpdate);
          if (!isValidDate(newLastUpdate)) {
            cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
          }
        } while (!isValidDate(newLastUpdate));
        
    updateDiagnosis(patientDiagnosisID, newPatientID, newDoctorID, newMedicalHistory, newMedicationsTaken, newDiagnosis, newLastUpdate, DB);
    
    } else {
      cout << "Diagnosis with ID '" << patientDiagnosisID << "' not found. No update performed." << endl;
    }
        }else {
            cout << "Diagnosis with ID '" << patientDiagnosisID << "' not found. No update performed." << endl;
        }

        sqlite3_finalize(stmt);
    } while (!found);
}

void  deleteMedicalDiagnosis(const string &loggedInDoctorID, const set<string> &validDoctorID) {
  string patientDiagnosisID;
  bool found = false;

  cout << "Enter Patient Diagnosis ID to delete (number only, will auto-prefix PH, or blank to exit): ";
  cin.ignore();
  getline(cin, patientDiagnosisID);

  if (patientDiagnosisID.empty()) {
    return; // Exit if the user input is blank
  }

  patientDiagnosisID = formatDiagnosisID(patientDiagnosisID);

 // Prepare SQL query to check if the ID exists and get the corresponding PatientID
    string selectSQL = "SELECT PatientID FROM medicalDiagnosis WHERE PatientDiagnosisID = ?";
    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the diagnosis ID
    sqlite3_bind_text(stmt, 1, patientDiagnosisID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        string patientID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        bool authorized = false;

        // Check if the patient belongs to the logged-in doctor
        // SQL query to check if the patient ID is valid for the doctor
        string patientCheckSQL = "SELECT 1 FROM PatientsAppointment WHERE PatientID = ? AND DoctorID = ?";
        sqlite3_stmt *patientStmt;
        exit = sqlite3_prepare_v2(DB, patientCheckSQL.c_str(), -1, &patientStmt, NULL);

        if (exit != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
            sqlite3_finalize(stmt);
            return;
        }

        // Bind the patient ID and doctor ID
        sqlite3_bind_text(patientStmt, 1, patientID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(patientStmt, 2, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the query
        exit = sqlite3_step(patientStmt);
        if (exit == SQLITE_ROW) {
            authorized = true; // Authorization successful
        } else {
            cout << "Diagnosis with ID '" << patientDiagnosisID << "' not found. No deletion performed." << endl;
        }

        sqlite3_finalize(patientStmt);

        if (authorized) {
            deleteDiagnosis(loggedInDoctorID, patientDiagnosisID, DB);
        }
    } else {
        cout << "Diagnosis with ID '" << patientDiagnosisID << "' not found. No deletion performed." << endl;
    }

    sqlite3_finalize(stmt);
}

void push_back(followUp *&head, followUp *&tail, const string &followUpRecordID, const string &patientDiagnosisID, const string &followUpDate, const string &description, const string &doctorOrders) {
  followUp *followUpNode = new followUp();
  followUpNode->followUpRecordID = followUpRecordID;
  followUpNode->patientDiagnosisID = patientDiagnosisID;
  followUpNode->followUpDate = followUpDate;
  followUpNode->description = description;
  followUpNode->doctorOrders = doctorOrders;
  followUpNode->next = nullptr;

  if (head == nullptr) {
    head = tail = followUpNode;
  } else {
    tail->next = followUpNode;
    tail = followUpNode;
  }
}

string generateFollowUpID(sqlite3 *DB) {
    sqlite3_stmt* stmt;
    string sql = "SELECT MAX(CAST(SUBSTR(FollowUpRecordID, 3) AS INTEGER)) FROM PatientFollowUp";
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    int maxID = 0;
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;

    return "FU" + to_string(maxID);
}

void addFollowUp(followUp *&head, followUp *&tail, user *currentUser) {
  // Set to store valid patient IDs for the current doctor
    set<string> validPatientIDs;
    sqlite3_stmt* stmt;

    // SQL query to get the Patient IDs for the current doctor
    string sql = "SELECT PatientDiagnosisID FROM MedicalDiagnosis WHERE DoctorID = ?;";
    
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, currentUser->userID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query and populate the set with patient IDs
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Ensure that the doctor has valid patients
    if (validPatientIDs.empty()) {
        cout << "No patients assigned to this doctor." << endl;
        return;
    }
    
    string addMore;
  do {
      string patientDiagnosisID, followUpDate, description, doctorOrders;

    cout << "\nAdd Patient Follow Up Record" << endl;
    cout << "*********************************" << endl;

    string followUpRecordID = generateFollowUpID(DB);

    bool validPatientID = false;
        do {
            cout << "Enter Patient DiagnosisID (valid options are: ";
            for (const auto &id : validPatientIDs) {
                cout << id << " ";
            }
            cout << "): ";

            getline(cin >> ws, patientDiagnosisID);

            // Validate and format input
            if (isNumeric(patientDiagnosisID) && !patientDiagnosisID.empty()) {
                patientDiagnosisID = "PH" + patientDiagnosisID; // Ensure the ID starts with 'PH'
            }

            patientDiagnosisID = toUpperCase(patientDiagnosisID); // Convert to uppercase

            // Direct validation check
            validPatientID = validPatientIDs.find(patientDiagnosisID) != validPatientIDs.end();

            if (!validPatientID) {
                cout << "Invalid Patient DiagnosisID. Please enter a valid ID from the list." << endl;
                cout << "Valid options are: ";
                for (const auto &id : validPatientIDs) {
                    cout << id << " ";
                }
                cout << endl;
            }
        } while (!validPatientID);

    do {
      cout << "Enter Last Update (YYYY-MM-DD): ";
      getline(cin >> ws, followUpDate);
      if (!isValidDate(followUpDate)) {
        cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)."
             << endl;
      }
    } while (!isValidDate(followUpDate));

    cout << "Enter Description: ";
    getline(cin >> ws, description);

    cout << "Enter Doctor Orders: ";
    getline(cin >> ws, doctorOrders);

    string doctorID = currentUser->userID; 

    push_back(head, tail, followUpRecordID, patientDiagnosisID, followUpDate, description, doctorOrders);

    createPatientFollowUp(followUpRecordID, patientDiagnosisID, followUpDate, description, doctorOrders, doctorID);

    cout << "Do you want to add another follow-up record? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// patient follow up
void printFollowUp(followUp *followUpHead, user *currentUser) {
  readPatientFollowUp(currentUser->userID);
}

void updateFollowUp(const string &loggedInDoctorID, const set<string> &validDoctorIDs) {
   // Set to store valid patient IDs for the current doctor
    set<string> validPatientIDs;

    // SQL query to get the Patient IDs for the current doctor
    string sql = "SELECT DISTINCT PatientDiagnosisID FROM PatientFollowUp WHERE DoctorID = ?";
    sqlite3_stmt* stmt;

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query and populate the set with patient IDs
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Ensure that the doctor has valid patients
    if (validPatientIDs.empty()) {
        cout << "No patients assigned to this doctor." << endl;
        return;
    }
    
    // Retrieve follow-up records for the doctor’s patients
    sql = "SELECT FollowUpRecordID, PatientDiagnosisID FROM PatientFollowUp WHERE DoctorID = ?";
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

    cout << "Available follow-ups for your patients:" << endl;
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        string followUpRecordID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string patientDiagnosisID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        // Only show follow-ups for valid patients
        if (validPatientIDs.find(patientDiagnosisID) != validPatientIDs.end());
  }

    sqlite3_finalize(stmt);

    string followUpRecordID;
    bool found = false;

    cout << "Enter Follow Up Record ID to update (Blank for exit): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, followUpRecordID);

    if (followUpRecordID.empty()) {
      return; // Exit if the user input is blank
    }

    followUpRecordID = formatFollowUpID(followUpRecordID);

    // Prepare the SQL query to check if the ID exists
    string selectSQL = "SELECT PatientDiagnosisID FROM PatientFollowUp WHERE FollowUpRecordID = ? AND DoctorID = ?";
        int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

   // Bind the follow-up record ID and doctor ID
        sqlite3_bind_text(stmt, 1, followUpRecordID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query
        exit = sqlite3_step(stmt);
        if (exit == SQLITE_ROW) {
            string patientID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            // Check if the patient belongs to the logged-in doctor
            if (validPatientIDs.find(patientID) != validPatientIDs.end()) {
                found = true;

        string newPatientDiagnosisID, newLastUpdate, newDescription, newDoctorOrders, newDoctorID;

        cout << "Enter new Patient Diagnosis ID (valid options are: ";
            for (const auto &id : validPatientIDs) {
                cout << id << " ";
            }
            cout << "): ";
            
            getline(cin, newPatientDiagnosisID);
            newPatientDiagnosisID = formatDiagnosisID(newPatientDiagnosisID);
            newPatientDiagnosisID = toUpperCase(newPatientDiagnosisID);

            bool validPatientID = validPatientIDs.find(newPatientDiagnosisID) != validPatientIDs.end();

            if (!validPatientID) {
                cout << "Invalid Patient Diagnosis ID. Please enter a valid ID from the list." << endl;
                cout << "Valid options are: ";
                for (const auto &id : validPatientIDs) {
                    cout << id << " ";
                }
                cout << endl;
                return;
            }

        do {
          cout << "Enter new Last Update (YYYY-MM-DD): ";
          getline(cin, newLastUpdate);
          if (!isValidDate(newLastUpdate)) {
            cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)" << endl;
          }
        } while (!isValidDate(newLastUpdate));

        cout << "Enter new Description: ";
        getline(cin, newDescription);

        cout << "Enter new Doctor Orders: ";
        getline(cin, newDoctorOrders);

        bool validDoctorID = false;
        do {
          cout << "Enter new Doctor ID ( valid options are: ";
          for (const auto &id : validDoctorIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, newDoctorID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(newDoctorID) && !newDoctorID.empty()) {
            newDoctorID = "E" + newDoctorID;
          }

          newDoctorID = toUpperCase(newDoctorID);

          // Direct validation check
          validDoctorID = validDoctorIDs.find(newDoctorID) != validDoctorIDs.end();

          if (!validDoctorID) {
            cout << "Invalid doctor ID. Please enter a valid patient ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validDoctorIDs) {
                        cout << id << " ";
                    }
                    cout << endl;
                }
        } while (!validDoctorID);

        updatePatientFollowUp(followUpRecordID, newPatientDiagnosisID, newLastUpdate, newDescription, newDoctorOrders, newDoctorID);
        } else {
                cout << "Follow-Up Record ID '" << followUpRecordID << "' not found. No update performed." << endl;
            }
        } else {
            cout << "Follow-Up Record ID '" << followUpRecordID << "' not found. No update performed." << endl;
        }

        sqlite3_finalize(stmt);
  }


void deleteFollowUp(const string &loggedInDoctorID, followUp *&head, sqlite3* DB) {
    string followUpRecordID;
    bool found = false;

    cout << "Enter Follow-Up Record ID to delete (number only, will auto-prefix FU, or blank to exit): ";
    cin.ignore();
    getline(cin, followUpRecordID);

    if (followUpRecordID.empty()) {
        return; // Exit if the user input is blank
    }

    followUpRecordID = formatFollowUpID(followUpRecordID);

    // Prepare SQL query to check if the ID exists and belongs to the logged-in doctor
    string selectSQL = "SELECT PatientDiagnosisID FROM PatientFollowUp WHERE FollowUpRecordID = ? AND DoctorID = ?";
    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the follow-up record ID and doctor ID
    sqlite3_bind_text(stmt, 1, followUpRecordID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        found = true; // ID exists
    } else {
        cout << "Patient follow up with ID '" << followUpRecordID << "' not found. No deletion performed." << endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    deletePatientFollowUp(followUpRecordID);
}

// patient admission transfer
void printAdmissionTransfer(admissionTransfer *admissionTransferHead, user *currentUser) {
  readAdmissionTransfer(currentUser->userID);
}

void push_back(admissionTransfer *&head, admissionTransfer *&tail, const string &patientStatusID, const string &patientID, const string &checkInTime, const string &checkOutTime, const string &currentStatus) {
  admissionTransfer *admissionTransferNode = new admissionTransfer();
  admissionTransferNode->patientStatusID = patientStatusID;
  admissionTransferNode->patientID = patientID;
  admissionTransferNode->checkInTime = checkInTime;
  admissionTransferNode->checkOutTime = checkOutTime;
  admissionTransferNode->currentStatus = currentStatus;
  admissionTransferNode->next = nullptr;

  if (head == nullptr) {
    head = tail = admissionTransferNode;
  } else {
    tail->next = admissionTransferNode;
    tail = admissionTransferNode;
  }
}

string generateAdmissionTransferID(sqlite3 *DB) {
    sqlite3_stmt* stmt;
    string sql = "SELECT MAX(CAST(SUBSTR(PatientStatusID, 3) AS INTEGER)) FROM PatientAdmissionTransfer";
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    int maxID = 0;
    exit = sqlite3_step(stmt);
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0); // Get the max ID (numeric part) from the table
    }

    sqlite3_finalize(stmt);

    maxID++; // Increment the ID for the new record

    return "PS" + to_string(maxID);
}

void addAdmissionTransfer(admissionTransfer *&head, admissionTransfer *&tail, user *currentUser, sqlite3* DB) {
  // Set to store valid patient IDs for the current doctor
    set<string> validPatientIDs;

    // SQL query to get the Patient IDs for the current doctor
    string sql = "SELECT PatientID FROM PatientsAppointment WHERE DoctorID = ?;";
    sqlite3_stmt* stmt;

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, currentUser->userID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query and populate the set with patient IDs
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Ensure that the doctor has valid patients
    if (validPatientIDs.empty()) {
        cout << "No patients assigned to this doctor." << endl;
        return;
    }
  
  string addMore, doctorID;

  do {
    string patientID, checkInTime, checkOutTime, currentStatus;

    cout << "\nAdd Patient Admission Transfer" << endl;
    cout << "*************************************" << endl;

    string patientStatusID = generateAdmissionTransferID(DB);

    bool validPatientID = false;
        do {
          cout << "Enter new Patient ID ( valid options are: ";
          for (const auto &id : validPatientIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, patientID);

          if (isNumeric(patientID) && !patientID.empty()) {
            patientID = "P" + patientID;
          }

          patientID = toUpperCase(patientID);

          // Direct validation check
          validPatientID = validPatientIDs.find(patientID) != validPatientIDs.end();

          if (!validPatientID) {
            cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validPatientIDs) {
              cout << id << " ";
            }
            cout << endl;
          }
        } while (!validPatientID);

    do {
      cout << "Enter new Check-in Date and Time (YYYY-MM-DD HH:MM): ";
      getline(cin >> ws, checkInTime);
      if (!isValidDateTime(checkInTime)) {
        cout << "Invalid date and time format. Please enter in the format YYYY-MM-DD HH:MM" << endl;
      }
    } while (!isValidDateTime(checkInTime));

    do {
      cout << "Enter new Check-out Date and Time (YYYY-MM-DD HH:MM): ";
      getline(cin >> ws, checkOutTime);
      if (!isValidDateTime(checkOutTime)) {
        cout << "Invalid date and time format. Please enter in the format YYYY-MM-DD HH:MM" << endl;
      }
    } while (!isValidDateTime(checkOutTime));

    cout << "Enter Current Status (Discharged/Transferred/Other): ";
    getline(cin >> ws, currentStatus);

    string doctorID = currentUser->userID;

    push_back(head, tail, patientStatusID, patientID, checkInTime, checkOutTime, currentStatus);

    createAdmissionTransfer(patientStatusID, patientID, checkInTime, checkOutTime, currentStatus, doctorID);

    cout << "Do you want to add another admission transfer record? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

/*void updateAdmissionTransfer(const string &loggedInDoctorID, const set<string> &validDoctorIDs, sqlite3 *DB) {
    // Set to store valid patient IDs for the current doctor
    set<string> validPatientIDs;

    // SQL query to get the Patient IDs for the current doctor
    string sql = "SELECT PatientID FROM PatientsAppointment WHERE DoctorID = ?;";
    sqlite3_stmt* stmt;

    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the current doctor's ID
    sqlite3_bind_text(stmt, 1, loggedInDoctorID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query and populate the set with patient IDs
    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Ensure that the doctor has valid patients
    if (validPatientIDs.empty()) {
        cout << "No patients assigned to this doctor." << endl;
        return;
    }
  
  string patientStatusID;
  bool found = true;

  do {
    cout << "Enter Patient Status ID to update (Blank for exit): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, patientStatusID);

    if (patientStatusID.empty()) {
      return; // Exit if the user input is blank
    }

    patientStatusID = formatAdmissionTransferID(patientStatusID);

    // Prepare the SQL query to check if the ID exists and belongs to the logged-in doctor
        string selectSQL = "SELECT PatientID FROM PatientAdmissionTransfer WHERE PatientStatusID = ?;";
        DBExit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

        if (DBExit != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
            return;
        }

        // Bind the Patient Status ID
        sqlite3_bind_text(stmt, 1, patientStatusID.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the query
        DBExit = sqlite3_step(stmt);
        if (DBExit == SQLITE_ROW) {
            found = true;

        string existingPatientID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            // Check if the existing PatientID is among the valid ones
            if (validPatientIDs.find(existingPatientID) == validPatientIDs.end()) {
                cout << "Patient ID '" << existingPatientID << "' is not assigned to this doctor. No update performed." << endl;
                sqlite3_finalize(stmt);
                return;
            }

    string newPatientID, newCheckInTime, newCheckOutTime, newCurrentStatus, newDoctorID;
        
    bool validPatientID = false;
        do {
          cout << "Enter new Patient ID ( valid options are: ";
          for (const auto &id : validPatientIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, newPatientID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(newPatientID) && !newPatientID.empty()) {
            newPatientID = "P" + newPatientID;
          }

          newPatientID = toUpperCase(newPatientID);

          // Direct validation check
          validPatientID = validPatientIDs.find(newPatientID) != validPatientIDs.end();

          if (!validPatientID) {
            cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;cout << "Valid options are: ";
                    for (const auto &id : validPatientIDs) {
                        cout << id << " ";
                    }
                    cout << endl;
                }
        } while (!validPatientID);

    do {
        cout << "Enter new Check-in Date and Time (YYYY-MM-DD HH:MM): ";
        getline(cin >> ws, newCheckInTime);

        if (!isValidDateTime(newCheckInTime)) {
            cout << "Invalid date and time format. Please enter in the format YYYY-MM-DD HH:MM." << endl;
          }
        } while (!isValidDateTime(newCheckInTime));

        do {
          cout << "Enter new Check-out Date and Time (YYYY-MM-DD HH:MM): ";
          getline(cin, newCheckOutTime);

          if (!isValidDateTime(newCheckOutTime)) {
            cout << "Invalid date and time format. Please enter in the format "
                    "YYYY-MM-DD HH:MM:SS."
                 << endl;
          }
        } while (!isValidDateTime(newCheckOutTime));

        cout << "Enter new Current Status (Discharged/Transferred/Other): ";
        getline(cin, newCurrentStatus);

        if (newCurrentStatus.empty()) {
          return; // Exit if the user input is blank
        }

        bool validDoctorID = false;
        do {
          cout << "Enter new Doctor ID ( valid options are: ";
          for (const auto &id : validDoctorIDs) {
            cout << id << " ";
          }
          cout << "): ";

          getline(cin >> ws, newDoctorID);

          // Check if the input is numeric and prefix with 'E' if true
          if (isNumeric(newDoctorID) && !newDoctorID.empty()) {
            newDoctorID = "E" + newDoctorID;
          }

          newDoctorID = toUpperCase(newDoctorID);

          // Direct validation check
          validDoctorID = validDoctorIDs.find(newDoctorID) != validDoctorIDs.end();

          if (!validDoctorID) {
            cout << "Invalid doctor ID. Please enter a valid patient ID from the list." << endl;
            cout << "Valid options are: ";
            for (const auto &id : validDoctorIDs) {
                        cout << id << " ";
                    }
                    cout << endl;
                }
        } while (!validDoctorID);

            // Update the record in the database
            updatePatientAdmissionTransfer(patientStatusID, newPatientID, newCheckInTime, newCheckOutTime, newCurrentStatus);

        } else {
            cout << "Patient Status with ID '" << patientStatusID << "' not found. No update performed." << endl;
        }

        sqlite3_finalize(stmt);
    } while (!found);
}*/

void updateAdmissionTransfer() {
    // Initialize necessary sets and variables
    set<string> validPatientIDs;
    string sql = "SELECT PatientID FROM PatientAdmissionTransfer;";
    sqlite3_stmt *stmt;

    // Prepare and execute the SQL statement to fetch valid patient IDs
    int DBExit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    while ((DBExit = sqlite3_step(stmt)) == SQLITE_ROW) {
        validPatientIDs.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    // Check if there are no patients in the system
    if (validPatientIDs.empty()) {
        cout << "No patients found in the system." << endl;
        return;
    }

    string patientStatusID;
    cout << "Enter Patient Status ID to update (Blank for exit): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, patientStatusID);

    if (patientStatusID.empty()) {
        return; // Exit if input is blank
    }

    patientStatusID = formatAdmissionTransferID(patientStatusID);

    string selectSQL = "SELECT * FROM PatientAdmissionTransfer WHERE PatientStatusID = ?;";
    DBExit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);
    if (DBExit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, patientStatusID.c_str(), -1, SQLITE_TRANSIENT);

    DBExit = sqlite3_step(stmt);
    if (DBExit != SQLITE_ROW) {
        cout << "Patient Status with ID '" << patientStatusID << "' not found. No update performed." << endl;
        sqlite3_finalize(stmt);
        return;
    }

    string newPatientID, newCheckInTime, newCheckOutTime, newCurrentStatus;
    bool validPatientID = false;

    // Loop to get a valid Patient ID
    do {
        cout << "Enter new Patient ID (valid options are: ";
        for (const auto &id : validPatientIDs) {
            cout << id << " ";
        }
        cout << "): ";

        getline(cin >> ws, newPatientID);

        if (isNumeric(newPatientID) && !newPatientID.empty()) {
            newPatientID = "P" + newPatientID;
        }

        newPatientID = toUpperCase(newPatientID);
        validPatientID = validPatientIDs.find(newPatientID) != validPatientIDs.end();

        if (!validPatientID) {
            cout << "Invalid patient ID. Please enter a valid patient ID from the list." << endl;
        }
    } while (!validPatientID);

    // Get and validate the new Check-in Time
    do {
        cout << "Enter new Check-in Date and Time (YYYY-MM-DD HH:MM): ";
        getline(cin >> ws, newCheckInTime);

        if (!isValidDateTime(newCheckInTime)) {
            cout << "Invalid date and time format. Please enter in the format YYYY-MM-DD HH:MM." << endl;
        }
    } while (!isValidDateTime(newCheckInTime));

    // Get and validate the new Check-out Time
    do {
        cout << "Enter new Check-out Date and Time (YYYY-MM-DD HH:MM): ";
        getline(cin >> ws, newCheckOutTime);

        if (!isValidDateTime(newCheckOutTime)) {
            cout << "Invalid date and time format. Please enter in the format YYYY-MM-DD HH:MM." << endl;
        }
    } while (!isValidDateTime(newCheckOutTime));

    cout << "Enter new Current Status (Discharged/Transferred/Other): ";
    getline(cin >> ws, newCurrentStatus);

    if (newCurrentStatus.empty()) {
        return; // Exit if the input is blank
    }

    // Perform the update operation
    updatePatientAdmissionTransfer(DB, patientStatusID, newPatientID, newCheckInTime, newCheckOutTime, newCurrentStatus);

    sqlite3_finalize(stmt);
}

void deleteAdmissionTransfer(admissionTransfer *&head, const string& doctorID) {
    string patientStatusID;

    cout << "Enter Admission Transfer Record ID to delete (number only, will auto-prefix PH, or blank to exit): ";
    cin.ignore();
    getline(cin, patientStatusID);

    if (patientStatusID.empty()) {
        return; // Exit if the user input is blank
    }

    patientStatusID = formatAdmissionTransferID(patientStatusID);

    // Check if the patientStatusID exists in the database
    string selectSQL = R"(
        SELECT 1 
        FROM PatientAdmissionTransfer 
        WHERE PatientStatusID = ?
    )";

    sqlite3_stmt *stmt;
    int exit = sqlite3_prepare_v2(DB, selectSQL.c_str(), -1, &stmt, NULL);

    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, patientStatusID.c_str(), -1, SQLITE_TRANSIENT);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_ROW) {
        cout << "Patient status with ID '" << patientStatusID << "' not found. No deletion performed." << endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    // Call the function to delete the record from the database
    deletePatientAdmissionTransfer(patientStatusID);
}



string generateVaccinationID() {
    sqlite3_stmt* stmt;
    int exit;
    string sql = "SELECT MAX(CAST(SUBSTR(VaccinationID, 2) AS INTEGER)) FROM vaccinationRecords";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "V" + to_string(maxID);
}

// patient allergy
void printVaccination() {
readVaccination();
}


string generateDoctorScheduleID() {
    sqlite3_stmt* stmt;
    string sql = "SELECT MAX(CAST(SUBSTR(DoctorScheduleID, 2) AS INTEGER)) FROM doctorSchedule";

    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "D" + to_string(maxID);
}


// doctor schedule
void printDoctorSchedule() {
  readDoctorSchedule();
}

string generatePatientAllergyID() {
    sqlite3_stmt* stmt;
    int exit;
    string sql = "SELECT MAX(CAST(SUBSTR(PatientAllergyID, 3) AS INTEGER)) FROM patientAllergy";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "AI" + to_string(maxID);
}

// patient allergy
void printPatientAllergy() {
readPatientAllergy();
}


string generateEmergencyToolsID() {
    sqlite3_stmt* stmt;
    int exit;
    string sql = "SELECT MAX(CAST(SUBSTR(EmergencyToolsID, 3) AS INTEGER)) FROM EmergencyTools";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "ET" + to_string(maxID);
}


// notice
void printEmergencyTools(emergencyTools *emergencyToolsHead) {
readEmergencyTools();
}

string generateProjectID() {
    sqlite3_stmt* stmt;
    int exit;
    string sql = "SELECT MAX(CAST(SUBSTR(ProjectID, 4) AS INTEGER)) FROM Project";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (exit != SQLITE_OK) {
        cerr << "Failed to prepare statement for ID generation: " << sqlite3_errmsg(DB) << endl;
        return "";
    }

    exit = sqlite3_step(stmt);
    int maxID = 0;
    if (exit == SQLITE_ROW) {
        maxID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    maxID++;
    return "PRO" + to_string(maxID);
}


// notice
void printProject(project *projectHead) {
readProject();
}

void appointmentSchedule() {
  cout << "\nPatient Appointment Schedule" << endl;
  cout << "**********************************" << endl;
  cout << "1. View Appointment" << endl;
  cout << "2. View Cancelled Appointment" << endl;
  cout << "0. Exit" << endl;
}

void patientMedicalDiagnosis() {
  cout << "\nPatient Medical Diagnosis" << endl;
  cout << "**********************************" << endl;
  cout << "1. Add Medical Diagnosis" << endl;
  cout << "2. View Medical Diagnosis" << endl;
  cout << "3. Update Medical Diagnosis" << endl;
  cout << "4. Delete Medical Diagnosis" << endl;
  cout << "0. Exit" << endl;
}

void followUpManagement() {
  cout << "\nFollow-up Management" << endl;
  cout << "*************************" << endl;
  cout << "1. Patient Follow-up" << endl;
  cout << "2. Patient Admission Transfer" << endl;
  cout << "0. Back to Doctor Menu" << endl;
}

void followUp() {
  cout << "\nPatient Follow-up" << endl;
  cout << "********************" << endl;
  cout << "1. Add Patient Follow-up" << endl;
  cout << "2. View Patient Follow-up" << endl;
  cout << "3. Update Patient Follow-up" << endl;
  cout << "4. Delete Patient Follow-up" << endl;
  cout << "0. Back to Follow-up Management" << endl;
}

void patientAdmissionTransfer() {
  cout << "\nPatient Admission Transfer" << endl;
  cout << "*********************************" << endl;
  cout << "1. Add Patient Admission Transfer" << endl;
  cout << "2. View Patient Admission Transfer" << endl;
  cout << "3. Update Patient Admission Transfer" << endl;
  cout << "4. Delete Patient Admission Transfer" << endl;
  cout << "0. Back to Follow-up Management" << endl;
}

void noticeType(){
   cout << "\nNotices" << endl;
  cout << "*************************" << endl;
  cout << "1. Emergency Tools" << endl;
  cout << "2. Project" << endl;
  cout << "0. Back to Doctor Menu" << endl;
}

void doctorMenu(user *userHead, string loggedInUser, patientAppointment *patientAppointmentHead, string doctorID, cancelAppointment *cancelAppointmentHead, doctorSchedule *doctorScheduleHead, patientAllergy *patientAllergyHead) {
  int choice, appointmentChoice, diagnosisChioce, followUpManagementChioce, followUpChioce = 0, patientAdmissionTransferChioce = 0;
  int allergyChoice, noticeChoice;
  bool exitMenu = false;
  medicalDiagnosis *medicalDiagnosisHead = nullptr, *medicalDiagnosisTail = nullptr;
  struct followUp *followUpHead = nullptr, *followUpTail = nullptr;
  admissionTransfer *admissionTransferHead = nullptr, *admissionTransferTail = nullptr;

  // temporary variables
  patientAppointment *patientAppointmentTail = nullptr;
  cancelAppointment *cancelAppointmentTail = nullptr;
  doctorSchedule *doctorScheduleTail = nullptr;
  patientAllergy *patientAllergyTail = nullptr;
  vaccination *vaccinationTail = nullptr;
  emergencyTools *emergencyToolsTail = nullptr;
  project *projectTail = nullptr;

  set<string> validPatientIDs;
  populateValidPatientIDs(validPatientIDs, DB);

  user *currentUser = userHead;

  while (currentUser != nullptr) {
    if (currentUser->position == "Patient") {
      validPatientIDs.insert(currentUser->userID);
    }
    currentUser = currentUser->next;
  }

  set<string> validDoctorIDs;
  populateValidDoctorIDs(validDoctorIDs, DB);

  while (currentUser != nullptr) {
    if (currentUser->position == "Doctor") {
      validDoctorIDs.insert(currentUser->userID);
    }
    currentUser = currentUser->next;
  }

  // Find the current logged-in user
  currentUser = userHead;
  while (currentUser != nullptr && currentUser->userID != loggedInUser) {
    currentUser = currentUser->next;
  }

  while (!exitMenu) {
    cout << "\nDoctor Menu" << endl;
    cout << "****************" << endl;
    cout << "1. Profile" << endl;
    cout << "2. Patient Appointment Schedule" << endl;
    cout << "3. Patient Medical Diagnosis" << endl;
    cout << "4. Follow-up Management" << endl;
    cout << "5. Vaccination Records" << endl;
    cout << "6. Doctor Schedule" << endl;
    cout << "7. Patient Allergies History" << endl;
    cout << "8. Notices" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    // Check if the input is valid
    if (cin.fail()) {
      handleInvalidInput();
      continue;
    }

    switch (choice) {
    case 1:
      cout << "\nProfile" << endl;
      searchUserProfile(userHead, loggedInUser);
      break;
    case 2:
      do {
        appointmentSchedule();
        while (true) {
          cout << "Enter your choice: ";
          cin >> appointmentChoice;

          // Check if the input is valid
          if (cin.fail()) {
            handleInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

        if (appointmentChoice == 1) {
          printPatientAppointment(patientAppointmentHead, loggedInUser, doctorID);
        } else if (appointmentChoice == 2) {
          printCancelAppointment(cancelAppointmentHead, loggedInUser, doctorID);
        } else if (appointmentChoice != 0) {
          cout << "Invalid choice. Please try again." << endl;
        }
      } while (appointmentChoice != 0);
      break;
    case 3:
      do {
        patientMedicalDiagnosis();
        while (true) {
          cout << "Enter your choice: ";
          cin >> diagnosisChioce;

          // Check if the input is valid
          if (cin.fail()) {
            handleInvalidInput();
          } else {
            break; // Valid input received, exit the loop
          }
        }

        if (diagnosisChioce == 1) {
          addDiagnosis(medicalDiagnosisHead, medicalDiagnosisTail, currentUser, DB);
        } else if (diagnosisChioce == 2) {
          printMedicalDiagnoses(medicalDiagnosisHead, currentUser);
        } else if (diagnosisChioce == 3) {
          printMedicalDiagnoses(medicalDiagnosisHead, currentUser);
          updateMedicalDiagnosis(doctorID, validDoctorIDs, DB);
        } else if (diagnosisChioce == 4) {
          printMedicalDiagnoses(medicalDiagnosisHead, currentUser);
          deleteMedicalDiagnosis(doctorID, validDoctorIDs);
        } else if (diagnosisChioce != 0) {
          cout << "Invalid choice. Please try again." << endl;
        }
      } while (diagnosisChioce != 0);
      break;
    case 4:
      do {
        followUpManagement();
        while (true) {
          cout << "Enter your choice: ";
          cin >> followUpManagementChioce;

          // Check if the input is valid
          if (cin.fail()) {
           handleInvalidInput();
          } else {
            break; // Valid input received, exit the loop
          }
        }

        if (followUpManagementChioce == 1) {
          do {
            followUp();
            while (true) {
              cout << "Enter your choice: ";
              cin >> followUpChioce;

              // Check if the input is valid
              if (cin.fail()) {
               handleInvalidInput();
              } else {
                break; // Valid input received, exit the loop
              }
            }

            if (followUpChioce == 1) { // add
              addFollowUp(followUpHead, followUpTail, currentUser);
            } else if (followUpChioce == 2) { // view
              printFollowUp(followUpHead, currentUser);
            } else if (followUpChioce == 3) { // update
              printFollowUp(followUpHead, currentUser);
              updateFollowUp(doctorID, validDoctorIDs);
            } else if (followUpChioce == 4) {
              string followUpRecordIDToDelete;
              printFollowUp(followUpHead, currentUser);
              deleteFollowUp(doctorID, followUpHead, DB); // delete
            } else if (followUpChioce != 0) {
              cout << "Invalid choice. Please try again." << endl;
            }
          } while (followUpChioce != 0);
          // followUpManagement();

        } else if (followUpManagementChioce == 2) {
          do {
            // if (followUpManagementChioce == 2) { // view
            patientAdmissionTransfer();
            while (true) {
              cout << "Enter your choice: ";
              cin >> patientAdmissionTransferChioce;

              // Check if the input is valid
              if (cin.fail()) {
               handleInvalidInput();
              } else {
                break; // Valid input received, exit the loop
              }
            }

            if (patientAdmissionTransferChioce == 1) { 
              addAdmissionTransfer(admissionTransferHead, admissionTransferTail, currentUser, DB);
            } else if (patientAdmissionTransferChioce == 2) {
              printAdmissionTransfer(admissionTransferHead, currentUser);
            } else if (patientAdmissionTransferChioce == 3) {
              printAdmissionTransfer(admissionTransferHead, currentUser);
              updateAdmissionTransfer();
            } else if (patientAdmissionTransferChioce == 4) {
              string patientStatusIDToDelete;
              printAdmissionTransfer(admissionTransferHead, currentUser);
              deleteAdmissionTransfer(admissionTransferHead, doctorID);
            } else if (patientAdmissionTransferChioce != 0) {
              cout << "Invalid choice. Please try again." << endl;
            }
          } while (patientAdmissionTransferChioce != 0);
        } else if (followUpManagementChioce != 0) {
          cout << "Invalid choice. Please try again." << endl;
        }
      } while (followUpManagementChioce != 0);
      break;
    case 5:
        printVaccination();
      break;
    case 6:
        printDoctorSchedule();
      break;
    case 7:
        printPatientAllergy();
      break;
    case 8:
      do {
            cout << "Enter your choice for more action of the notice: ";
            cin >> noticeChoice;

            if (cin.fail()) {
              handleInvalidInput();
              continue;
            }

            if (noticeChoice == 1) {
              printEmergencyTools(emergencyToolsHead);
            }else if(noticeChoice == 2) {
              printProject(projectHead);
            }else if (noticeChoice != 0){
              cout << "Invalid choice. Please try again." << endl;
            }
          } while (noticeChoice != 0);
      break;
    case 0:
      exitMenu = true;
      break;
    default:
      cout << "Invalid choice. Please try again." << endl;
    }
  }
  cout << "Exiting Doctor Menu" << endl;
}


#endif 