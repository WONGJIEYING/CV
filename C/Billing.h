#ifndef BILLING_H // checks if this file has not been defined.
#define BILLING_H // defines this file
#include "TypeOfService.h"
#include "Discount.h"
#include "globalCRUD.h"
#include "sqlite3.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// structure
struct Billing
{
  string BillID;
  string PatientID;
  string DateIssued;
  double SubTotal;
  string VoucherCode;
  double TotalAmount;
  string PaymentStatus;
  Billing *next;
};

Billing *Billinghead = NULL;

// print table
void printBilling(Billing *n)
{
  // Print table header
  cout << setw(5) << left << "Bill ID"
       << " | " << setw(5) << left << "Patient ID"
       << " | " << setw(5) << left << "Date Issued"
       << " | " << setw(5) << left << "Sub Total"
       << " | " << setw(5) << left << "Voucher Code"
       << " | " << setw(5) << left << "Total Amount"
       << " | " << setw(5) << left << "Payment Status" << endl;
  // call data
  while (n != NULL)
  {
    cout << setw(5) << left << n->BillID << " | " << setw(5) << left
         << n->PatientID << " | " << setw(5) << left << n->DateIssued << " | "
         << setw(5) << left << "RM"
         << fixed << setprecision(2) << n->SubTotal << " | " // fixed in 2 digit
         << setw(5) << left << n->VoucherCode << " | " << setw(5) << left
         << "RM" << fixed << setprecision(2) << n->TotalAmount
         << " | " // fixed in 2 digit
         << setw(5) << left << n->PaymentStatus << endl;
    n = n->next;
  }
  cout << endl;
}

// push back funtion
void PushBackBilling(string billID, string patientID, string dateIssued,
                     double SubTotal, string voucherCode,
                     double totalAmount, string paymentStatus)
{
  Billing *newBilling = new Billing();
  newBilling->BillID = billID;
  newBilling->PatientID = patientID;
  newBilling->DateIssued = dateIssued;
  newBilling->SubTotal = SubTotal;
  newBilling->VoucherCode = voucherCode;
  newBilling->TotalAmount = totalAmount;
  newBilling->PaymentStatus = paymentStatus;
  newBilling->next = NULL;

  if (Billinghead == NULL)
  {
    Billinghead = newBilling;
  }
  else
  {
    Billing *temp = Billinghead;
    while (temp->next != NULL)
      temp = temp->next;

    temp->next = newBilling;
  }
}
// input to add new data
void addNewBilling()
{
  string BillID, PatientID, DateIssued, VoucherCode, PaymentStatus;
  double SubTotal, TotalAmount;
  cout << "Enter Bill ID: ";
  cin >> BillID;
  cout << "Enter Patient ID: ";
  cin >> PatientID;
  cout << "Enter Date Issued: ";
  cin >> DateIssued;
  cout << "Enter Sub Total: ";
  cin >> SubTotal;
  cout << "Enter Voucher Code: ";
  cin >> VoucherCode;
  cout << "Enter Total Amount: ";
  cin >> TotalAmount;
  cout << "Enter Payment Status: ";
  cin >> PaymentStatus;

  PushBackBilling(BillID, PatientID, DateIssued, SubTotal,
                  VoucherCode, TotalAmount, PaymentStatus);
  cout << "Billing added successfully." << endl;
}

// database CRUD
//==========================================================================================================
// Function to check if a string is a valid date in YYYY-MM-DD format
bool isValidDateBilling(const string &date)
{
  if (date.size() != 10 || date[4] != '-' || date[7] != '-')
  {
    return false;
  }
  for (int i = 0; i < date.size(); ++i)
  {
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
  if (month == 2)
  {
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > 29 || (day == 29 && !isLeapYear))
      return false;
  }
  return true;
}

