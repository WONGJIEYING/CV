#ifndef GLOBALCRUD_H
#define GLOBALCRUD_H

#include "sqlite3.h"
#include <set>
#include <string> // Ensure this is included for std::string

extern sqlite3* DB; // Global database connection
extern char* messageError;

extern int callback(void* data, int argc, char** argv, char** azColName);

// Declare validPatientIDs
extern set<string> validPatientIDs;
set<string> validPatientIDs;

#endif // GLOBALCRUD_H