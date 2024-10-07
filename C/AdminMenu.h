#ifndef ADMINMENU_H // checks if this file has not been defined.
#define ADMINMENU_H // defines this file
#include "Billing.h"
#include "CancelAppointment.h"
#include "DoctorSchedule.h"
#include "EmergencyTools.h"
#include "Feedback.h"
#include "Inventory.h"
#include "MedicalDiagnosis.h"
#include "MedicalInsurance.h"
#include "Notice.h"
#include "PatientAdmissionTransfer.h"
#include "PatientAllergy.h"
#include "PatientAppointment.h"
#include "PatientFollowUp.h"
#include "project.h"
#include "Purchase.h"
#include "Supplier.h"
#include "TypeOfService.h"
#include "User.h"
#include "VaccinationRecords.h"
#include "Discount.h"
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// function handleInvalidInput
void handleAdminInvalidInput() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cout << "Invalid input. Please enter a valid integer." << endl;
}


// Function to handle inventory management
void inventoryManagement(bool& returnToAdminMenu) {
  char selectInventoryManagement;
  bool exitInventoryManagement = false;

  set<string> validUserIDs = fetchValidUserIDs(); 
  set<string> validSupplierIDs = fetchValidSupplierIDs(); 
  set<string> validItemIDs = fetchValidItemIDs(); 

  while (!exitInventoryManagement) {
    cout << "\nType of Inventory Management" << endl;
    cout << "--------------------------" << endl;
    cout << "1. Inventory" << endl;
    cout << "2. Supplier" << endl;
    cout << "3. Purchase Order" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> selectInventoryManagement;

    switch (selectInventoryManagement) {
      case '1':
        cout << "\nInventory" << endl;
        adminReadInventory(); // Show inventory table
        cout << endl;

        char editInventory;
        do {
          cout << "Do you want to have action for Inventory? (Y/N): "; // Ask action
          cin >> editInventory;

          if (editInventory == 'Y' || editInventory == 'y') {
            int actionInventory;
            bool validChoice = false;

            do {
              cout << "\nSelect an action:\n";
              cout << "1. Add New Inventory\n";
              cout << "2. Edit Inventory\n";
              cout << "3. Delete Inventory\n";
              cout << "0. Exit\n";
              cout << "Enter your choice: ";
              cin >> actionInventory;

              // Check if the input is valid
              if (cin.fail()) {
                handleAdminInvalidInput();
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
              } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                switch (actionInventory) {
                  case 1:
                    adminCreateInventory(validUserIDs); // Input to add new data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 2:
                    adminUpdateInventory(validUserIDs); // Update existing data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 3:
                    adminDeleteInventory(); // Function to delete data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 0:
                    validChoice = true;
                    exitInventoryManagement = true; // Exit inventory management
                    break;
                  default:
                    cout << "Invalid choice. Please try again." << endl;
                    validChoice = false;
                    break;
                }
              }
            } while (!validChoice);
          }
        } while (editInventory == 'Y' || editInventory == 'y'); // Continue looping if 'y'
        break;

      case '2':
        cout << "\nSupplier" << endl;
        adminReadSupplier(); // Show supplier table
        cout << endl;

        char editSupplier;
        do {
          cout << "Do you want to have action for Supplier? (Y/N): "; // Ask
          cin >> editSupplier; // action

          if (editSupplier == 'Y' || editSupplier == 'y') {
            int actionSupplier;
            bool validChoice = false;

            do {
              cout << "\nSelect an action:\n";
              cout << "1. Add New Supplier\n";
              cout << "2. Edit Supplier\n";
              cout << "3. Delete Supplier\n";
              cout << "0. Exit\n";
              cout << "Enter your choice: ";
              cin >> actionSupplier;

              // Check if the input is valid
              if (cin.fail()) {
                handleAdminInvalidInput();
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
              } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                switch (actionSupplier) {
                  case 1:
                    adminCreateSupplier(); // Input to add new data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 2:
                    adminUpdateSupplier(); // Update existing data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 3:
                    adminDeleteSupplier(); // Function to delete data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 0:
                    validChoice = true;
                    exitInventoryManagement = true; // Exit inventory management
                    break;
                  default:
                    cout << "Invalid choice. Please try again." << endl;
                    validChoice = false;
                    break;
                }
              }
            } while (!validChoice);
          }
        } while (editSupplier == 'Y' || editSupplier == 'y'); // Continue looping if 'y'
        break;

      case '3':
        cout << "\nPurchase Order" << endl;
        adminReadPurchase(); // Show purchase table
        cout << endl;

        char editPurchase;
        do {
          cout << "Do you want to have action for Purchase Order? (Y/N): "; // Ask action
          cin >> editPurchase;

          if (editPurchase == 'Y' || editPurchase == 'y') {
            int actionPurchase;
            bool validChoice = false;

            do {
              cout << "\nSelect an action:\n";
              cout << "1. Add New Purchase Order\n";
              cout << "2. Edit Purchase Order\n";
              cout << "3. Delete Purchase Order\n";
              cout << "0. Exit\n";
              cout << "Enter your choice: ";
              cin >> actionPurchase;

              // Check if the input is valid
              if (cin.fail()) {
                handleAdminInvalidInput();
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
              } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                switch (actionPurchase) {
                  case 1:
                    adminCreatePurchase(validSupplierIDs, validItemIDs); // Input to add new data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 2:
                    adminUpdatePurchase(validSupplierIDs, validItemIDs); // Update existing data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 3:
                    adminDeletePurchase(); // Function to delete data
                    cout << endl;
                    validChoice = true;
                    break;
                  case 0:
                    validChoice = true;
                    exitInventoryManagement = true; // Exit inventory management
                    break;
                  default:
                    cout << "Invalid choice. Please try again." << endl;
                    validChoice = false;
                    break;
                }
              }
            } while (!validChoice);
          }
        } while (editPurchase == 'Y' || editPurchase == 'y'); // Continue looping if 'y'
        break;

      case '0':
        exitInventoryManagement = true; // Exit inventory management
        returnToAdminMenu = true; // Indicate to return to admin menu
        break;

      default:
        cout << "Invalid choice. Please try again." << endl;
        break;
    }
  }
  cout << "Exiting Inventory Management" << endl;
}