// Function to generate the next BillID
string getNextBillID()
{
  sqlite3_stmt *stmt;
  string sql =
      "SELECT MAX(CAST(SUBSTR(BillID, 2) AS INTEGER)) FROM Billing";
  int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

  if (exit != SQLITE_OK)
  {
    cerr << "Failed to prepare statement for ID generation: "
         << sqlite3_errmsg(DB) << endl;
    return "";
  }

  int maxID = 0;
  exit = sqlite3_step(stmt);
  if (exit == SQLITE_ROW)
  {
    maxID = sqlite3_column_int(
        stmt, 0); // Get the max ID (numeric part) from the table
  }

  sqlite3_finalize(stmt);

  maxID++; // Increment the ID for the new record

  return "B" + to_string(maxID);
}

// Function to read all Billings
void adminReadBilling()
{
  string query = "SELECT * FROM Billing;";
  sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "sqlite3.h"

using namespace std;

string formatCurrency(double amount)
{
  stringstream ss;
  ss << fixed << setprecision(2) << "RM" << amount;
  return ss.str();
}

void adminCreateBilling(const set<string> &validPatientIDs)
{
  string addMore;
  do
  {
    string PatientID, DateIssued, PaymentStatus;
    double SubTotal, TotalAmount;
    double DiscountPercentage = 0.0;

    cout << "\nAdd New Billing" << endl;
    cout << "***********************************" << endl;

    // Automatically generate Billing ID
    string BillID = getNextBillID();

    bool validPatientID = false;
    do
    {
      cout << "Enter Patient ID (valid options are: ";
      for (const auto &id : validPatientIDs)
      {
        cout << id << " ";
      }
      cout << "): ";

      getline(cin >> ws, PatientID);

      if (isdigit(PatientID[0]))
      {
        PatientID = "P" + PatientID;
      }

      PatientID = toUpperCase(PatientID);

      // Direct validation check
      validPatientID = validPatientIDs.find(PatientID) != validPatientIDs.end();

      if (!validPatientID)
      {
        cout << "Invalid Patient ID. Please enter a valid Patient ID from the list." << endl;
        cout << "Valid options are: ";
        for (const auto &id : validPatientIDs)
        {
          cout << id << " ";
        }
        cout << endl;
      }
    } while (!validPatientID);

    // Input Date Issued
    do
    {
      cout << "Enter Date Issued (YYYY-MM-DD): ";
      getline(cin, DateIssued);
      if (!isValidDateBilling(DateIssued))
      {
        cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << endl;
      }
    } while (!isValidDateBilling(DateIssued));

    // Input Sub Total
    cout << "Enter Sub Total: RM ";
    cin >> SubTotal;

    while (cin.fail())
    {
      handleInvalidInput();
      cin >> SubTotal;
    }

    // Round SubTotal to two decimal places
    SubTotal = std::round(SubTotal * 100.0) / 100.0;

    string input;
    cout << "Enter Discount Percentage (use '-' if no discount, or percentage e.g., 10%): ";
    getline(cin >> ws, input);

    string formattedDiscount;

    if (input.empty() || input == "-")
    {
      DiscountPercentage = 0.0;    // Handle empty input or '-' for no discount
      formattedDiscount = "0.00%"; // Default formatted discount
    }
    else
    {
      // Remove '%' if present
      size_t percentPos = input.find('%');
      if (percentPos != string::npos)
      {
        input.erase(percentPos);
      }

      try
      {
        // Convert to double
        DiscountPercentage = stod(input);

        // Format DiscountPercentage to two decimal places and append '%'
        stringstream ss;
        ss << fixed << setprecision(2) << DiscountPercentage;
        formattedDiscount = ss.str() + "%";
      }
      catch (exception &e)
      {
        cout << "Invalid Discount Percentage. Defaulting to 0.00%." << endl;
        DiscountPercentage = 0.0;    // Default to 0.0 on error
        formattedDiscount = "0.00%"; // Default formatted discount
      }
    }

    // Calculate Discount Amount and Total Amount
    double DiscountAmount = (SubTotal * DiscountPercentage) / 100.0;
    TotalAmount = SubTotal - DiscountAmount;

    TotalAmount = std::round(TotalAmount * 100.0) / 100.0;
    string formattedTotalAmount = formatCurrency(TotalAmount);

    // Input Payment Status
    cout << "Enter Payment Status (Unpaid/Paid): ";
    getline(cin >> ws, PaymentStatus);

    // Insert the new Billing into the database
    string sql =
        "INSERT INTO Billing (BillID, PatientID, DateIssued, SubTotal, DiscountPercentage, TotalAmount, PaymentStatus) VALUES ('" +
        BillID + "', '" + PatientID + "', '" + DateIssued + "', '" + formatCurrency(SubTotal) + "', '" + formattedDiscount + "', '" +
        formattedTotalAmount + "', '" + PaymentStatus + "');";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK)
    {
      cerr << "Error inserting data: " << messageError << endl;
      sqlite3_free(messageError);
    }
    else
    {
      cout << "Billing added successfully!\n"
           << endl;
    }

    // Optionally read and print all Billings to confirm the addition
    adminReadBilling();

    cout << "Do you want to add another Billing? (Y/N): ";
    getline(cin >> ws, addMore);

  } while (addMore == "Y" || addMore == "y");
}

