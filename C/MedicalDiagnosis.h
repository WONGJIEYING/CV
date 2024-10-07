#ifndef MEDICALDIAGNOSIS_H // checks if this file has not been defined.
#define MEDICALDIAGNOSIS_H // defines this file
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// structure
struct MedicalDiagnosis {
  string PatientDiagnosisID;
  string PatientID;
  string DoctorID;
  string MedicalHistory;
  string MedicationsTaken;
  string Diagnosis;
  string LastUpdate;
  MedicalDiagnosis *next;
};

MedicalDiagnosis *MedicalDiagnosishead = NULL;

// print table
void printMedicalDiagnosis(MedicalDiagnosis *n) {
  // Print table header
  cout << setw(5) << left << "Patient Diagnosis ID"
       << " | " << setw(5) << left << "PatientID"
       << " | " << setw(5) << left << "Doctor No"
       << " | " << setw(5) << left << "Medical History"
       << " | " << setw(5) << left << "Medications Taken"
       << " | " << setw(5) << left << "Diagnosis"
       << " | " << setw(5) << left << "Last Update" << endl;
  // call data
  while (n != NULL) {
    cout << setw(5) << left << n->PatientDiagnosisID << " | " 
         << setw(5) << left << n->PatientID << " | " 
         << setw(5) << left << n->DoctorID << " | "
         << setw(5) << left << n->MedicalHistory << " | " 
         << setw(5) << left << n->MedicationsTaken << " | " 
         << setw(5) << left << n->Diagnosis << " | " 
         << setw(5) << left << n->LastUpdate << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackMedicalDiagnosis(
  string patientDiagnosisID, 
  string patientID,
  string doctorID, 
  string medicalHistory,
  string medicationsTaken,
  string diagnosis,
  string lastUpdate) {
  MedicalDiagnosis *newMedicalDiagnosis = new MedicalDiagnosis();
  newMedicalDiagnosis->PatientDiagnosisID = patientDiagnosisID;
  newMedicalDiagnosis->PatientID = patientID;
  newMedicalDiagnosis->DoctorID = doctorID;
  newMedicalDiagnosis->MedicalHistory = medicalHistory;
  newMedicalDiagnosis->MedicationsTaken = medicationsTaken;
  newMedicalDiagnosis->Diagnosis = diagnosis;
  newMedicalDiagnosis->LastUpdate = lastUpdate;
  newMedicalDiagnosis->next = NULL;

  if (MedicalDiagnosishead == NULL) {
    MedicalDiagnosishead = newMedicalDiagnosis;
  } else {
    MedicalDiagnosis *temp = MedicalDiagnosishead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newMedicalDiagnosis;
  }
}

// input to add new data
void addNewMedicalDiagnosis() {
  string PatientDiagnosisID, PatientID, DoctorID, MedicalHistory,
      MedicationsTaken, Diagnosis, LastUpdate;
  cout << "Enter Medical Diagnosis ID: ";
  cin >> PatientDiagnosisID;
  cout << "Enter Patient ID: ";
  cin >> PatientID;
  cout << "Enter Doctor ID: ";
  cin >> DoctorID;
  cout << "Enter Medical History: ";
  cin >> MedicalHistory;
  cout << "Enter Medications Taken: ";
  cin >> MedicationsTaken;
  cout << "Enter Diagnosis: ";
  cin >> Diagnosis;
  cout << "Enter Last Update: ";
  cin >> LastUpdate;

  PushBackMedicalDiagnosis(PatientDiagnosisID, PatientID, DoctorID,
                           MedicalHistory, MedicationsTaken, Diagnosis,
                           LastUpdate);
  cout << "Medical Diagnosis added successfully." << endl;
}
// database CRUD
//==========================================================================================================
// Function to read all MedicalDiagnosiss
void adminReadMedicalDiagnosis() {
  string query = "SELECT * FROM MedicalDiagnosis;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}


void patientReadMedicalDiagnosis(const string& PatientID) {
    string query = "SELECT * FROM MedicalDiagnosis WHERE PatientID = '" + PatientID + "';";
    char* errMsg;
    if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to read allergies: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}
#endif // ends this file conditional inclusion (must in the last line)
