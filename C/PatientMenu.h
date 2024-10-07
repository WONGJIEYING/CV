#ifndef PATIENTMENU_H // checks if this file has not been defined.
#define PATIENTMENU_H // defines this file
#include "Billing.h"
#include "CancelAppointment.h"
#include "EmergencyTools.h"
#include "Feedback.h"
#include "MedicalInsurance.h"
#include "Notice.h"
#include "PatientAppointment.h"
#include "Project.h"
#include "User.h"
#include "VaccinationRecords.h"
#include "Discount.h"
#include "PatientAllergy.h"
#include "MedicalDiagnosis.h"
#include "globalCRUD.h"
#include "sqlite3.h"
#include "newUser.h"
#include "function.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// patient menu
void patientMenu(const string &UserID)
{
  int choice;
  bool exitMenu = false;
  string PatientID = UserID;

  set<string> validDoctorIDs = fetchValidDoctorIDs();
  set<string> validServiceIDs = fetchValidServiceIDs();
  set<string> validAppointments = patientFetchValidAppointmentIDs(PatientID);

  while (!exitMenu)
  {
    cout << "\nPatient Menu" << endl;
    cout << " 1. Profile" << endl;
    cout << " 2. Doctor Info" << endl;
    cout << " 3. Make Appointment" << endl;
    cout << " 4. Cancel Appointment" << endl;
    cout << " 5. Appointment History" << endl;
    cout << " 6. Medical Diagnosis" << endl;
    cout << " 7. Allergy" << endl;
    cout << " 8. Notices" << endl;
    cout << " 9. Give Feedback" << endl;
    cout << "10. Vaccination History" << endl;
    cout << "11. Billing" << endl; //
    cout << "12. Medical Insurance" << endl;
    cout << "13. Emergency Tool" << endl;
    cout << "14. Project" << endl;
    cout << " 0. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    // Check if the input is valid
    if (cin.fail())
    {
      handleInvalidInput();
      continue;
    }

    bool exitLoop = false;
    switch (choice)
    {
    case 1:
      cout << "\nProfile" << endl;
      patientReadProfile(UserID); // Show profile here

      char editProfile;

      do
      {
        if (!exitLoop)
        {
          cout << "\nDo you want to have action for Profile? (Y/N): ";
          cin >> editProfile;
        }

        if (editProfile == 'Y' || editProfile == 'y')
        {
          int actionProfile;
          bool validChoice = false;

          do
          {
            cout << "\nSelect an action:\n";
            cout << "1. Edit User Profile\n";
            cout << "2. Delete User\n";
            cout << "0. Exit\n";

            cout << "Enter your choice: ";
            cin >> actionProfile;

            if (cin.fail())
            {
              handleInvalidInput();
              continue;
            }

            switch (actionProfile)
            {
            case 1:
              patientUpdateUser();
              validChoice = true;
              exitLoop = true;
              break;
            case 2:
              patientDeleteUser(UserID);
              validChoice = true;
              break;
            case 0:
              validChoice = true;
              exitLoop = true;
              break;
            default:
              cout << "Invalid choice. Please try again." << endl;
              break;
            }
          } while (!validChoice && actionProfile != 0);
        }
      } while ((editProfile == 'Y' || editProfile == 'y') && !exitLoop); // Continue looping if 'y'
      break;

    case 2:
      cout << "\nDoctor Info" << endl;
      readDoctorInfo(); // Show doctor information
      break;

    case 3:
      cout << "\nMake Appointment" << endl;
      patientReadAppointment(PatientID);
      patientCreatePatientsAppointment(UserID, validServiceIDs, validDoctorIDs); // make appointment
      break;

    case 4:
      cout << "\nPatient Cancel Appoinment" << endl;
      patientReadCancelAppointment(PatientID);

      char editCancelAppointment;
        cout << "\nDo you want to have action for Cancel Appoinment? (Y/N): "; // Ask action
        cin >> editCancelAppointment;

        if (editCancelAppointment == 'Y' || editCancelAppointment == 'y')
        {
          int actionCancelAppointment;
          bool validChoice = false;

          do
          {
            cout << "\nSelect an action:\n";
            cout << "1. Add New Canceled Appoinment\n";
            cout << "2. Edit Canceled Appoinment\n";
            cout << "0. Exit\n";

            cout << "Enter your choice: ";
            cin >> actionCancelAppointment;
            cin.ignore();
            cout << endl;

            if (cin.fail())
            {
              handleInvalidInput();
              continue;
            }

            // patientReadCancelAppointment(UserID);
            switch (actionCancelAppointment)
            {
            case 1:
              patientCreateCancelAppointment(validAppointments, PatientID); // Input to add new data
              validChoice = true;
              break;
            case 2:
              // patientReadCancelAppointment();
              patientUpdateCancelAppointment(validAppointments, validDoctorIDs); // Update existing data
              validChoice = true;
              break;
            case 0:
              validChoice = true;
              break;
            default:
              cout << "Invalid choice. Please try again." << endl;
              break;
            }
          } while (!validChoice); // Continue looping until a valid choice is made
        }
      //} while (editCancelAppointment == 'Y' || editCancelAppointment == 'y'); // Continue looping if 'y'
      break;

    case 5:
      cout << "\nPatient Appointment History" << endl;
      patientReadAppointment(UserID); // Show Medical Diagosis table
      break;

    case 6:
      cout << "\nPatient Medical Diagosis Info\n"
           << endl;
      patientReadMedicalDiagnosis(UserID); // Show Medical Diagosis table
      break;

    case 7:
      cout << "\nAllergy" << endl;
      patientReadPatientAllergy(UserID);

      char editAllergy;

      cout << "\nDo you want to have action for Allergy? (Y/N): "; // Ask action
      cin >> editAllergy;

      if (editAllergy == 'Y' || editAllergy == 'y')
      {
        int actionAllergy;
        bool validChoice = false;

        do
        {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Patient Allergy\n";
          cout << "2. Edit Patient Allergy\n";
          cout << "3. Delete Patient Allergy\n";
          cout << "0. Exit\n";

          cout << "Enter your choice: ";
          cin >> actionAllergy;
          cin.ignore();

          if (cin.fail())
          {
            handleInvalidInput();
            continue;
          }

          switch (actionAllergy)
          {
          case 1:
            addPatientAllergy(UserID); // Input to add new data
            validChoice = true;
            break;
          case 2:
            patientUpdatePatientAllergy(); // Update existing data
            validChoice = true;
            break;
          case 3:
            patientDeleteAllergy();
            validChoice = true;
            break;
          case 0:
            validChoice = true;
            break;
          default:
            cout << "Invalid choice. Please try again." << endl;
            break;
          }
        } while (!validChoice); // Continue looping until a valid choice is made
      }
      //} while (editAllergy == 'Y' || editAllergy == 'y'); // Continue looping if 'y'
      break;

    case 8:
      cout << "\nNotices" << endl;
      patientReadNotice(); // Show notice table
      break;

    case 9:
      cout << "\nGive Feedback" << endl;
      patientReadFeedback(UserID); // Show feedback table

      char editFeedback;
      cout << "\nDo you want to have action for Feedback? (Y/N): ";
      cin >> editFeedback;

      if (editFeedback == 'Y' || editFeedback == 'y')
      {
        int actionFeedback;
        bool validChoice = false;

        do
        {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Feedback\n";
          cout << "2. Edit Feedback\n";
          cout << "3. Delete Feedback\n";
          cout << "0. Exit\n";

          cout << "Enter your choice: ";
          cin >> actionFeedback;

          if (cin.fail())
          {
            handleInvalidInput();
            continue;
          }

          switch (actionFeedback)
          {
          case 1:
            patientCreateFeedback(UserID); // Add new feedback

            validChoice = true;
            break;

          case 2:
            patientUpdateFeedback(); // Update feedback
            validChoice = true;
            break;

          case 3:
            patientDeleteFeedback(); // Delete feedback
            validChoice = true;
            break;

          case 0:
            validChoice = true;
            break;

          default:
            cout << "Invalid choice. Please try again." << endl;
            break;
          }
        } while (!validChoice);
      }
      //} while (editFeedback == 'Y' || editFeedback == 'y'); // Continue looping if 'y'
      break;

    case 10:
      cout << "\nVaccination History" << endl;
      patientReadVaccinationRecord(UserID); // Show feedback table

      char editVaccination;
      cout << "\nDo you want to have action for Vaccination History? (Y/N): ";
      cin >> editVaccination;

      if (editVaccination == 'Y' || editVaccination == 'y')
      {
        int actionVaccination;
        bool validChoice = false;

        do
        {
          cout << "\nSelect an action:\n";
          cout << "1. Add New Vaccination History\n";
          cout << "2. Edit Vaccination History\n";
          cout << "3. Delete Vaccination History\n";
          cout << "0. Exit\n";

          cout << "Enter your choice: ";
          cin >> actionVaccination;

          if (cin.fail())
          {
            handleInvalidInput();
            continue;
          }

          switch (actionVaccination)
          {
          case 1:
            patientCreateVaccinationRecord(UserID); // Add new feedback

            validChoice = true;
            break;

          case 2:
            patientUpdateVaccinationRecords(); // Update feedback
            validChoice = true;
            break;

          case 3:
            patientDeleteVaccinationRecords(); // Delete feedback
            validChoice = true;
            break;

          case 0:
            validChoice = true;
            break;

          default:
            cout << "Invalid choice. Please try again." << endl;
            break;
          }
        } while (!validChoice);
      }
      //} while (editVaccination == 'Y' || editVaccination == 'y'); // Continue looping if 'y'
      break;

    case 11:
      cout << "\nBilling" << endl;
      patientReadBilling(UserID);
      patientProcessPayment(UserID); // Show billing information
      break;
    case 12:
      cout << "\nMedical Insurance" << endl;
      patientReadMedicalInsurance(UserID); // Show medical insurance table

      char editMedicalInsurance;
      do
      {
        cout << "\nDo you want to take action for Medical Insurance? (Y/N): ";
        cin >> editMedicalInsurance;

        if (editMedicalInsurance == 'Y' || editMedicalInsurance == 'y')
        {
          int actionMedicalInsurance;
          bool validChoice = false;

          do
          {
            cout << "\nSelect an action:\n";
            cout << "1. Add New Medical Insurance\n";
            cout << "2. Edit Medical Insurance\n";
            cout << "3. Delete Medical Insurance\n";
            cout << "0. Exit\n";

            cout << "Enter your choice: ";
            cin >> actionMedicalInsurance;

            if (cin.fail())
            {
              handleInvalidInput();
              continue;
            }

            switch (actionMedicalInsurance)
            {
            case 1:
              patientCreateMedicalInsurance(UserID); // Add new medical insurance
              validChoice = true;
              break;

            case 2:
              patientUpdateMedicalInsurance(); // Edit medical insurance
              validChoice = true;
              break;

            case 3:
              patientDeleteMedicalInsurance(); // Delete medical insurance
              validChoice = true;
              break;

            case 0:
              validChoice = true;
              break;

            default:
              cout << "Invalid choice. Please try again." << endl;
              break;
            }
          } while (!validChoice);
        }
      } while (editMedicalInsurance == 'Y' || editMedicalInsurance == 'y'); // Continue looping if 'y'
      break;

    case 13:
      cout << "\nEmergency Tool" << endl;
      adminReadEmergencyTools(); // Show emergency tools
      break;

    case 14:
      cout << "\nProject" << endl;
      adminReadProject(); // Show project information
      break;

    case 0:
      exitMenu = true;
      return;

    default:
      cout << "Invalid choice. Please try again." << endl;
      break;
    }
  }
  cout << "Exiting Patient Menu" << endl;
}

#endif // ends this file conditional inclusion (must be the last line)