// Function to update Billing in the database
void adminUpdateBilling(const set<string> &validPatientIDs)
{
  string BillID;

  cout << "Enter Billing ID to update (blank to return to menu): ";
  getline(cin, BillID);

  if (BillID.empty())
  {
    cout << "Update process exited.";
    return;
  }

  if (BillID.find("B") != 0)
  {
    BillID = "B" + BillID;
  }

  // Check if BillID exists
  string checkSql = "SELECT COUNT(*) FROM Billing WHERE BillID = '" + BillID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK)
  {
    cerr << "Error checking BillID: " << sqlite3_errmsg(DB) << endl;
    return;
  }

  result = sqlite3_step(stmt);
  if (result == SQLITE_ROW)
  {
    int count = sqlite3_column_int(stmt, 0);
    if (count == 0)
    {
      cout << "No relevant search results found." << endl;
      sqlite3_finalize(stmt);
      return;
    }
  }
  sqlite3_finalize(stmt);

  // Variables for new billing data
  string PatientID, DateIssued, PaymentStatus;
  double SubTotal = 0.0;
  double TotalAmount = 0.0;
  double DiscountPercentage = 0.0; // Initialize discount percentage
  bool isSubTotalUpdated = false;

  // Input new values
  cout << "Enter new values for the following fields. Leave empty to skip updating the field." << endl;

  // PatientID
  cout << "PatientID: ";
  getline(cin, PatientID);
  if (!PatientID.empty())
  {
    if (isdigit(PatientID[0]))
    {
      PatientID = "P" + PatientID;
    }
    PatientID = toUpperCase(PatientID);

    if (validPatientIDs.find(PatientID) == validPatientIDs.end())
    {
      cout << "Invalid PatientID. Please enter a valid PatientID." << endl;
      PatientID.clear(); // Clear invalid input
    }
  }

  // DateIssued
  cout << "DateIssued (YYYY/MM//DD): ";
  getline(cin, DateIssued);
  if (!DateIssued.empty() && !isValidDateBilling(DateIssued))
  {
    cout << "Invalid DateIssued value. Please enter the date in YYYY/MM//DD format." << endl;
    DateIssued.clear(); // Clear invalid input
  }

  // SubTotal
  cout << "Enter Sub Total: ";
  string subTotalInput;
  getline(cin, subTotalInput);
  if (!subTotalInput.empty())
  {
    try
    {
      SubTotal = stod(subTotalInput);
      SubTotal = std::round(SubTotal * 100.0) / 100.0; // Round to two decimal places
      isSubTotalUpdated = true;                        // Mark that SubTotal has been updated
    }
    catch (exception &e)
    {
      cout << "Invalid input for Sub Total. Please enter a valid number." << endl;
      SubTotal = 0.0; // Default value or handle as needed
    }
  }

  // Discount Percentage
  string input;
  cout << "Enter Discount Percentage (use '-' if no discount, or percentage e.g., 10%): ";
  getline(cin >> ws, input);

  string formattedDiscount;

  if (input.empty() || input == "-")
  {
    DiscountPercentage = 0.0;    // Handle empty input or '-' for no discount
    formattedDiscount = "0.00%"; // Default formatted discount
  }
  else
  {
    // Remove '%' if present
    size_t percentPos = input.find('%');
    if (percentPos != string::npos)
    {
      input.erase(percentPos);
    }

    try
    {
      // Convert to double
      DiscountPercentage = stod(input);

      // Format DiscountPercentage to two decimal places and append '%'
      stringstream ss;
      ss << fixed << setprecision(2) << DiscountPercentage;
      formattedDiscount = ss.str() + "%";
    }
    catch (exception &e)
    {
      cout << "Invalid Discount Percentage. Defaulting to 0.00%." << endl;
      DiscountPercentage = 0.0;    // Default to 0.0 on error
      formattedDiscount = "0.00%"; // Default formatted discount
    }
  }

  // Calculate Discount Amount
  double DiscountAmountValue = (SubTotal * DiscountPercentage) / 100.0;
  TotalAmount = SubTotal - DiscountAmountValue;

  // Round TotalAmount to two decimal places
  TotalAmount = std::round(TotalAmount * 100.0) / 100.0;
  string formattedTotalAmount = formatCurrency(TotalAmount);

  // PaymentStatus
  cout << "Payment Status (Unpaid/Paid): ";
  getline(cin, PaymentStatus);
  if (PaymentStatus != "Unpaid" && PaymentStatus != "Paid" && PaymentStatus != "unpaid" && PaymentStatus != "paid"&& !PaymentStatus.empty())
  {
    cout << "Invalid Payment Status. Please enter 'Unpaid' or 'Paid'." << endl;
    PaymentStatus.clear(); // Clear invalid input
  }

  // Prepare SQL update statement
  string updates;
  if (!PatientID.empty())
  {
    if (!updates.empty())
    {
      updates += ", ";
    }
    updates += "PatientID = '" + PatientID + "'";
  }
  if (!DateIssued.empty())
  {
    if (!updates.empty())
    {
      updates += ", ";
    }
    updates += "DateIssued = '" + DateIssued + "'";
  }
  if (isSubTotalUpdated)
  {
    if (!updates.empty())
    {
      updates += ", ";
    }
    updates += "SubTotal = '" + formatCurrency(SubTotal) + "'";
  }
  if (DiscountPercentage != 0.0)
  {
    if (!updates.empty())
    {
      updates += ", ";
    }
    updates += "DiscountPercentage = '" + formattedDiscount + "'";
  }
  if (!PaymentStatus.empty())
  {
    if (!updates.empty())
    {
      updates += ", ";
    }
    updates += "PaymentStatus = '" + PaymentStatus + "'";
  }
  if (!updates.empty())
  {
    updates += ", TotalAmount = '" + formattedTotalAmount + "'"; // Always update TotalAmount if any field is updated
    string sql = "UPDATE Billing SET " + updates + " WHERE BillID = '" + BillID + "';";

    char *messageError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK)
    {
      cerr << "Error updating data: " << messageError << endl;
      sqlite3_free(messageError);
    }
    else
    {
      cout << "Data updated successfully!" << endl;
    }
  }
  else
  {
    cout << "No fields were updated." << endl;
  }

  adminReadBilling();
}

