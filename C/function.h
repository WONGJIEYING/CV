#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>
#include <iomanip>
#include <regex>
#include <cctype>

using namespace std;

string toUpperCase(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

// Utility function to check if a string is numeric
bool isNumeric(const string& str) {
    // Check if the string is empty
    if (str.empty()) return false;

    // Iterate through each character to check if all are digits
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void handleInvalidInput() {
    cin.clear(); // Clear the error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
    cout << "Invalid input. Please enter a valid integer." << endl;
}

set<string> fetchValidPatientIDs() {
    set<string> validPatientIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT UserID FROM User WHERE Position IN ('patient', 'Patient');";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        std::cerr << "Error fetching valid Patient IDs: " << sqlite3_errmsg(DB) << std::endl;
        return validPatientIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validPatientIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validPatientIDs;
}

set<string> fetchValidDoctorIDs() {
    set<string> validDoctorIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT UserID FROM User WHERE Position IN ('doctor', 'Doctor');";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        std::cerr << "Error fetching valid Doctor IDs: " << sqlite3_errmsg(DB) << std::endl;
        return validDoctorIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validDoctorIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validDoctorIDs;
}

set<string> fetchValidServiceIDs() {
    set<string> validServiceIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT ServiceID FROM TypeOfService;";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        std::cerr << "Error fetching valid Service IDs: " << sqlite3_errmsg(DB) << endl;
        return validServiceIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validServiceIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validServiceIDs;
}

set<string> adminfetchValidAppointmentIDs() {
    set<string> validAppointmentIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT AppointmentID FROM PatientsAppointment;";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        std::cerr << "Error fetching valid Service IDs: " << sqlite3_errmsg(DB) << endl;
        return validAppointmentIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validAppointmentIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validAppointmentIDs;
}

set<string> patientFetchValidAppointmentIDs(const string& PatientID) {
    set<string> validAppointmentIDs;
    sqlite3_stmt *stmt;
    string sql = "SELECT AppointmentID FROM PatientsAppointment WHERE PatientID = ?;";

    // Prepare the SQL statement
    int result = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
        return validAppointmentIDs;
    }

    // Bind the PatientID to the SQL statement
    sqlite3_bind_text(stmt, 1, PatientID.c_str(), -1, SQLITE_STATIC);

    // Execute the query and fetch the results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validAppointmentIDs.insert(id);
    }

    // Finalize the statement to avoid memory leaks
    sqlite3_finalize(stmt);

    return validAppointmentIDs;
}

set<string> fetchValidCancelAppointmentIDs() {
    set<string> validCancelAppointmentIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT CancelAppointmentID FROM CancelAppointment WHERE PatientID = ?;";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "Error fetching valid CancelAppointment IDs: " << sqlite3_errmsg(DB) << endl;
        return validCancelAppointmentIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validCancelAppointmentIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validCancelAppointmentIDs;
}

set<string> fetchValidUserIDs() {
    set<string> validUserIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT UserID FROM User WHERE Position IN ('doctor', 'Doctor', 'admin', 'Admin');";
    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "Error fetching valid User IDs: " << sqlite3_errmsg(DB) << endl;
        return validUserIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validUserIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validUserIDs;
}

set<string> fetchValidSupplierIDs() {
    set<string> validSupplierIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT SupplierID FROM Supplier;";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "Error fetching valid Supplier IDs: " << sqlite3_errmsg(DB) << endl;
        return validSupplierIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validSupplierIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validSupplierIDs;
}

set<string> fetchValidItemIDs() {
    set<string> validItemIDs;
    sqlite3_stmt *stmt;
    string query = "SELECT ItemID FROM Inventory;";

    int result = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "Error fetching valid Item IDs: " << sqlite3_errmsg(DB) << endl;
        return validItemIDs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        validItemIDs.insert(id);
    }

    sqlite3_finalize(stmt);
    return validItemIDs;
}

#endif