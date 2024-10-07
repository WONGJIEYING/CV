#include "newUser.h"
#include "crud.h"
#include "doctorMenu.h"
#include "Logo.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

sqlite3 *DB; // Declare global database connection
char *messageError;

int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  fprintf(stderr, "%s:", (const char *)data);

  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NU");
  }

  printf("\n");
  return 0;
}

void executeSQL(const string &sql) {
  char *messageError;
  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK) {
    cerr << "Error executing SQL:" << messageError << endl;
    sqlite3_free(messageError);
  } else {
    cout << "Tables created successfully." << endl;
  }
}

void createTable() {
  string sql =
      "CREATE TABLE User ("
      "UserID TEXT PRIMARY KEY NOT NULL,"
      "Name TEXT NOT NULL,"
      "ICNO TEXT NOT NULL,"
      "Gender TEXT CHECK(Gender IN ('Male','Female', 'male','female')) NOT NULL,"
      "Position TEXT CHECK(Position IN ('Patient','Doctor','Admin', 'patient','doctor','admin')) NOT NULL,"
      "ContactNumber TEXT NOT NULL,"
      "Email TEXT NOT NULL,"
      "Address TEXT NOT NULL,"
      "Username TEXT NOT NULL UNIQUE,"
      "Password TEXT NOT NULL);"

      "CREATE TABLE DoctorSchedule ("
      "DoctorScheduleID TEXT PRIMARY KEY NOT NULL,"
      "DoctorID TEXT NOT NULL,"
      "Monday TEXT,"
      "Tuesday TEXT,"
      "Wednesday TEXT,"
      "Thursday TEXT,"
      "Friday TEXT,"
      "Saturday TEXT,"
      "Sunday TEXT);"

      "CREATE TABLE TypeOfService ("
      "ServiceID TEXT PRIMARY KEY NOT NULL,"
      "TypeOfService TEXT NOT NULL,"
      "Description TEXT);"

      "CREATE TABLE PatientsAppointment ("
      "AppointmentID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "ServiceID TEXT NOT NULL,"
      "Date DATE NOT NULL,"
      "Time TIME NOT NULL,"
      "Status TEXT,"
      "DoctorID TEXT NOT NULL);"

      "CREATE TABLE CancelAppointment ("
      "CancelAppointmentID TEXT PRIMARY KEY NOT NULL,"
      "AppointmentID TEXT NOT NULL,"
      "DoctorID TEXT NOT NULL,"
      "Reason TEXT);"

      "CREATE TABLE MedicalDiagnosis ("
      "PatientDiagnosisID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "DoctorID TEXT NOT NULL,"
      "MedicalHistory TEXT,"
      "MedicationsTaken TEXT,"
      "Diagnosis TEXT,"
      "LastUpdate DATE);"

      "CREATE TABLE PatientAllergy ("
      "PatientAllergyID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "AllergyName TEXT NOT NULL,"
      "Severity TEXT,"
      "Reaction TEXT);"

      "CREATE TABLE VaccinationRecords ("
      "VaccinationID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "VaccineType TEXT NOT NULL,"
      "DoseNumber TEXT,"
      "AdministrationDate DATE);"

      "CREATE TABLE PatientAdmissionTransfer ("
      "PatientStatusID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "CheckInTime DATETIME NOT NULL,"
      "CheckOutTime DATETIME,"
      "CurrentStatus TEXT NOT NULL);"

      "CREATE TABLE PatientFollowUp ("
      "FollowUpRecordID TEXT PRIMARY KEY NOT NULL,"
      "PatientDiagnosisID TEXT NOT NULL,"
      "FollowUpDate DATE NOT NULL,"
      "Description TEXT,"
      "DoctorOrders TEXT,"
      "DoctorID TEXT NOT NULL);"

      "CREATE TABLE Feedback ("
      "FeedbackID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "Comment TEXT,"
      "Rating INT NOT NULL CHECK (Rating >= 0 AND Rating <= 5),"
      "Date DATE NOT NULL);"

      "CREATE TABLE Discount ("
      "DiscountID TEXT PRIMARY KEY NOT NULL,"
      "DiscountAmount DOUBLE NOT NULL,"
      "IssuedDate DATE NOT NULL);"

      "CREATE TABLE Billing ("
      "BillID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "DateIssued TEXT NOT NULL,"
      "SubTotal DOUBLE NOT NULL,"
      "DiscountPercentage TEXT,"
      "TotalAmount DOUBLE NOT NULL,"
      "PaymentStatus TEXT NOT NULL);"

      "CREATE TABLE Supplier ("
      "SupplierID TEXT PRIMARY KEY NOT NULL,"
      "SupplierName TEXT NOT NULL,"
      "ContactNumber TEXT NOT NULL);"

      "CREATE TABLE Purchase ("
      "POID TEXT PRIMARY KEY NOT NULL,"
      "SupplierID TEXT NOT NULL,"
      "ItemID TEXT NOT NULL,"
      "PricePerBox DOUBLE NOT NULL,"
      "QuantityInBox INT NOT NULL,"
      "PurchaseQuantityBox INT NOT NULL,"
      "TotalAmount DOUBLE NOT NULL);"

      "CREATE TABLE Inventory ("
      "ItemID TEXT PRIMARY KEY NOT NULL,"
      "PurchaseItem TEXT NOT NULL,"
      "Quantity INT NOT NULL,"
      "PersonChecking TEXT NOT NULL);"

      "CREATE TABLE MedicalInsurance ("
      "MedicalInsuranceID TEXT PRIMARY KEY NOT NULL,"
      "PatientID TEXT NOT NULL,"
      "InsuranceProvider TEXT NOT NULL,"
      "PolicyNumber TEXT NOT NULL,"
      "Deductible INT NOT NULL);"

      "CREATE TABLE Notice ("
      "NoticeID TEXT PRIMARY KEY NOT NULL,"
      "Description TEXT NOT NULL,"
      "PostManageBy TEXT NOT NULL);"

      "CREATE TABLE EmergencyTools ("
      "EmergencyToolsID TEXT PRIMARY KEY NOT NULL,"
      "Type TEXT NOT NULL,"
      "Tool TEXT NOT NULL,"
      "EmergencyNumber TEXT NOT NULL);"

      "CREATE TABLE Project ("
      "ProjectID TEXT PRIMARY KEY NOT NULL,"
      "Title TEXT NOT NULL,"
      "Description TEXT,"
      "DateOfPublication DATE NOT NULL);";

  executeSQL(sql);
}

int main(int argc, char **argv) {

  char *messageError;
  int exit = sqlite3_open("Clinic.db", &DB);
  if (exit != SQLITE_OK) {
    cerr << "Error opening database:" << sqlite3_errmsg(DB) << endl;
    return exit;
  }

  // Create tables if they don't already exist
  createTable();
  Logo();

  // Call the function
  patientAppointment *patientAppointmentHead = nullptr;
  cancelAppointment *cancelAppointmentHead = nullptr;
  doctorSchedule *doctorScheduleHead = nullptr;
  patientAllergy *patientAllergyHead = nullptr;

  string userID, password, role, doctorID;
  login(userID, password, userHead, role, userID, patientAppointmentHead, doctorID, cancelAppointmentHead, doctorScheduleHead, patientAllergyHead, DB);

  // Close database connection
  sqlite3_close(DB);
  return 0;
}