// Function to delete Billing from the database
void adminDeleteBilling()
{
  string BillID;

  cout << "Enter Billing ID to delete (blank to exit delete process): ";
  getline(cin, BillID);

  if (BillID.empty())
  {
    cout << "Deletion process exited." << endl;
    return;
  }

  if (BillID.find("B") != 0)
  {
    BillID = "B" + BillID;
  }

  // Check if BillID exists
  string checkSql =
      "SELECT COUNT(*) FROM Billing WHERE BillID = '" + BillID + "';";
  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmt, NULL);

  if (result != SQLITE_OK)
  {
    cerr << "Error checking BillID: " << sqlite3_errmsg(DB) << endl;
    return;
  }

  result = sqlite3_step(stmt);
  if (result == SQLITE_ROW)
  {
    int count = sqlite3_column_int(stmt, 0);
    if (count == 0)
    {
      cout << "No relevant search results found." << endl;
      sqlite3_finalize(stmt);
      return;
    }
  }
  sqlite3_finalize(stmt);

  string sql = "DELETE FROM Billing WHERE BillID = '" + BillID + "';";

  int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
  if (exit != SQLITE_OK)
  {
    cerr << "Error Delete: " << messageError << endl;
    sqlite3_free(messageError);
  }
  else
  {
    cout << "Data deleted successfully!\n"
         << endl;
  }
  adminReadBilling();
}

