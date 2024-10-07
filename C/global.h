#ifndef GLOBALCRUD_H // checks if this file has not been defined.
#define GLOBALCRUD_H //defines this file

extern sqlite3* DB; // Global database connection
extern char* messageError;

extern int callback(void* data, int argc, char** argv, char** azColName);

#endif // ends this file conditional inclusion (must in the last line)