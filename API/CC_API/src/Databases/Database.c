#include "Database.h"

WebFrameworkException getOrCreateTable(Database implementation, const char* tableName, const char* createTableQuery, Table* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getOrCreateTable)(void* implementation, const char* tableName, const char* createTableQuery, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateTable, tableName, createTableQuery, &exception);

	return exception;
}

WebFrameworkException getTable(Database implementation, const char* tableName, Table* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getTable)(void* implementation, const char* tableName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTable, tableName, &exception);

	return exception;
}

WebFrameworkException containsTable(Database implementation, const char* tableName, Table* outTable, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool (*containsTable)(void* implementation, const char* tableName, void** outTable, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsTable, tableName, outTable, &exception);

	return exception;
}

WebFrameworkException getDatabaseName(Database implementation, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getDatabaseName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseName, &exception);

	return exception;
}

WebFrameworkException getDatabaseFileName(Database implementation, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getDatabaseFileName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseFileName, &exception);

	return exception;
}