//=====================================================================================
void patientReadBilling(const string &PatientID)
{
  string query = "SELECT * FROM Billing WHERE PatientID = '" + PatientID + "';";
  char *errMsg;
  if (sqlite3_exec(DB, query.c_str(), callback, nullptr, &errMsg) !=
      SQLITE_OK)
  {
    cerr << "Failed to read Billing: " << errMsg << endl;
    sqlite3_free(errMsg);
  }
}

void patientProcessPayment(const string &UserID)
{
  string showBillingChoice, BillID, PaymentStatus;
  double SubTotal, TotalAmount;
  double DiscountPercentage = 0.0; // Initialize discount percentage

  // Step 2: Ask user if they want to make a payment
  cout << "Do you want to make a payment? (Y/N): ";
  char paymentChoice;
  cin >> paymentChoice;
  cin.ignore(); // Clear newline character from input buffer

  if (paymentChoice == 'Y' || paymentChoice == 'y')
  {
    // Step 3: Enter BillID to make payment
    cout << "Enter the BillID to make payment: ";
    getline(cin >> ws, BillID);

    if (BillID.find("B") != 0)
    {
      BillID = "B" + BillID;
    }

    // Check if BillID exists
    string checkSql = "SELECT COUNT(*) FROM Billing WHERE BillID = '" + BillID + "';";
    sqlite3_stmt *stmtCheck;
    int rc = sqlite3_prepare_v2(DB, checkSql.c_str(), -1, &stmtCheck, NULL);

    if (rc != SQLITE_OK)
    {
      cerr << "Error checking BillID: " << sqlite3_errmsg(DB) << endl;
      return;
    }

    rc = sqlite3_step(stmtCheck);
    if (rc == SQLITE_ROW)
    {
      int count = sqlite3_column_int(stmtCheck, 0);
      if (count == 0)
      {
        cout << "BillID not found." << endl;
        sqlite3_finalize(stmtCheck);
        return;
      }
    }
    sqlite3_finalize(stmtCheck);

    // Step 5: Update Payment Status
    string sqlGetDetails = "SELECT SubTotal FROM Billing WHERE BillID = '" + BillID + "';";
    sqlite3_stmt *stmtGetDetails;
    rc = sqlite3_prepare_v2(DB, sqlGetDetails.c_str(), -1, &stmtGetDetails, NULL);
    if (rc != SQLITE_OK)
    {
      cerr << "Failed to fetch billing details: " << sqlite3_errmsg(DB) << endl;
      return;
    }

    if (sqlite3_step(stmtGetDetails) == SQLITE_ROW)
    {
      string subtotalText = reinterpret_cast<const char *>(sqlite3_column_text(stmtGetDetails, 0));
      SubTotal = stod(subtotalText.substr(2)); // Remove 'RM' prefix

      double DiscountAmount = (SubTotal * DiscountPercentage) / 100.0;
      TotalAmount = SubTotal - DiscountAmount;

      // Round TotalAmount to two decimal places
      TotalAmount = std::round(TotalAmount * 100.0) / 100.0;
    }
    else
    {
      cout << "Failed to retrieve billing details." << endl;
      sqlite3_finalize(stmtGetDetails);
      return;
    }
    sqlite3_finalize(stmtGetDetails);

    // Update payment status
    string sqlUpdate = "UPDATE Billing SET PaymentStatus = 'Paid' WHERE BillID = '" + BillID + "';";
    char *messageError;
    rc = sqlite3_exec(DB, sqlUpdate.c_str(), NULL, 0, &messageError);
    if (rc != SQLITE_OK)
    {
      cerr << "Error updating payment status: " << messageError << endl;
      sqlite3_free(messageError);
    }
    else
    {
      cout << "Payment successful." << endl;
    }
  }
  else
  {
    cout << "Payment process canceled." << endl;
  }
}

#endif // ends this file conditional inclusion (must in the last line)
