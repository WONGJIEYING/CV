#ifndef PATIENTFOLLOWUP_H // checks if this file has not been defined.
#define PATIENTFOLLOWUP_H //defines this file
#include <iostream>
#include <string>
#include <iomanip>
#include "globalCRUD.h"
#include "sqlite3.h"

using namespace std;

// structure
struct PatientFollowUp {
    string FollowUpRecordID;
    string PatientDiagnosisID;
    string FollowUpDate;
    string Description;
    string DoctorOrdersDescription;
    string DoctorID;
    PatientFollowUp* next;
};

PatientFollowUp* PatientFollowUphead = NULL;

//print table
void printPatientFollowUp(PatientFollowUp* n) {
    // Print table header
    cout << setw(5) << left << "Follow-up Record ID" << " | "
         << setw(5) << left << "Patient Health ID" << " | "
         << setw(5) << left << "Follow-up Date" << " | "
         << setw(5) << left << "Description" << " | "
         << setw(5) << left << "Doctor Orders Description"
         << setw(5) << left << "Doctor ID" << endl;
    //call data
    while (n != NULL) {
        cout << setw(5) << left << n->FollowUpRecordID << " | "
             << setw(5) << left << n->PatientDiagnosisID << " | "
             << setw(5) << left << n->FollowUpDate << " | "
             << setw(5) << left << n->Description << " | "
             << setw(5) << left << n->DoctorOrdersDescription << " | "
             << setw(5) << left << n->DoctorID << endl;
        n = n->next;
    }
    cout << endl;
}

//push back funtion
void PushBackPatientFollowUp(
    string followUpRecordID,
    string PatientDiagnosisID,
    string followUpDate,
    string description,
    string doctorOrdersDescription,
    string doctorID) {
    PatientFollowUp* newPatientFollowUp = new PatientFollowUp();
    newPatientFollowUp->FollowUpRecordID = followUpRecordID;
    newPatientFollowUp->PatientDiagnosisID = PatientDiagnosisID;
    newPatientFollowUp->FollowUpDate = followUpDate;
    newPatientFollowUp->Description = description;
    newPatientFollowUp->DoctorOrdersDescription = doctorOrdersDescription;
    newPatientFollowUp->DoctorID = doctorID;
    newPatientFollowUp->next = NULL;

    if (PatientFollowUphead == NULL) {
        PatientFollowUphead = newPatientFollowUp;
    } else {
        PatientFollowUp* temp = PatientFollowUphead;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newPatientFollowUp;
    }
}

// input to add new data
void addNewPatientFollowUp() {
    string FollowUpRecordID, PatientDiagnosisID, FollowUpDate, Description, DoctorOrdersDescription, DoctorID;
    cout << "Enter Follow-up Record ID: ";
    cin >> FollowUpRecordID;
    cout << "Enter Patient Health ID: ";
    cin >> PatientDiagnosisID;
    cout << "Enter Follow-up Date: ";
    cin >> FollowUpDate;
    cout << "Enter Description: ";
    cin >> Description;
    cout << "Enter Doctor Orders Description: ";
    cin >> DoctorOrdersDescription;
    cout << "Enter Doctor ID: ";
    cin >> DoctorID;

    PushBackPatientFollowUp(FollowUpRecordID, PatientDiagnosisID, FollowUpDate, Description, DoctorOrdersDescription, DoctorID);
    cout << "User added successfully." << endl;
}

// database CRUD
//==========================================================================================================
void adminReadPatientFollowUp() {
  string query = "SELECT * FROM PatientFollowUp;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}
#endif // ends this file conditional inclusion (must in the last line)