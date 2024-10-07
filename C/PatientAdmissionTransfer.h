#ifndef PATIENTADMISSIONTRANSFER_H // checks if this file has not been defined.
#define PATIENTADMISSIONTRANSFER_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"

using namespace std;

// structure
struct PatientAdmissionTransfer {
    string PatientStatusID;
    string PatientID;
    string CheckInTime;
    string CheckOutTime;
    string CurrentStatus;
    PatientAdmissionTransfer* next;
};

PatientAdmissionTransfer* PatientAdmissionTransferhead = NULL;

//print table
void printPatientAdmissionTransfer(PatientAdmissionTransfer* n) {
    // Print table header
    cout << setw(5) << left << "Patient Status ID" << " | "
         << setw(5) << left << "Patient ID" << " | "
         << setw(5) << left << "Check-in Time" << " | "
         << setw(5) << left << "Check-out Time" << " | "
         << setw(5) << left << "Current Status" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->PatientStatusID << " | "
             << setw(5) << left << n->PatientID << " | "
             << setw(5) << left << n->CheckInTime << " | "
             << setw(5) << left << n->CheckOutTime << " | "
             << setw(5) << left << n->CurrentStatus << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackPatientAdmissionTransfer(
    string patientStatusID,
    string patientID,
    string checkInTime,
    string checkOutTime,
    string currentStatus) {
    PatientAdmissionTransfer* newPatientAdmissionTransfer = new PatientAdmissionTransfer();
    newPatientAdmissionTransfer->PatientStatusID = patientStatusID;
    newPatientAdmissionTransfer->PatientID = patientID;
    newPatientAdmissionTransfer->CheckInTime = checkInTime;
    newPatientAdmissionTransfer->CheckOutTime = checkOutTime;
    newPatientAdmissionTransfer->CurrentStatus = currentStatus;
    newPatientAdmissionTransfer->next = NULL;

    if (PatientAdmissionTransferhead == NULL) {
        PatientAdmissionTransferhead = newPatientAdmissionTransfer;
    } else {
        PatientAdmissionTransfer* temp = PatientAdmissionTransferhead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newPatientAdmissionTransfer;
    }
}

// input to add new data
void addNewPatientAdmissionTransfer() {
    string PatientStatusID, PatientID, CheckInTime, CheckOutTime, CurrentStatus;
    cout << "Enter Patient Status ID: ";
    cin >> PatientStatusID;
    cout << "Enter Patient ID: ";
    cin >> PatientID;
    cout << "Enter Check-in Time: ";
    cin >> CheckInTime;
    cout << "Enter Check-out Time: ";
    cin >> CheckOutTime;
    cout << "Enter Current Status: ";
    cin >> CurrentStatus;

    PushBackPatientAdmissionTransfer(PatientStatusID, PatientID, CheckInTime, CheckOutTime, CurrentStatus);
    cout << "Patient Admission Transfer added successfully." << endl;
}

// database CRUD
//==========================================================================================================
void adminReadPatientAdmissionTransfer() {
  string query = "SELECT * FROM PatientAdmissionTransfer;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}
#endif // ends this file conditional inclusion (must in the last line)