// admin menu
void adminMenu(const string& UserID) {
  int choice;
  bool exitMenu = false;
  string PatientID = UserID;
  

  set<string> validPatientIDs = fetchValidPatientIDs(); 
  set<string> validDoctorIDs = fetchValidDoctorIDs(); 
  set<string> validServiceIDs = fetchValidServiceIDs(); 
  set<string> validAppointments = adminfetchValidAppointmentIDs();
  set<string> validUserIDs = fetchValidUserIDs(); 
  set<string> validSupplierIDs = fetchValidSupplierIDs(); 
  set<string> validItemIDs = fetchValidItemIDs(); 

  while (!exitMenu) {
    cout << "\nAdmin Menu" << endl;
    cout << " 1. All User" << endl;
    cout << " 2. Medical Insurance" << endl;
    cout << " 3. Doctor Schedule" << endl;
    cout << " 4. Patient Medical Diagnosis" << endl;
    cout << " 5. Type of Service" << endl;
    cout << " 6. Patient Appointment" << endl;
    cout << " 7. Patient Canceled Appointment" << endl;
    cout << " 8. Inventory Management" << endl;
    cout << " 9. View and Publish Notices" << endl;
    cout << "10. View and Publish Project" << endl;
    cout << "11. Discount" << endl;
    cout << "12. Patient Admission Transfer" << endl;
    cout << "13. View Feedback" << endl;
    cout << "14. Patient Vaccination Records" << endl;
    cout << "15. History of Patient Allergies" << endl;
    cout << "16. Latest Patient Status" << endl;
    cout << "17. Emergency Tools" << endl;
    cout << "18. Billing" << endl;
    cout << " 0. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    // Check if the input is valid
    if (cin.fail()) {
      handleAdminInvalidInput();
      continue;
    }

    switch (choice) {
    case 1:
      cout << "\nAll User" << endl;
      adminReadUser(); // Show all users

      char filterOption;
      do {
        cout
            << "\nDo you want to filter the data? (Y/N): "; // Ask action filter
        cin >> filterOption;

        if (filterOption == 'Y' || filterOption == 'y') {
          filterUser(); // Display filtered users
        }
      } while (filterOption == 'Y' ||
               filterOption == 'y'); // Continue looping if 'y'

      char editUser;
      do {
      cout << "\nDo you want to have action for User Info? (Y/N): "; // Ask
                                                                     // action
      cin >> editUser;

      if (editUser == 'Y' || editUser == 'y') {
        int actionUser;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New User\n";
          cout << "2. Edit User\n";
          cout << "3. Delete User\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionUser;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }
          switch (actionUser) {
          case 1:
            adminCreateUser(); // Input to add new data
            cout << endl;
             // go back admin menu after add data
            validChoice = true;
            break;
          case 2:
            adminUpdateUser(); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteUser(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editUser == 'Y' || editUser == 'y'); // Continue looping if 'y' 
      break;

    case 2:
      cout << "\nMedical Insurance" << endl;
      adminReadMedicalInsurance(); // Show Medical Insurance table

      char editMedicalInsurance;
      do {
      cout
          << "\nDo you want to have action for Medical Insurance? (Y/N): "; // Ask action
      cin >> editMedicalInsurance;

      if (editMedicalInsurance == 'Y' || editMedicalInsurance == 'y') {
        int actionMedicalInsurance;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Medical Insurance\n";
          cout << "2. Edit Medical Insurance\n";
          cout << "3. Delete Medical Insurance\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionMedicalInsurance;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionMedicalInsurance) {
          case 1:
            adminCreateMedicalInsurance(validPatientIDs); // Input to add new data
            cout << endl;
             // go back admin menu after add new data
            validChoice = true;
            break;
          case 2:
            adminUpdateMedicalInsurance(validPatientIDs); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;

          case 3:
            adminDeleteMedicalInsurance(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editMedicalInsurance == 'Y' || editMedicalInsurance == 'y'); // Continue looping if 'y'
      break;

    case 3:
      cout << "\nDoctor Schedule" << endl;
      adminReadDoctorSchedule(); // Show doctor schedule table

      char editDoctorSchedule;
      do {
      cout
          << "\nDo you want to have action for Doctor Schedule? (Y/N): "; // Askaction
      cin >> editDoctorSchedule;

      if (editDoctorSchedule == 'Y' || editDoctorSchedule == 'y') {
        int actionDoctorSchedule;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Doctor Scheduler\n";
          cout << "2. Edit Doctor Scheduler\n";
          cout << "3. Delete Doctor Scheduler\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionDoctorSchedule;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionDoctorSchedule) {
          case 1:
            adminCreateDoctorSchedule(validDoctorIDs); // Input to add new data
            cout << endl;
             // go back admin menu after add new data
            validChoice = true;
            break;
          case 2:
            adminUpdateDoctorSchedule(validDoctorIDs); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteDoctorSchedule(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editDoctorSchedule == 'Y' || editDoctorSchedule == 'y'); // Continue looping if 'y'
      break;

    case 4:
      cout << "\nPatient Medical Diagosis Info\n" << endl;
      adminReadMedicalDiagnosis(); // Show Medical Diagosis table
      cout << endl;
       // Return to adminMenu
      break;

    case 5:
      cout << "\nType Of Service" << endl;
      adminReadService(); // Show Type OF Service table

      char editTypeOfService;
      do {
      cout
          << "\nDo you want to have action for Type of Service? (Y/N): "; // Ask action
      cin >> editTypeOfService;

      if (editTypeOfService == 'Y' || editTypeOfService == 'y') {
        int actionTypeOfService;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Type of Service\n";
          cout << "2. Edit Type of Service\n";
          cout << "3. Delete Type of Service\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionTypeOfService;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionTypeOfService) {
          case 1:
            adminCreateService(); // Input to add new data
            cout << endl;
             // go back admin menu after add new data
            validChoice = true;
            break;
          case 2:
            adminUpdateService(); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteService(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editTypeOfService == 'Y' || editTypeOfService == 'y'); // Continue looping if 'y'
      break;

    case 6:
      cout << "\nPatient Appointment" << endl;
      adminReadPatientsAppointment(); // Show Patient Appointment table

      char editPatientsAppointment;
      do {
      cout << "\nDo you want to have action for Patient Appointment? (Y/N): "; // Ask action
      cin >> editPatientsAppointment;

      if (editPatientsAppointment == 'Y' || editPatientsAppointment == 'y') {
        int actionPatientsAppointment;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Patients Appointment\n";
          cout << "2. Edit Patients Appointment\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionPatientsAppointment;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionPatientsAppointment) {
          case 1:
            adminCreatePatientsAppointment(validPatientIDs, validServiceIDs, validDoctorIDs); // Input to add new data
            cout << endl;
             // go back admin menu after add new data
            validChoice = true;
            break;
          case 2:
            adminUpdatePatientsAppointment(validPatientIDs, validServiceIDs, validDoctorIDs); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editPatientsAppointment == 'Y' || editPatientsAppointment == 'y'); // Continue looping if 'y'
      break;

    case 7:
      cout << "\nPatient Canceled Appoinment" << endl;
      adminReadCancelAppointment(); // Show Canceled Appoinment table

      char editCancelAppointment;
      do {
      cout << "\nDo you want to have action for Canceled Appoinment? (Y/N): "; // Ask action
      cin >> editCancelAppointment;

      if (editCancelAppointment == 'Y' || editCancelAppointment == 'y') {
        int actionCancelAppointment;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Canceled Appoinment\n";
          cout << "2. Edit Canceled Appoinment\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionCancelAppointment;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionCancelAppointment) {
          case 1:
            adminCreateCancelAppointment(validAppointments, validDoctorIDs); // Input to add new data
            cout << endl;
             // go back admin menu after add new data
            validChoice = true;
            break;
          case 2:
            adminUpdateCancelAppointment(validAppointments, validDoctorIDs); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editCancelAppointment == 'Y' || editCancelAppointment == 'y'); // Continue looping if 'y'
      break;

    case 8:
    {
      bool returnToAdminMenu = false;
      inventoryManagement(returnToAdminMenu);
          if (returnToAdminMenu) {
            continue; // Return to admin menu if indicated
          }
        }
      break;

    case 9:
      cout << "\nView and Publish Notices" << endl;
      adminReadNotice(); // Show Notice table

      char editNotice;
      do {
      cout << "\nDo you want to have action for Notice? (Y/N): "; // Ask action
      cin >> editNotice;

      if (editNotice == 'Y' || editNotice == 'y') {
        int actionNotice;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Notice\n";
          cout << "2. Edit Notice\n";
          cout << "3. Delete Notice\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionNotice;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionNotice) {
          case 1:
            adminCreateNotice(validUserIDs); // Input to add new data
            cout << endl;
             // go back admin menu after add data
            validChoice = true;
            break;
          case 2:
            adminUpdateNotice(validUserIDs); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteNotice(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editNotice == 'Y' || editNotice == 'y'); // Continue looping if 'y'
      break;

    case 10:
      cout << "\nView and Publish Project" << endl;
      adminReadProject(); // Show Project table

      char editProject;
      do {
      cout << "\nDo you want to have action for Project? (Y/N): "; // Ask action
      cin >> editProject;

      if (editProject == 'Y' || editProject == 'y') {
        int actionProject;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Project\n";
          cout << "2. Edit Project\n";
          cout << "3. Delete Project\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionProject;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionProject) {
          case 1:
            adminCreateProject(); // Input to add new data
            cout << endl;
             // go back admin menu after add data
            validChoice = true;
            break;
          case 2:
            adminUpdateProject(); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteProject(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editProject == 'Y' || editProject == 'y'); // Continue looping if 'y'
      break;

    case 11:
      cout << "\nDiscount" << endl;
      adminReadDiscount(); // Show Discount table

      char editDiscount;
      do {
      cout << "\nDo you want to have action for Discount? (Y/N): "; // Ask action
      cin >> editDiscount;

      if (editDiscount == 'Y' || editDiscount == 'y') {
        int actionDiscount;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Discount\n";
          cout << "2. Edit Discount\n";
          cout << "3. Delete Discount\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionDiscount;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;
          }
        }

          switch (actionDiscount) {
          case 1:
            adminCreateDiscount(); // Input to add new data
            cout << endl;
             // go back admin menu after add data
            validChoice = true;
            break;
          case 2:
            adminUpdateDiscount(); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteDiscount(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editDiscount == 'Y' || editDiscount == 'y'); // Continue looping if 'y'
      break;

    case 12:
      cout << "\nPatient Admission Trasfer" << endl;
      adminReadPatientAdmissionTransfer(); // Show Patient Admission Trasfer table
      cout << endl;
       // Return to adminMenu
      break;

    case 13:
      cout << "\nView Feedback" << endl;
      readFeedback(); // Show Feedback table
      cout << endl;
       // Return to adminMenu
      break;

    case 14:
      cout << "\nPatient Vaccination Records" << endl;
      adminReadVaccinationRecords(); // Show Vaccination Records table
      cout << endl;
       // Return to adminMenu
      break;

    case 15:
      cout << "\nHistory of Patient Allergies" << endl;
      adminReadPatientAllergy(); // Show Patient Allergies table
      cout << endl;
       // Return to adminMenu
      break;

    case 16:
      cout << "\nLatest Patient Status" << endl;
      adminReadPatientFollowUp(); // Show Patient Follow-up table
      cout << endl;
       // Return to adminMenu
      break;

    case 17:
      cout << "\nEmergency Tools" << endl;
      adminReadEmergencyTools(); // Show Emergency Tools table

      char editEmergencyTools;
      do {
      cout << "\nDo you want to have action for Emergency Tools? (Y/N): "; // Ask action
      cin >> editEmergencyTools;

      if (editEmergencyTools == 'Y' || editEmergencyTools == 'y') {
        int actionEmergencyTools;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Emergency Tools\n";
          cout << "2. Edit Emergency Tools\n";
          cout << "3. Delete Emergency Tools\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionEmergencyTools;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionEmergencyTools) {
          case 1:
            adminCreateEmergencyTools(); // Input to add new data
            cout << endl;
             // go back admin menu after add data
            validChoice = true;
            break;
          case 2:
            adminUpdateEmergencyTools(); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteEmergencyTools(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      } 
      } while (editEmergencyTools == 'Y' || editEmergencyTools == 'y'); // Continue looping if 'y'
      break;

    case 18:
      cout << "\nBilling" << endl;
      adminReadBilling(); // Show Billing table

      char editBilling;
      do {
      cout << "\nDo you want to have action for Billing? (Y/N): "; // Ask action
      cin >> editBilling;

      if (editBilling == 'Y' || editBilling == 'y') {
        int actionBilling;
        bool validChoice = false;

        do {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Billing\n";
          cout << "2. Edit Billing\n";
          cout << "3. Delete Billing\n";
          cout << "0. Exit\n";
          while (true) {
          cout << "Enter your choice: ";
          cin >> actionBilling;

          // Check if the input is valid
          if (cin.fail()) {
            handleAdminInvalidInput();
          } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            break;            // Valid input received, exit the loop
          }
        }

          switch (actionBilling) {
          case 1:
            adminCreateBilling(validPatientIDs); // Input to add new data
            cout << endl;
             // go back admin menu after add data
            validChoice = true;
            break;
          case 2:
            adminUpdateBilling(validPatientIDs); // Update existing data
            cout << endl;
             // go back admin menu after update data
            validChoice = true;
            break;
          case 3:
            adminDeleteBilling(); // Function to delete data
            cout << endl;
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            validChoice = false;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      }
      } while (editBilling == 'Y' || editBilling == 'y'); // Continue looping if 'y'
      break;

    case 0:
      exitMenu = true;
      return;

    default:
      cout << "Invalid choice. Please try again." << endl;
    }
  }
  cout << "Exiting Patient Menu" << endl;
}


#endif // ends this file conditional inclusion (must in the last line)
